#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

########################################################################################
# make sure that a valid path is set bellow                                            #
# this is an Interop module so this file should be placed in the CMakes module folder  #
# usually CMake\Modules                                                                #
########################################################################################

# native code directory
set(BASE_PATH_FOR_THIS_MODULE ${PROJECT_SOURCE_DIR}/InteropAssemblies/EleniHealth.Firmware.Native)


# set include directories
list(APPEND EleniHealth.Firmware.Native_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)
list(APPEND EleniHealth.Firmware.Native_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Include)
list(APPEND EleniHealth.Firmware.Native_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/HAL/Include)
list(APPEND EleniHealth.Firmware.Native_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)
list(APPEND EleniHealth.Firmware.Native_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})


# source files
set(EleniHealth.Firmware.Native_SRCS

    EleniHealth_Firmware_Native.cpp


    EleniHealth_Firmware_Native_EleniHealth_Firmware_Native_RMTHelper_mshl.cpp
    EleniHealth_Firmware_Native_EleniHealth_Firmware_Native_RMTHelper.cpp

)

foreach(SRC_FILE ${EleniHealth.Firmware.Native_SRCS})

    set(EleniHealth.Firmware.Native_SRC_FILE SRC_FILE-NOTFOUND)

    find_file(EleniHealth.Firmware.Native_SRC_FILE ${SRC_FILE}
        PATHS
	        ${BASE_PATH_FOR_THIS_MODULE}
	        ${TARGET_BASE_LOCATION}
            ${PROJECT_SOURCE_DIR}/src/EleniHealth.Firmware.Native

	    CMAKE_FIND_ROOT_PATH_BOTH
    )

    if (BUILD_VERBOSE)
        message("${SRC_FILE} >> ${EleniHealth.Firmware.Native_SRC_FILE}")
    endif()

    list(APPEND EleniHealth.Firmware.Native_SOURCES ${EleniHealth.Firmware.Native_SRC_FILE})

endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(EleniHealth.Firmware.Native DEFAULT_MSG EleniHealth.Firmware.Native_INCLUDE_DIRS EleniHealth.Firmware.Native_SOURCES)
