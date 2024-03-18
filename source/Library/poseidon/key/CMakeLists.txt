# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/kswitchkeys.cpp

)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/kswitchkeys.h
        ${CMAKE_CURRENT_LIST_DIR}/publickey.h
        ${CMAKE_CURRENT_LIST_DIR}/secretkey.h
        ${CMAKE_CURRENT_LIST_DIR}/relinkeys.h
        ${CMAKE_CURRENT_LIST_DIR}/galoiskeys.h
        ${CMAKE_CURRENT_LIST_DIR}/keys.h



    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/key
)


set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
