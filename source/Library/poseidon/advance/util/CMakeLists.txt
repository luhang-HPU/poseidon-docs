# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/cosine_approx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/chebyshev_interpolation.cpp




)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/cosine_approx.h
        ${CMAKE_CURRENT_LIST_DIR}/chebyshev_interpolation.h
    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/advance/util
)




set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
