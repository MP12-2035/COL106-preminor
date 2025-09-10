#!/bin/bash

# Compile the program
g++ -std=c++17 -Wall -Wextra main.cpp -o file_version_system

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    # Run the program, allowing user interaction for commands
    ./file_version_system
else
    echo "Compilation failed. Please check the errors above."
fi
