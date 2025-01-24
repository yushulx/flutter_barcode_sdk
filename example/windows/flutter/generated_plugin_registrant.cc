//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <file_selector_windows/file_selector_windows.h>
#include <flutter_barcode_sdk/flutter_barcode_sdk_plugin.h>
#include <flutter_lite_camera/flutter_lite_camera_plugin_c_api.h>

void RegisterPlugins(flutter::PluginRegistry* registry) {
  FileSelectorWindowsRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("FileSelectorWindows"));
  FlutterBarcodeSdkPluginRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("FlutterBarcodeSdkPlugin"));
  FlutterLiteCameraPluginCApiRegisterWithRegistrar(
      registry->GetRegistrarForPlugin("FlutterLiteCameraPluginCApi"));
}
