add_executable (test_HCTree.cpp.executable test_HCTree.cpp)
target_link_libraries(test_HCTree.cpp.executable PRIVATE gtest_main huffman_encoder)
add_test(test_HCTree test_HCTree.cpp.executable)

# add_executable (test_BitOutputStream.cpp.executable test_BitOutputStream.cpp)
# target_link_libraries(test_BitOutputStream.cpp.executable PRIVATE gtest_main bit_output_stream)
# add_test(test_BitOutputStream test_BitOutputStream.cpp.executable)

# add_executable (test_BitInputStream.cpp.executable test_BitInputStream.cpp)
# target_link_libraries(test_BitInputStream.cpp.executable PRIVATE gtest_main bit_input_stream)
# add_test(test_BitInputStream test_BitInputStream.cpp.executable)