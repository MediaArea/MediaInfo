#!/usr/bin/env bash

WINDOWS=
if [[ "${OSTYPE}" == "msys"* || "${OSTYPE}" == "cygwin"* ]] ; then
  WINDOWS=1 # Git Bash, msys or cygwin depending on its version
fi

fd=1
if (command exec >&9) ; then
    fd=9
fi >/dev/null 2>&1

# byte strings in the scripts, the tested programs get the locale of run_command
export LC_ALL=C

status=0

timeout=30

# directory for the temporary files
temp_path="$(mktemp -d -t 'mediainfo_testsuite.XXXXXX')"
trap 'rm -fr "${temp_path}"' EXIT
if test -n "${WINDOWS}" ; then
    temp_path="$(cygpath -m "${temp_path}")" # same path for Git Bash and the tested programs, without path conversion
fi

fatal() {
    local test="${1}"
    local message="${2}"

    echo "NOK: ${test}, ${message}" >&${fd}
    status=1

    exit ${status}
}

# first locale of ${4}... in the list of the system in the variable named ${1}, else the locale ${2} with the encoding ${3}
# (glibc) compiled in the directory for the temporary files, without administrator rights
find_locale() {
    local variable="${1}" source="${2}" charmap="${3}" locale
    shift 3
    for locale in "${@}" ; do
        if [[ "${installed_locales}" == *$'\n'"${locale}"$'\n'* ]] ; then
            printf -v "${variable}" '%s' "${locale}"
            return 0
        fi
    done
    if command -v localedef >/dev/null 2>&1 ; then
        mkdir -p "${temp_path}/locales"
        localedef -c -i "${source}" -f "${charmap}" "${temp_path}/locales/${source}.${charmap}" >/dev/null 2>&1
        if [ -d "${temp_path}/locales/${source}.${charmap}" ] ; then
            printf -v "${variable}" '%s' "${source}.${charmap}"
            locpath="${temp_path}/locales"
            return 0
        fi
    fi
    return 1
}

# locales of the tests: C, POSIX and no locale variable (ASCII, e.g. programs which don't call setlocale(), Docker
# images, macOS apps launched from the Finder), UTF-8, Latin-1, EUC-JP and GB18030 (neither ASCII nor UTF-8) if available
# (Windows: no locale variable only, the program calls setlocale() or not for the C locale or the code page of the system)
if test -n "${WINDOWS}" ; then
    locale_configs="unset"
else
    locale_configs="C POSIX unset"
    installed_locales=$'\n'"$(locale -a 2>/dev/null)"$'\n'
    utf8_locale=
    latin1_locale=
    eucjp_locale=
    gb18030_locale=
    locpath=
    for locale in C.UTF-8 C.utf8 en_US.UTF-8 en_US.utf8 ; do
        if [[ "${installed_locales}" == *$'\n'"${locale}"$'\n'* ]] ; then
            utf8_locale="${locale}"
            locale_configs="${locale_configs} utf8"
            break
        fi
    done
    if find_locale latin1_locale fr_FR ISO-8859-1 fr_FR.ISO8859-1 fr_FR.iso88591 ; then
        locale_configs="${locale_configs} latin1"
    fi
    if find_locale eucjp_locale ja_JP EUC-JP ja_JP.eucJP ja_JP.eucjp ja_JP.EUC-JP ; then
        locale_configs="${locale_configs} eucjp"
    fi
    if find_locale gb18030_locale zh_CN GB18030 zh_CN.GB18030 zh_CN.gb18030 ; then
        locale_configs="${locale_configs} gb18030"
    fi
fi

# command ${2}... with the locale configuration ${1} and a timeout (an endless directory listing is one of the tested issues)
# output in cmd_stdout and cmd_stderr, exit code in cmd_status ("timeout" if the command did not end)
run_command() {
    local config="${1}"
    shift

    unset cmd_status
    unset cmd_stdout
    unset cmd_stderr

    rm -f "${temp_path}/timeout"
    if test -n "${WINDOWS}" ; then
        # Windows: no pkill for the watcher, so timeout
        timeout ${timeout} "${@}" >"${temp_path}/stdout" 2>"${temp_path}/stderr"
        cmd_status="${?}"
        if [ "${cmd_status}" == "124" ] ; then
            cmd_status=timeout
        fi
    else
        # locale variables set by the shell, not by env (macOS removes the DYLD_* variables when it runs a system program)
        (
            case "${config}" in
                unset)
                    unset LC_ALL LC_CTYPE LANG LANGUAGE
                    ;;
                utf8)
                    export LC_ALL="${utf8_locale}"
                    ;;
                latin1|eucjp|gb18030)
                    case "${config}" in
                        latin1) export LC_ALL="${latin1_locale}" ;;
                        eucjp) export LC_ALL="${eucjp_locale}" ;;
                        gb18030) export LC_ALL="${gb18030_locale}" ;;
                    esac
                    if [ -n "${locpath}" ] ; then
                        export LOCPATH="${locpath}"
                    fi
                    ;;
                *)
                    export LC_ALL="${config}"
                    ;;
            esac
            exec "${@}"
        ) >"${temp_path}/stdout" 2>"${temp_path}/stderr" & local pid=${!}
        (sleep ${timeout} && : >"${temp_path}/timeout" && kill -KILL ${pid}) >/dev/null 2>&1 & local watcher=${!}
        wait ${pid} 2>/dev/null # without the message of the shell if the command is killed
        cmd_status="${?}"
        # stop the watcher: its sleep, and the watcher itself in case its sleep is not started yet
        pkill -P ${watcher} 2>/dev/null
        kill ${watcher} 2>/dev/null
        wait ${watcher} 2>/dev/null
        if [ -e "${temp_path}/timeout" ] ; then
            cmd_status=timeout
        fi
    fi
    cmd_stdout="$(cat "${temp_path}/stdout")"
    cmd_stderr="$(cat "${temp_path}/stderr")"
    if test -n "${WINDOWS}" ; then
        cmd_stdout="${cmd_stdout//$'\r'/}" # CR of CR LF line endings
    fi
}

# result lines of a test program ("NOK: check, message" or "OK: summary") with the configuration ${1}
report() {
    local config="${1}"
    local line nok=

    if [ "${cmd_status}" == "77" ] ; then
        exit 77 # skipped by the test program, e.g. build without Unicode
    fi

    # lines before a timeout or a crash too
    while IFS= read -r line ; do
        case "${line}" in
            OK:*)
                echo "OK: ${test}/${config}, ${line#OK: }" >&${fd}
                ;;
            NOK:*)
                echo "NOK: ${test}/${config}, ${line#NOK: }" >&${fd}
                nok=1
                status=1
                ;;
        esac
    done <<< "${cmd_stdout}"

    if [ "${cmd_status}" == "timeout" ] ; then
        echo "NOK: ${test}/${config}, no result after ${timeout} s (e.g. endless directory listing)" >&${fd}
        status=1
    elif [ "${cmd_status}" -gt "1" ] ; then
        echo "NOK: ${test}/${config}, exit code ${cmd_status}${cmd_stderr:+, }${cmd_stderr}" >&${fd}
        status=1
    elif [ "${cmd_status}" -ne "0" ] && [ -z "${nok}" ] ; then
        echo "NOK: ${test}/${config}, exit code ${cmd_status} without message" >&${fd}
        status=1
    fi
}
