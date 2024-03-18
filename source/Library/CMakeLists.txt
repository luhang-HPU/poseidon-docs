include(CMakePushCheckState)
include(CheckCXXSourceCompiles)
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

cmake_minimum_required(VERSION 3.12)


# [option] CMAKE_BUILD_TYPE (default: "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
            STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "Build type (CMAKE_BUILD_TYPE): ${CMAKE_BUILD_TYPE}")

project(Poseidon VERSION 3.0.0 LANGUAGES CXX C)

# Always build position-independent-code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Make the install target depend on the all target (required by vcpkg)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY OFF)


set(POSEIDON_USE_CXX17_OPTION_STR "Use C++17")
option(POSEIDON_USE_CXX17 ${POSEIDON_USE_CXX17_OPTION_STR} ON)
message(STATUS "POSEIDON_USE_CXX17: ${POSEIDON_USE_CXX17}")
include(EnableCXX17)

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -fPIC   -pthread ")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -fPIC   -pthread ")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp  -fopenmp")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)




message("-- Install Path: ${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")


set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Build tree
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTLIB_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTLIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

## Install
set(POSEIDON_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake)
set(POSEIDON_INCLUDES_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})





# Source Tree
set(POSEIDON_INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(poseidon)
#################################
# Installation and CMake config #
#################################

# Create the CMake config file
#include(CMakePackageConfigHelpers)
#configure_package_config_file(
#        ${POSEIDON_CONFIG_IN_FILENAME} ${POSEIDON_CONFIG_FILENAME}
#        INSTALL_DESTINATION ${SEAL_CONFIG_INSTALL_DIR})
#
## Install the export
#install(
#        EXPORT SEALTargets
#        NAMESPACE SEAL::
#        DESTINATION ${SEAL_CONFIG_INSTALL_DIR})
#
## Version file; we require exact version match for downstream
#write_basic_package_version_file(
#        ${SEAL_CONFIG_VERSION_FILENAME}
#        VERSION ${SEAL_VERSION}
#        COMPATIBILITY SameMinorVersion)
#
## Install config and module files
#install(
#        FILES
#        ${SEAL_CONFIG_FILENAME}
#        ${SEAL_CONFIG_VERSION_FILENAME}
#        DESTINATION ${SEAL_CONFIG_INSTALL_DIR})
#
## We export SEALTargets from the build tree so it can be used by other projects
## without requiring an install.
#export(
#        EXPORT SEALTargets
#        NAMESPACE SEAL::
#        FILE ${SEAL_TARGETS_FILENAME})
#
## Install header files of dependencies if SEAL_BUILD_DEPS is ON
#if(SEAL_BUILD_DEPS)
#    # MSGSL
#    if(SEAL_USE_MSGSL)
#        install(
#                DIRECTORY ${msgsl_SOURCE_DIR}/include/gsl
#                DESTINATION ${SEAL_INCLUDES_INSTALL_DIR})
#    endif()
#endif()
#export(
#        EXPORT POSEIDONTargets
#        NAMESPACE POSEIDON::
#        FILE ${POSEIDON_TARGETS_FILENAME})

set(POSEIDON_BUILD_EXAMPLES_OPTION_STR "Build C++ examples for POSEIDON")
option(POSEIDON_BUILD_EXAMPLES "Build C++ examples for POSEIDON" ON)
message(STATUS "POSEIDON_BUILD_EXAMPLES: ${POSEIDON_BUILD_EXAMPLES}")


if(POSEIDON_BUILD_EXAMPLES)
    add_subdirectory(test/ckks)
    add_subdirectory(test/bfv)
    add_subdirectory(test/bgv)
endif()


set(BUILD_SHARED_LIBS_STR "Build shared library")
option(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_STR} ON)
if(BUILD_SHARED_LIBS)
    message(${BUILD_SHARED_LIBS_STR})
    add_library(poseidon_shared SHARED ${POSEIDON_SOURCE_FILES}
    )
    target_link_libraries(poseidon_shared gmp)
    target_include_directories(poseidon_shared PUBLIC ${POSEIDON_INCLUDES_DIR} )
    target_include_directories(poseidon_shared PUBLIC ${POSEIDON_INCLUDES_DIR}/poseidon)

endif()



#message(${CMAKE_INSTALL_PREFIX})


install(
        TARGETS poseidon_shared
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)

#
#install(EXPORT POSEIDONTargets FILE POSEIDONConfig.cmake NAMESPACE POSEIDON:: DESTINATION lib/cmake/POSEIDON)
#install(DIRECTORY include/seal DESTINATION include)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gun++17")



















