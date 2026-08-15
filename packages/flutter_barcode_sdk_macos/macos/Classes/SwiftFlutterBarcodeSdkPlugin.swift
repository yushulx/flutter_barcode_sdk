import FlutterMacOS
import Foundation

/// macOS implementation of the flutter_barcode_sdk plugin.
///
/// Uses [BarcodeManagerBridge] (an ObjC++ wrapper around the Dynamsoft C++
/// CaptureVisionRouter SDK) to decode barcodes. All methods are exposed
/// over the `flutter_barcode_sdk` MethodChannel, matching the wire format
/// of the other platform implementations.
public class SwiftFlutterBarcodeSdkPlugin: NSObject, FlutterPlugin {
  /// Bridge to the Dynamsoft C++ SDK.
  private let bridge = BarcodeManagerBridge()

  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(
      name: "flutter_barcode_sdk",
      binaryMessenger: registrar.messenger
    )
    let instance = SwiftFlutterBarcodeSdkPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "init":
      result(bridge.initSdk())
    case "setLicense":
      let args = call.arguments as! [String: Any]
      let ret = bridge.setLicense(args["license"] as! String)
      result(ret)
    case "decodeFile":
      let args = call.arguments as! [String: Any]
      let res = bridge.decodeFile(args["filename"] as! String)
      result(res)
    case "decodeImageBuffer":
      // Decode asynchronously via StartCapturing callbacks (same approach
      // as the Windows/Linux implementations). The bridge invokes the
      // completion on the main queue, where the FlutterResult reply is safe.
      let args = call.arguments as! [String: Any]
      let bytes = (args["bytes"] as! FlutterStandardTypedData).data
      let width = args["width"] as! Int
      let height = args["height"] as! Int
      let stride = args["stride"] as! Int
      let format = args["format"] as! Int
      let rotation = args["rotation"] as! Int
      bridge.decodeImageBuffer(
        bytes,
        width: Int32(width),
        height: Int32(height),
        stride: Int32(stride),
        format: Int32(format),
        rotation: Int32(rotation)
      ) { res in
        result(res)
      }
    case "setBarcodeFormats":
      let args = call.arguments as! [String: Any]
      let formats = args["formats"] as! NSNumber
      let ret = bridge.setBarcodeFormats(formats.uint64Value)
      result(ret)
    case "getParameters":
      result(bridge.getParameters())
    case "setParameters":
      let args = call.arguments as! [String: Any]
      let ret = bridge.setParameters(args["params"] as! String)
      result(ret)
    default:
      result(FlutterMethodNotImplemented)
    }
  }
}