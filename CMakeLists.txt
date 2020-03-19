cmake_minimum_required(VERSION 3.10)
project(CoronaRoyale)

add_definitions(
  -D_UNICODE
  -DUNICODE
)

# Server
add_executable(CoronaRoyale
  src/main.c
)
target_compile_definitions(CoronaRoyale PRIVATE

)
target_include_directories(CoronaRoyale PRIVATE
  src
  deps/SDL2/include
  # ${PROJECT_SOURCE_DIR}
)
target_link_directories(CoronaRoyale PRIVATE
  deps/SDL2/lib/x64
)
target_link_libraries(CoronaRoyale PRIVATE
  SDL2
  SDL2main
  # ${PROJECT_SOURCE_DIR}
)
set_target_properties(CoronaRoyale PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Platform specific stuff
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

  target_include_directories(CoronaRoyale PRIVATE

  )
  target_link_libraries(CoronaRoyale PRIVATE

  )
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fms-extensions -fdelayed-template-parsing -fexceptions")
  message(FATAL_ERROR "LINUX NOT SUPPORTED")
endif()

# file(COPY "deps/SDL2/lib/x64/SDL2.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2.dll")
configure_file("deps/SDL2/lib/x64/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2.dll" COPYONLY)
