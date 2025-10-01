# StartingPointVector test suite
add_executable(startingPointVectorTests startingPointVectorTests.c)
target_link_libraries(startingPointVectorTests pathFinderC_lib)

# Register test with CTests
add_test(NAME StartingPointVectorTestSuite COMMAND startingPointVectorTests)

set_target_properties(startingPointVectorTests PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)
