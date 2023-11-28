#!/bin/bash

# Program name (replace with the actual name of your program)
program_name="scoundrills"

# Function to start instances of the program
start_instances() {
    for ((i=1; i<=100; i++)); do
        ./$program_name &  # Replace this line with the actual command to start your program
    done
}

# Function to wait for user input
wait_for_input() {
    echo "Press Enter to kill all instances..."
    read -r
}

# Function to kill all instances of the program
kill_instances() {
    pkill -f "$program_name"
}

# Main execution
start_instances
wait_for_input
kill_instances

echo "All instances killed."

