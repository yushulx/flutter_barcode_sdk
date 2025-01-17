import Cocoa
import DCV
import FlutterMacOS

public class FlutterBarcodeSdkPlugin: NSObject, FlutterPlugin {

    let cvr: CaptureVisionWrapper = CaptureVisionWrapper()

    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(
            name: "flutter_barcode_sdk", binaryMessenger: registrar.messenger)
        let instance = FlutterBarcodeSdkPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "getPlatformVersion":
            result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
        case "init":
            let ret = self.initObj()
            result(ret)
        case "setLicense":
            let ret = self.setLicense(arg: call.arguments as! NSDictionary)
            result(ret)
        case "setBarcodeFormats":
            let ret = self.setBarcodeFormats(arg: call.arguments as! NSDictionary)
            result(ret)
        case "getParameters":
            result(self.getParameters())
        case "setParameters":
            let ret = self.setParameters(arg: call.arguments as! NSDictionary)
            result(ret)
        case "decodeFile":
            let res = self.decodeFile(arg: call.arguments as! NSDictionary)
            result(res)
        case "decodeImageBuffer":
            DispatchQueue.global().async {
                let res = self.decodeBuffer(arguments: call.arguments as! NSDictionary)
                result(res)
            }
        default:
            result(FlutterMethodNotImplemented)
        }
    }

    func decodeBuffer(arguments: NSDictionary) -> NSArray {
        guard
            let buffer = arguments.value(forKey: "bytes") as? FlutterStandardTypedData,
            let width = arguments.value(forKey: "width") as? Int,
            let height = arguments.value(forKey: "height") as? Int,
            let stride = arguments.value(forKey: "stride") as? Int,
            let format = arguments.value(forKey: "format") as? Int,
            let pixelFormat = PixelFormat(rawValue: format)
        else {
            return []
        }

        // Convert Data to UnsafeMutableRawPointer
        let dataPointer = UnsafeMutableRawPointer(mutating: (buffer.data as NSData).bytes)

        let barcodeArray =
            cvr.decodeBuffer(
                withData: dataPointer,
                width: Int32(width),
                height: Int32(height),
                stride: Int32(stride),
                pixelFormat: pixelFormat) as? [[String: Any]] ?? []

        return self.wrapResults(results: barcodeArray)
    }

    public override init() {
        super.init()
    }

    func initObj() -> Int {
        return 0
    }

    func decodeFile(arg: NSDictionary) -> NSArray {
        guard let path = arg.value(forKey: "filename") as? String else { return [] }
        let barcodeArray = cvr.decodeFile(withPath: path) as? [[String: Any]] ?? []
        return self.wrapResults(results: barcodeArray)
    }

    func setLicense(arg: NSDictionary) -> Int {
        guard let license = arg.value(forKey: "license") as? String else { return -1 }
        let ret = CaptureVisionWrapper.initializeLicense(license)
        return Int(ret)
    }

    func setBarcodeFormats(arg: NSDictionary) -> Int {
        guard let formats = arg.value(forKey: "formats") as? UInt64 else { return -1 }
        let ret = cvr.setBarcodeFormats(formats)
        return Int(ret)
    }

    func getParameters() -> String {
        return cvr.getSettings() ?? ""
    }

    func setParameters(arg: NSDictionary) -> Int {
        guard let params = arg.value(forKey: "params") as? String else { return -1 }
        let ret = cvr.setSettings(params)
        return Int(ret)
    }

    func wrapResults(results: [[String: Any]]) -> NSArray {
        let outResults = NSMutableArray()

        for barcode in results {
            let subDic = NSMutableDictionary()

            guard
                let points = barcode["points"] as? [[String: NSNumber]],
                let format = barcode["format"] as? String,
                let text = barcode["text"] as? String,
                let angle = barcode["angle"] as? Int,
                let barcodeBytes = barcode["barcodeBytes"] as? Data
            else {
                continue
            }

            subDic.setObject(format, forKey: "format" as NSCopying)
            subDic.setObject(text, forKey: "text" as NSCopying)
            subDic.setObject(angle, forKey: "angle" as NSCopying)
            subDic.setObject(barcodeBytes, forKey: "barcodeBytes" as NSCopying)

            if points.count >= 4 {
                subDic.setObject(points[0]["x"]?.intValue ?? 0, forKey: "x1" as NSCopying)
                subDic.setObject(points[0]["y"]?.intValue ?? 0, forKey: "y1" as NSCopying)
                subDic.setObject(points[1]["x"]?.intValue ?? 0, forKey: "x2" as NSCopying)
                subDic.setObject(points[1]["y"]?.intValue ?? 0, forKey: "y2" as NSCopying)
                subDic.setObject(points[2]["x"]?.intValue ?? 0, forKey: "x3" as NSCopying)
                subDic.setObject(points[2]["y"]?.intValue ?? 0, forKey: "y3" as NSCopying)
                subDic.setObject(points[3]["x"]?.intValue ?? 0, forKey: "x4" as NSCopying)
                subDic.setObject(points[3]["y"]?.intValue ?? 0, forKey: "y4" as NSCopying)
            }

            outResults.add(subDic)
        }

        return outResults
    }
}
