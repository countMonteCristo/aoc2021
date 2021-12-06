#! /bin/bash

RED='\033[0;31m'    # red for fail
GREEN='\033[0;32m'  # green for success
LGREY='\033[1;30m'  # light grey for verbose text
NC='\033[0m'        # No Color

verbose="0"
while getopts v flag
do
    case "${flag}" in
        v) verbose="1";;
        *) exit 1;;
    esac
done


for f in day[0-2][0-9].cpp; do
    target=$(echo "${f}" | grep -oP 'day\d\d')
    day=$(echo "${f}" | grep -oP '\d\d')
    [ "$verbose" == "1" ] && echo -e "${LGREY}Check Day${day}:${NC}"
    output=$(make -B "${target}" && "./${target}" )
    result="$?"
    [ "$verbose" == "1" ] && echo "${output}"
    [ "${result}" != "0" ] && echo -e "${RED}Day${day} failed!${NC}" || echo -e "${GREEN}Day${day} succeded!${NC}"
done;
