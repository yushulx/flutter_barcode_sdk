cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "flutter_barcode_sdk")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "flutter_barcode_sdk_linux_plugin")

link_directories("${PROJECT_SOURCE_DIR}/lib/") 

add_library(${PLUGIN_NAME} SHARED
  "flutter_barcode_sdk_plugin.cc"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter DynamsoftCore DynamsoftLicense DynamsoftCaptureVisionRouter DynamsoftUtility)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)

set_target_properties(${PLUGIN_NAME} PROPERTIES
  INSTALL_RPATH "$ORIGIN"
  BUILD_RPATH "$ORIGIN"
)

# List of absolute paths to libraries that should be bundled with the plugin
set(flutter_barcode_sdk_linux_bundled_libraries
  ""
  PARENT_SCOPE
)

# Install the Dynamsoft shared libraries and resources into the app bundle.
# Use relative DESTINATION paths so they resolve against the bundle prefix
# set by the parent runner project (avoids writing to /usr/local).
install(DIRECTORY
  "${PROJECT_SOURCE_DIR}/lib/"
  DESTINATION "lib/"
)

install(DIRECTORY
  "${PROJECT_SOURCE_DIR}/../resources/Templates"
  "${PROJECT_SOURCE_DIR}/../resources/Models"
  DESTINATION "lib/"
)