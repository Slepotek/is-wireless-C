# CMakeLists.txt for tests directory
# Add subdirectories for each test module
add_subdirectory(someTests)
add_subdirectory(matrixWorldTests)
add_subdirectory(pathTests)


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

    add_test(
        NAME pathTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:pathTests>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(someTests_memcheck PROPERTIES DEPENDS SomeTestSuite)
    set_tests_properties(matrixWorldTests_memcheck PROPERTIES DEPENDS MatrixWorldTestSuite)
    set_tests_properties(pathTests_memcheck PROPERTIES DEPENDS PathTestSuite)
endif()