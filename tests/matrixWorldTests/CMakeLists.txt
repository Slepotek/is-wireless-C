# MatrixWorld test suite
add_executable(matrixWorldTests matrixWorldTests.c)
target_link_libraries(matrixWorldTests pathFinderC_lib)

# Register test with CTests
add_test(NAME MatrixWorldTestSuite COMMAND matrixWorldTests)

set_target_properties(matrixWorldTests PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)