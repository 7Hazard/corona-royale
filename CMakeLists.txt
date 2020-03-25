cmake_minimum_required(VERSION 3.10)

######
###### CLIENT
######
project(CoronaRoyale)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  configure_file("deps/SDL2/lib/x64/SDL2.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/SDL2_image.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2_image.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/zlib1.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/zlib1.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/libjpeg-9.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libjpeg-9.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/libpng16-16.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libpng16-16.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/libtiff-5.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libtiff-5.dll" COPYONLY)
  configure_file("deps/SDL2_image/lib/x64/libwebp-7.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/libwebp-7.dll" COPYONLY)
  configure_file("deps/SDL2_net/lib/x64/SDL2_net.dll" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/SDL2_net.dll" COPYONLY)
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_net REQUIRED)
endif()

add_definitions(
  -D_UNICODE
  -DUNICODE
)

# Server
add_executable(CoronaRoyale
  src/client/main.c
  src/client/events.c
  src/client/collision.c
  src/client/player.c
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
    deps/SDL2_image/include
    deps/SDL2_net/include
  )
  target_link_directories(CoronaRoyale PRIVATE
    deps/SDL2/lib/x64
    deps/SDL2_image/lib/x64
    deps/SDL2_net/lib/x64
  )
  target_link_libraries(CoronaRoyale PRIVATE
    SDL2
    SDL2main
    SDL2_image
    SDL2_net
  )
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fms-extensions -fdelayed-template-parsing -fexceptions")
  
  target_include_directories(CoronaRoyale PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
  )
  target_link_libraries(CoronaRoyale PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
  )
endif()



######
###### SERVER
######
project(CoronaRoyaleServer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_net REQUIRED)
endif()

add_definitions(
  -D_UNICODE
  -DUNICODE
)

add_executable(CoronaRoyaleServer
  src/server/main.c
)
target_compile_definitions(CoronaRoyaleServer PRIVATE

)
target_include_directories(CoronaRoyaleServer PRIVATE
  src
  # ${PROJECT_SOURCE_DIR}
)

set_target_properties(CoronaRoyaleServer PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Platform specific stuff
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_include_directories(CoronaRoyaleServer PRIVATE
    deps/SDL2/include
    deps/SDL2_net/include
  )
  target_link_directories(CoronaRoyaleServer PRIVATE
    deps/SDL2/lib/x64
    deps/SDL2_net/lib/x64
  )
  target_link_libraries(CoronaRoyaleServer PRIVATE
    SDL2
    SDL2main
    SDL2_net
  )
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fms-extensions -fdelayed-template-parsing -fexceptions")
  
  target_include_directories(CoronaRoyaleServer PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
  )
  target_link_libraries(CoronaRoyaleServer PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_NET_LIBRARIES}
  )
endif()
