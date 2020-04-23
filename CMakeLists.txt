cmake_minimum_required(VERSION 3.10)
project(CoronaRoyale)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Copy DLLs from SDL2 libraries into build output
  file(GLOB SDL2_SHARED_BINARIES
    "deps/SDL2*/lib/x64/*.dll"
  )
  file(COPY ${SDL2_SHARED_BINARIES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  find_package(SDL2_net REQUIRED)
  find_package(SDL2_ttf REQUIRED)
endif()

add_definitions(
  -D_UNICODE
  -DUNICODE
)

# Copy resources into build output
file(COPY "res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

######
###### CLIENT
######
add_executable(CoronaRoyale
  src/client/main.c
  src/client/game.c
  src/client/audio.c
  src/client/player.c
  src/client/events.c
  src/client/collision.c
  src/client/menu.c
  src/client/timer.c
    src/client/texture.c

  deps/SDL_FontCache/SDL_FontCache.c
)
target_compile_definitions(CoronaRoyale PRIVATE

)
target_include_directories(CoronaRoyale PRIVATE
  src
  deps/SDL_FontCache
  # ${PROJECT_SOURCE_DIR}
)


# Platform specific stuff
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
  
  set_target_properties(CoronaRoyale PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  target_include_directories(CoronaRoyale PRIVATE
    deps/SDL2/include
    deps/SDL2_image/include
    deps/SDL2_mixer/include
    deps/SDL2_net/include
    deps/SDL2_ttf/include
  )
  target_link_directories(CoronaRoyale PRIVATE
    deps/SDL2/lib/x64
    deps/SDL2_image/lib/x64
    deps/SDL2_mixer/lib/x64
    deps/SDL2_net/lib/x64
    deps/SDL2_ttf/lib/x64
  )
  target_link_libraries(CoronaRoyale PRIVATE
    SDL2
    SDL2main
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
  )
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fms-extensions -fdelayed-template-parsing -fexceptions")

  set_target_properties(CoronaRoyale PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
  )

  target_include_directories(CoronaRoyale PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
  )
  target_link_libraries(CoronaRoyale PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
  )
endif()



######
###### SERVER
######
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
