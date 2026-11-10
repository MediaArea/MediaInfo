#!/usr/bin/env bash

script_path="${PWD}/test"
. "${script_path}/helpers.sh"

test="filename"

# file names and texts with non-ASCII characters with an ASCII locale (e.g. Docker images, ssh or cron without locale
# variable, macOS apps launched from the Finder), a UTF-8 locale, a Latin-1 locale and an EUC-JP locale
# (on Windows, the command line and the file names are UTF-16, without locale conversion)

mediainfo="${PWD}/mediainfo"

# same results with and without Unicode, and with the C API of the library loaded at run time

python=
for command in python3 python ; do
    if command -v "${command}" >/dev/null 2>&1 && "${command}" -c '' >/dev/null 2>&1 ; then
        python="${command}" # JSON and XML checks
        break
    fi
done

# UTF-8 names: Latin-1 range, CJK, outside of the BMP
names=($'caf\xC3\xA9' $'\xC3\x85snan \xC3\xA9 \xE6\xB5\x8B\xE8\xAF\x95' $'\xF0\x9F\x8E\xB5')
title=$'caf\xC3\xA9 \xE6\xB5\x8B\xE8\xAF\x95 \xF0\x9F\x8E\xB5'

# names for the locales which are neither ASCII nor UTF-8, in the encoding of the locale, and in UTF-8
name_latin1=$'caf\xE9'
name_eucjp=$'\xA5\xC6\xA5\xB9\xA5\xC8'
name_eucjp_utf8=$'\xE3\x83\x86\xE3\x82\xB9\xE3\x83\x88'
name_gb18030=$'\xB2\xE2\xCA\xD4'
name_gb18030_utf8=$'\xE6\xB5\x8B\xE8\xAF\x95'

# macOS only accepts valid UTF-8 for file names, the program converts them whatever the locale
# Windows only accepts UTF-16, Git Bash converts the names which are not valid UTF-8 as another name
names_utf8=
if test -n "${WINDOWS}" || [[ "${OSTYPE}" == "darwin"* ]] ; then
    names_utf8=1
fi

# value ${1} on ${2} bytes, little endian or big endian if ${3} is "be", as printf escapes
int_bytes() {
    local value="${1}" count="${2}" escapes= i byte
    for ((i = 0; i < count; i++)) ; do
        byte=$(( (value >> (i * 8)) & 255 ))
        if [ "${3}" == "be" ] ; then
            escapes="$(printf '\\x%02x' ${byte})${escapes}"
        else
            escapes="${escapes}$(printf '\\x%02x' ${byte})"
        fi
    done
    printf '%s' "${escapes}"
}

# FLAC file ${1} with the title ${2} (Vorbis comment, UTF-8), returns 1 if the file can not be created
flac_file() {
    local comment="TITLE=${2}"
    local size=$(( 16 + ${#comment} )) # length in bytes, LC_ALL=C in the scripts
    {
        printf 'fLaC\x00\x00\x00\x22' # STREAMINFO, 34 bytes
        printf '\x10\x00\x10\x00\x00\x00\x00\x00\x00\x00' # block and frame sizes
        printf '\x0A\xC4\x42\xF0\x00\x00\x00\x00' # 44100 Hz, 2 channels, 16 bits, unknown duration
        printf '\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' # MD5
        printf "\\x84$(int_bytes ${size} 3 be)" # last block, VORBIS_COMMENT
        printf "$(int_bytes 4 4)test$(int_bytes 1 4)$(int_bytes ${#comment} 4)"
        printf '%s' "${comment}"
    } 2>/dev/null >"${1}"
}

# FLAC file ${1} after an ID3v2 tag with a title, encoding and text as printf escapes in ${2}, of ${3} bytes, returns 1 if the file can not be created
id3_file() {
    {
        printf "ID3\\x03\\x00\\x00\\x00\\x00\\x00$(int_bytes $((10 + ${3})) 1)TIT2$(int_bytes ${3} 4 be)\\x00\\x00${2}"
        printf 'fLaC\x80\x00\x00\x22' # last block, STREAMINFO, 34 bytes
        printf '\x10\x00\x10\x00\x00\x00\x00\x00\x00\x00' # block and frame sizes
        printf '\x0A\xC4\x42\xF0\x00\x00\x00\x00' # 44100 Hz, 2 channels, 16 bits, unknown duration
        printf '\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' # MD5
    } 2>/dev/null >"${1}"
}

# Matroska file ${1} with a Segment of size 0, the program writes in a copy of the file a size which is not 0, returns 1 if the file can not be created
mkv_file() {
    {
        printf '\x1A\x45\xDF\xA3\x8B\x42\x82\x88matroska' # EBML header, DocType
        printf '\x18\x53\x80\x67\x01\x00\x00\x00\x00\x00\x00\x00' # Segment, size 0
    } 2>/dev/null >"${1}"
}

# PNG file ${1}, 1 pixel, returns 1 if the file can not be created
png_file() {
    {
        printf '\x89PNG\r\n\x1A\n'
        printf '\x00\x00\x00\x0DIHDR\x00\x00\x00\x01\x00\x00\x00\x01\x08\x00\x00\x00\x00\x3A\x7E\x9B\x55'
        printf '\x00\x00\x00\x0AIDAT\x78\x9C\x63\x60\x00\x00\x00\x02\x00\x01\x73\x75\x01\x18'
        printf '\x00\x00\x00\x00IEND\xAE\x42\x60\x82'
    } 2>/dev/null >"${1}"
}

# result of the check ${1}: the output of the last command must be ${2}
check_output() {
    local check="${1}"
    local expected="${2}"
    local shown

    if [ "${cmd_status}" == "timeout" ] ; then
        echo "NOK: ${test}/${config}, ${check}, no result after ${timeout} s" >&${fd}
        status=1
    elif [ "${cmd_status}" != "0" ] || [ "${cmd_stdout}" != "${expected}" ] ; then
        printf -v shown '%q' "${cmd_stdout}"
        echo "NOK: ${test}/${config}, ${check}, exit code ${cmd_status}, output ${shown}" >&${fd}
        status=1
    else
        echo "OK: ${test}/${config}, ${check}" >&${fd}
    fi
}

# result of the check ${1}: the exit code of the last command must be ${2}
check_status() {
    local check="${1}"
    local expected="${2}"

    if [ "${cmd_status}" == "${expected}" ] ; then
        echo "OK: ${test}/${config}, ${check}" >&${fd}
    else
        echo "NOK: ${test}/${config}, ${check}, exit code ${cmd_status}" >&${fd}
        status=1
    fi
}

# result of the check ${1}: the Python code ${2} must accept the output of the last command (file ${3}), with the arguments ${4}...
check_python() {
    local check="${1}"
    local code="${2}"
    shift 2

    if [ "${cmd_status}" == "timeout" ] ; then
        echo "NOK: ${test}/${config}, ${check}, no result after ${timeout} s" >&${fd}
        status=1
    elif [ "${cmd_status}" != "0" ] || ! "${python}" -c "${code}" "${@}" >/dev/null 2>&1 ; then
        echo "NOK: ${test}/${config}, ${check}, exit code ${cmd_status}, output not accepted" >&${fd}
        status=1
    else
        echo "OK: ${test}/${config}, ${check}" >&${fd}
    fi
}

# result of the check ${1}: the directory ${2} must contain only the file ${3} and the files ${4}...
check_created() {
    local check="${1}"
    local directory="${2}"
    shift 2
    local created=("${directory}"/*) shown expected=("${@}") expected_paths=() file

    for file in "${expected[@]}" ; do
        expected_paths=("${expected_paths[@]}" "${directory}/${file}")
    done
    if [ "$(printf '%s\n' "${created[@]}" | sort)" == "$(printf '%s\n' "${expected_paths[@]}" | sort)" ] ; then
        echo "OK: ${test}/${config}, ${check}" >&${fd}
    else
        printf -v shown '%q ' "${created[@]##*/}"
        echo "NOK: ${test}/${config}, ${check}, created: ${shown}" >&${fd}
        status=1
    fi
}

# template file, text in an argument, log file, fixed copy of a file and external command in the directory ${1}, with the
# name ${2} in the encoding of the arguments and of the output and ${3} in the encoding of the file system, checks named with ${4}
check_names() {
    local path="${1}" name_local="${2}" name_system="${3}" label="${4}"
    local log_path="${path}/log" fixed_path="${path}/fixed" ffmpeg_path="${path}/bin_${name_system}" ffmpeg_argument= line

    # template file with a non-ASCII name
    printf '%s' 'General;%CompleteName%|%FileExtension%' >"${path}/template_${name_system}.txt" || fatal "internal" "file not created"
    flac_file "${path}/plain.flac" "Plain title" || fatal "internal" "file not created"
    run_command "${config}" "${mediainfo}" "--Output=file://${path}/template_${name_local}.txt" "${path}/plain.flac"
    check_output "template file${label}" "${path}/plain.flac|flac"

    # non-ASCII text in an argument which is not a file name
    run_command "${config}" "${mediainfo}" "--Output=General;${name_local}|%FileExtension%" "${path}/plain.flac"
    check_output "non-ASCII text in an argument${label}" "${name_local}|flac"

    # log file with a non-ASCII name
    mkdir "${log_path}" || fatal "internal" "mkdir command failed"
    run_command "${config}" "${mediainfo}" "--LogFile=${log_path}/log_${name_local}.txt" "${path}/plain.flac"
    check_created "log file${label}" "${log_path}" "log_${name_system}.txt"

    # fixed copy of a file with a non-ASCII name
    mkdir "${fixed_path}" || fatal "internal" "mkdir command failed"
    mkv_file "${fixed_path}/${name_system}.mkv" || fatal "internal" "file not created"
    run_command "${config}" "${mediainfo}" --TryToFix=1 '--Output=General;%FileExtension%' "${fixed_path}/${name_local}.mkv"
    check_created "fixed copy of a file${label}" "${fixed_path}" "${name_system}.mkv" "${name_system}.mkv.Fixed"

    # external command (FFmpeg) found with a non-ASCII name in PATH, with a non-ASCII file name as argument
    if test -z "${WINDOWS}" ; then
        mkdir "${ffmpeg_path}" || fatal "internal" "mkdir command failed"
        printf '#!/bin/sh\nfor argument in "${@}" ; do printf "%%s\\n" "${argument}" ; done >"${0}.txt"\n' >"${ffmpeg_path}/ffmpeg-ma" || fatal "internal" "file not created"
        chmod +x "${ffmpeg_path}/ffmpeg-ma" || fatal "internal" "chmod command failed"
        png_file "${path}/${name_system}.png" || fatal "internal" "file not created"
        PATH="${ffmpeg_path}:${PATH}" run_command "${config}" "${mediainfo}" --Enable_FFmpeg=1 '--Output=General;%FileExtension%' "${path}/${name_local}.png"
        if [ -e "${ffmpeg_path}/ffmpeg-ma.txt" ] ; then
            while IFS= read -r line ; do
                if [ "${line}" == "${path}/${name_system}.png" ] ; then
                    ffmpeg_argument=1
                fi
            done <"${ffmpeg_path}/ffmpeg-ma.txt"
        fi
        if test -n "${ffmpeg_argument}" ; then
            echo "OK: ${test}/${config}, external command${label}" >&${fd}
        else
            echo "NOK: ${test}/${config}, external command${label}, not called or without the file name, exit code ${cmd_status}" >&${fd}
            status=1
        fi
    fi
}

files_path="${temp_path}/files"
mkdir "${files_path}" || fatal "internal" "mkdir command failed"
files=(ascii.flac)
for name in "${names[@]}" ; do
    files=("${files[@]}" "${name}.flac")
done
for file in "${files[@]}" ; do
    flac_file "${files_path}/${file}" "${title}" || fatal "internal" "file not created"
done
ascii_title_file="${names[0]}_ascii_title.flac" # only the file name is not ASCII
flac_file "${files_path}/${ascii_title_file}" "Plain title" || fatal "internal" "file not created"

# directory with UTF-8 names, and names which are not valid UTF-8 (Latin-1, EUC-JP, overlong sequences of "/" and "..")
list_path="${temp_path}/list"
sub_directory="sub_${names[0]}"
mkdir -p "${list_path}/${sub_directory}" || fatal "internal" "mkdir command failed"
expected=()
for file in "${files[@]}" "${sub_directory}/inner.flac" ; do
    flac_file "${list_path}/${file}" "${title}" || fatal "internal" "file not created"
    expected=("${expected[@]}" "${list_path}/${file}")
done
expected_all=("${expected[@]}")
if test -z "${names_utf8}" ; then
    for name in "${name_latin1}.flac" "${name_eucjp}.flac" $'\xC0\xAF' $'a\xC0\xAF''b.flac' $'\xC0\xAE\xC0\xAE' ; do
        flac_file "${list_path}/${name}" "${title}" || fatal "internal" "file not created"
        expected_all=("${expected_all[@]}" "${list_path}/${name}")
    done
fi
expected_sorted="$(printf '%s\n' "${expected[@]}" | sort)"
expected_all_sorted="$(printf '%s\n' "${expected_all[@]}" | sort)"

for config in ${locale_configs} ; do

    # first name in the encoding of the arguments and of the output, and in the encoding of the file system, and the same text in UTF-8
    # (macOS: wchar_t is not Unicode with some of these locales, iconv converts them)
    other_locale=1
    case "${config}" in
        latin1)
            name_local="${name_latin1}"
            text_utf8="${names[0]}"
            ;;
        eucjp)
            name_local="${name_eucjp}"
            text_utf8="${name_eucjp_utf8}"
            ;;
        gb18030)
            name_local="${name_gb18030}"
            text_utf8="${name_gb18030_utf8}"
            ;;
        *)
            other_locale=
            name_local="${names[0]}"
            name_system="${names[0]}"
            ;;
    esac
    if test -n "${other_locale}" ; then
        name_system="${name_local}"
        if test -n "${names_utf8}" ; then
            name_system="${text_utf8}" # UTF-8 names of the file system
        fi
    fi
    config_path="${temp_path}/config_${config}"
    mkdir "${config_path}" || fatal "internal" "mkdir command failed"

    if test -n "${other_locale}" ; then
        # file with a name in the encoding of the locale, not valid UTF-8 so it is decoded with the locale also on macOS
        flac_file "${config_path}/${name_system}.flac" "Plain title" || fatal "internal" "file not created"
        printf -v shown '%q' "${name_local}.flac"
        run_command "${config}" "${mediainfo}" '--Output=General;%CompleteName%|%Title%' "${config_path}/${name_local}.flac"
        check_output "name and title of ${shown}" "${config_path}/${name_local}.flac|Plain title"

        # file with a UTF-8 name: bytes kept with the encoding of the locale, the file must still be opened
        printf -v shown '%q' "${names[0]}.flac"
        run_command "${config}" "${mediainfo}" '--Output=General;%FileExtension%' "${files_path}/${names[0]}.flac"
        check_output "file ${shown} opened" "flac"

        # title of the file (UTF-8) in the encoding of the locale
        flac_file "${config_path}/title_local.flac" "${text_utf8}" || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_local.flac"
        check_output "title in the encoding of the locale" "${name_local}"

        # name which is not valid in the encoding of the locale, the file with this name, with the same bytes as name (not on macOS and Windows)
        if test -z "${names_utf8}" && [ "${config}" != "latin1" ] ; then
            flac_file "${config_path}/${name_latin1}.flac" "Plain title" || fatal "internal" "file not created"
            run_command "${config}" "${mediainfo}" '--Output=General;%CompleteName%|%Title%' "${config_path}/${name_latin1}.flac"
            check_output "name which is not valid in the encoding of the locale" "${config_path}/${name_latin1}.flac|Plain title"
        fi
    else
        for file in "${files[@]}" "${ascii_title_file}" ; do
            path="${files_path}/${file}"
            printf -v shown '%q' "${file}"
            expected_title="${title}"
            if [ "${file}" == "${ascii_title_file}" ] ; then
                expected_title="Plain title"
            fi

            # file opened, name and title in UTF-8 (with an ASCII locale, not "?" or Latin-1 bytes)
            run_command "${config}" "${mediainfo}" '--Output=General;%CompleteName%|%Title%' "${path}"
            check_output "name and title of ${shown}" "${path}|${expected_title}"

            if [ -n "${python}" ] ; then
                run_command "${config}" "${mediainfo}" --Output=JSON "${path}"
                check_python "JSON of ${shown}" 'import json, os, sys; Media=json.loads(open(sys.argv[1], "rb").read().decode("utf-8"))["media"]; sys.exit(Media["@ref"].encode("utf-8")!=os.fsencode(sys.argv[2]))' "${temp_path}/stdout" "${path}"
                run_command "${config}" "${mediainfo}" --Output=XML "${path}"
                check_python "XML of ${shown}" 'import sys, xml.dom.minidom; xml.dom.minidom.parse(sys.argv[1])' "${temp_path}/stdout"
            fi
        done

        # folder, name and extension of a file in a directory whose name is not ASCII (on Windows without Unicode, the second byte of
        # a character of a multibyte code page, e.g. "\" in cp932, is not in UTF-8 text)
        for name in "${names[@]}" "${name_eucjp_utf8}" ; do
            mkdir "${config_path}/dir_${name}" || fatal "internal" "mkdir command failed"
            flac_file "${config_path}/dir_${name}/file.flac" "Plain title" || fatal "internal" "file not created"
            printf -v shown '%q' "dir_${name}/file.flac"
            run_command "${config}" "${mediainfo}" '--Output=General;%FolderName%|%FileName%|%FileExtension%' "${config_path}/dir_${name}/file.flac"
            check_output "folder, name and extension of ${shown}" "${config_path}/dir_${name}|file|flac"
        done

        # titles in Latin-1 and in UTF-16 with a character outside of the BMP
        id3_file "${config_path}/title_latin1.flac" '\x00caf\xE9' 5 || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_latin1.flac"
        check_output "title in Latin-1" "${names[0]}"
        id3_file "${config_path}/title_utf16.flac" '\x01\xFF\xFE\x61\x00\xE9\x00\x4B\x6D\x3C\xD8\xB5\xDF\x62\x00' 15 || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_utf16.flac"
        check_output "title in UTF-16" $'a\xC3\xA9\xE6\xB5\x8B\xF0\x9F\x8E\xB5b'
        id3_file "${config_path}/title_utf16_inverted.flac" '\x02\xFF\xFE\x61\x00\xE9\x00' 7 || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_utf16_inverted.flac"
        check_output "title in UTF-16 big endian with the bytes of little endian" $'a\xC3\xA9'

        # names of the text output aligned by characters, also a translated name which is not ASCII: in bytes, the separator after the
        # name with an "e" with acute accent (2 bytes in UTF-8) is 1 byte after the separator after the ASCII name
        printf 'CompleteName;Nom complet \xC3\xA9\n' >"${config_path}/language.csv"
        run_command "${config}" "${mediainfo}" "--Language=file://${config_path}/language.csv" "${config_path}/title_latin1.flac"
        name_ascii=
        name_translated=
        while IFS= read -r line ; do
            case "${line}" in
                "Format "*) name_ascii="${line%%: *}" ;;
                $'Nom complet \xC3\xA9 '*) name_translated="${line%%: *}" ;;
            esac
        done <<< "${cmd_stdout}"
        if [ "${cmd_status}" == "0" ] && test -n "${name_ascii}" && [ "${#name_translated}" -eq "$(( ${#name_ascii} + 1 ))" ] ; then
            echo "OK: ${test}/${config}, alignment of a translated name which is not ASCII" >&${fd}
        else
            printf -v shown '%q' "${cmd_stdout}"
            echo "NOK: ${test}/${config}, alignment of a translated name which is not ASCII, exit code ${cmd_status}, output ${shown}" >&${fd}
            status=1
        fi

        # title which should be in UTF-8 but is not: the program tries Latin-1, which needs a failure of the UTF-8 decoding
        flac_file "${config_path}/title_not_utf8.flac" "${name_latin1}" || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_not_utf8.flac"
        check_output "title which is not valid UTF-8" "${names[0]}"

        # title with sequences which are not valid UTF-8 but accepted, with and without Unicode: an overlong sequence ("A"), a UTF-16
        # surrogate pair (one character) and a value after U+10FFFF (U+FFFD)
        flac_file "${config_path}/title_malformed.flac" $'a\xC1\x81\xED\xA0\xBC\xED\xBE\xB5\xF4\x90\x80\x80b' || fatal "internal" "file not created"
        run_command "${config}" "${mediainfo}" '--Output=General;%Title%' "${config_path}/title_malformed.flac"
        check_output "title with sequences which are not valid UTF-8" $'aA\xF0\x9F\x8E\xB5\xEF\xBF\xBDb'

        # name which is not valid UTF-8, next to the UTF-8 name of the same characters: the file with this name, with the same bytes
        # as name, with an ASCII and a UTF-8 locale, and valid JSON and XML (not on macOS and Windows, the program must end normally)
        if test -z "${names_utf8}" ; then
            flac_file "${config_path}/${name_latin1}.flac" "Plain title" || fatal "internal" "file not created"
            flac_file "${config_path}/${names[0]}.flac" "${title}" || fatal "internal" "file not created"
        fi
        run_command "${config}" "${mediainfo}" '--Output=General;%CompleteName%|%Title%' "${config_path}/${name_latin1}.flac"
        if test -z "${names_utf8}" ; then
            check_output "name which is not valid UTF-8" "${config_path}/${name_latin1}.flac|Plain title"
            run_command "${config}" "${mediainfo}" '--Output=General;%FileNameExtension%' "${config_path}/${name_latin1}.flac"
            check_output "name and extension which are not valid UTF-8" "${name_latin1}.flac"
            if [ -n "${python}" ] ; then
                run_command "${config}" "${mediainfo}" --Output=JSON "${config_path}/${name_latin1}.flac"
                check_python "JSON with a name which is not valid UTF-8" 'import json, sys; json.loads(open(sys.argv[1], "rb").read().decode("utf-8"))' "${temp_path}/stdout"
                run_command "${config}" "${mediainfo}" --Output=XML "${config_path}/${name_latin1}.flac"
                check_python "XML with a name which is not valid UTF-8" 'import sys, xml.dom.minidom; xml.dom.minidom.parse(sys.argv[1])' "${temp_path}/stdout"
                for format in EBUCore PBCore2 MPEG-7 ; do
                    run_command "${config}" "${mediainfo}" --Output=${format} "${config_path}/${name_latin1}.flac"
                    check_python "${format} with a name which is not valid UTF-8" 'import sys, xml.dom.minidom; xml.dom.minidom.parse(sys.argv[1])' "${temp_path}/stdout"
                done
            fi
            mkdir "${config_path}/not_utf8" || fatal "internal" "mkdir command failed"
            check_names "${config_path}/not_utf8" "${name_latin1}" "${name_latin1}" " (not valid UTF-8)"
        elif [ "${cmd_status}" == "timeout" ] || [ "${cmd_status}" -ge "128" ] ; then
            echo "NOK: ${test}/${config}, name which is not valid UTF-8, exit code ${cmd_status}" >&${fd}
            status=1
        else
            echo "OK: ${test}/${config}, name which is not valid UTF-8" >&${fd}
        fi
    fi

    check_names "${config_path}" "${name_local}" "${name_system}" ""

    # name of a file which does not exist: file not opened, so an error, also when another file is opened
    run_command "${config}" "${mediainfo}" "${config_path}/missing.flac"
    check_status "file which does not exist" 1
    run_command "${config}" "${mediainfo}" "${config_path}/missing.flac" "${files_path}/ascii.flac"
    check_status "file which does not exist and file opened" 1

    # directory listing: it must end, with all the names with their bytes with all the locales, else (macOS, Windows) the UTF-8 names of the file
    # system, in the encoding of a locale which is neither ASCII nor UTF-8
    run_command "${config}" "${mediainfo}" "${list_path}"
    listed=
    count=0
    while IFS= read -r line ; do
        if [[ "${line}" == "Complete name"* ]] ; then
            listed="${listed}${line#*: }"$'\n'
            count=$((count + 1))
        fi
    done <<< "${cmd_stdout}"
    if test -n "${WINDOWS}" ; then
        listed="${listed//\\//}"
    fi
    listed_sorted="$(printf '%s' "${listed}" | sort)"
    # with a locale which is neither ASCII nor UTF-8, bytes from 0x80 to 0x9F may be C1 control characters, replaced in the output
    names_listed=1
    for name in "${expected_all[@]}" ; do
        case "${name}" in
            *[$'\x80'-$'\x9F']*) ;;
            *) printf '%s' "${listed}" | grep -F -x -q -- "${name}" || names_listed= ;;
        esac
    done
    if [ "${cmd_status}" == "timeout" ] ; then
        echo "NOK: ${test}/${config}, directory listing, no result after ${timeout} s" >&${fd}
        status=1
    elif test -z "${names_utf8}" && [ "${listed_sorted}" == "${expected_all_sorted}" ] ; then
        echo "OK: ${test}/${config}, directory listing, all names" >&${fd}
    elif test -z "${names_utf8}" && test -n "${other_locale}" && [ "${count}" -eq "${#expected_all[@]}" ] && test -n "${names_listed}" ; then
        echo "OK: ${test}/${config}, directory listing, all names (C1 control characters replaced)" >&${fd}
    elif test -n "${names_utf8}" && test -z "${other_locale}" && [ "${listed_sorted}" == "${expected_sorted}" ] ; then
        echo "OK: ${test}/${config}, directory listing" >&${fd}
    elif test -n "${names_utf8}" && test -n "${other_locale}" && [ "${count}" -eq "${#expected[@]}" ] ; then
        echo "OK: ${test}/${config}, directory listing, names in the encoding of the locale" >&${fd} # "?" for the characters not in it
    else
        printf -v shown '%q' "${listed_sorted}"
        echo "NOK: ${test}/${config}, directory listing, exit code ${cmd_status}, listed: ${shown}" >&${fd}
        status=1
    fi
done

exit ${status}
