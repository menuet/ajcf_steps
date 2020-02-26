#!/bin/bash

pushd ./out/build/lin

ctest -C Debug -V

popd
