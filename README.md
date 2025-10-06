# pathFinderC

## Description

`pathFinderC` is a C-based command-line application designed to find a contiguous path of a specified length within an N x M matrix. The algorithm navigates through unblocked cells (value 0) while avoiding blocked cells (value 1).

The core of the application is a Depth-First Search (DFS) algorithm with backtracking. It can be run in either a standard single-threaded mode or a high-performance multithreaded mode to accelerate the search on large matrices.

## Building and Running

The project uses `cmake` for building and a `Makefile` for convenience scripts.

### Makefile Targets

*   `make build_debug`
    *   Builds the project in debug mode. The executable will be located at `build/pathFinderC`.

*   `make build_release`
    *   Builds the project in release mode with optimizations.

*   `make run_tests`
    *   Runs the full test suite (including memory checks with Valgrind) for all modules.

*   `make run_proj`
    *   A convenience script to execute the compiled program. Note: This will fail unless the project has been built first.

*   `make clean`
    *   Removes the `build` directory.

## Usage

The `pathFinderC` executable is configured via command-line arguments.

```
USAGE:
    pathFinderC --rows R --cols C --pathLength N [OPTIONS]

REQUIRED:
    --rows R                Number of matrix rows (e.g., --rows 5)
    --cols C                Number of matrix columns (e.g., --cols 5)
    --pathLength N          Target path length (e.g., --pathLength 12)

OPTIONAL:
    --blockedCells COORDS   Blocked cell coordinates (e.g., --blockedCells {1,0} {2,1})
    --blockedCellsFile FILE Path to file containing blocked cell coordinates
    --multithreading        Flag enabling the execution of the program on parallel threads
    --help, -h              Show this help message

EXAMPLES:
    ./build/pathFinderC --rows 5 --cols 5 --pathLength 6
    ./build/pathFinderC --rows 8 --cols 8 --pathLength 12 --blockedCells {1,0} {2,0} {1,1}
    ./build/pathFinderC --rows 100 --cols 100 --pathLength 50 --blockedCellsFile blocked_cells.txt --multithreading
```

### Python Test Harness

The `tools/` directory contains a Python script for running large-scale tests.

```bash
cd tools
python3 generate_blocked_cells_coord.py
```
This script will:
1.  Generate large files of blocked cell coordinates.
2.  Build the C project.
3.  Prompt you to select a matrix size (`small`, `medium`, `large`) and whether to use multithreading.
4.  Run the `pathFinderC` executable with the selected configuration and report the results.

## License

This project is licensed under the **GPL**.
