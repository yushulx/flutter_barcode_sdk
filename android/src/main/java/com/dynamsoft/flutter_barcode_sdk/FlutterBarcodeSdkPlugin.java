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
            case "initLicense":
                final String license = call.argument("license");
                mBarcodeManager.initLicense(license);
                break;
            case "decodeFile": {
                final String filename = call.argument("filename");
                String results = mBarcodeManager.decodeFile(filename);
                result.success(results);
            }

            break;
            case "decodeBytes": {
                final byte[] bytes = call.argument("bytes");
                String results = mBarcodeManager.decodeBytes(bytes);
                result.success(results);
            }

            break;
            default:
                result.notImplemented();
        }

        // if (call.method.equals("getPlatformVersion")) {
        //   result.success("Android " + android.os.Build.VERSION.RELEASE);
        // } else if (call.method.equals("initLicense")) {
        //   final String license = call.argument("license");
        //   mBarcodeManager.initLicense(license);
        // } else if (call.method.equals("decodeFile")) {
        //   final String filename = call.argument("filename")
        //   String results = mBarcodeManager.decodeFile(filename);
        //   result.success(results);
        // } else if (call.method.equals("decodeBytes")) {
        //   final byte[] bytes = call.argument("bytes");
        //   String results = mBarcodeManager.decodeBytes(bytes);
        //   result.success(results);
        // } else {
        //   result.notImplemented();
        // }
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        channel.setMethodCallHandler(null);
    }
}
