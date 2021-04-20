package com.dynamsoft.flutter_barcode_sdk;

import androidx.annotation.NonNull;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

/**
 * FlutterBarcodeSdkPlugin
 */
public class FlutterBarcodeSdkPlugin implements FlutterPlugin, MethodCallHandler {
    /// The MethodChannel that will the communication between Flutter and native Android
    ///
    /// This local reference serves to register the plugin with the Flutter Engine and unregister it
    /// when the Flutter Engine is detached from the Activity
    private MethodChannel channel;

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
        channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "flutter_barcode_sdk");
        channel.setMethodCallHandler(this);
    }

    private BarcodeManager mBarcodeManager;

    public FlutterBarcodeSdkPlugin() {
        mBarcodeManager = new BarcodeManager();
    }

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
        switch (call.method) {
            case "getPlatformVersion":
                result.success("Android " + android.os.Build.VERSION.RELEASE);
                break;
            case "decodeFile": {
                final String filename = call.argument("filename");
                String results = mBarcodeManager.decodeFile(filename);
                result.success(results);
            }
            break;
            case "decodeFileBytes": {
                final byte[] bytes = call.argument("bytes");
                String results = mBarcodeManager.decodeFileBytes(bytes);
                result.success(results);
            }
            break;
            case "decodeImageBuffer": {
                final byte[] bytes = call.argument("bytes");
                final int width = call.argument("width");
                final int height = call.argument("height");
                final int stride = call.argument("stride");
                final int format = call.argument("format");
                String results = mBarcodeManager.decodeImageBuffer(bytes, width, height, stride, format);
                result.success(results);
            }
            break;
            default:
                result.notImplemented();
        }
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        channel.setMethodCallHandler(null);
    }
}
