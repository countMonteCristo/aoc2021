#!/bin/bash

set -e

DAY=$1

TEMPLATE_SRC="template.cpp"
TARGET="day${DAY}"
SRC="${TARGET}.cpp"
MAKEFILE="Makefile"
INPUTS="inputs"
INPUT_FILE="${INPUTS}/${TARGET}_1.txt"

# If template source file does not exists - this is an error
[ -e "${TEMPLATE_SRC}" ] || { echo "[ERROR] Template ${TEMPLATE_SRC} does not exist"; exit 1; }

# Check if new target is not ipresent in Makefile
[ -e "${MAKEFILE}" ] || { echo "[ERROR] ${MAKEFILE} does not exist"; exit 1; }
output=$(grep -c "${TARGET}: ${SRC}" "${MAKEFILE}"  || true)
if [[ "$output" != "0" ]]; then
    echo "[ERROR] Target ${TARGET} is present"
    exit 1
fi

# Check if new source file does not exist
[ -e "${SRC}" ] && { echo "[ERROR] ${SRC} exists"; exit 1; } || echo "[INFO] Create source file ${SRC}..."

# Create empty input file for source
[ -d "${INPUTS}" ] || mkdir "${INPUTS}"
# [ -f "${INPUT_FILE}" ] || { touch "${INPUT_FILE}"; echo "[INFO] Successfuly add input file ${INPUT_FILE}"; } &&
if [[ -f "${INPUT_FILE}" ]]
then
    echo "[INFO] Input file ${INPUT_FILE} already exists"
else
    touch "${INPUT_FILE}"
    echo "[INFO] Successfuly add input file ${INPUT_FILE}"
fi

# Create source file
cp "${TEMPLATE_SRC}" "${SRC}"
sed -i -- "s/<%DAY>/${DAY}/g" "${SRC}"
echo "[INFO] Successfuly add new source file ${SRC}"

# Add new target to Makefile
{
    echo ""
    echo "${TARGET}: ${SRC}"
    echo "	\${CXX} \${CXXFLAGS} ${SRC} -o ${TARGET}"
} >> ${MAKEFILE}
echo "[INFO] Succesfully add target ${TARGET} to ${MAKEFILE}"
