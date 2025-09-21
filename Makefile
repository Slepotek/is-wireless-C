clean:
	@rm -rf build
	@echo "Build folder cleaned"

build_debug:
	@echo "Building project in debug mode"
	@cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build build

build_release:
	@echo "Building project in release mode"
	@cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build build

run_tests:
	@echo "Running tests for the project"
	@ctest --test-dir build

run_proj:
	@./build/pathFinderC

.PHONY: clean build_debug build_release run_tests run_proj