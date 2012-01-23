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

#include "STLoaderConfiguration.h"
#include "WDBDatabaseConnection.h"
#include "STInfosysDatabaseConnection.h"

// WDB
//
#include <wdbLogHandler.h>

// PQXX
//
#include <pqxx/pqxx>


// BOOST
//
#include <boost/filesystem/path.hpp>

// STD
//
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace wdb::load;

// Support Functions
namespace
{

/**
 * Write the program version to stream
 * @param	out		Stream to write to
 */
void version( ostream & out )
{
//    out << PACKAGE_STRING << endl;
}

/**
 * Write help information to stram
 * @param	options		Description of the program options
 * @param	out			Stream to write to
 */
void help( const boost::program_options::options_description & options, ostream & out )
{
    version( out );
    out << '\n';
//    out << "Usage: "PACKAGE_NAME" [OPTIONS] FILES...\n\n";
    out << "Options:\n";
    out << options << endl;
}
} // namespace


int main(int argc, char ** argv)
{
//    std::string wdbConnectionString = "host=proffdb-test dbname=wdb user=proffread";
//    std::string stinfosysConnectionString = "host=stinfosys dbname=stinfosys user=pstinfosys port=5435 password=info12";

    STLoaderConfiguration config;
    try {
        config.parse(argc, argv);
        if(config.general().help) {
            help(config.shownOptions(), cerr);
            return 0;
        }
        if(config.general().version) {
            version(cerr);
            return 0;
        }
    } catch(exception & e) {
        cerr << e.what() << endl;
        help(config.shownOptions(), clog);
        return 1;
    }

   try {

        map<string, WDBStationRecord> wdb_stations;
        WDBDatabaseConnection wdb(config);
        wdb.getAllStations(wdb_stations);

        map<string, STIStationRecord> sti_stations;
        STInfosysDatabaseConnection stinfosys(config);
        stinfosys.getAllStations(sti_stations);

        wdb.updateStations(sti_stations);
    } catch (pqxx::sql_error & e) {
        // Handle sql specific errors, such as connection problems, here.
        clog << e.what() << endl;
        return 1;
    } catch (exception & e) {
        clog << e.what() << endl;
        return 1;
    }
}

