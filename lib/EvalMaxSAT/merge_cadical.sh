#!/bin/bash
set -e

MERGE_DIR="$1"
CADICAL_LIB="$2"
EVALMAXSAT_LIB="$3"

# Prefix to add to all CaDiCaL symbols
PREFIX="EvalMaxSAT_"

# Create temp directories
mkdir -p "${MERGE_DIR}/cadical"
mkdir -p "${MERGE_DIR}/evalmaxsat"

# Extract cadical objects
cd "${MERGE_DIR}/cadical"
ar x "${CADICAL_LIB}"

# Rename all symbols in cadical objects by adding prefix
for obj in *.o; do
    objcopy --prefix-symbols="${PREFIX}" "$obj"
done

# Get list of object files before creating combined one
OBJ_FILES=(*.o)

# Combine all cadical objects using partial linking
ld -r -o ../cadical_combined.o "${OBJ_FILES[@]}"

# Go back up and remove the extracted directory
cd ..
rm -rf cadical

# Extract EvalMaxSAT objects
cd "${MERGE_DIR}/evalmaxsat"
ar x "${EVALMAXSAT_LIB}"

# Generate a symbol redefine file for cadicalinterface.cpp.o
# We only want to prefix CaDiCaL namespace symbols, not MySolver/Solver_cadical
if [ -f cadicalinterface.cpp.o ]; then
    # Extract undefined CaDiCaL symbols and create redefine rules
    nm cadicalinterface.cpp.o | grep "U _ZN.*CaDiCaL" | \
        awk -v prefix="${PREFIX}" '{print $2 " " prefix $2}' > ../symbol_map.txt

    if [ -s ../symbol_map.txt ]; then
        objcopy --redefine-syms=../symbol_map.txt cadicalinterface.cpp.o
    fi
fi

# Merge everything back into the final library
ar rcs "${EVALMAXSAT_LIB}" "${MERGE_DIR}/cadical_combined.o" *.o

# Cleanup
cd /
#rm -rf "${MERGE_DIR}"  # Temporarily disabled for debugging
echo "DEBUG: Merge dir is at ${MERGE_DIR}"
[ -f "${MERGE_DIR}/symbol_map.txt" ] && echo "DEBUG: symbol_map.txt contents:" && cat "${MERGE_DIR}/symbol_map.txt" | head -10
