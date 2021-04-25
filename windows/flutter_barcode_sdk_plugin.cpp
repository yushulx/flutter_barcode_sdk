#include "include/flutter_barcode_sdk/flutter_barcode_sdk_plugin.h"
#include "include/barcode_manager.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace
{

  using flutter::EncodableMap;
  using flutter::EncodableValue;
  using flutter::EncodableList;

  class FlutterBarcodeSdkPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    FlutterBarcodeSdkPlugin();

    virtual ~FlutterBarcodeSdkPlugin();

  private:
    BarcodeManager *manager;
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  };

  // static
  void FlutterBarcodeSdkPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "flutter_barcode_sdk",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<FlutterBarcodeSdkPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  FlutterBarcodeSdkPlugin::FlutterBarcodeSdkPlugin()
  {
    manager = new BarcodeManager();
  }

  FlutterBarcodeSdkPlugin::~FlutterBarcodeSdkPlugin()
  {
    delete manager;
  }

  void FlutterBarcodeSdkPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    const auto* arguments = std::get_if<EncodableMap>(method_call.arguments());

    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      version_stream << ". Dynamsoft Barcode Reader version: ";
      version_stream << manager->GetVersion();
      result->Success(EncodableValue(version_stream.str()));
    }
    else if (method_call.method_name().compare("decodeFile") == 0)
    {
      std::string filename;
      EncodableList results;

      if (arguments) {
        auto filename_it = arguments->find(EncodableValue("filename"));
        if (filename_it != arguments->end()) {
          filename = std::get<std::string>(filename_it->second);
          results = manager->DecodeFile(filename.c_str());
          result->Success(results);
          return;
        }
      }

      result->Success(results);
    }
    else if (method_call.method_name().compare("decodeFileBytes") == 0)
    {
    }
    else if (method_call.method_name().compare("decodeImageBuffer") == 0)
    {
    }
    else
    {
      result->NotImplemented();
    }
  }

} // namespace

void FlutterBarcodeSdkPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  FlutterBarcodeSdkPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
