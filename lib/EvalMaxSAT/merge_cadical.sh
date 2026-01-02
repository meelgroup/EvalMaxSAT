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

# Combine all cadical objects using partial linking first
ld -r -o ../cadical_combined.o *.o

# Go back up and create a symbol rename map for only CaDiCaL symbols
cd ..
# Extract global CaDiCaL symbols and create redefine rules
# Only rename CaDiCaL namespace symbols, not system symbols
nm cadical_combined.o | grep -E "^[0-9a-f]+ [TtRrDdBb] " | \
    grep -E "(_ZN7CaDiCaL|_ZNK7CaDiCaL|_ZTI.*CaDiCaL|_ZTV.*CaDiCaL|_ZTS.*CaDiCaL|ccadical_)" | \
    awk -v prefix="${PREFIX}" '{print $3 " " prefix $3}' | sort -u > cadical_symbol_map.txt

# Apply the symbol renaming to the combined object
if [ -s cadical_symbol_map.txt ]; then
    objcopy --redefine-syms=cadical_symbol_map.txt cadical_combined.o
fi

# Remove the extracted directory
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
rm -rf "${MERGE_DIR}"
