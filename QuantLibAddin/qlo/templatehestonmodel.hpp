/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Sebastian Schlenkrich

 */

#ifndef qla_hestonmodel_hpp
#define qla_hestonmodel_hpp

#include <ql/types.hpp>

#include <qlo/baseinstruments.hpp>
#include <qlo/vanillaswap.hpp>
#include <qlo/termstructures.hpp>
#include <qlo/pricingengines.hpp>
#include <qlo/indexes/iborindex.hpp>

#include <ql/processes/hestonprocess.hpp>
#include <ql/models/equity/hestonmodel.hpp>
#include <ql/pricingengines/vanilla/analytichestonengine.hpp>
#include <ql/pricingengines/vanilla/analytichestonengine.hpp>
#include <ql/experimental/templatemodels/stochvol/stochvolmodels.hpp>
#include <ql/experimental/templatemodels/stochvol/stochvolcalibrator.hpp>

#include <ql/experimental/models/hestonslvfdmmodel.hpp>
#include <ql/experimental/models/hestonslvmcmodel.hpp>

#include <ql/models/equity/hestonmodelhelper.hpp>
#include <ql/termstructures/volatility/equityfx/hestonblackvolsurface.hpp>

#include <ql/experimental/templatemodels/multiasset/multiassetbsmodel.hpp>


// #include <qlo/templatequasigaussian.hpp>


namespace QuantLib {
    template <class T>
    class Handle;
	class StochasticProcess;
}


namespace QuantLibAddin {

    OH_LIB_CLASS(StochasticProcess, QuantLib::StochasticProcess);

    class HestonProcess : public StochasticProcess {
    public:
        //HestonProcess(const boost::shared_ptr<ObjectHandler::ValueObject>& properties, bool permanent) : StochasticProcess(properties,permanent) {}
        HestonProcess(
                      const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                      const QuantLib::Handle<QuantLib::YieldTermStructure>& riskFreeRate,
                      const QuantLib::Handle<QuantLib::YieldTermStructure>& dividendYield,
                      const QuantLib::Handle<QuantLib::Quote>& s0,
                      QuantLib::Real v0, 
                      QuantLib::Real kappa,
                      QuantLib::Real theta, 
                      QuantLib::Real sigma, 
                      QuantLib::Real rho,
                      bool permanent);
    };

    OH_LIB_CLASS(CalibratedModel, QuantLib::CalibratedModel);

    class HestonModel : public CalibratedModel {
    public:
        HestonModel(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const boost::shared_ptr<QuantLib::HestonProcess>& process,
            bool permanent);
		void calibrate(const std::vector<boost::shared_ptr<QuantLib::CalibrationHelper> >& instruments,
			const boost::shared_ptr<QuantLib::OptimizationMethod>&              method,
			const boost::shared_ptr<QuantLib::EndCriteria>&                     endCriteria,
			const std::vector<QuantLib::Real>&                                  weights,
			const std::vector<bool>&                                            fixParameters,
			const QuantLib::Real                                                hestonRelTolerance,
			const QuantLib::Size                                                hestonMaxEvaluations );
    };

    class AnalyticHestonEngine : public PricingEngine {
    public:
        AnalyticHestonEngine(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                const boost::shared_ptr<QuantLib::HestonModel>& model,
                QuantLib::Real relTolerance, 
                QuantLib::Size maxEvaluations,
                bool permanent);
    };

	class HestonModelHelper : public CalibrationHelper {
	public:
		HestonModelHelper(
			const boost::shared_ptr<ObjectHandler::ValueObject>&    properties,
			const QuantLib::Period&                                 maturity,
			const QuantLib::Calendar&                               calendar,
			const QuantLib::Handle<QuantLib::Quote>&                s0,
			const QuantLib::Real                                    strikePrice,
			const QuantLib::Handle<QuantLib::Quote>&                volatility,
			const QuantLib::Handle<QuantLib::YieldTermStructure>&   riskFreeRate,
			const QuantLib::Handle<QuantLib::YieldTermStructure>&   dividendYield,
			const QuantLib::CalibrationHelper::CalibrationErrorType& errorType,
			bool                                                    permanent);
	};

	class HestonBlackVolSurface : public BlackVolTermStructure {
	public:
		HestonBlackVolSurface(
			const boost::shared_ptr<ObjectHandler::ValueObject>&    properties,
			const QuantLib::Handle<QuantLib::HestonModel>&          model,
			bool                                                    permanent);
	};

	class RealHestonModel : public ObjectHandler::LibraryObject<QuantLib::RealHestonModel> {
    public:
        RealHestonModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real  kappa,                             
                QuantLib::Real  theta,
                QuantLib::Real  sigma,
                QuantLib::Real  rho,
                QuantLib::Real  v0,
                bool            permanent);
    };

    class ActiveHestonModel : public ObjectHandler::LibraryObject<QuantLib::ActiveHestonModel> {
    public:
        ActiveHestonModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real  kappa,                             
                QuantLib::Real  theta,
                QuantLib::Real  sigma,
                QuantLib::Real  rho,
                QuantLib::Real  v0,
                bool            permanent);
    };

	class RealStochVolModel : public ObjectHandler::LibraryObject<QuantLib::RealStochVolModel> {
    public:
        RealStochVolModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                QuantLib::Real  lambda,                             
                QuantLib::Real  b,
                QuantLib::Real  L,
                QuantLib::Real  theta,
                QuantLib::Real  m,
                QuantLib::Real  eta,
                QuantLib::Real  z0,
                QuantLib::Real  rho,
                bool            permanent);
    };

    OH_LIB_CLASS(RealStochasticProcess, QuantLib::RealStochasticProcess);
	OH_OBJ_CLASS(RealTDStochVolModel, RealStochasticProcess);
      
	class RealPWCStochVolModel : public RealTDStochVolModel {
	public:
		RealPWCStochVolModel(
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
  				bool                                                 permanent);			   
	};

	class RealShiftedSABRModel : public RealStochasticProcess {
	public:
		RealShiftedSABRModel(
                const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
				const QuantLib::Real                                 S0,
				const QuantLib::Real                                 lambda,
				const QuantLib::Real                                 alpha,
				const QuantLib::Real                                 beta,
				const QuantLib::Real                                 rho,
				const QuantLib::Real                                 nu,
  				bool                                                 permanent);			   
	};


	class StochVolModelCalibrator : public ObjectHandler::LibraryObject<QuantLib::StochVolModelCalibrator> {
	public:
		StochVolModelCalibrator(
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
  				bool                                                 permanent);
	};

	// Heston Local-Stochastic Volatility Model

	// this class captures deep-in-the-model parameters
	class HestonSLVFokkerPlanckFdmParams : public ObjectHandler::LibraryObject<QuantLib::HestonSLVFokkerPlanckFdmParams> {
	public:
		HestonSLVFokkerPlanckFdmParams(
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
			bool                                                  permanent);
		HestonSLVFokkerPlanckFdmParams(
			const boost::shared_ptr<ObjectHandler::ValueObject>&  properties,
			const std::string                                     setID,
			bool                                                  permanent);
	};

	class HestonSLVFDMModel : public ObjectHandler::LibraryObject<QuantLib::HestonSLVFDMModel> {
	public:
		HestonSLVFDMModel(
			const boost::shared_ptr<ObjectHandler::ValueObject>&                properties,
			const QuantLib::Handle<QuantLib::LocalVolTermStructure>&           localVol,
			const QuantLib::Handle<QuantLib::HestonModel>&                     hestonModel,
			const QuantLib::Date&                                               endDate,
			const boost::shared_ptr<QuantLib::HestonSLVFokkerPlanckFdmParams>&  params,
			const bool                                                          logging,
			const std::vector<QuantLib::Date>&                                  mandatoryDates,
		    bool                                                                permanent);
	};

	class HestonSLVMCModel : public ObjectHandler::LibraryObject<QuantLib::HestonSLVMCModel> {
	public:
		HestonSLVMCModel(
			const boost::shared_ptr<ObjectHandler::ValueObject>&                properties,
			const QuantLib::Handle<QuantLib::LocalVolTermStructure>&			localVol,
			const QuantLib::Handle<QuantLib::HestonModel>&						hestonModel,
			const QuantLib::Date&												endDate,
			QuantLib::Size														timeStepsPerYear,
			QuantLib::Size														nBins,
			QuantLib::Size														calibrationPaths,
			const std::vector<QuantLib::Date>&									mandatoryDates,
			bool                                                                permanent);
	};

	class MultiAssetBSModel : public RealStochasticProcess {
	public:
		MultiAssetBSModel(
			const boost::shared_ptr<ObjectHandler::ValueObject>&                            properties,
			const QuantLib::Handle<QuantLib::YieldTermStructure>&                           termStructure,
			const std::vector<std::string>&                                                 aliases,
			const std::vector<boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess>>& processes,
			const QuantLib::RealStochasticProcess::MatA&                                    correlations,
			bool                                                                            permanent);
	};

}

#endif

