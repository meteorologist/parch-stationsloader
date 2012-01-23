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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "STLoaderConfiguration.h"

// BOOST
//
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;
using namespace wdb::load;

// Support Functions
namespace
{

/**
 * Define the input options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getInput(STLoaderConfiguration::InputOptions & out)
{
    options_description input( "Input" );
    input.add_options()
    ( "name", value( & out.file ), "Name of file to process" )
    ;

        return input;
}

/**
 * Define the output options for the Loader
 * @param	out		Reference to option structure
 */
options_description getOutput(STLoaderConfiguration::OutputOptions & out)
{
    options_description output( "Output" );
    output.add_options()
       ( "list,l", bool_switch( & out.list ), "List content of file instead of inserting into database" )
    ;

    return output;
}

/**
 * Define the general options for the Loader
 * @param	out		Reference to option structure
 */
options_description
getLoading( STLoaderConfiguration::STLoadingOptions & out, const std::string & defaultDataProvider )
{
    out.defaultDataProvider = defaultDataProvider;
    options_description loading("Loading");
    loading.add_options()
//    ( "loadPlaceDefinition", bool_switch( & out.loadPlaceDefinition )->default_value( false ), "Load place definition" )
//        ( "dataprovider", value( & out.dataProvider ), "Override WCI Data Provider Name decoded from file" )
        ( "stdatabase", value( & out.stdatabase ), "Specify stations database name" )
        ( "sthost", value( & out.sthost ), "Specify stations database host" )
        ( "stuser", value( & out.stuser ), "Specify stations database user" )
        ( "stpass", value( & out.stpass ), "Specify stations database password" )
//        ( "referencetime", value( & out.referenceTime ), "Override reference time decoded from file" )
//        ( "valueparameter", value( & out.valueParameter ), "Override value parameter decoded from file" )
//        ( "levelparameter", value( & out.levelParameter ), "Override level parameter decoded from file" )
//        // TODO Fix magic number
//        // KLUDGE Magic number - should not be used
        ( "stport", value( & out.stport )->default_value( 5432 ), "Specify stations database port (default 5432)" )
//        ( "confidencecode", value( & out.confidenceCode )->default_value( 0 ), "Set WCI Confidence Code" )
//        ( "namespace", value(& out.nameSpace), "Specify a non-default namespace. Currently supported are 'default' and 'test'")
    ;

    return loading;
}

} // namespace support functions


STLoaderConfiguration::STLoaderConfiguration(const std::string & defaultDataProvider)
        : defaultDataProvider_(defaultDataProvider)
{
        cmdOptions().add( getInput( input_ ) );
        cmdOptions().add( getOutput( output_ ) );
        configOptions().add( getLoading( loading_, defaultDataProvider_ ) );

        shownOptions().add( getOutput( output_ ) );
        shownOptions().add( getLoading( loading_, defaultDataProvider_  ) );

        positionalOptions().add( "name", -1 );
}

STLoaderConfiguration::~STLoaderConfiguration()
{
    // NOOP
}

void STLoaderConfiguration::parse_( int argc, char ** argv )
{
        options_description opt;
        opt.add( cmdOptions() ).add( configOptions() );

        store( command_line_parser( argc, argv ).
                                options( opt ).
                                positional( positionalOptions_ ).
                                run(),
                        givenOptions_ );
}

string STLoaderConfiguration::STLoadingOptions::pqDatabaseConnection() const
{
        ostringstream ret;
        ret << "dbname=" << stdatabase.c_str() << " ";
        if(sthost != "")
            ret << "host=" << sthost.c_str() << " ";
        ret << "port=" << stport << " ";
        if(stuser != "")
            ret << "user=" << stuser.c_str() << " ";
        if(stpass != "")
            ret << "password=" << stpass.c_str() << " ";
        return ret.str();
}

string STLoaderConfiguration::STLoadingOptions::psqlDatabaseConnection() const
{
        ostringstream ret;
        ret << "-d" << stdatabase;
        if (!sthost.empty())
            ret << " -h" << sthost;
        ret << " -p" << stport;
        ret << " -U" << stuser;
        return ret.str();
}
