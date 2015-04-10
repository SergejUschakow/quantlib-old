/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Sebastian Schlenkrich

 */


#include <qlo/templatehestonmodel.hpp>

namespace QuantLibAddin {

    HestonProcess::HestonProcess(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& riskFreeRate,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& dividendYield,
            const QuantLib::Handle<QuantLib::Quote>& s0,
            QuantLib::Real v0, 
            QuantLib::Real kappa,
            QuantLib::Real theta, 
            QuantLib::Real sigma, 
            QuantLib::Real rho,
            bool permanent) : StochasticProcess(properties,permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::HestonProcess>(
			new QuantLib::HestonProcess( riskFreeRate, dividendYield, s0, v0, kappa, theta, sigma, rho ));            
    }

    HestonModel::HestonModel(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const boost::shared_ptr<QuantLib::HestonProcess>& process,
            bool permanent) : CalibratedModel(properties,permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::HestonModel>(
			new QuantLib::HestonModel( process ));            
    }

    AnalyticHestonEngine::AnalyticHestonEngine(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                const boost::shared_ptr<QuantLib::HestonModel>& model,
                QuantLib::Real relTolerance,
                QuantLib::Size maxEvaluations,
                bool permanent) : PricingEngine(properties,permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::AnalyticHestonEngine>(
			new QuantLib::AnalyticHestonEngine( model, relTolerance, maxEvaluations ));            
    }

    RealHestonModel::RealHestonModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real                                       kappa,                             
                QuantLib::Real                                       theta,
                QuantLib::Real                                       sigma,
                QuantLib::Real                                       rho,
                QuantLib::Real                                       v0,
                bool                                                 permanent)
        : ObjectHandler::LibraryObject<QuantLib::RealHestonModel>(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::RealHestonModel>(
			new QuantLib::RealHestonModel( kappa, theta, sigma, rho, v0 ));            
    }

    ActiveHestonModel::ActiveHestonModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real                                       kappa,                             
                QuantLib::Real                                       theta,
                QuantLib::Real                                       sigma,
                QuantLib::Real                                       rho,
                QuantLib::Real                                       v0,
                bool                                                 permanent)
        : ObjectHandler::LibraryObject<QuantLib::ActiveHestonModel>(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::ActiveHestonModel>(
			new QuantLib::ActiveHestonModel( kappa, theta, sigma, rho, v0 ));            
    }

	RealStochVolModel::RealStochVolModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real  lambda,                             
                QuantLib::Real  b,
                QuantLib::Real  L,
                QuantLib::Real  theta,
                QuantLib::Real  m,
                QuantLib::Real  eta,
                QuantLib::Real  z0,
                QuantLib::Real  rho,
                bool            permanent)
        : ObjectHandler::LibraryObject<QuantLib::RealStochVolModel>(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::RealStochVolModel>(
			new QuantLib::RealStochVolModel( lambda, b, L, theta, m, eta, z0, rho ));
    }


	RealPWCStochVolModel::RealPWCStochVolModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                const std::vector<QuantLib::Time>&                   times,
				const std::vector<QuantLib::Real>&                   lambda,
				const std::vector<QuantLib::Real>&                   b,
				const std::vector<QuantLib::Real>&                   eta,
				const QuantLib::Real                                 L,
				const QuantLib::Real                                 theta,
				const QuantLib::Real                                 m,
				const QuantLib::Real                                 z0,
				const QuantLib::Real                                 rho,
				const QuantLib::Real                                 S0,
				const QuantLib::RealStochasticProcess::VolEvolv      volEvolv,
  				bool                                                 permanent)
	: RealTDStochVolModel(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::RealPWCStochVolModel>(
			new QuantLib::RealPWCStochVolModel( times, lambda, b, eta, L, theta, m, z0, rho, S0, volEvolv ) );
	}

	RealShiftedSABRModel::RealShiftedSABRModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
				const QuantLib::Real                                 S0,
				const QuantLib::Real                                 lambda,
				const QuantLib::Real                                 alpha,
				const QuantLib::Real                                 beta,
				const QuantLib::Real                                 rho,
				const QuantLib::Real                                 nu,
  				bool                                                 permanent)
	: RealStochasticProcess(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::RealStochasticProcess>(
			new QuantLib::RealShiftedSABRModel( S0, lambda, alpha, beta, rho, nu ) );
	}



	StochVolModelCalibrator::StochVolModelCalibrator(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                // initial model parameters
		        const QuantLib::Real                                 lambda,
		        const QuantLib::Real                                 b,
		        const QuantLib::Real                                 L,
		        const QuantLib::Real                                 theta,
		        const QuantLib::Real                                 m,
		        const QuantLib::Real                                 eta,
		        const QuantLib::Real                                 z0,
		        const QuantLib::Real                                 rho,
		        // calibration flags
		        const bool                                           lambdaIsFixed,
		        const bool                                           bIsFixed,
		        const bool                                           LIsFixed,
		        const bool                                           thetaIsFixed,
		        const bool                                           mIsFixed,
		        const bool                                           etaIsFixed,
		        const bool                                           z0IsFixed,
		        const bool                                           rhoIsFixed,
		        // calibration targets
		        const QuantLib::Real                                 exercTime,
		        const QuantLib::Real                                 forward,
		        const std::vector<QuantLib::Real>&                   strikes,
		        const std::vector<QuantLib::Real>&                   vols,
				const std::vector<QuantLib::Real>&                   optimizationParams,  // { [min], [max], epsfcn, ftol, xtol, gtol, maxfev, glAbsAcc, glMaxEval }							 
  				bool                                                 permanent)
				: ObjectHandler::LibraryObject<QuantLib::StochVolModelCalibrator>(properties, permanent) {
        libraryObject_ = boost::shared_ptr<QuantLib::StochVolModelCalibrator>(
			new QuantLib::StochVolModelCalibrator(lambda,b,L,theta,m,eta,z0,rho,
			     lambdaIsFixed,bIsFixed,LIsFixed,thetaIsFixed,mIsFixed,etaIsFixed,
				 z0IsFixed,rhoIsFixed,exercTime,forward,strikes,vols,optimizationParams));
	}


}


