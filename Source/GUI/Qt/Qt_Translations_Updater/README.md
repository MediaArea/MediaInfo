# Translation Update Script

This folder contains scripts to automate the process of updating Qt `.ts` translation files and generating `.qm` files for MediaInfo's Qt GUI. The process involves using `lupdate` to update the `.ts` files, a Python script to apply custom translations from MediaInfo's `Languages` CSV file, and `lrelease` to generate the final `.qm` files.

## Prerequisites

1. **Qt Tools**: Ensure you have `lupdate` and `lrelease` installed and available in your system's PATH.
2. **Python**: Ensure you have Python installed and available in your system's PATH.

## Files

### `update_Qt_translations.cmd`

This Windows Command Script automates the process of updating `.ts` files and generating `.qm` files.

### `update_Qt_translations.py`

This Python script updates the `.ts` files with translations from the CSV file and handles special cases.

## Usage

1. **Run the Windows Command Script**:
    - Double-click `update_Qt_translations.cmd` or execute it from the command line.
    - The script will:
      - Run `lupdate` to update the `.ts` files.
      - Use `update_Qt_translations.py` to apply custom translations from `Languages.csv`.
      - Run `lrelease` to generate the `.qm` files.

2. **Verify the Output**:
    - Check the `Source\Resource\Translations` folder to ensure that the `.ts` files have been generated/updated.
    - Verify that the `.qm` files have been generated/updated in the same directory as above.
