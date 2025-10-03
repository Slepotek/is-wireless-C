# DfsPathFinding test suite
add_executable(dfsPathFindingTests dfsPathFindingTests.c)
target_link_libraries(dfsPathFindingTests PRIVATE pathFinderC_lib)

# Register test with CTest
add_test(NAME DfsPathFindingTestSuite COMMAND dfsPathFindingTests)

set_target_properties(dfsPathFindingTests PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)
