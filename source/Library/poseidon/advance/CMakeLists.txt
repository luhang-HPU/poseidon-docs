# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_dft.cpp
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_linear_transform.cpp
        ${CMAKE_CURRENT_LIST_DIR}/polynomial_evaluation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_mod.cpp




)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_dft.h
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_linear_transform.h
        ${CMAKE_CURRENT_LIST_DIR}/polynomial_evaluation.h
        ${CMAKE_CURRENT_LIST_DIR}/homomorphic_mod.h


    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon/advance
)

add_subdirectory(util)
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
