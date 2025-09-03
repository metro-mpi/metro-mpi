#!/bin/bash
set -e
ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd $ROOT/tmp

if [ -z ${NUM_JOBS} ]; then
    NUM_JOBS=4
fi

if [ ! -e "$VERILATOR_ROOT/bin/verilator" ]; then
    echo "Installing Verilator"
    if [ -d verilator-5.036 ]; then rm -rf verilator-5.036; fi
    git clone https://github.com/metro-mpi/verilator.git verilator-5.036
    cd verilator-5.036
    git checkout metro-mpi
    mkdir -p $VERILATOR_ROOT
    # copy scripts
    autoconf && ./configure --prefix="$VERILATOR_ROOT" && make -j${NUM_JOBS}
    cp -r * $VERILATOR_ROOT/
    make test
else
    echo "Using Verilator from cached directory."
fi
