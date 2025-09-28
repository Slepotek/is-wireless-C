# Path test suite
add_executable(pathTests pathTests.c)
target_link_libraries(pathTests pathFinderC_lib)

# Register test with CTests
add_test(NAME PathTestSuite COMMAND pathTests)

set_target_properties(pathTests PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED ON)
