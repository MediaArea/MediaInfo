# Translation Update Script

This folder contains scripts to automate the process of updating Qt `.ts` translation files and generating `.qm` files for MediaInfo's Qt GUI. The process involves using `lupdate` to update the `.ts` files, a Python script to apply custom translations from MediaInfo's languages CSV files, and `lrelease` to generate the final `.qm` files.

## Prerequisites

1. **Qt Tools**: Ensure you have `lupdate` and `lrelease` installed and available in your system's PATH.
2. **Python**: Ensure you have Python installed and available in your system's PATH.

## Files

### `update_Qt_translations.cmd`/`update_Qt_translations.sh`

This Windows Command Script / Unix Shell Script automates the process of updating `.ts` files and generating `.qm` files.

### `update_Qt_translations.py`

This Python script updates the `.ts` files with translations from the CSV files and handles special cases.

## Usage

1. **Run the Windows Command Script or Unix Shell Script**:
    - Execute `update_Qt_translations.cmd`/`update_Qt_translations.sh`.
    - The script will:
      - Delete existing `.ts` and `.qm` files.
      - Run `lupdate` to generate the `.ts` files.
      - Use `update_Qt_translations.py` to apply translations from `Source\Resource\Plugin\Language\*.csv` files.
      - Run `lrelease` to generate the `.qm` files.

2. **Verify the Output**:
    - Check the `Source\Resource\Translations` folder to ensure that the `.ts` files have been generated/updated.
    - Verify that the `.qm` files have been generated/updated in the same directory as above.
