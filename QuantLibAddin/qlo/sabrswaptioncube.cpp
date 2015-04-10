/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2010 Sebastian Schlenkrich

*/

#include <qlo/sabrswaptioncube.hpp>

namespace QuantLibAddin {

    SabrSwaptionCube::SabrSwaptionCube (
                           const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                           const std::vector<QuantLib::Time>&                   optionTimes,
                           const std::vector<QuantLib::Time>&                   swapTimes,
                           const std::vector< std::vector< QuantLib::Real > >&  alpha,
                           const std::vector< std::vector< QuantLib::Real > >&  beta,
                           const std::vector< std::vector< QuantLib::Real > >&  rho,
                           const std::vector< std::vector< QuantLib::Real > >&  nu,
                           const std::vector< std::vector< QuantLib::Real > >&  fwd,
                           QuantLib::BusinessDayConvention                      bdc,
                           const QuantLib::DayCounter&                          dc,
						   const bool                                           useNormalVols,
                           bool permanent )
    : SwaptionVolatilityStructure(properties, permanent) {                
        libraryObject_ = boost::shared_ptr<QuantLib::SwaptionVolatilityStructure>(new
            QuantLib::SabrSwaptionCube(optionTimes,swapTimes,alpha,beta,rho,nu,fwd,bdc,dc,useNormalVols));
    }

	SABRCapletSurface::SABRCapletSurface(
		                   const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
			               const boost::shared_ptr<QuantLib::SwaptionVolatilityStructure> cube,
					       const QuantLib::Real                                           swapTerm,
                           bool permanent ) : OptionletVolatilityStructure(properties, permanent) { 
        libraryObject_ = boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(new QuantLib::SABRCapletSurface( cube, swapTerm ));
	}



}