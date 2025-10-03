#!/bin/bash
# @file pythonTest.sh
# @brief Shell script for setting up Python environment and running pathfinding tests
# @author Slepotek
# @date September 2025
# @version 1.0
#
# This script automates the setup of Python environment with required dependencies
# and executes the blocked cells coordinate generation and pathfinding tests.
# Supports multiple Linux distributions and macOS with automatic dependency installation.

set -e  # Exit on any error

echo "=== Python Environment Setup for Pathfinding Tests ==="

# Check if python3 is installed
if ! command -v python3 &> /dev/null; then
    echo "Python 3 not found. Installing..."

    # Detect OS and install accordingly
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "Detected Linux system"
        # Ubuntu/Debian
        if command -v apt &> /dev/null; then
            echo "Using apt package manager..."
            sudo apt update && sudo apt install -y python3 python3-venv python3-pip
        # CentOS/RHEL/Fedora
        elif command -v yum &> /dev/null; then
            echo "Using yum package manager..."
            sudo yum install -y python3 python3-venv python3-pip
        elif command -v dnf &> /dev/null; then
            echo "Using dnf package manager..."
            sudo dnf install -y python3 python3-venv python3-pip
        else
            echo "Unsupported Linux distribution. Please install Python 3 manually."
            exit 1
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "Detected macOS system"
        # macOS with Homebrew
        if command -v brew &> /dev/null; then
            echo "Using Homebrew package manager..."
            brew install python3
        else
            echo "Homebrew not found. Please install Python 3 manually or install Homebrew first."
            exit 1
        fi
    else
        echo "Unsupported operating system: $OSTYPE"
        echo "Please install Python 3 manually."
        exit 1
    fi

    # Verify installation
    if ! command -v python3 &> /dev/null; then
        echo "Failed to install Python 3. Please install manually."
        exit 1
    fi
    
    echo "Python 3 installation successful."
else
    echo "Python 3 found: $(python3 --version)"
fi

# Create virtual environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo "Creating Python virtual environment..."
    python3 -m venv venv
    echo "Virtual environment created successfully."
else
    echo "Virtual environment already exists."
fi

# Activate virtual environment
echo "Activating virtual environment..."
source venv/bin/activate

# Upgrade pip to latest version
echo "Upgrading pip..."
pip install --upgrade pip

# Install numpy if not already installed
if ! python -c "import numpy" &> /dev/null; then
    echo "Installing numpy..."
    pip install numpy
    echo "NumPy installation successful."
else
    echo "NumPy already installed: $(python -c 'import numpy; print(numpy.__version__)')"
fi

echo "Python environment setup complete."
echo ""

# Run the Python script to generate blocked cells coordinates and run tests
echo "=== Executing Pathfinding Test Suite ==="
python3 generate_blocked_cells_coord.py

# Deactivate virtual environment when done
echo ""
echo "=== Cleaning Up ==="
deactivate
echo "Virtual environment deactivated."
echo "Test execution complete."

