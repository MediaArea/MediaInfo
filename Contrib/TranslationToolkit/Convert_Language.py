#   Type: Python 3 script
# Author: Steven Sun <stevenlele@outlook.com>
#   Date: Feb 28, 2020
#  Notes: Put "Language.csv" in this folder and run the script.
#         It generates "Language_parsed.csv" for translation.
#         Download the latest file from "MediaInfo/Source/Resource".
#   Date: Mar 1, 2020
# Update: Fix bug in note adding (keyword mode).

import csv
import sys
import os

#################################
######### Configuration #########

# edit the filename if the CSV file is placed elsewhere
language_file = 'Language.csv'

# output filename
output_file = 'Language_parsed.csv'

# enter your language codes here
# it should be one in
# ar;be;bg;ca;cs;da;de;es;eu;fa;fr;gl;gr;hu;it;ja;ko;lt;nl;
# pl;pt;pt-BR;ro;ru;sk;sq;sv;th;tr;uk;zh-CN;zh-HK;zh-TW;hr;hy;ka
# the first one is target language
# the others are reference languages (if any)
# English (en) is the default source language
lang_codes = ['zh-CN', 'ja']

# comments file
comments_file = 'Data_Comments.csv'

# notes file
notes_file = 'Data_Notes.csv'

######### Configuration #########
#################################

csv.register_dialect('MediaInfo', delimiter=';')

if not os.path.exists(language_file):
    print('Error: Language.csv file does not exist!')
    sys.exit(1)

if not lang_codes:
    print('Error: No language code is specified!')
    sys.exit(1)

dict_comments = {}
if os.path.exists(comments_file):
    with open(comments_file, 'r', encoding='utf_8_sig') as f:
        reader = csv.reader(f)
        next(reader)  # skip header
        for row in reader:
            # key, type or unit, comment
            dict_comments[row[0]] = [row[1], row[2]]
else:
    print('Info: Comments file does not exist. Ignoring comments.')

notes = False
dict_notes_key = {}
dict_notes_keyword = {}
dict_notes_key_keyword = {}
if os.path.exists(notes_file):
    notes = True
    with open(notes_file, 'r', encoding='utf_8_sig') as f:
        reader = csv.reader(f)
        next(reader)  # skip header
        for row in reader:
            mode = row[1].lower().strip()
            if mode == '' or mode == 'keyword':
                dict_notes_keyword[row[0].lower().strip()] = row[2]
            elif mode == 'key':
                dict_notes_key[row[0]] = row[2]
            elif mode == 'key_keyword':
                dict_notes_key_keyword[row[0].strip()] = row[2]
else:
    print('Info: Notes file does not exist. Ignoring notes.')

output_rows = []

with open(language_file, 'r', encoding='utf_8') as f:
    reader = csv.reader(f, dialect='MediaInfo')

    header = next(reader)
    index_lang = []
    lang_codes.insert(0, 'en')
    for lang_code in lang_codes:
        if lang_code not in header:
            print(f'Error: Language code "{lang_code}" is not found in the language file!')
            sys.exit(1)
        index_lang.append(header.index(lang_code))

    row_header = ['Key']
    row_header.extend(lang_codes)
    if dict_comments:
        row_header.extend(['Type or Unit', 'Comment'])
    if notes:
        row_header.append('Notes')
    output_rows.append(row_header)

    for row in reader:
        key = row[0]
        row_string = [key]
        for index in index_lang:
            cell_content = row[index] if index < len(row) else ''
            if cell_content.startswith(' :'):
                row_string.append(f'"{cell_content}"')
            else:
                row_string.append(cell_content)
        if dict_comments:
            if key in dict_comments:
                row_string.extend(dict_comments[key])
            else:
                row_string.extend(['', ''])
        if notes:
            row_notes = []
            if key in dict_notes_key:
                row_notes.append(dict_notes_key[key])
            for key_keyword in dict_notes_key_keyword:
                if key_keyword in key:
                    row_notes.append(dict_notes_key_keyword[key_keyword])
            for keyword in dict_notes_keyword:
                if keyword in row_string[1].lower():
                    row_notes.append(dict_notes_keyword[keyword])
            row_string.append('; '.join(row_notes))
        output_rows.append(row_string)

with open(output_file, 'w', encoding='utf_8_sig', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(output_rows)

print('Info: Parsing completed!')
