/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Roland Lichters

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

#include <ql/indexes/bmaindex.hpp>
#include <ql/currencies/america.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/daycounters/actualactual.hpp>

namespace QuantLib {

    namespace {

        Date previousWednesday(const Date& date) {
            Weekday w = date.weekday();
            if (w >= 4) // roll back w-4 days
                return date - (w - 4) * Days;
            else // roll forward 4-w days and back one week
                return date + (4 - w - 7) * Days;
        }

    }


    BMAIndex::BMAIndex(const Handle<YieldTermStructure>& h)
    : InterestRateIndex("BMA",
                        1 * Weeks,
                        1,
                        USDCurrency(),
                        UnitedStates(UnitedStates::NYSE),
                        ActualActual(ActualActual::ISMA)),
      termStructure_(h) {
        registerWith (h);
    }

    std::string BMAIndex::name() const {
        return "BMA";
    }

    bool BMAIndex::isValidFixingDate(const Date& fixingDate) const {
        // either the fixing date is last Wednesday, or all days
        // between last Wednesday included and the fixing date are
        // holidays
        for (Date d = previousWednesday(fixingDate); d < fixingDate; ++d) {
            if (fixingCalendar_.isBusinessDay(d))
                return false;
        }
        // also, the fixing date itself must be a business day
        return fixingCalendar_.isBusinessDay(fixingDate);
    }

    Handle<YieldTermStructure> BMAIndex::termStructure() const {
        return termStructure_;
    }

    Date BMAIndex::maturityDate(const Date& valueDate) const {
        Date fixingDate = fixingCalendar_.advance(valueDate,-1,Days);
        Date nextWednesday = previousWednesday(fixingDate+7);
        return fixingCalendar_.advance(nextWednesday,1,Days);
    }

    Schedule BMAIndex::fixingSchedule(const Date& start, const Date& end) {
        return MakeSchedule(previousWednesday(start),
                            previousWednesday(end),
                            1 * Weeks,
                            fixingCalendar_, Following).forwards();
    }

    Rate BMAIndex::forecastFixing(const Date& fixingDate) const {
        QL_REQUIRE(!termStructure_.empty(),
                   "no forecasting term structure set to " << name());
        Date start = fixingCalendar_.advance(fixingDate,1,Days);
        Date end = maturityDate(start);
        return termStructure_->forwardRate(start, end,
                                           dayCounter_,
                                           Simple);
    }

}
