/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Sebastian Schlenkrich

 */


#include <qlo/templatehestonmodel.hpp>

#include <boost/algorithm/string.hpp>

#include <ql/models/marketmodels/browniangenerators/mtbrowniangenerator.hpp>

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

	void HestonModel::calibrate(const std::vector<boost::shared_ptr<QuantLib::CalibrationHelper> >& instruments,
		const boost::shared_ptr<QuantLib::OptimizationMethod>&              method,
		const boost::shared_ptr<QuantLib::EndCriteria>&                     endCriteria,
		const std::vector<QuantLib::Real>&                                  weights,
		const std::vector<bool>&                                            fixParameters,
		const QuantLib::Real                                                hestonRelTolerance,
	    const QuantLib::Size                                                hestonMaxEvaluations ) {
		boost::shared_ptr<QuantLib::HestonModel> model = boost::dynamic_pointer_cast<QuantLib::HestonModel>(libraryObject_);
		for (QuantLib::Size k = 0; k<instruments.size(); ++k) {
			instruments[k]->setPricingEngine(boost::shared_ptr<QuantLib::PricingEngine>(new QuantLib::AnalyticHestonEngine(model, hestonRelTolerance, hestonMaxEvaluations)));
		}
		model->calibrate(instruments, *method, *endCriteria, QuantLib::NoConstraint(), weights, fixParameters);
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

	HestonModelHelper::HestonModelHelper(
		const boost::shared_ptr<ObjectHandler::ValueObject>&    properties,
		const QuantLib::Period&                                 maturity,
		const QuantLib::Calendar&                               calendar,
		const QuantLib::Handle<QuantLib::Quote>&                s0,
		const QuantLib::Real                                    strikePrice,
		const QuantLib::Handle<QuantLib::Quote>&                volatility,
		const QuantLib::Handle<QuantLib::YieldTermStructure>&   riskFreeRate,
		const QuantLib::Handle<QuantLib::YieldTermStructure>&   dividendYield,
		const QuantLib::CalibrationHelper::CalibrationErrorType& errorType,
		bool                                                    permanent)
		: CalibrationHelper(properties, permanent) {
		libraryObject_ = boost::shared_ptr<QuantLib::CalibrationHelper>(
			new QuantLib::HestonModelHelper(maturity, calendar, s0, strikePrice, volatility, riskFreeRate, dividendYield, errorType));
	}

	HestonBlackVolSurface::HestonBlackVolSurface(
		const boost::shared_ptr<ObjectHandler::ValueObject>&    properties,
		const QuantLib::Handle<QuantLib::HestonModel>&          model,
		bool                                                    permanent)
		: BlackVolTermStructure(properties, permanent) {
		libraryObject_ = boost::shared_ptr<QuantLib::BlackVolTermStructure>(new QuantLib::HestonBlackVolSurface(model));
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

	HestonSLVFokkerPlanckFdmParams::HestonSLVFokkerPlanckFdmParams(
		const boost::shared_ptr<ObjectHandler::ValueObject>&  properties,
		const QuantLib::Size                                  xGrid,
		const QuantLib::Size                                  vGrid,
		const QuantLib::Size                                  tMaxStepsPerYear,
		const QuantLib::Size                                  tMinStepsPerYear,
		const QuantLib::Real                                  tStepNumberDecay,
		const QuantLib::Size                                  predictionCorretionSteps,
		// local volatility forward equation
		const QuantLib::Real                                  x0Density,
		const QuantLib::Real                                  localVolEpsProb,
		const QuantLib::Size                                  maxIntegrationIterations,
		// variance mesher definition
		const QuantLib::Real                                  vLowerEps,
		const QuantLib::Real                                  vUpperEps,
		const QuantLib::Real                                  vMin,
		const QuantLib::Real                                  v0Density,
		const QuantLib::Real                                  vLowerBoundDensity,
		const QuantLib::Real                                  vUpperBoundDensity,
		// do not calculate leverage function if prob is smaller than eps
		const QuantLib::Real                                  leverageFctPropEps,
		// algorithm to get to the start configuration at time point one
		const std::string                                     greensAlgorithmString,
		const std::string                                     trafoTypeString,
		// define finite difference scheme
		const std::string                                     schemeDescString,
		bool                                                  permanent)
		: ObjectHandler::LibraryObject<QuantLib::HestonSLVFokkerPlanckFdmParams>(properties, permanent) {
		// parse string arguments
		std::string desc;
		desc = greensAlgorithmString;
		boost::to_upper(desc);
		QuantLib::FdmHestonGreensFct::Algorithm greensAlgorithm(QuantLib::FdmHestonGreensFct::Gaussian);
		if (desc == "GAUSSIAN")        greensAlgorithm = QuantLib::FdmHestonGreensFct::Gaussian;
		if (desc == "ZEROCORRELATION") greensAlgorithm = QuantLib::FdmHestonGreensFct::ZeroCorrelation;
		if (desc == "SEMIANALYTICAL")  greensAlgorithm = QuantLib::FdmHestonGreensFct::SemiAnalytical;
		desc = trafoTypeString;
		boost::to_upper(desc);
		QuantLib::FdmSquareRootFwdOp::TransformationType trafoType(QuantLib::FdmSquareRootFwdOp::Plain);
		if (desc == "PLAIN")  trafoType = QuantLib::FdmSquareRootFwdOp::Plain;
		if (desc == "POWER")  trafoType = QuantLib::FdmSquareRootFwdOp::Power;
		if (desc == "LOG")    trafoType = QuantLib::FdmSquareRootFwdOp::Log;
		desc = schemeDescString;
		boost::to_upper(desc);
		QuantLib::FdmSchemeDesc *schemeDesc(&QuantLib::FdmSchemeDesc::ModifiedCraigSneyd());
		if (desc == "DOUGLAS")              schemeDesc = &(QuantLib::FdmSchemeDesc::Douglas());
		if (desc == "IMPLICITEULER")        schemeDesc = &(QuantLib::FdmSchemeDesc::ImplicitEuler());
		if (desc == "EXPLICITEULER")        schemeDesc = &(QuantLib::FdmSchemeDesc::ExplicitEuler());
		if (desc == "CRAIGSNEYDL")          schemeDesc = &(QuantLib::FdmSchemeDesc::CraigSneyd());
		if (desc == "MODIFIEDCRAIGSNEYDL")  schemeDesc = &(QuantLib::FdmSchemeDesc::ModifiedCraigSneyd());
		if (desc == "HUNDSDORFER")          schemeDesc = &(QuantLib::FdmSchemeDesc::Hundsdorfer());
		if (desc == "MODIFIEDHUNDSDORFER")  schemeDesc = &(QuantLib::FdmSchemeDesc::ModifiedHundsdorfer());
		QuantLib::HestonSLVFokkerPlanckFdmParams params = {
			xGrid, vGrid,  tMaxStepsPerYear, tMinStepsPerYear, tStepNumberDecay, predictionCorretionSteps,
			x0Density, localVolEpsProb, maxIntegrationIterations, vLowerEps, vUpperEps,
			vMin, v0Density, vLowerBoundDensity, vUpperBoundDensity, leverageFctPropEps,
			greensAlgorithm, trafoType, *schemeDesc
		};
		libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>(
		    new QuantLib::HestonSLVFokkerPlanckFdmParams(params));
	}

	HestonSLVFokkerPlanckFdmParams::HestonSLVFokkerPlanckFdmParams(
		const boost::shared_ptr<ObjectHandler::ValueObject>&  properties,
		const std::string                                     setID,
		bool                                                  permanent)
		: ObjectHandler::LibraryObject<QuantLib::HestonSLVFokkerPlanckFdmParams>(properties, permanent) {
		// some pre-defined sets from unit tests
		const QuantLib::HestonSLVFokkerPlanckFdmParams plainParams =
		{ 201, 301, 1000, 25, 3.0, 2,
			0.1, 1e-4, 10000,
			1e-8, 1e-8, 0.0, 1.0, 1.0, 1.0, 1e-6,
			QuantLib::FdmHestonGreensFct::Gaussian,
			QuantLib::FdmSquareRootFwdOp::Plain,
			QuantLib::FdmSchemeDesc::ModifiedCraigSneyd()
		};
		const QuantLib::HestonSLVFokkerPlanckFdmParams logParams =
		{ 301, 601, 2000, 30, 2.0, 2,
			0.1, 1e-4, 10000,
			1e-5, 1e-5, 0.0000025, 1.0, 0.1, 0.9, 1e-5,
			QuantLib::FdmHestonGreensFct::Gaussian,
			QuantLib::FdmSquareRootFwdOp::Log,
			QuantLib::FdmSchemeDesc::ModifiedCraigSneyd()
		};
		const QuantLib::HestonSLVFokkerPlanckFdmParams powerParams =
		{ 401, 801, 2000, 30, 2.0, 2,
			0.1, 1e-3, 10000,
			1e-6, 1e-6, 0.001, 1.0, 0.001, 1.0, 1e-5,
			QuantLib::FdmHestonGreensFct::Gaussian,
			QuantLib::FdmSquareRootFwdOp::Power,
			QuantLib::FdmSchemeDesc::ModifiedCraigSneyd()
		};
		std::string setString = setID;
		boost::to_upper(setString);
		if (setString == "PLAIN") libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>(new QuantLib::HestonSLVFokkerPlanckFdmParams(plainParams));
		if (setString == "LOG") libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>(new QuantLib::HestonSLVFokkerPlanckFdmParams(logParams));
		if (setString == "POWER") libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>(new QuantLib::HestonSLVFokkerPlanckFdmParams(powerParams));
	}

	HestonSLVFDMModel::HestonSLVFDMModel(
		const boost::shared_ptr<ObjectHandler::ValueObject>&                properties,
		const QuantLib::Handle<QuantLib::LocalVolTermStructure>&           localVol,
		const QuantLib::Handle<QuantLib::HestonModel>&                     hestonModel,
		const QuantLib::Date&                                               endDate,
		const boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>&  params,
		const bool                                                          logging,
		const std::vector<QuantLib::Date>&                                  mandatoryDates,
		bool                                                                permanent)
		: ObjectHandler::LibraryObject<QuantLib::HestonSLVFDMModel>(properties, permanent) {
		libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVFDMModel>(
			new QuantLib::HestonSLVFDMModel(localVol, hestonModel, endDate, *params, logging, mandatoryDates));
	}

	HestonSLVMCModel::HestonSLVMCModel(
		const boost::shared_ptr<ObjectHandler::ValueObject>&                properties,
		const QuantLib::Handle<QuantLib::LocalVolTermStructure>&           localVol,
		const QuantLib::Handle<QuantLib::HestonModel>&                     hestonModel,
		const QuantLib::Date&												endDate,
		QuantLib::Size														timeStepsPerYear,
		QuantLib::Size														nBins,
		QuantLib::Size														calibrationPaths,
		const std::vector<QuantLib::Date>&									mandatoryDates,
		bool                                                                permanent)
		: ObjectHandler::LibraryObject<QuantLib::HestonSLVMCModel>(properties, permanent) {
		libraryObject_ = boost::shared_ptr<QuantLib::HestonSLVMCModel>(
			new QuantLib::HestonSLVMCModel(localVol, hestonModel, boost::shared_ptr<QuantLib::BrownianGeneratorFactory>(
				new QuantLib::MTBrownianGeneratorFactory()), endDate, timeStepsPerYear, nBins, calibrationPaths, mandatoryDates));
	}


	MultiAssetBSModel::MultiAssetBSModel(
		const boost::shared_ptr<ObjectHandler::ValueObject>&                            properties,
		const QuantLib::Handle<QuantLib::YieldTermStructure>&                           termStructure,
		const std::vector<std::string>&                                                 aliases,
		const std::vector<boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>>& processes,
		const QuantLib::RealStochasticProcess::MatA&                                    correlations,
		bool                                                                            permanent)
		: RealStochasticProcess(properties, permanent) {
		libraryObject_ = boost::shared_ptr<QuantLib::RealStochasticProcess>(
			new QuantLib::MultiAssetBSModel(termStructure, aliases, processes, correlations));
	}

}



