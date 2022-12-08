#include "include/flutter_barcode_sdk/flutter_barcode_sdk_plugin.h"
#include "include/barcode_manager.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#define FLUTTER_BARCODE_SDK_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), flutter_barcode_sdk_plugin_get_type(), \
                              FlutterBarcodeSdkPlugin))

struct _FlutterBarcodeSdkPlugin {
  GObject parent_instance;
  BarcodeManager *manager;
};

G_DEFINE_TYPE(FlutterBarcodeSdkPlugin, flutter_barcode_sdk_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void flutter_barcode_sdk_plugin_handle_method_call(
    FlutterBarcodeSdkPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  FlValue* args = fl_method_call_get_args(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s. Dynamsoft Barcode Reader version: %s", uname_data.version, self->manager->GetVersion());
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } 
  else if (strcmp(method, "init") == 0) {
    int ret = self->manager->Init();

    g_autoptr(FlValue) result = fl_value_new_int(ret);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  }
  else if (strcmp(method, "setLicense") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "license");
    if (value == nullptr) {
      return;
    }
  
    const char* license = fl_value_get_string(value);
    int ret = self->manager->SetLicense(license);

    g_autoptr(FlValue) result = fl_value_new_int(ret);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  }
  else if (strcmp(method, "decodeFile") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "filename");
    if (value == nullptr) {
      return;
    }
  
    const char* filename = fl_value_get_string(value);
    g_autoptr(FlValue) results = self->manager->DecodeFile(filename);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(results));
  }
  else if (strcmp(method, "decodeFileBytes") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "bytes");
    if (value == nullptr) {
      return;
    }
  
    int size = fl_value_get_length(value);
    unsigned char* bytes = (unsigned char*)fl_value_get_uint8_list(value);
    g_autoptr(FlValue) results = self->manager->DecodeFileBytes(bytes, size);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(results));
  }
  else if (strcmp(method, "decodeImageBuffer") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "bytes");
    if (value == nullptr) {
      return;
    }
    unsigned char* bytes = (unsigned char*)fl_value_get_uint8_list(value);

    value = fl_value_lookup_string(args, "width");
    if (value == nullptr) {
      return;
    }
    int width = fl_value_get_int(value);

    value = fl_value_lookup_string(args, "height");
    if (value == nullptr) {
      return;
    }
    int height = fl_value_get_int(value);

    value = fl_value_lookup_string(args, "stride");
    if (value == nullptr) {
      return;
    }
    int stride = fl_value_get_int(value);

    value = fl_value_lookup_string(args, "format");
    if (value == nullptr) {
      return;
    }
    int format = fl_value_get_int(value);

    g_autoptr(FlValue) results = self->manager->DecodeImageBuffer(bytes, width, height, stride, format);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(results));
  } 
  else if (strcmp(method, "setBarcodeFormats") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "formats");
    if (value == nullptr) {
      return;
    }
  
    int formats = fl_value_get_int(value);
    int ret = self->manager->SetFormats(formats);
    g_autoptr(FlValue) result = fl_value_new_int(ret);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } 
  else if (strcmp(method, "getParameters") == 0) {
    g_autoptr(FlValue) result = self->manager->GetParameters();
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } 
  else if (strcmp(method, "setParameters") == 0) {
    if (fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
      return;
    }

    FlValue* value = fl_value_lookup_string(args, "params");
    if (value == nullptr) {
      return;
    }
  
    const char* params  = fl_value_get_string(value);
    g_autoptr(FlValue) result = self->manager->SetParameters(params);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void flutter_barcode_sdk_plugin_dispose(GObject* object) {
  FlutterBarcodeSdkPlugin* self = FLUTTER_BARCODE_SDK_PLUGIN(object);
  delete self->manager;
  G_OBJECT_CLASS(flutter_barcode_sdk_plugin_parent_class)->dispose(object);
}

static void flutter_barcode_sdk_plugin_class_init(FlutterBarcodeSdkPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = flutter_barcode_sdk_plugin_dispose;
}

static void flutter_barcode_sdk_plugin_init(FlutterBarcodeSdkPlugin* self) {
  self->manager = new BarcodeManager();
}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  FlutterBarcodeSdkPlugin* plugin = FLUTTER_BARCODE_SDK_PLUGIN(user_data);
  flutter_barcode_sdk_plugin_handle_method_call(plugin, method_call);
}

void flutter_barcode_sdk_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  FlutterBarcodeSdkPlugin* plugin = FLUTTER_BARCODE_SDK_PLUGIN(
      g_object_new(flutter_barcode_sdk_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "flutter_barcode_sdk",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
