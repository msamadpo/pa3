add_subdirectory(bitStream) # Uncomment this line for final submission
add_subdirectory(encoder)

add_executable (compress.cpp.executable compress.cpp FileUtils.hpp)
target_link_libraries(compress.cpp.executable PRIVATE huffman_encoder)

add_executable (uncompress.cpp.executable uncompress.cpp FileUtils.hpp)
target_link_libraries(uncompress.cpp.executable PRIVATE huffman_encoder)

add_executable (bitconverter.cpp.executable bitconverter.cpp FileUtils.hpp)
target_link_libraries(bitconverter.cpp.executable PRIVATE bit_input_stream bit_output_stream)
