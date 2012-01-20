/*
 wdb

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

#ifndef STINFOSYSDATABASECONNECTION_H_
#define STINFOSYSDATABASECONNECTION_H_

#include <pqxx/connection>
#include <string>
#include <limits>

namespace wdb { namespace load {

    struct STIStationRecord
    {
        std::string id_;
        std::string name_;
        float       lon_;
        float       lat_;
        int         wmo_;
    };
    class STInfosysDatabaseConnection : public pqxx::connection
    {
    public:
        explicit STInfosysDatabaseConnection(const std::string& configuration);
        virtual ~STInfosysDatabaseConnection();

        void getAllStations(std::map<std::string, STIStationRecord>& result);

    private:
        void setup_();
    };

} } /* namespaces */

#endif /* STINFOSYSDATABASECONNECTION */
