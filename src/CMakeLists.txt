# CMakeLists.txt

# cmake_policy(SET CMP0042 NEW)  #Just leave this line here and ignore it

# Always set the cmake min version.
cmake_minimum_required(VERSION 3.0)

# Set the variable GP_SDK_ROOT to the path of the root folder where you installed the GP_SDK
set (GP_SDK_ROOT "../../..")
set (PROJECT_VERSION "1.0")
set (PROJECT_NAME "MCU-GP-Extension")

# Set the variable PROJ_NAME to whatever your library's name is, PROJECT_VERSION should be a version string like "0.1"
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#Leave this group where it is - it MUST come before any target_source clauses
# SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

# Create our library target - this line MUST appear before the first target_source command
add_library(${PROJECT_NAME} SHARED)



#If you need any more source files, uncomment the following entry and add your files one line at a time

target_sources(${PROJECT_NAME}
  PUBLIC ${GP_SDK_ROOT}/interfaces/CPP/GPUtils.cpp
  PUBLIC ${GP_SDK_ROOT}/interfaces/CPP/GigPerformerFunctions.cpp
  PUBLIC ${GP_SDK_ROOT}/interfaces/C/imports.c
  PUBLIC ${GP_SDK_ROOT}/interfaces/C/utils.c
  PUBLIC ${GP_SDK_ROOT}/interfaces/CPP/entry.cpp
  PUBLIC ${GP_SDK_ROOT}/interfaces/CPP/GPMidiMessages.cpp
  PUBLIC ./Display.cpp
  PUBLIC ./Songlist.cpp
  PUBLIC ./Inputs.cpp
  PUBLIC ./Faders.cpp
  PUBLIC ./Buttons.cpp
  PUBLIC ./General_Utils.cpp
  PUBLIC ./LibMain.cpp)

# You should not need to change anything below this line
#########################################

include_directories(${GP_SDK_ROOT})                 # So that the interface files can be found
include_directories(${CMAKE_CURRENT_SOURCE_DIR})    # So that the LibMain.h can be found


set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


# Let's set compiler-specific flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # G++
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /MTd /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
endif()