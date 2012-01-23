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

#include "WDBDatabaseConnection.h"
#include "STLoaderConfiguration.h"
#include "transactors/addPlacePoint.h"
#include "transactors/updatePlacePoint.h"
#include "transactors/wciTransactors.h"

// WDB
//
#include <wdbMath.h>
#include <wdbMathC.h>
#include <wdb/wdbSetup.h>
#include <wdb/LoaderConfiguration.h>

// GEOS
//
#include <geos_c.h>
#include <cstdarg>

// PQXX
//
#include <pqxx/pqxx>

// BOOST
//
#include <boost/lexical_cast.hpp>

// STD
//
#include <cassert>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace pqxx;
using namespace pqxx::prepare;

namespace wdb { namespace load {

    static void notice(const char *fmt, ...)
    {
        std::fprintf(stderr, "GEOS: ");

        va_list ap;
        va_start(ap, fmt);
        std::vfprintf(stderr, fmt, ap);
        va_end(ap);

        std::fprintf(stderr, "\n");
    }


    WDBDatabaseConnection::WDBDatabaseConnection(const STLoaderConfiguration & config)
        : pqxx::connection(config.database().pqDatabaseConnection()), config_(new STLoaderConfiguration(config))
    {
        if(config.loading().nameSpace.empty())
            perform(WciBegin((config.database().user)));
        else if(config.loading().nameSpace == "test")
            perform(WciBegin(config.database().user, 999, 999, 999));
        else if(config.loading().nameSpace == "default")
            perform(WciBegin(config.database().user, 0, 0, 0));
        else
            throw std::logic_error("Unknown name space specification: " + config.loading().nameSpace );

        setup_();
    }

    WDBDatabaseConnection::~WDBDatabaseConnection()
    {
//        unprepare("GetPlaceName");
        perform(WciEnd(), 1);

        delete config_;
    }
    void WDBDatabaseConnection::setup_()
    {
        // Statement Get PlaceId
//	prepare("GetPlaceName",
//			"SELECT * FROM wci.getplacename ($1, $2, $3, $4, $5, $6, $7)" )
//		   ("int4", treat_direct )
//		   ("int4", treat_direct )
//		   ("real", treat_direct )
//		   ("real", treat_direct )
//		   ("real", treat_direct )
//		   ("real", treat_direct )
//		   ("varchar", treat_direct );
    }

    void WDBDatabaseConnection::getAllStations(std::map<std::string, wdb::load::WDBStationRecord>& result)
    {
        WDB_LOG & log = WDB_LOG::getInstance("wdb.load.wdbdatabaseconnection");

        // Create a transaction.
        pqxx::work transaction(*this);

        // This is the read query
        std::string query = "select placeid, placename, originalsrid, ST_AsText(placegeometry) from wci.getPlacePoint('%');";

        pqxx::result rows = transaction.exec(query);
        size_t rCount = rows.size();
        for(size_t r = 0; r < rCount; ++r) {
            WDBStationRecord rec;
            rec.id_   = rows[r][0].as<std::string>();
            rec.name_ = rows[r][1].as<std::string>();
            rec.srid_ = rows[r][2].as<std::string>();
            rec.wkt_  = rows[r][3].as<std::string>();

            if(result.count(rec.id_) != 0) {
                log.debugStream() << "already have entry with STATIONID: " << rec.id_;
            }

            log.debugStream()<< rec.id_<<" | "<< rec.name_<<" | "<<rec.srid_<< " | " << rec.wkt_;

            result.insert(std::make_pair<std::string, WDBStationRecord>(rec.name_, rec));
        }

        transaction.commit();

        log.debugStream() << "rows size: "<< rCount;
    }

    void WDBDatabaseConnection::updateStations(std::map<std::string, STIStationRecord>& sti_stations)
    {
        WDB_LOG & log = WDB_LOG::getInstance("wdb.load.wdbdatabaseconnection");

        std::map<std::string, WDBStationRecord> wdb_stations;
        getAllStations(wdb_stations);

        initGEOS(notice, notice);

        std::map<std::string, STIStationRecord>::const_iterator cit;
        for(cit = sti_stations.begin(); cit != sti_stations.end(); ++cit) {
            STIStationRecord sti_st = cit->second;
            if(wdb_stations.find(sti_st.id_) == wdb_stations.end()) {
                std::string wkt("POINT(");
                wkt.append(boost::lexical_cast<std::string>(wdb::round(sti_st.lon_, 4))).append(" ").append(boost::lexical_cast<std::string>(wdb::round(sti_st.lat_, 4))).append(")");
                if(config_->output().list)
                    log.debugStream()<<"ADD statioinid: "<<sti_st.id_<<" WKT: "<<AddPlacePoint(sti_st.id_, wkt).toString();
                else
                    perform(AddPlacePoint(sti_st.id_, wkt));
            } else {
                // check if station param have been changed
                WDBStationRecord wdb_st = wdb_stations[sti_st.id_];
                GEOSGeometry* g = GEOSGeomFromWKT(wdb_st.wkt_.c_str());
                assert(g != 0);
                assert(GEOSGeomTypeId(g) == GEOS_POINT);

                GEOSCoordSeq coords = const_cast<GEOSCoordSeq>(GEOSGeom_getCoordSeq(g));
                assert(coords);

                double wdb_lon;
                double wdb_lat;
                GEOSCoordSeq_getX(coords, 0, &wdb_lon);
                GEOSCoordSeq_getY(coords, 0, &wdb_lat);

                if(wdb::round(wdb_lon, 4) == wdb::round(sti_st.lon_, 4)
                        && wdb::round(wdb_lat, 4) == wdb::round(sti_st.lat_, 4)) {
                    log.debugStream()<<"NOT CHANGED statioinid: "<<sti_st.id_<<" WKT: "<<GEOSGeomToWKT(g);
                } else {
                    std::string wkt("POINT(");
                    wkt.append(boost::lexical_cast<std::string>(wdb::round(sti_st.lon_, 4))).append(" ").append(boost::lexical_cast<std::string>(wdb::round(sti_st.lat_, 4))).append(")");
                    if(config_->output().list)
                        log.debugStream()<<"UPDATE statioinid: "<<sti_st.id_<<" WKT: "<<AddPlacePoint(sti_st.id_, wkt).toString();
                    else
                        perform(UpdatePlacePoint(sti_st.id_, wkt));
                }

                GEOSGeom_destroy(g);
            }
        }
    }

} } /* end namespaces */
