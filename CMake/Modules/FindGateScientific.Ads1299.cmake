#
# Copyright (c) Gate Scientific Inc.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/GateScientific.Ads1299)


# set include directories
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Include)
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/HAL/Include)
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
list(APPEND GateScientific.Ads1299_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/GateScientific.Ads1299)

# source files
set(GateScientific.Ads1299_SRCS

    gatescientific_ads1299.cpp


    gatescientific_ads1299_GateScientific_Ads1299_Ads1299.cpp

)

foreach(SRC_FILE ${GateScientific.Ads1299_SRCS})

    set(GateScientific.Ads1299_SRC_FILE SRC_FILE-NOTFOUND)

    find_file(GateScientific.Ads1299_SRC_FILE ${SRC_FILE}
        PATHS
	        ${BASE_PATH_FOR_THIS_MODULE}
	        ${TARGET_BASE_LOCATION}
            ${PROJECT_SOURCE_DIR}/src/GateScientific.Ads1299

	    CMAKE_FIND_ROOT_PATH_BOTH
    )

    if (BUILD_VERBOSE)
        message("${SRC_FILE} >> ${GateScientific.Ads1299_SRC_FILE}")
    endif()

    list(APPEND GateScientific.Ads1299_SOURCES ${GateScientific.Ads1299_SRC_FILE})

endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GateScientific.Ads1299 DEFAULT_MSG GateScientific.Ads1299_INCLUDE_DIRS GateScientific.Ads1299_SOURCES)
