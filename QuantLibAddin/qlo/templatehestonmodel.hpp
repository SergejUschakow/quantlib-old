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
#include <ql/experimental/template/stochvol/hestonmodels.hpp>

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


}

#endif

