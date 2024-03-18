# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Source files in this directory
set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/CrtContext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ParametersLiteral.cpp
        ${CMAKE_CURRENT_LIST_DIR}/plaintext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ciphertext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CKKSEncoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/keygenerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/encryptor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/decryptor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Evaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SoftwareEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RNSPoly.cpp
        ${CMAKE_CURRENT_LIST_DIR}/batchencoder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PoseidonContext.cpp

)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/defines.h
        ${CMAKE_CURRENT_LIST_DIR}/ParametersLiteral.h
        ${CMAKE_CURRENT_LIST_DIR}/CrtContext.h
        ${CMAKE_CURRENT_LIST_DIR}/pke_params_defines.h
        ${CMAKE_CURRENT_LIST_DIR}/PoseidonContext.h
        ${CMAKE_CURRENT_LIST_DIR}/CKKSEncoder.h
        ${CMAKE_CURRENT_LIST_DIR}/plaintext.h
        ${CMAKE_CURRENT_LIST_DIR}/ciphertext.h
        ${CMAKE_CURRENT_LIST_DIR}/encryptor.h
        ${CMAKE_CURRENT_LIST_DIR}/decryptor.h
        ${CMAKE_CURRENT_LIST_DIR}/keygenerator.h
        ${CMAKE_CURRENT_LIST_DIR}/Evaluator.h
        ${CMAKE_CURRENT_LIST_DIR}/SoftwareEvaluator.h
        ${CMAKE_CURRENT_LIST_DIR}/RNSPoly.h
        ${CMAKE_CURRENT_LIST_DIR}/HardwareParamsGen.h
        ${CMAKE_CURRENT_LIST_DIR}/HardwareEvaluator.h
        ${CMAKE_CURRENT_LIST_DIR}/batchencoder.h
        ${CMAKE_CURRENT_LIST_DIR}/HardwareContext.h



    DESTINATION
        ${POSEIDON_INCLUDES_INSTALL_DIR}/poseidon
)
add_subdirectory(seal)
add_subdirectory(util)
add_subdirectory(key)
add_subdirectory(keyswitch)
add_subdirectory(advance)
add_subdirectory(scheme)



set(POSEIDON_SOURCE_FILES ${POSEIDON_SOURCE_FILES} PARENT_SCOPE)
