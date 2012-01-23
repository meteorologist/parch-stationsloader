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


#ifndef STLOADERCONFIGURATION_H_
#define STLOADERCONFIGURATION_H_

// PROJECT INCLUDES
#include <wdbConfiguration.h>

// SYSTEM INCLUDES
#include <string>
#include <boost/program_options/positional_options.hpp>


namespace wdb { namespace load {

    /**
      * STLoaderConfiguration handles the options
      * of the station laoder application.
      *
      * @see WdbConfiguration
      */
class STLoaderConfiguration : public WdbConfiguration
{
public:
        // LIFECYCLE
        /**
         * Default Constructor
         */
        explicit STLoaderConfiguration( const std::string & defaultDataProvider = "");
        /**
         * Default Destructor
         */
        virtual ~STLoaderConfiguration();

        /** Container for input options
         */
        struct InputOptions
        {
                std::vector<std::string> file;
        };
        /** Container for output options
         */
        struct OutputOptions
        {
                bool list;
        };
        /** Container for general loader options
         */
        struct STLoadingOptions
        {
//                bool loadPlaceDefinition;
//                std::string dataProvider; ///< The data provider name.
//                /**
//                 * In case you use the LoaderDatabaseConnection, this value will be
//                 * chosen automatically if dataProvider is empty
//                 */
                std::string defaultDataProvider;
                std::string stdatabase; /// < station database name.
                std::string sthost; /// < station database host.
                std::string stuser; /// < station database user.
                std::string stpass; /// < station database password.
                int         stport; /// < station database port.
//                std::string placeName; /// < The place name.
//                std::string referenceTime; /// < Reftime.
//                std::string valueParameter; /// < Value Parameter.
//                std::string levelParameter; /// < Level Parameter.
//                int dataVersion; /// The Data Version
//                int confidenceCode; /// Confidence Code

                std::string nameSpace; /// what namespace to use

                /**
                  * Generate and return a pq compatible database connection string using
                  * the options given for the program.
                  */
                std::string pqDatabaseConnection() const;
                std::string psqlDatabaseConnection() const;
        };


        /**	Get the input options
         * @return 	The input options
         */
        const InputOptions & input() const { return input_; }
        /** Get the output options
         * @return 	The output options
         */
        const OutputOptions & output() const { return output_; }
        /** Get the general loading options
         */
        const STLoadingOptions & loading() const { return loading_; }
        /** Get the positional options of the loader
         * Used for returning the file name
         * @return 	The positional options
         */
        boost::program_options::positional_options_description & positionalOptions() { return positionalOptions_; }

protected:
        using WdbConfiguration::parse_;
        /**
         * Parse arguments from a given file
         * @param	argc		Number of command line arguments
         * @param	argv		The actual command line arguments
         */
        virtual void parse_( int argc, char ** argv );
        /** Commands to be performed after parsing arguments
         */
        //    virtual void postParseHook();
        /// Positional options
        boost::program_options::positional_options_description positionalOptions_;
        /// Input options
        InputOptions input_;
        /// Output options
        OutputOptions output_;
        /// loading options
        STLoadingOptions loading_;

        const std::string defaultDataProvider_;
};

} }

#endif /*STLOADERCONFIGURATION_H_*/
