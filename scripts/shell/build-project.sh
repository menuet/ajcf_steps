#!/bin/bash

mkdir -p ./out/build/lin

pushd ./out/build/lin

cmake -DCMAKE_VERBOSE_MAKEFILE=TRUE -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake ../../..

cmake --build . --config Debug

popd
