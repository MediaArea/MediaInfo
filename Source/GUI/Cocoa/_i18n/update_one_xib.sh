#!/bin/bash

base_language="English.lproj"

create_localized_xib () {
    echo "(Re)creating $xibfile.xib in $language"
    ibtool --strings-file $language/$xibfile.strings --write $language/$xibfile.xib $base_language/$xibfile.xib
}

update_all_langs () {
    cd ..
    for lproj in `ls . | grep lproj`; do
        language=$lproj;
        if [ $language != "$base_language" ]; then
            create_localized_xib;
        fi;
    done;
    #cd `pwd`
}

#main()

if [ "$1" == "" ]; then
 echo "Usage: $0 MyWindow"
 exit
fi;

if [ ! -f "../$base_language/$1.xib" ]; then
    echo "$1.xib doesnt exist. exiting"
    exit
fi;

xibfile="$1"
update_all_langs;
