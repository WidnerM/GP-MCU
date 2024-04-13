cmake_minimum_required(VERSION 3.21)

# Let's choose a name and version for the extension. Change the name to your
# liking. The version should be a string like "1.0".
set(PROJECT_NAME "GP-MCU") # Change this to your liking
project(${PROJECT_NAME} VERSION 1.0)

# Import the SDK
include(FetchContent)
FetchContent_Declare(
  gp-sdk
  GIT_REPOSITORY https://github.com/gigperformer/gp-sdk.git
  # GIT_TAG 6c5432518ef42ea0870fb44597e9d1d3780e2f98 # v44
  # GIT_TAG 3d4fa49c3d6f96851961893ce51896b4450e59ae # v46
  # GIT_TAG 23c6583ff0137a620a421f6378bb2f6fc761de61 # v50
  # GIT_TAG 6d1b92fbb59fafbf47e38dd80f33d46692fb3425 # v51
  # GIT_TAG f6b1ad94a3af83afd269a327e93ddc6cf94cb1b1
  # GIT_TAG 897b3452b99f2d7fc02502d0c90d89cd54b51c7e
  # GIT_TAG d264ab15312c2aac722feef523a1b3cbc809996b # 4.7
  GIT_TAG 756ecd249f1ff00f3d3a5955769408f74c9fa453 # SDK 60

)
FetchContent_MakeAvailable(gp-sdk)

# Define our library including sources, include directories and dependencies
add_library(${PROJECT_NAME} SHARED)
target_sources(
  ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/LibMain.h"
                          "${CMAKE_CURRENT_LIST_DIR}/src/Display.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/Inputs.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/Songlist.cpp"
                          "${CMAKE_CURRENT_LIST_DIR}/src/General_Utils.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE gigperformer::sdk::cpp)
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Language options: this will be a pure C++20 project
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_STANDARD 20
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS NO)

# Install the extension on the development machine
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}"
  RUNTIME DESTINATION "${GIG_PERFORMER_EXTENSIONS_DIRECTORY}")
