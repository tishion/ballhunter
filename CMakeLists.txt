cmake_minimum_required(VERSION 3.4.1)
project(BallHunter)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set proejct architecture
# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()

  if(OS_MACOSX)
    # PROJECT_ARCH should be specified on Mac OS X.
    message(WARNING "No PROJECT_ARCH value specified, using ${PROJECT_ARCH}")
  endif()
endif()


# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING=1)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOS 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
endif()

if (NOT OS_WINDOWS)
  add_compile_options(
    "-g"
    "$<$<CONFIG:DEBUG>:-O0>"
    "$<$<CONFIG:RELEASE>:-O3>"
  )
endif()

include_directories(
  third-party
  source
)

add_subdirectory(third-party)

# Source files
file(GLOB_RECURSE SRC_FILES
  source/*.h
  source/*.cpp
  source/common/*.h
  source/common/*.cpp
  source/engine/*.h
  source/engine/*.cpp
  source/engine/audio/*.h
  source/engine/audio/*.cpp
  source/engine/renderer/*.h
  source/engine/renderer/*.cpp
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source
  PREFIX Source 
  FILES ${SRC_FILES}
)

file(GLOB_RECURSE SRC_PLATFORM_FILES
  source/engine/object/windows/*.h
  source/engine/object/windows/*.cpp
  source/engine/renderer/windows/*.h
  source/engine/renderer/windows/*.cpp
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source/engine
  PREFIX Source/engine
  FILES ${SRC_PLATFORM_FILES}
)

add_executable(${PROJECT_NAME} 
  ${SRC_FILES}
  ${SRC_PLATFORM_FILES}
)

find_package(SDL2 REQUIRED)

set(LINK_LIBS 
  SDL2::SDL2
  SDL2::SDL2main
  easyloggingpp
)

if(OS_WINDOWS)
  find_package(directxtk REQUIRED)
  list(APPEND LINK_LIBS 
    d2d1
    d3d11
    dxgi
    dwrite
    Microsoft::DirectXTK
  )
endif()

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${LINK_LIBS}
)

if(OS_WINDOWS)
  # target_link_options(${PROJECT_NAME}
  #   PRIVATE
  #     /SUBSYSTEM:WINDOWS
  # )
endif()
