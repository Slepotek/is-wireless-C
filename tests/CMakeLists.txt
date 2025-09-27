# CMakeLists.txt for tests directory
# Add subdirectories for each test module
add_subdirectory(someTests)
add_subdirectory(matrixWorldTests)


# Find valgrind
find_program(VALGRIND_PROGRAM valgrind)

if(VALGRIND_PROGRAM)
    # Add memory check tests for each module
    add_test(
        NAME someTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:someTests>
    )

    add_test(
        NAME matrixWorldTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:matrixWorldTests>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(someTests_memcheck PROPERTIES DEPENDS SomeTestSuite)
    set_tests_properties(matrixWorldTests_memcheck PROPERTIES DEPENDS MatrixWorldTestSuite)
endif()