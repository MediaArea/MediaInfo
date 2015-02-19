#!/bin/bash

if [ "$1" == "" ]; then
 echo "Usage: $0 <language>"
 exit
fi;
lang=$1;

echo "New localization: $lang"

# In lproj replace lang.csv file with symlink (or just a copy) to appropriate lang file
rm -f ../$lang.lproj/lang.csv
ln -s ../Language/$lang.csv ../$lang.lproj/lang.csv

## Download Cyberduck .strings file and replace app name with %%app%%:
#wget -nv http://svn.cyberduck.ch/trunk/$lang.lproj/Main.strings -O ../Language/Main.$lang.orig.strings
#iconv -f utf-16 -t utf-8 ../Language/Main.$lang.orig.strings | sed 's/Cyberduck/%%app%%/g' | iconv -f utf-8 -t utf-16 > ../Language/Main.$lang.strings
#rm -f ../Language/Main.$lang.orig.strings

# Create(replace) localized .strings files in .lproj
./create_lang_strings.pl $lang

# Create(replace) localized .xib files in .lproj
./update_xibs.sh $lang