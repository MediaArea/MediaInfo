#   Type: Python 3 script
# Author: Steven Sun <stevenlele@outlook.com>
#   Date: Feb 27, 2020
#  Notes: Put this script in "MediaInfoLib/Source/Resource/Text/Stream" and run.
#         It reads "Audio,General,Image,Menu,Other,Text,Video.csv" and generates
#         "Data_Comments.csv" in Excel dialect.

import csv

filenames = ('Audio.csv', 'General.csv', 'Image.csv',
             'Menu.csv', 'Other.csv', 'Text.csv', 'Video.csv')

csv.register_dialect('MediaInfo', delimiter=';')

index_key = 0
index_type_unit = 2
index_comment = 6

# initiate CSV header
comment_rows = [['Key', 'Type or Unit', 'Comment']]
comment_dict = {}

for file in filenames:
    with open(file, 'r', encoding='utf_8') as f:
        reader = csv.reader(f, dialect='MediaInfo')

        for row in reader:
            row_key = row[index_key]
            row_type_unit = row[index_type_unit]
            row_comment = row[index_comment]

            if row_key.endswith('/String'):
                # merge Key and Key/String
                row_key = row_key[:-7]
            elif row_key[:-1].endswith('/String'):
                # ignore Key/String1, 2, 3,...
                continue
            elif '/' in row_key:
                row_key = row_key.split('/')[-1]

            if row_key in comment_dict:
                # if row_key exists, append information
                if row_type_unit and row_type_unit not in comment_dict[row_key][0]:
                    comment_dict[row_key][0].append(row_type_unit)
                if row_comment and row_comment not in comment_dict[row_key][1]:
                    comment_dict[row_key][1].append(row_comment)
            elif row_type_unit or row_comment:
                # if row_key does not exist, add to dictionary
                # format: {'key': ([type_or_unit_1, ...], [comment_1, ...]), ...}
                list_type_unit = []
                list_comment = []
                if row_type_unit:
                    list_type_unit.append(row_type_unit)
                if row_comment:
                    list_comment.append(row_comment)
                comment_dict[row_key] = (list_type_unit, list_comment)

for key, value in sorted(comment_dict.items()):
    comment_rows.append([key, '; '.join(value[0]), '; '.join(value[1])])

with open('Data_Comments.csv', 'w', encoding='utf_8_sig', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(comment_rows)
