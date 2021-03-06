/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005, 2006 Eric Ehlers

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef qla_barrieroption_hpp
#define qla_barrieroption_hpp

#include <qlo/baseinstruments.hpp>

#include <ql/instruments/barriertype.hpp>
#include <ql/experimental/barrieroption/doublebarriertype.hpp>
#include <ql/types.hpp>

namespace QuantLib {
    class StrikedTypePayoff;
    class Exercise;
}   

namespace QuantLibAddin {

    class BarrierOption : public OneAssetOption {
    public:
        BarrierOption(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            QuantLib::Barrier::Type barrierType,
            QuantLib::Real barrier,
            QuantLib::Real rebate,
            const boost::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
            const boost::shared_ptr<QuantLib::Exercise>& exercise,
            bool permanent);
    };

    class DoubleBarrierOption : public OneAssetOption {
    public:
        DoubleBarrierOption(
            const boost::shared_ptr<ObjectHandler::ValueObject>&  properties,
            QuantLib::DoubleBarrier::Type                         barrierType,
            QuantLib::Real                                        barrier_lo,
            QuantLib::Real                                        barrier_hi,
            QuantLib::Real                                        rebate,
            const boost::shared_ptr<QuantLib::StrikedTypePayoff>& payoff,
            const boost::shared_ptr<QuantLib::Exercise>&          exercise,
            bool                                                  permanent);
    };

}

#endif
