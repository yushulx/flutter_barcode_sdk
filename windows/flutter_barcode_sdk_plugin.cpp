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

  using flutter::EncodableList;
  using flutter::EncodableMap;
  using flutter::EncodableValue;

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
    const auto *arguments = std::get_if<EncodableMap>(method_call.arguments());

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
    else if (method_call.method_name().compare("init") == 0)
    {
      int ret = manager->Init();

      result->Success(EncodableValue(ret));
    }
    else if (method_call.method_name().compare("setLicense") == 0)
    {
      std::string license;
      int ret = 0;
      if (arguments)
      {
        auto license_it = arguments->find(EncodableValue("license"));
        if (license_it != arguments->end())
        {
          license = std::get<std::string>(license_it->second);
        }
        ret = manager->SetLicense(license.c_str());
      }

      result->Success(EncodableValue(ret));
    }
    else if (method_call.method_name().compare("decodeFile") == 0)
    {
      std::string filename;
      EncodableList results;

      if (arguments)
      {
        auto filename_it = arguments->find(EncodableValue("filename"));
        if (filename_it != arguments->end())
        {
          filename = std::get<std::string>(filename_it->second);
        }
        results = manager->DecodeFile(filename.c_str());
      }

      result->Success(results);
    }
    else if (method_call.method_name().compare("decodeFileBytes") == 0)
    {
      std::vector<unsigned char> bytes;
      EncodableList results;

      if (arguments)
      {
        auto bytes_it = arguments->find(EncodableValue("bytes"));
        if (bytes_it != arguments->end())
        {
          bytes = std::get<vector<unsigned char>>(bytes_it->second);
        }

        results = manager->DecodeFileBytes(reinterpret_cast<unsigned char*>(bytes.data()), (int)bytes.size());
      }

      result->Success(results);
    }
    else if (method_call.method_name().compare("decodeImageBuffer") == 0)
    {
      std::vector<unsigned char> bytes;
      EncodableList results;
      int width = 0, height = 0, stride = 0, format = 0;

      if (arguments)
      {
        auto bytes_it = arguments->find(EncodableValue("bytes"));
        if (bytes_it != arguments->end())
        {
          bytes = std::get<vector<unsigned char>>(bytes_it->second);
        }

        auto width_it = arguments->find(EncodableValue("width"));
        if (width_it != arguments->end())
        {
          width = std::get<int>(width_it->second);
        }

        auto height_it = arguments->find(EncodableValue("height"));
        if (height_it != arguments->end())
        {
          height = std::get<int>(height_it->second);
        }

        auto stride_it = arguments->find(EncodableValue("stride"));
        if (stride_it != arguments->end())
        {
          stride = std::get<int>(stride_it->second);
        }

        auto format_it = arguments->find(EncodableValue("format"));
        if (format_it != arguments->end())
        {
          format = std::get<int>(format_it->second);
        }
        manager->DecodeImageBuffer(result, reinterpret_cast<unsigned char*>(bytes.data()), width, height, stride, format);
      }

      // result->Success(results);
    }
    else if (method_call.method_name().compare("setBarcodeFormats") == 0)
    {
      int formats = 0;
      int ret = 0;
      if (arguments)
      {
        auto formts_it = arguments->find(EncodableValue("formats"));
        if (formts_it != arguments->end())
        {
          formats = std::get<int>(formts_it->second);
        }

        ret = manager->SetFormats(formats);
      }

      result->Success(EncodableValue(ret));
    }
    else if (method_call.method_name().compare("getParameters") == 0)
    {
      result->Success(manager->GetParameters());
    }
    else if (method_call.method_name().compare("setParameters") == 0)
    {
      std::string params;
      int ret = 0;

      if (arguments)
      {
        auto params_it = arguments->find(EncodableValue("params"));
        if (params_it != arguments->end())
        {
          params = std::get<std::string>(params_it->second);
        }
        ret = manager->SetParameters(params.c_str());
      }

      result->Success(EncodableValue(ret));
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
