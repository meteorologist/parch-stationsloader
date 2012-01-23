/*
    wdb - weather and water data storage

    Copyright (C) 2012 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


#ifndef UPDATEPLACEPOINT_H_
#define UPDATEPLACEPOINT_H_

#include "addPlacePoint.h"

// WDB
//
#include <wdbException.h>
#include <wdbLogHandler.h>

// PQXX
//
#include <pqxx/transactor>
#include <pqxx/result>

// STD
#include <iostream>
#include <string>
#include <sstream>

namespace wdb { namespace load {

    /**
     * Transactor to create a user
     */
    class UpdatePlacePoint : public AddPlacePoint
    {
    public:
        /**
         * Default Constructor
         * @param 	placeName	placeName of the new/existing point
         * @param 	geometry	place geometry as WKT
         */
        UpdatePlacePoint(const std::string& placename, const std::string& geometry)
            : AddPlacePoint(placename, geometry, "UpdatePlacePoint")
        {
            // NOOP
        }
    };

} } /* end namespaces */


#endif /*SETPLACEPOINT_H_*/
