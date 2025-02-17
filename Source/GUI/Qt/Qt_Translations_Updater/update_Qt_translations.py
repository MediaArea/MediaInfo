# Update Qt translations .ts files with translations from MediaInfo's Languages.csv
# Script generated with Qwen2.5-Coder-32B-Instruct

import os
import csv
import xml.etree.ElementTree as ET
import sys

def load_translations(csv_file):
    translations = {}
    with open(csv_file, mode='r', encoding='utf-8') as file:
        reader = csv.DictReader(file, delimiter=';')
        for row in reader:
            source = row['  Language_ISO639']
            translations[source] = {lang: row[lang] for lang in row if lang != '  Language_ISO639'}
    return translations

def clean_translation(translation):
    if translation is not None:
        return translation.replace('\\r\\n', '\n')
    return translation

def update_ts_file(ts_file, translations, lang_code):
    tree = ET.parse(ts_file)
    root = tree.getroot()

    # Update the TS tag with language and sourcelanguage attributes
    root.set('language', lang_code.replace('-', '_'))
    root.set('sourcelanguage', 'en')

    for context in root.findall('context'):
        for message in context.findall('message'):
            source = message.find('source').text
            translation_tag = message.find('translation')

            if source in translations:
                lang_translation = translations[source].get(lang_code, '')
                if not lang_translation:
                    lang_translation = translations[source].get('en', '')

                lang_translation = clean_translation(lang_translation)

                if translation_tag is None:
                    translation_tag = ET.SubElement(message, 'translation')
                translation_tag.text = lang_translation

                # Remove type="unfinished" if present
                if 'type' in translation_tag.attrib and translation_tag.attrib['type'] == 'unfinished':
                    del translation_tag.attrib['type']
            elif source == 'MediaInfo v%1\nCopyright (C) 2002-2025 MediaArea.net SARL\n':
                # Special case handling for Copyright
                copyright_translation = translations['Copyright'].get(lang_code, '')
                if not copyright_translation:
                    copyright_translation = translations['Copyright'].get('en', '')

                copyright_translation = clean_translation(copyright_translation)

                if translation_tag is None:
                    translation_tag = ET.SubElement(message, 'translation')
                
                # Replace only the word "Copyright" with the translated version
                translation_text = source.replace('Copyright', copyright_translation)
                translation_tag.text = translation_text

                # Remove type="unfinished" if present
                if 'type' in translation_tag.attrib and translation_tag.attrib['type'] == 'unfinished':
                    del translation_tag.attrib['type']
            elif source == 'Translator : Zen':
                # Special case handling for Translator
                translator_translation = translations['Translator'].get(lang_code, '')
                if not translator_translation:
                    translator_translation = translations['Translator'].get('en', '')

                author_name = translations['  Author_Name'].get(lang_code, 'Zen')
                author_name = clean_translation(author_name)

                if translation_tag is None:
                    translation_tag = ET.SubElement(message, 'translation')
                
                # Replace "Translator" and "Zen" with the translated versions
                translation_text = f'{translator_translation} : {author_name}'
                translation_tag.text = translation_text

                # Remove type="unfinished" if present
                if 'type' in translation_tag.attrib and translation_tag.attrib['type'] == 'unfinished':
                    del translation_tag.attrib['type']

    tree.write(ts_file, encoding='utf-8', xml_declaration=True)

def process_ts_files(folder, csv_file):
    translations = load_translations(csv_file)
    for filename in os.listdir(folder):
        if filename.endswith('.ts'):
            lang_code = filename[:-3]  # Remove '.ts' extension
            ts_file = os.path.join(folder, filename)
            update_ts_file(ts_file, translations, lang_code)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python update_translations.py <ts_files_folder> <csv_file>")
        sys.exit(1)

    ts_files_folder = sys.argv[1]
    csv_file_path = sys.argv[2]
    process_ts_files(ts_files_folder, csv_file_path)
