/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010, Sebastian Schlenkrich

*/



#ifndef quantlib_templateauxilliaries_regression_hpp
#define quantlib_templateauxilliaries_regression_hpp

//#include <ql/types.hpp>
//#include <boost/function.hpp>

#include <ql/experimental/template/auxilliaries/templatesvd.hpp>


namespace TemplateAuxilliaries {

    template <class Type>
    class Regression {

	protected:

		size_t                               maxDegree_;  // max polynomial degree
	    std::vector< std::vector<size_t> >   multIdx_;    // list of all multi-indeces with degree <= maxDegree_
	    std::vector<Type>                    beta_;       // linear coefficients

		inline void divide( std::vector<size_t> x, size_t idx, size_t degree) {
			if (idx==x.size()-1) {
				x[idx] = degree;
				multIdx_.push_back(x);
			} else {
			    for (size_t k=0; k<=degree; ++k) {
				    x[idx] = k;
				    divide(x, idx+1, degree-k);
				}
			}
		}

		// initialise multi-index matrix via recursive call of divide()
		inline void setUpMultiIndex(const size_t dim, const size_t maxDegree) {
			multIdx_.clear();
			std::vector<size_t> x(dim,0);
			for (size_t k=0; k<=maxDegree; ++k) divide(x,0,k);
		}

		// perform actual regression calculation
		inline void calculateRegression( const std::vector< std::vector<Type> >& controls,
			                             const std::vector< Type >&              observations ) {
			size_t nRows = controls.size();
			size_t nCols = multIdx_.size();

			// set up matrices for SVD
			Type *A  = new Type[nRows*nCols];
			Type *U  = new Type[nRows*nRows];
			Type *S  = new Type[nCols];
			Type *VT = new Type[nCols*nCols];
			// dummy auxilliary variables
			Type work;
			int lwork, info;
			// assume column-wise representation A = U S V^T (Fortran-style!)
			for (size_t i=0; i<nRows; ++i) {
				std::vector<Type> y = monomials(controls[i]);
				for (size_t j=0; j<nCols; ++j) {
					A[i+j*nRows] = y[j];
				}
			}
			TemplateAuxilliaries::svd("S","S",(int*)&nRows,(int*)&nCols,A,(int*)&nRows,S,U,(int*)&nRows,VT,(int*)&nCols,&work,&lwork,&info);
			// check min(S)>0
			Type minS=S[0];
			for (size_t i=1; i<nCols; ++i) if (S[i]<minS) minS = S[i];
			QL_REQUIRE(minS>0,"Regression non-zero singular value required.");
			std::vector<Type> y(nCols,0.0);
			for (size_t i=0; i<nCols; ++i) // only the first nCols elements are required
				for (size_t j=0; j<nRows; ++j) y[i] += U[j+i*nRows]*observations[j];
			for (size_t i=0; i<nCols; ++i) y[i] /= S[i];
			for (size_t i=0; i<nCols; ++i)
				for (size_t j=0; j<nCols; ++j) beta_[i] += VT[j+i*nCols]*y[j];

			// finished
			delete A;
			delete U;
			delete S;
			delete VT;

		}

	public:

		Regression ( const std::vector< std::vector<Type> >& controls,
			         const std::vector< Type >&              observations,
					 const size_t                            maxDegree ) : maxDegree_(maxDegree) {
			// check dimensions
			size_t nRows = 0;
			if (controls.size()==observations.size()) nRows = controls.size();

			if (nRows>0) setUpMultiIndex(controls[0].size(),maxDegree_);
			size_t nCols = multIdx_.size();

			// initialise beta
			beta_.resize(nCols,0.0);
			if ((nRows>0)&&(nRows>=nCols))
				calculateRegression(controls,observations);  // if nRows < nCols regression does not really make sense

		}

        const std::vector<Type> monomials( const std::vector<Type>& x ) const {
			std::vector<Type> y(multIdx_.size(),0.0);
			if ((multIdx_.size()==0)||(multIdx_[0].size()!=x.size())) return y;  // dimension mismatch
            for (size_t i=0; i<y.size(); ++i) {
				y[i] = 1.0;
				for (size_t j=0; j<x.size(); ++j) {  // don't want to use pow coz not clear how it's implemented
					for (size_t k=0; k<multIdx_[i][j]; ++k) y[i] *= x[j];
				}
			}
			return y;
		}

		const Type value( const std::vector<Type>& x ) const {
			std::vector<Type> y = monomials(x);
			if (y.size()!=beta_.size()) return 0.0;  // dimension mismatch
			Type res = 0.0;
			for (size_t i=0; i<y.size(); ++i) res += beta_[i] * y[i];
			return res;
		}

		// inspectors

		const size_t                              maxDegree() const { return maxDegree_; }
		const std::vector< std::vector<size_t> >& multIdx()   const { return multIdx_;   }
		const std::vector<Type>&                  beta()      const { return beta_;      }

		const std::vector< std::vector<Type> >    multiIndex() const {  // workaround for Excel interface debugging
			std::vector< std::vector<Type> > M(multIdx_.size());
			for (size_t i=0; i<multIdx_.size(); ++i) {
				M[i].resize(multIdx_[i].size());
				for (size_t j=0; j<multIdx_[i].size(); ++j) M[i][j] = multIdx_[i][j];
			}
			return M;
		}

	};
	
}

#endif  /* ifndef quantlib_templateauxilliaries_regression_hpp */
