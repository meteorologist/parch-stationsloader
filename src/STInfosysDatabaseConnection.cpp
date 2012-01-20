/*
 wdb

 Copyright (C) 2008 met.no

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

#include "STInfosysDatabaseConnection.h"

#include <pqxx/pqxx>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;

namespace wdb { namespace load {

    /*
     * configuration - ATM is just connection string
     */
    STInfosysDatabaseConnection::STInfosysDatabaseConnection(const std::string& configuration)
        : pqxx::connection(configuration)
    {
        setup_();
    }

    STInfosysDatabaseConnection::~STInfosysDatabaseConnection()
    {
//        unprepare("GetAllStations");
    }

    void STInfosysDatabaseConnection::setup_()
    {
            //
//            prepare("GetAllStations",
//                    "SELECT stationid, fromtime, edited_at FROM station WHERE edited_at = (SELECT MAX(edited_at) FROM station st WHERE station.stationid = st.stationid) order by station.stationid");

    }

    void STInfosysDatabaseConnection::getAllStations(std::map<std::string, STIStationRecord>& result)
    {
//        std::map<int, STIRecord> map_by_wmo;
//        std::map<std::string, STIRecord> map_by_name;

        // Create a transaction.
        pqxx::work transaction(*this);
        // This is the read query
        std::string query =
                          " SELECT st1.stationid, st1.name, st1.lon, st1.lat, st1.wmono FROM station st1 "
                          " INNER JOIN "
                          " (SELECT stationid, MAX(edited_at) AS last_update_time FROM station GROUP BY stationid) stfiltered "
                          " ON (st1.stationid = stfiltered.stationid AND st1.edited_at = stfiltered.last_update_time) "
                          " WHERE (st1.lon IS NOT NULL AND st1.lat IS NOT NULL AND st1.name IS NOT NULL AND st1.wmono IS NOT NULL AND st1.totime IS NULL) ORDER BY st1.name; ";

        pqxx::result rows = transaction.exec(query);
        size_t rCount = rows.size();
//        size_t cCount = rows.columns();
        for(size_t r = 0; r < rCount; ++r) {
            STIStationRecord rec;
            rec.id_   = rows[r][0].as<std::string>();
            rec.name_ = rows[r][1].as<std::string>();
            rec.lon_  = rows[r][2].as<float>();
            rec.lat_  = rows[r][3].as<float>();
            rec.wmo_   = rows[r][4].as<int>();

            if(result.count(rec.id_) != 0)
                std::cout << "already have entry with STATIONID: " << rec.id_<<std::endl;
//            if(map_by_wmo.count(rec.wmo_) != 0)
//                std::cout << "already have entry with WMONO: " << rec.wmo_<<std::endl;
//            if(map_by_name.count(rec.name_) != 0)
//                std::cout << "already have entry with name: " << rec.name_<<" [stationid="<<rec.id_<<"]"<<" [wmono="<<rec.wmo_<<"]"<<std::endl;

            result.insert(std::make_pair<std::string, STIStationRecord>(rec.id_, rec));
//            map_by_name.insert(std::make_pair<std::string, STIRecord>(rec.name_, rec));
//            map_by_wmo.insert(std::make_pair<int, STIRecord>(rec.wmo_, rec));
        }

        std::cout << "result size: "<< rCount << std::endl;
    }

} } /* end namespaces */
