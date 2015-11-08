#!/bin/bash

csvdbdir="../../../Resource/Plugin/Language"
lprojdir=".."

update_lang_csv () {
    echo "Updating lang.csv in $language"
    source_lang=$language
    if [ "$language" == "English" ]; then
        source_lang="en"
    elif [ "$language" == "el" ]; then
        source_lang="gr"
    elif [ "$language" == "pt" ]; then
        source_lang="pt-BR"
    elif [ "$language" == "pt-PT" ]; then
        source_lang="pt"
    elif [ "$language" == "zh-Hans" ]; then
        source_lang="zh-CN"
    elif [ "$language" == "zh-Hant" ]; then
        source_lang="zh-TW"
    fi;
    source_file="${csvdbdir}/${source_lang}.csv"
    
    if [ ! -f "${source_file}" ]; then
        echo "Error: Source file (${source_file}) does not exist, skipping."
    else
        cp "${source_file}" ${lprojdir}/${language}.lproj/lang.csv
    fi;
}

update_all_csv () {
    for lproj in `ls $lprojdir/ | grep lproj | sed 's/.lproj//g'`; do
        language=$lproj;
        update_lang_csv;
    done;
    #cd `pwd`
}

#main()

if [ "$1" == "" ]; then
 echo "Usage: $0 <language>"
 echo "Usage: $0 all"
 exit
fi;


if [ "$1" == "all" ]; then
    update_all_csv;
elif [ ! -d "$lprojdir/$1.lproj" ]; then
    echo "$1.lproj doesnt exist. exiting"
    exit
else
    language="$1"
    update_lang_csv;
fi;

