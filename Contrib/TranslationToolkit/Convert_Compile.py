#   Type: Python 3 script
# Author: Steven Sun <stevenlele@outlook.com>
#   Date: Feb 28, 2020
#  Notes: Put "Language.csv" and "Language_parsed.csv" in this folder
#         and run the script. It converts the parsed file back into the
#         original CSV, and generates a single language file for testing.

import csv
import sys
import os

#################################
######### Configuration #########

# edit the filename if the CSV file is placed elsewhere
language_file = 'Language.csv'
translated_file = 'Language_parsed.csv'

# output filename
output_file = 'Language_translated.csv'

######### Configuration #########
#################################

csv.register_dialect('MediaInfo', delimiter=';')

if not os.path.exists(language_file):
    print('Error: Language.csv file does not exist!')
    sys.exit(1)

if not os.path.exists(translated_file):
    print('Error: Translated file does not exist!')
    sys.exit(1)

output_rows = []
output_lang_rows = []
translated_rows = []

with open(translated_file, 'r', encoding='utf_8') as f:
    reader = csv.reader(f)

    header = next(reader)
    lang_code = header[2]
    for row in reader:
        key = row[0]
        translated = row[2]
        translated_rows.append((key, translated))

with open(language_file, 'r', encoding='utf_8') as f:
    reader = csv.reader(f, dialect='MediaInfo')

    header = next(reader)
    index_lang = header.index(lang_code)
    output_rows.append(header)
    output_lang_rows.append([header[0], header[index_lang]])

    for row in reader:
        key = row[0]
        translated_row = translated_rows.pop(0)
        if key != translated_row[0]:
            print(f'Error: Key "{key}" does not match the translated file "{translated_rows[0][0]}".')
            print('Please check whether you have changed the translated file or not.')
            sys.exit(1)
        translated_string = translated_row[1]
        if translated_string.startswith('"') and translated_string.endswith('"'):
            translated_string = translated_string.strip('"')
        new_row = row
        new_row[index_lang] = translated_string
        output_rows.append(new_row)
        output_lang_rows.append([key, translated_string])

with open(output_file, 'w', encoding='utf_8', newline='') as f:
    writer = csv.writer(f, dialect='MediaInfo')
    writer.writerows(output_rows)

with open(f'{lang_code}.csv', 'w', encoding='utf_8', newline='') as f:
    writer = csv.writer(f, dialect='MediaInfo')
    writer.writerows(output_lang_rows)

print('Info: Compile completed!')
