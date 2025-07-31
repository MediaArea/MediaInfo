# Update Qt translations .ts files with translations from MediaInfo's Languages.csv
# Script generated with Qwen2.5-Coder-32B-Instruct

import os
import csv
import xml.etree.ElementTree as ET
import sys

def load_translations_from_directory(directory):
    translations = {}
    for filename in os.listdir(directory):
        if filename.endswith('.csv'):
            lang_code = filename[:-4]  # Remove '.csv' extension
            csv_file = os.path.join(directory, filename)
            with open(csv_file, mode='r', encoding='utf-8') as file:
                reader = csv.DictReader(file, delimiter=';')
                for row in reader:
                    source = row['  Language_ISO639']
                    translations.setdefault(source, {})
                    translations[source][lang_code] = row[lang_code]
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
                copyright_translation = translations.get('Copyright', {}).get(lang_code, '')
                if not copyright_translation:
                    copyright_translation = translations.get('Copyright', {}).get('en', '')

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
                translator_translation = translations.get('Translator', {}).get(lang_code, '')
                if not translator_translation:
                    translator_translation = translations.get('Translator', {}).get('en', '')

                author_name = translations.get('  Author_Name', {}).get(lang_code, 'Zen')
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

def process_ts_files(folder, translations_dir):
    translations = load_translations_from_directory(translations_dir)
    for filename in os.listdir(folder):
        if filename.endswith('.ts'):
            lang_code = filename[:-3]  # Remove '.ts' extension
            ts_file = os.path.join(folder, filename)
            update_ts_file(ts_file, translations, lang_code)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python update_translations.py <ts_files_folder> <translations_directory>")
        sys.exit(1)

    ts_files_folder = sys.argv[1]
    translations_directory = sys.argv[2]
    process_ts_files(ts_files_folder, translations_directory)
