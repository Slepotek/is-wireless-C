# CMakeLists.txt for tests directory
# Add subdirectories for each test module
add_subdirectory(matrixWorldTests)
add_subdirectory(pathTests)
add_subdirectory(startingPointVectorTests)
add_subdirectory(dfsPathFindingTests)
add_subdirectory(cliHandlingTests)


# Find valgrind
find_program(VALGRIND_PROGRAM valgrind)

if(VALGRIND_PROGRAM)
    # Add memory check tests for each module
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

    add_test(
        NAME startingPointVectorTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:startingPointVectorTests>
    )

    add_test(
        NAME dfsPathFindingTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:dfsPathFindingTests>
    )

    add_test(
        NAME cliHandlingTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:cliHandlingTests>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(matrixWorldTests_memcheck PROPERTIES DEPENDS MatrixWorldTestSuite)
    set_tests_properties(pathTests_memcheck PROPERTIES DEPENDS PathTestSuite)
    set_tests_properties(startingPointVectorTests_memcheck PROPERTIES DEPENDS StartingPointVectorTestSuite)
    set_tests_properties(dfsPathFindingTests_memcheck PROPERTIES DEPENDS DfsPathFindingTestSuite)
    set_tests_properties(cliHandlingTests_memcheck PROPERTIES DEPENDS CliHandlingTestSuite)
endif()