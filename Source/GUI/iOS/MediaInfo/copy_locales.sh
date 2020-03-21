#!/bin/bash

update_strings() {
    local locale="$1"
    local template="$2"
    local lines="$(grep '' -c Templates/$template)"

    cp -f "Templates/$template" "$locale.lproj/$template"

    for line in $(seq 1 $lines) ; do
        local title=$(sed -n "${line}s/^\".*\" = \".*\"; \/\* csv=\"\(.*\)\"; \*\//\1/p" "$locale.lproj/$template")

        if [ -z "$title" ] ; then
            continue
        fi

        local value="$(grep "^$title;" $locale.lproj/lang.csv | cut -d ';' -f 2-)"

        if [ -z "$value" ] ; then
            echo "warning: no translation found for label $title in locale $locale $value"
            continue
        fi

        value="${value//\\r/}"
        value="${value//\\n/\\\\n}"
        sed -i "" "${line}s/\(^\".*\" = \"\).*/\1$value\"; \/* csv=\"$title\"; *\//g" "$locale.lproj/$template"
    done
}

for source in ../../../Resource/Plugin/Language/*.csv; do
   locale=$(basename -s ".csv" "$source")

    if [ "$locale" == "gr" ] ; then
        locale="el"
    elif [ "$locale" == "zh-CN" ] ; then
        locale="zh-Hans"
    elif [ "$locale" == "zh-TW" ] ; then
        locale="zh-Hant"
    fi

    mkdir "$locale.lproj"

    tr -d '\r' < "$source" > "$locale.lproj/lang.csv"
    update_strings "$locale" "Main.strings"
    update_strings "$locale" "Core.strings"

    if [ "$locale" == "en" ] ; then
        tr -d '\r' < "$source" > "Base.lproj/lang.csv"
        update_strings "Base" "Main.strings"
        update_strings "Base" "Core.strings"
    fi
done
