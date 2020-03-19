:: Buildscript for building on windows with vs2019
:: Run from the root directory of the project

cmake . -B"BUILD/VS2019" -DCMAKE_BUILD_TYPE=Debug -G"Visual Studio 16 2019" -A x64
cmake --build "BUILD/VS2019" --config Debug
