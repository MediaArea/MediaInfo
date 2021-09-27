# MediaInfo Translation Toolkit

| Author                              | Date         |
| ----------------------------------- | ------------ |
| Steven Sun (stevenlele@outlook.com) | Feb 28, 2020 |

This toolkit helps you translate the MediaInfo software.

## Usage

### Get the language file

Download the latest `Language.csv` file [here](https://github.com/MediaArea/MediaInfo/blob/master/Source/Resource/Language.csv).

### Parse the language file

1. Put `Language.csv` in this folder.

2. Install [Python 3](https://www.python.org/downloads/) if you don't have it.

3. Open `Convert_Language.py` with your text editor and edit the configurations. You need to enter your language codes there.

4. Run `Convert_Language.py` with Python 3, and you will get `Language_parsed.csv`.

### Translate

Open `Language_parsed.csv` with Microsoft Excel and put your translation in the third column (your language).

Tips:

1. You will see comments and notes in the last 3 columns.

2. You can enable “Wrap Text” to see the full text in a cell.

3. You can save the file as `.xlsx` to preserve your settings when translating.

### Compile

1. After you finish translating, save the translated file as `Language_parsed.csv`.

2. Put the file in this folder, along with the original `Language.csv`.

3. (optional) Open `Convert_Compile.py` with your text editor and edit the configurations.

4. Run `Convert_Compile.py` with Python 3, and you will get `Language_translated.csv`.

5. You will also get a single language CSV file. For example, if your target language is `zh-CN`, you will get `zh-CN.csv`. Copy it to `%ProgramFiles%\MediaInfo\Plugin\Language\` and `%APPDATA%\MediaInfo\Plugin\Language\` and overwrite the original file. Then you can see the changes in the software.

### Submit

Create a pull request to merge your translated file `Language_translated.csv`. If you don't know how, create an [issue](https://github.com/MediaArea/MediaInfo/issues) and attach your file.

## About the files

### Data files

The CSV files are Excel-style. You can open them with Microsoft Excel.

- `Data_Comments.csv`: Official comments from `MediaInfoLib` source code. This is used in parsing the language file.
- `Data_Notes.csv`: Translation notes that I wrote. This includes suggestions, links to reference web pages, etc. This is used in parsing the language file.
- `Data_Levels_Info.md`: Official information about metadata levels, from `MediaInfoLib` source code. This is not used in the scripts. You can check the chart for reference.

The included `Data_Comments.csv` and `Data_Levels_Info.md` are generated from version [af00508](https://github.com/MediaArea/MediaInfoLib/commit/af0050890690f41d463c410c96f42d01bacd497c) (Feb 11, 2020).

### Scripts

- `Convert_Comments.py`: Converts official comments from `MediaInfoLib` source code into Excel-style CSV file. You don’t have to use this script. Just use the included comments file instead.

- `Convert_Language.py`: Converts `Language.csv` into your target language file and adds comments and notes.

- `Convert_Compile.py`: Fills the translated text in `Language_parsed.csv` into the original `Language.csv`, generating a full CSV file. It also generates a single language CSV file for testing.
