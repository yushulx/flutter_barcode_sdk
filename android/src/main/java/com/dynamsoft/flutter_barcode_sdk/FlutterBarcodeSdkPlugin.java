package com.dynamsoft.flutter_barcode_sdk;

import androidx.annotation.NonNull;
import java.util.ArrayList;
import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;

/** FlutterBarcodeSdkPlugin */
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
	ArrayList<String> args = (ArrayList<String>)call.arguments;
    if (call.method.equals("getPlatformVersion")) {
      result.success("Android " + android.os.Build.VERSION.RELEASE);
    } else if (call.method.equals("initLicense")) {
      mBarcodeManager.initLicense(args.get(0));
    } else if (call.method.equals("decodeFile")) {
      String results = mBarcodeManager.decodeFile(args.get(0));
      result.success(results);
    } else {
      result.notImplemented();
    }
  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    channel.setMethodCallHandler(null);
  }
}
