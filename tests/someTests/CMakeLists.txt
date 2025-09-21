# Some test suite
add_executable(someTests someTests.c)
target_link_libraries(someTests pathFinderC_lib)

# Register test with CTests
add_test(NAME SomeTestSuite COMMAND someTests)

set_target_properties(someTests PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)