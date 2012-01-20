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


#ifndef WCITRANSACTORS_H_
#define WCITRANSACTORS_H_

#include <pqxx/transactor>
#include <pqxx/result>
#include <pqxx/largeobject>
#include <iostream>
#include <string>
#include <vector>

namespace wdb { namespace load {

    class WciBegin : public pqxx::transactor<>
    {
    public:
        WciBegin() : pqxx::transactor<>("WciBegin")
        {
            // NOOP
        }

        void operator()(argument_type &T)
        {
            std::stringstream query;
            query << "select wci.begin('proffread')";
            pqxx::result R;
            R = T.exec(query.str());
        }

        void on_abort(const char Reason[]) throw ()
        {
            std::cerr<<__FUNCTION__<<__LINE__ <<  " pqxx::transactor " << Name() << " failed: " << Reason << std::endl;
        }

        void on_commit()
        {
            std::cerr<<__FUNCTION__<<__LINE__ <<  " pqxx::transactor " << Name() << " successful" << std::endl;
        }

        void on_doubt() throw ()
        {
            std::cerr<<__FUNCTION__<<__LINE__<<  " pqxx::transactor " << Name() << " in indeterminate state!" << std::endl;
        }
    };


    class WciEnd : public pqxx::transactor<>
    {
    public:
        WciEnd( ) : pqxx::transactor<>("WciEnd")
        {
            // NOOP
        }

        void operator()(argument_type &T)
        {
            std::stringstream query;
            query << "select wci.end ( )";
            pqxx::result R;
            R = T.exec(query.str());
        }

        void on_abort(const char Reason[]) throw ()
        {
            std::cerr <<  "pqxx::transactor " << Name() << " failed: " << Reason << std::endl;
        }

        void on_commit()
        {
            std::cerr <<  "pqxx::transactor " << Name() << " successful" << std::endl;
        }

        void on_doubt() throw ()
        {
            std::cerr <<  "pqxx::transactor " << Name() << " in indeterminate state!";
        }
    };

} }

#endif /*WCITRANSACTORS_H_*/
