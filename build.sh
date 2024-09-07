#!/bin/bash

set -e

CXX=clang++

PKGS="glfw3 glew"
CXXFLAGS="-Wall -Wextra -std=c++20 -pedantic -ggdb $(pkg-config --cflags $PKGS)"
LIBS="$(pkg-config --libs $PKGS)"

BUILDDIR="$(pwd)/build"
mkdir -p $BUILDDIR

function subdir() {
    local dir=$1

    echo
    echo "======================================"
    echo "Entering subdirectory \`$dir\`"
    echo "======================================"
    echo

    pushd $dir
    source ./build.sh
    popd
}

function build() {
    local files=$@

    local main_file=$1
    local out_file=$(echo $main_file | rev | cut -f 2- -d '.' | rev)

    echo
    echo "    > ======================================"
    echo "    > Building \`$out_file\`"
    echo "    > ======================================"
    echo

    local needs_rebuild=0
    
    for file in $files; do
        if [ "$file" -nt "$BUILDDIR/$out_file" ]; then
            needs_rebuild=1
            break
        fi
    done

    if [ ! -f "$BUILDDIR/$out_file" ]; then
        needs_rebuild=1
    fi

    if [ "$needs_rebuild" -eq 1 ]; then
        $CXX $CXXFLAGS -o $BUILDDIR/$out_file $files $LIBS
    fi
}

function build_parallel() {
    build $1 &
    sleep 0.001
}

subdir src