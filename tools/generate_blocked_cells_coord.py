#!/usr/bin/env python3
"""
@file generate_blocked_cells_coord.py
@brief Generates blocked cell coordinates for testing large-scale pathfinding algorithms
@author Slepotek
@date September 2025
@version 1.0

This script generates blocked cell coordinate files for different matrix sizes
and runs pathfinding tests to validate algorithm performance on large matrices.
Supports reproducible testing with fixed random seed.
"""

import os
import sys
import numpy as np
import subprocess
import threading
import time

# Matrix configurations for different test scenarios
matrix_sizes = {"small": (100, 100), "medium": (200, 200), "large": (500, 500)}
np.random.seed(42)  # The "Answer to everything" for reproducible test results
file_paths = {size: f"blocked_cells_{size}.txt" for size in matrix_sizes.keys()}

class Spinner:
    """
    A class to create and control a simple command-line spinner.
    """
    def __init__(self, message="Processing"):
        """
        Initializes the Spinner object.
        
        Args:
            message (str): The message to display next to the spinner.
        """
        self._message = message
        self._is_running = False
        self._thread = None
        # Use a threading.Event for a clean way to signal the thread to stop
        self._stop_event = threading.Event()

    def _spin_method(self):
        """
        The method run by the spinner thread to perform the animation.
        """
        symbols = ['|', '/', '-', '\\']
        i = 0
        # The loop runs until the stop_event is set
        while not self._stop_event.is_set():
            sys.stdout.write(f'\r{self._message} {symbols[i]}')
            sys.stdout.flush()
            time.sleep(0.1)
            i = (i + 1) % len(symbols)
        
        # Clear the line before the thread exits
        sys.stdout.write('\r' + ' ' * (len(self._message) + 3) + '\r')
        sys.stdout.flush()

    def start(self):
        """
        Starts the spinner animation in a new thread.
        """
        if not self._is_running:
            self._is_running = True
            # Clear any previous stop signals
            self._stop_event.clear()
            self._thread = threading.Thread(target=self._spin_method, daemon=True)
            self._thread.start()
            
    def stop(self):
        """
        Stops the spinner animation and waits for the thread to join.
        """
        if self._is_running:
            self._stop_event.set()
            if self._thread:
                self._thread.join()
            self._is_running = False

def generate_blocked_cells_coord():
    """
    Generates blocked cell coordinate files for different matrix sizes.
    
    Creates files with blocked cell coordinates in CSV format (row,col) with
    comment headers. Blocks up to 45% of cells to ensure path finding remains
    feasible while providing realistic obstacle density.
    
    File format:
    - Header comments explaining format
    - One coordinate per line: row,col
    - No duplicate coordinates (uses set to prevent duplicates)
    
    Matrix sizes:
    - small: 100x100 (4,500 blocked cells)
    - medium: 200x200 (18,000 blocked cells) 
    - large: 500x500 (112,500 blocked cells)
    """
    for size, (rows, cols) in matrix_sizes.items():
        filename = f"blocked_cells_{size}.txt"
        if os.path.exists(filename):
            print(f"File {filename} already exists. Skipping generation.")
            continue
            
        with open(filename, 'w') as file:
            file.write("# Blocked cells coordinates for pathfinding tests\n")
            file.write("# Format: row,col (0-indexed)\n")
            file.write(f"# Matrix size: {rows}x{cols}\n")
            
            # Block up to 45% of cells to ensure path finding remains feasible
            total_cells = rows * cols
            target_blocked_cells = int(total_cells * 0.45)
            blocked_coordinates = set()  # Use set to prevent duplicates
            
            print(f"Generating {target_blocked_cells} blocked cells for {rows}x{cols} matrix.")
            
            # Generate unique random coordinates
            while len(blocked_coordinates) < target_blocked_cells:
                blocked_row = np.random.randint(0, rows)
                blocked_col = np.random.randint(0, cols)
                blocked_coordinates.add((blocked_row, blocked_col))
            
            # Write coordinates to file
            for row, col in sorted(blocked_coordinates):
                file.write(f"{row},{col}\n")
                
        print(f"Generated {filename} with {len(blocked_coordinates)} unique blocked cell coordinates.")

def run_cli_with_blocked_cells(file_paths_dict, spinner):
    """
    Runs pathfinding CLI tests with generated blocked cell files.
    
    Builds the project and executes pathfinding tests for each matrix size
    using the corresponding blocked cells file. Captures and displays output
    for performance analysis and validation.
    
    Args:
        file_paths_dict: Dictionary mapping size names to blocked cells file paths
        
    Test parameters:
    - Path length: 10% of total matrix cells
    - Uses DFS algorithm with default starting points (5)
    - Validates successful path finding or reports failures
    """
    spinner.start()
    try:
        spinner.stop()
        print("Building the project...\n")
        spinner.start()

        # Clean and rebuild project
        result = subprocess.run(["make", "clean"], cwd="../", capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Clean failed: {result.stderr}")
            return
            
        result = subprocess.run(["make", "build_debug"], cwd="../", capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Error: Build failed with code {result.returncode}")
            print(f"Build errors:\n{result.stderr}")
            return

        spinner.stop()
        print("Build successful.\n")
        spinner.start()
        
        # Find the pathFinder executable
        find_result = subprocess.run(
            ["find", "../", "-name", "pathFinderC", "-type", "f"], 
            capture_output=True, text=True
        )
        path_finder_path = find_result.stdout.strip()
        
        if not path_finder_path:
            print("Error: pathFinder executable not found")
            return
        
        spinner.stop()
        print(f"Found pathFinder executable: {path_finder_path}\n")

        # Run tests for given matrix size
        size = input("Enter matrix size to test (small, medium(might take long)," \
                     " large(will take very long)): ").strip().lower()

        if size not in matrix_sizes:
            print(f"Invalid size '{size}'. Valid options are: {', '.join(matrix_sizes.keys())}")
            return

        # Ask for multithreading option
        multithreading_choice = input("Run with multithreading? (y/n): ").strip().lower()

        rows, cols = matrix_sizes[size]
        print(f"Running pathfinding test for {size} matrix ({rows}x{cols})...")
        
        # Set path length to 10% of total cells (reasonable for testing)
        path_length = int((rows * cols) * 0.008)
        blocked_cells_file = file_paths_dict[size]
        
        # Execute pathfinding with blocked cells file
        cmd = [
            path_finder_path,
            "--rows", str(rows),
            "--cols", str(cols), 
            "--pathLength", str(path_length),
            "--blockedCellsFile", blocked_cells_file,
        ]

        if multithreading_choice in ['y', 'yes']:
            cmd.append("--multithreading")

        spinner.start()
        result = subprocess.run(cmd, capture_output=True, text=True)
        spinner.stop()
        
        print(f"=== Results for {size} matrix ===")
        print(f"Command: {' '.join(cmd)}")
        print(f"Exit code: {result.returncode}")
        print(f"Output:\n{result.stdout}")
        
        if result.returncode != 0:
            print(f"Error output:\n{result.stderr}")
        print("-" * 50)
            
    except Exception as e:
        print(f"Unexpected error during CLI testing: {e}")

    finally:
        spinner.stop()

if __name__ == "__main__":
    print("=== Blocked Cells Coordinate Generator ===")
    print("Generating test files for pathfinding algorithm validation\n")
    generate_blocked_cells_coord()
    print("\n=== Running Pathfinding Tests ===")
    spinner = Spinner()
    run_cli_with_blocked_cells(file_paths, spinner)