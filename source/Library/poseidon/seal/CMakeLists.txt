# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/modulus.cpp
        ${CMAKE_CURRENT_LIST_DIR}/memorymanager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/randomgen.cpp

)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/modulus.h
#        ${CMAKE_CURRENT_LIST_DIR}/serialization.h
        ${CMAKE_CURRENT_LIST_DIR}/memorymanager.h
        ${CMAKE_CURRENT_LIST_DIR}/dynarray.h
        ${CMAKE_CURRENT_LIST_DIR}/randomtostd.h
        ${CMAKE_CURRENT_LIST_DIR}/randomgen.h


    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/seal
)

add_subdirectory(util)
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
