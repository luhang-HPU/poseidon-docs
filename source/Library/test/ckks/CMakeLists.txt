

#test ckks encode
add_executable(test_ckks_encode test_ckks_encode.cpp)
target_link_libraries(test_ckks_encode poseidon_shared )
#test ckks encyrpt
add_executable(test_ckks_encrypt test_ckks_encrypt.cpp)
target_link_libraries(test_ckks_encrypt poseidon_shared )

#test ckks add
add_executable(test_ckks_add test_ckks_add.cpp)
target_link_libraries(test_ckks_add poseidon_shared )







#test ckks basic
add_executable(test_ckks_basic test_ckks_basic.cpp)
target_link_libraries(test_ckks_basic poseidon_shared )



#test ckks mult_matrix
add_executable(test_ckks_multMatrix test_ckks_multMatrix.cpp)
target_link_libraries(test_ckks_multMatrix poseidon_shared )


#test ckks s2c
add_executable(test_ckks_slot_to_coeff test_ckks_slot_to_coeff.cpp)
target_link_libraries(test_ckks_slot_to_coeff poseidon_shared )

#test ckks evaluate
add_executable(test_ckks_evaluatePolyVector test_ckks_evaluatePolyVector.cpp)
target_link_libraries(test_ckks_evaluatePolyVector poseidon_shared )




#test ckks bootstrap
add_executable(test_ckks_bootstrap test_ckks_bootstrap.cpp)
target_link_libraries(test_ckks_bootstrap poseidon_shared )

#test test_ckks_heartstudy
add_executable(test_ckks_heartstudy test_ckks_heartstudy.cpp)
target_link_libraries(test_ckks_heartstudy poseidon_shared )


#test test_ckks__pir
add_executable(test_ckks_pir_demo test_ckks_pir_demo.cpp)
target_link_libraries(test_ckks_pir_demo poseidon_shared )


