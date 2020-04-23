#!/bin/bash

cmake . -B"BUILD/macos" -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles"
cmake --build "BUILD/macos" --config Debug
