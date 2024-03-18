# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/BGVEvaluatorBase.cpp


)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/BGVEvaluatorBase.h
        ${CMAKE_CURRENT_LIST_DIR}/bgv.h

    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/scheme/bgv
)

add_subdirectory(Software)

set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
