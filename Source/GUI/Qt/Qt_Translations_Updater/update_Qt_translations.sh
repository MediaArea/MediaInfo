#!/bin/bash

# Enable ANSI escape sequences for colors
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RED=$(tput setaf 1)
RESET=$(tput sgr0)

# Set paths
PROJECT_FILE=${BASH_SOURCE%/*}/../../../../Project/QMake/GUI/MediaInfoQt.pro
TS_FILES_FOLDER=${BASH_SOURCE%/*}/../../../Resource/Translations
CSV_DIRECTORY=${BASH_SOURCE%/*}/../../../Resource/Plugin/Language
PYTHON_SCRIPT=${BASH_SOURCE%/*}/update_Qt_translations.py

# Function to print colored messages
print_message() {
    local color="$1"
    local message="$2"
    echo -e "${color}${message}${RESET}"
}

# Step 0: Delete and recreate the TS_FILES_FOLDER directory
print_message "$YELLOW" "Deleting and recreating the TS_FILES_FOLDER directory..."
rm -rf "$TS_FILES_FOLDER"
mkdir -p "$TS_FILES_FOLDER"
echo

# Step 1: Run lupdate to update .ts files
print_message "$YELLOW" "Running lupdate..."
lupdate -noobsolete "$PROJECT_FILE"
if [ $? -ne 0 ]; then
    print_message "$RED" "lupdate failed!"
    exit 1
fi
print_message "$GREEN" "lupdate completed successfully!"
echo

# Step 2: Run the Python script to update .ts files
print_message "$YELLOW" "Updating .ts files with translations..."
python3 "$PYTHON_SCRIPT" "$TS_FILES_FOLDER" "$CSV_DIRECTORY"
if [ $? -ne 0 ]; then
    print_message "$RED" "Python script failed!"
    exit 1
fi
print_message "$GREEN" ".ts files updated successfully!"
echo

# Step 3: Run lrelease to generate .qm files
print_message "$YELLOW" "Running lrelease..."
lrelease "$PROJECT_FILE"
if [ $? -ne 0 ]; then
    print_message "$RED" "lrelease failed!"
    exit 1
fi
print_message "$GREEN" "lrelease completed successfully!"
echo

print_message "$GREEN" "All steps completed successfully!"
