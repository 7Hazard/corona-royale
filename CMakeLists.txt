cmake_minimum_required(VERSION 3.10)
project(CoronaRoyale)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
else()
  find_package(SDL2 REQUIRED)
endif()

add_definitions(
  -D_UNICODE
  -DUNICODE
)

# Server
add_executable(CoronaRoyale
  src/main.c
  src/events.c
  src/collision.c
  src/player.c
)
target_compile_definitions(CoronaRoyale PRIVATE

)
target_include_directories(CoronaRoyale PRIVATE
  src
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
    deps/SDL2/include
  )
  target_link_directories(CoronaRoyale PRIVATE
    deps/SDL2/lib/x64
  )
  target_link_libraries(CoronaRoyale PRIVATE
    SDL2
    SDL2main
  )
  configure_file("deps/SDL2/lib/x64/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2.dll" COPYONLY)
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fms-extensions -fdelayed-template-parsing -fexceptions")
  
  target_include_directories(CoronaRoyale PRIVATE
    ${SDL2_INCLUDE_DIR}
  )
  target_link_libraries(CoronaRoyale PRIVATE
    ${SDL2_LIBRARY}
  )
endif()

# file(COPY "deps/SDL2/lib/x64/SDL2.dll" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2.dll")
