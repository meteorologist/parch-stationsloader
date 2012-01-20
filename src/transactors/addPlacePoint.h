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


#ifndef ADDPLACEPOINT_H_
#define ADDPLACEPOINT_H_

// PROJECT INCLUDES
//

// SYSTEM INCLUDES
#include <pqxx/transactor>
#include <pqxx/result>
#include <iostream>
#include <string>
#include <sstream>

// FORWARD REFERENCES
//

namespace wdb { namespace load {

    /**
     * Transactor to create a user
     */
    class AddPlacePoint : public pqxx::transactor<>
    {
    public:
        /**
         * Default Constructor
         * @param 	placeName	placeName of the new/existing point
         * @param 	geometry	place geometry as WKT
         */
        AddPlacePoint(const std::string& placename, const std::string& geometry) :
            pqxx::transactor<>("AddPlacePoint"),
            placeName_(placename),
            geometry_(geometry)
        {
            // NOOP
        }

        /**
         * Functor. The transactors functor executes the query.
         */
        void operator()(argument_type &T)
        {
            std::ostringstream query;
            query << "SELECT wci.addPlacePoint"<<"("<<"\'"<< placeName_ <<"\'"<< "," << "ST_GeomFromText("<< "\'"<< geometry_<<"\'"<<", 4030 ) )";
            R = T.exec(query.str());
        }

        /**
         * Commit handler. This is called if the transaction succeeds.
         */
        void on_commit()
        {
            // NOOP
        }

        /**
         * Abort handler. This is called if the transaction fails.
         * @param	Reason	The reason for the abort of the call
         */
        void on_abort(const char Reason[]) throw ()
        {
            // NOOP
        }

        /**
         * Special error handler. This is called if the transaction fails with an unexpected error.
         * Read the libpqxx documentation on transactors for details.
         */
        void on_doubt() throw ()
        {
            // NOOP
        }

    private:
        /// The result returned by the query
        pqxx::result R;
        /// Place Name
        std::string placeName_;
        /// Place Geometry - WKT
        std::string geometry_;
    };

} } /* end namespaces */


#endif /*ADDPLACEPOINT_H_*/
