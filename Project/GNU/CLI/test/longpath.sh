#!/usr/bin/env bash

script_path="${PWD}/test"
. "${script_path}/helpers.sh"

test="longpath"
config="default"

# file and directory names longer than MAX_PATH (260 characters with the null character), the limit of the functions of Windows when the
# process does not support long paths (ZenLib adds then the extended-length prefix, not in the names it gives): file, directory listing,
# relative path, network path (Windows, administrative share of the drive of the temporary directory if available), log file, template
# file, fixed copy of a file

mediainfo="${PWD}/mediainfo"

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

# FLAC file ${1} with the title ${2} (Vorbis comment), returns 1 if the file can not be created
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

# Matroska file ${1} with a Segment of size 0, the program writes in a copy of the file a size which is not 0, returns 1 if the file can not be created
mkv_file() {
    {
        printf '\x1A\x45\xDF\xA3\x8B\x42\x82\x88matroska' # EBML header, DocType
        printf '\x18\x53\x80\x67\x01\x00\x00\x00\x00\x00\x00\x00' # Segment, size 0
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

# result of the check ${1}: the condition ${2}... must be true after a command which ended normally
check_true() {
    local check="${1}"
    shift

    if [ "${cmd_status}" == "timeout" ] || [ "${cmd_status}" != "0" ] ; then
        echo "NOK: ${test}/${config}, ${check}, exit code ${cmd_status}" >&${fd}
        status=1
    elif ! "${@}" ; then
        echo "NOK: ${test}/${config}, ${check}, not done" >&${fd}
        status=1
    else
        echo "OK: ${test}/${config}, ${check}" >&${fd}
    fi
}

# complete names of the files of the text output of the last command, with "/" as separator, sorted
listed() {
    local line names=
    while IFS= read -r line ; do
        if [[ "${line}" == "Complete name"* ]] ; then
            names="${names}${line#*: }"$'\n'
        fi
    done <<< "${cmd_stdout}"
    if test -n "${WINDOWS}" ; then
        names="${names//\\//}"
    fi
    printf '%s' "${names}" | sort | tr '\n' '|'
}

# directory tree longer than MAX_PATH also from the directory, each part shorter than the limit of a name (255)
directory="${temp_path}/longpath"
relative_directory=
for i in 1 2 3 4 5 6 ; do
    relative_directory="${relative_directory}${relative_directory:+/}directory${i}$(printf 'x%.0s' {1..40})"
done
first_directory="${relative_directory%%/*}"
long_directory="${directory}/${relative_directory}"
mkdir -p "${long_directory}/sub" || fatal "internal" "mkdir command failed"
flac_file "${long_directory}/file.flac" "Long path" || fatal "internal" "file not created"
flac_file "${long_directory}/sub/inner.flac" "Long path" || fatal "internal" "file not created"
echo "OK: ${test}/${config}, name of ${#long_directory} characters for the directory" >&${fd}
fields='--Output=General;%CompleteName%|%FolderName%|%FileNameExtension%|%Title%'

# file, directory listing
run_command unset "${mediainfo}" "${fields}" "${long_directory}/file.flac"
check_output "file" "${long_directory}/file.flac|${long_directory}|file.flac|Long path"
run_command unset "${mediainfo}" "${directory}/${first_directory}"
cmd_stdout="$(listed)"
check_output "directory listing" "$(printf '%s\n' "${long_directory}/file.flac" "${long_directory}/sub/inner.flac" | sort | tr '\n' '|')"

# relative path, from a short current directory
cd "${directory}" || fatal "internal" "cd command failed"
run_command unset "${mediainfo}" "${fields}" "${relative_directory}/file.flac"
check_output "relative path" "${relative_directory}/file.flac|${relative_directory}|file.flac|Long path"
run_command unset "${mediainfo}" "${first_directory}"
cmd_stdout="$(listed)"
check_output "relative directory listing" "$(printf '%s\n' "${relative_directory}/file.flac" "${relative_directory}/sub/inner.flac" | sort | tr '\n' '|')"
cd "${OLDPWD}" || fatal "internal" "cd command failed"

# log file, template file, fixed copy of a file in the long directory
run_command unset "${mediainfo}" "--LogFile=${long_directory}/log.txt" "${long_directory}/file.flac"
check_true "log file" grep -q 'Long path' "${long_directory}/log.txt"
printf '%s' 'General;%FileExtension%|%Title%' >"${long_directory}/template.txt" || fatal "internal" "file not created"
run_command unset "${mediainfo}" "--Output=file://${long_directory}/template.txt" "${long_directory}/file.flac"
check_output "template file" "flac|Long path"
mkv_file "${long_directory}/fixed.mkv" || fatal "internal" "file not created"
run_command unset "${mediainfo}" --TryToFix=1 '--Output=General;%FileExtension%' "${long_directory}/fixed.mkv"
check_true "fixed copy of a file" test -s "${long_directory}/fixed.mkv.Fixed"

# network path
if test -n "${WINDOWS}" && [[ "${directory}" == [A-Za-z]:/* ]] ; then
    network_directory="//localhost/${directory:0:1}\$${long_directory:2}"
    if [ -d "${network_directory}" ] ; then
        run_command unset "${mediainfo}" "${fields}" "${network_directory}/file.flac"
        check_output "network path" "${network_directory}/file.flac|${network_directory}|file.flac|Long path"
        run_command unset "${mediainfo}" "${network_directory}/sub"
        cmd_stdout="$(listed)"
        check_output "network directory listing" "${network_directory}/sub/inner.flac|"
    else
        echo "SKIP: ${test}/${config}, network path, ${network_directory%%/${relative_directory}} not available" >&${fd}
    fi
fi

exit ${status}
