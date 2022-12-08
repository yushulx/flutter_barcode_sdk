package com.dynamsoft.flutter_barcode_sdk;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

import androidx.annotation.NonNull;

import java.util.List;
import java.util.Map;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

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
    private HandlerThread mHandlerThread;
    private Handler mHandler;
    private final Executor mExecutor;

    public FlutterBarcodeSdkPlugin() {
        mBarcodeManager = new BarcodeManager();
        mHandler = new Handler(Looper.getMainLooper());
        mExecutor = Executors.newSingleThreadExecutor();
    }

    @Override
    public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
        switch (call.method) {
            case "getPlatformVersion":
                result.success("Android " + android.os.Build.VERSION.RELEASE);
                break;
            case "init": {
                int ret = mBarcodeManager.init();
                result.success(ret);
            }
            break;
            case "setLicense": {
                final String license = call.argument("license");
                mBarcodeManager.setLicense(license, result);
            }
            break;
            case "decodeFile": {
                final String filename = call.argument("filename");
                List<Map<String, Object>> results = mBarcodeManager.decodeFile(filename);
                result.success(results);
            }
            break;
            case "decodeFileBytes": {
                final byte[] bytes = call.argument("bytes");
                List<Map<String, Object>> results = mBarcodeManager.decodeFileBytes(bytes);
                result.success(results);
            }
            break;
            case "decodeImageBuffer": {
                final byte[] bytes = call.argument("bytes");
                final int width = call.argument("width");
                final int height = call.argument("height");
                final int stride = call.argument("stride");
                final int format = call.argument("format");
                final Result r = result;
                mExecutor.execute(new Runnable() {
                    @Override
                    public void run() {
                        final List<Map<String, Object>> results = mBarcodeManager.decodeImageBuffer(bytes, width, height, stride, format);
                        mHandler.post(new Runnable() {
                            @Override
                            public void run() {
                                r.success(results);
                            }
                        });

                    }
                });
            }
            break;
            case "setBarcodeFormats": {
                final int formats = call.argument("formats");
                int ret = mBarcodeManager.setBarcodeFormats(formats);
                result.success(ret);
            }
            break;
            case "getParameters": {
                result.success(mBarcodeManager.getParameters());
            }
            break;
            case "setParameters": {
                final String params = call.argument("params");
                int ret = mBarcodeManager.setParameters(params);
                result.success(ret);
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
