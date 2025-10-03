# Add the executable for the cliHandling tests
add_executable(cliHandlingTests cliHandlingTests.c)

# Link the main project library
target_link_libraries(cliHandlingTests PRIVATE pathFinderC_lib)

# Register test with CTest
add_test(NAME CliHandlingTestSuite COMMAND cliHandlingTests)