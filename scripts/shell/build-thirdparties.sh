#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    vcpkg install --triplet x64-linux catch2 fmt date tl-expected ms-gsl
elif [[ "$OSTYPE" == "darwin"* ]]; then
    vcpkg install --triplet x64-osx catch2 fmt date tl-expected ms-gsl
fi
