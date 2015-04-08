#!/bin/bash

base_language="English"

update_lang_strings () {
    echo "Updating .strings in $language"
    ./create_lang_strings.pl $language
}

update_all_strings () {
    for lproj in `ls .. | grep lproj | sed 's/.lproj//g'`; do
        language=$lproj;
        if [ $language != "$base_language" ]; then
            update_lang_strings;
        fi;
    done;
}

#main()

if [ "$1" == "" ]; then
 echo "Usage: $0 <language>"
 echo "Usage: $0 all"
 exit
fi;


if [ "$1" == "all" ]; then
    update_all_strings;
elif [ ! -d "../$1.lproj" ]; then
    echo "$1.lproj doesnt exist. exiting"
    exit
else
    language=$1
    update_lang_strings;
fi;