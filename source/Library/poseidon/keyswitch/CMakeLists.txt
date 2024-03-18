# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_base.cpp
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_bv.cpp
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_ghs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_hybrid.cpp
)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_base.h
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_bv.h
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_ghs.h
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch_hybrid.h
        ${CMAKE_CURRENT_LIST_DIR}/keyswitch.h




    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/keyswitch
)


set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
