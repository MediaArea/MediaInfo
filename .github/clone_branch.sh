#!/bin/sh

# Clone of the project ${1} in the directory ${2}: branch of the same name in the same fork if any (change in several projects), else master of MediaArea

if [ "${BRANCH_NAME}" != "master" ] && GIT_TERMINAL_PROMPT=0 git ls-remote --exit-code "https://github.com/${BRANCH_OWNER}/${1}.git" "refs/heads/${BRANCH_NAME}" >/dev/null 2>&1 ; then
    git clone --depth=1 --branch "${BRANCH_NAME}" "https://github.com/${BRANCH_OWNER}/${1}.git" "${2}"
else
    git clone --depth=1 "https://github.com/MediaArea/${1}.git" "${2}"
fi
