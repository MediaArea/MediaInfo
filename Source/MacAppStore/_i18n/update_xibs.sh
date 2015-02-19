#!/bin/bash

base_language="English.lproj"

update_lang_xibs () {
    echo "Updating xibs in $language"
    #echo "`pwd` ibtool --strings-file $language/About.strings --write $language/About.xib $base_language/About.xib"
    ibtool --strings-file $language/About.strings --write $language/About.xib $base_language/About.xib
    ibtool --strings-file $language/MainMenu.strings --write $language/MainMenu.xib $base_language/MainMenu.xib
    ibtool --strings-file $language/MyWindow.strings --write $language/MyWindow.xib $base_language/MyWindow.xib
    ibtool --strings-file $language/Preferences.strings --write $language/Preferences.xib $base_language/Preferences.xib
}

update_all_xibs () {
    cd ..
    for lproj in `ls . | grep lproj`; do
        language=$lproj;
        if [ $language != "$base_language" ]; then
            update_lang_xibs;
        fi;
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
    update_all_xibs;
elif [ ! -d "../$1.lproj" ]; then
    echo "$1.lproj doesnt exist. exiting"
    exit
else
    language="$1.lproj"
    cd ..
    #ls .
    update_lang_xibs;
    #cd `pwd`
fi;

