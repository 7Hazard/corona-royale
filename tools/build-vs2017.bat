:: Buildscript for building on windows with vs2019
:: Run from the root directory of the project

cmake . -B"BUILD/VS2017" -DCMAKE_BUILD_TYPE=Debug -G"Visual Studio 15 2017" -A x64
cmake --build "BUILD/VS2017" --config Debug
