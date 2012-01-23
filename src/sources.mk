wdbstationsloader_SOURCES = src/main.cpp \
                            src/STLoaderConfiguration.h \
							src/STLoaderConfiguration.cpp \
                            src/STInfosysDatabaseConnection.h \
							src/STInfosysDatabaseConnection.cpp \
							src/WDBDatabaseConnection.h \
							src/WDBDatabaseConnection.cpp \
							src/transactors/addPlacePoint.h \
							src/transactors/updatePlacePoint.h \
							src/transactors/wciTransactors.h
EXTRA_DIST += src/sources.mk
