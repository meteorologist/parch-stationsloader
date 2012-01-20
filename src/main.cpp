/*
 wdb

 Copyright (C) 2007-2009 met.no

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

#include "WDBDatabaseConnection.h"
#include "STInfosysDatabaseConnection.h"

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

int main(int argc, char ** argv)
{
    std::string wdbConnectionString = "host=proffdb-test dbname=wdb user=proffread";
    std::string stinfosysConnectionString = "host=stinfosys dbname=stinfosys user=pstinfosys port=5435 password=info12";

    try {

        std::map<std::string, wdb::load::WDBStationRecord> wdb_stations;
        wdb::load::WDBDatabaseConnection wdb(wdbConnectionString);
        wdb.getAllStations(wdb_stations);

        std::map<std::string, wdb::load::STIStationRecord> sti_stations;
        wdb::load::STInfosysDatabaseConnection stinfosys(stinfosysConnectionString);
        stinfosys.getAllStations(sti_stations);

        wdb.updateStations(sti_stations);
    } catch ( pqxx::sql_error & e ) {
        // Handle sql specific errors, such as connection problems, here.
        std::clog << e.what() << std::endl;
        return 1;
    } catch ( std::exception & e ) {
        std::clog << e.what() << std::endl;
        return 1;
    }
}

