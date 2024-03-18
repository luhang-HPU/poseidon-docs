# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/random_sample.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precision.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RNSTool_QP.cpp
        ${CMAKE_CURRENT_LIST_DIR}/debug.cpp
        ${CMAKE_CURRENT_LIST_DIR}/matrix_operation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fft.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/example.cpp

)

# Add header files for installation
install(
    FILES
#        ${CMAKE_CURRENT_LIST_DIR}/defines.h
        ${CMAKE_CURRENT_LIST_DIR}/config.h
        ${CMAKE_CURRENT_LIST_DIR}/random_sample.h
        ${CMAKE_CURRENT_LIST_DIR}/precision.h
        ${CMAKE_CURRENT_LIST_DIR}/RNSTool_QP.h
        ${CMAKE_CURRENT_LIST_DIR}/debug.h
        ${CMAKE_CURRENT_LIST_DIR}/matrix_operation.h
        ${CMAKE_CURRENT_LIST_DIR}/fft.h
        ${CMAKE_CURRENT_LIST_DIR}/exception.h
        ${CMAKE_CURRENT_LIST_DIR}/examples.h


    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/util
)

set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
