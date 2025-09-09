#!/bin/bash

update_strings() {
    local locale="$1"
    local template="$2"
    local lines="$(grep '' -c English.lproj/$template)"

    if [ "$locale" != "English" ] ; then
        cp -f "English.lproj/$template" "$locale.lproj/$template"
    fi

    for line in $(seq 1 $lines) ; do
        #TODO: replace by perl to use non-greedy matching instead of [^\"]*
        local old=$(sed -n "${line}s/^\".*\" = \"\([^\"]*\)\"; \/\* .* \*\//\1/p" "$locale.lproj/$template")
        old="${old//\\r/}"
        old="${old//\\n/\\\\n}"
        old="${old//\//\\/}"
        
        local title=$(sed -n "${line}s/^\".*\" = \".*\"; \/\*.* csv=\"\([^\"]*\)\";.* \*\//\1/p" "$locale.lproj/$template")
        local prefix=$(sed -n "${line}s/^\".*\" = \".*\"; \/\*.* prefix=\"\([^\"]*\)\";.* \*\//\1/p" "$locale.lproj/$template")
        local suffix=$(sed -n "${line}s/^\".*\" = \".*\"; \/\*.* suffix=\"\([^\"]*\)\";.* \*\//\1/p" "$locale.lproj/$template")

        if [ -z "$title" ] ; then
            continue
        fi

        local value="$(grep "^$title;" $locale.lproj/lang.csv | cut -d ';' -f 2-)"

        if [ -z "$value" ] ; then
            value="$(grep "^$title;" English.lproj/lang.csv | cut -d ';' -f 2-)"
            if [ -z "$value" ] ; then
                echo "warning: no entry found for label $title in base locale"
                continue
            fi
            echo "warning: no translation found for label $title in locale $locale"
        fi
        value="${value//\\r/}"
        value="${value//\\n/\\\\n}"
        value="${value//\//\\/}"
        sed -i "" "${line}s/\(^\".*\" = \"\)$old\"/\1$prefix$value$suffix\"/g" "$locale.lproj/$template"
    done

    if [ -e "English.lproj/${template%.strings}.xib" ] ; then
        ibtool --strings-file "$locale.lproj/$template" --write "$locale.lproj/${template%.strings}.xib" "English.lproj/${template%.strings}.xib"
    fi
}

# Ensure base csv is updated first
tr -d '\r' < "../../Resource/Plugin/Language/en.csv" > "English.lproj/lang.csv"

for source in ../../Resource/Plugin/Language/*.csv; do
   locale=$(basename -s ".csv" "$source")

    if [ "$locale" == "en" ] ; then
        locale="English"
    elif [ "$locale" == "gr" ] ; then
        locale="el"
    elif [ "$locale" == "pt" ] ; then
        locale="pt-PT"
    elif [ "$locale" == "zh-CN" ] ; then
        locale="zh-Hans"
    elif [ "$locale" == "zh-TW" ] ; then
        locale="zh-Hant"
    fi

    if [ "$locale" != "English" ] ; then
        rm -fr "$locale.lproj"
        mkdir "$locale.lproj"
    fi

    tr -d '\r' < "$source" > "$locale.lproj/lang.csv"
    update_strings "$locale" "About.strings"
    update_strings "$locale" "CompareView.strings"
    update_strings "$locale" "InfoPlist.strings"
    update_strings "$locale" "Localizable.strings"
    update_strings "$locale" "MainMenu.strings"
    update_strings "$locale" "MyWindow.strings"
    update_strings "$locale" "Preferences.strings"
    update_strings "$locale" "ProgressDialog.strings"
    update_strings "$locale" "ServicesMenu.strings"
    update_strings "$locale" "Subscribe.strings"
    update_strings "$locale" "TreeView.strings"
done
