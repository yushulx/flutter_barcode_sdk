import DynamsoftBarcodeReader
import DynamsoftCaptureVisionRouter
import DynamsoftCore
import DynamsoftLicense
import Flutter
import UIKit

public class SwiftFlutterBarcodeSdkPlugin: NSObject, FlutterPlugin, LicenseVerificationListener {
    let cvr = CaptureVisionRouter()
    var completionHandlers: [FlutterResult] = []

    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(
            name: "flutter_barcode_sdk", binaryMessenger: registrar.messenger())
        let instance = SwiftFlutterBarcodeSdkPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    func initObj() -> Int {
        return 0
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "init":
            let ret = self.initObj()
            result(ret)
        case "setLicense":
            completionHandlers.append(result)
            self.setLicense(arg: call.arguments as! NSDictionary)
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
            result(.none)
        }
    }

    func decodeBuffer(arguments: NSDictionary) -> NSArray {
        let buffer: FlutterStandardTypedData =
            arguments.value(forKey: "bytes") as! FlutterStandardTypedData
        let w: Int = arguments.value(forKey: "width") as! Int
        let h: Int = arguments.value(forKey: "height") as! Int
        let stride: Int = arguments.value(forKey: "stride") as! Int
        let format: Int = arguments.value(forKey: "format") as! Int
        let enumImagePixelFormat = ImagePixelFormat(rawValue: format)

        let imageData = ImageData(
            bytes: buffer.data, width: UInt(w), height: UInt(h), stride: UInt(stride),
            format: enumImagePixelFormat!, orientation: 0, tag: nil)

        let ret = cvr.captureFromBuffer(imageData, templateName: "")
        return self.wrapResults(result: ret)
    }

    func setBarcodeFormats(arg: NSDictionary) -> Int {
        let formats: Int = arg.value(forKey: "formats") as! Int
        let settings = try! cvr.getSimplifiedSettings("")
        settings.barcodeSettings?.barcodeFormatIds = BarcodeFormat(rawValue: UInt(formats))
        do {
            try cvr.updateSettings("", settings: settings)
            return 0  // Success
        } catch {
            print("Error initializing settings: \(error)")
            return -1  // Failure
        }
    }

    func getParameters() -> String {
        let ret = try! cvr.outputSettings("")
        return ret
    }

    func setParameters(arg: NSDictionary) -> Int {
        let params: String = arg.value(forKey: "params") as! String
        do {
            try cvr.initSettings(params)
            return 0  // Success
        } catch {
            print("Error initializing settings: \(error)")
            return -1  // Failure
        }
    }

    func decodeFile(arg: NSDictionary) -> NSArray {
        let path: String = arg.value(forKey: "filename") as! String
        let ret = self.cvr.captureFromFile(path, templateName: "")
        return self.wrapResults(result: ret)
    }

    func setLicense(arg: NSDictionary) {
        let license: String = arg.value(forKey: "license") as! String
        LicenseManager.initLicense(license, verificationDelegate: self)
    }

    public func onLicenseVerified(_ isSuccess: Bool, error: Error?) {
        if isSuccess {
            completionHandlers.first?(0)
        } else {
            completionHandlers.first?(-1)
        }
    }

    func wrapResults(result: CapturedResult) -> NSArray {
        let outResults = NSMutableArray()
        let items = result.items

        if items != nil && items!.count > 0 {
            for item in items! {
                if item.type == .barcode {
                    let barcodeItem: BarcodeResultItem = item as! BarcodeResultItem
                    let subDic = NSMutableDictionary()
                    subDic.setObject(barcodeItem.formatString, forKey: "format" as NSCopying)
                    subDic.setObject(barcodeItem.text, forKey: "text" as NSCopying)
                    let points = barcodeItem.location.points as! [CGPoint]
                    subDic.setObject(Int(points[0].x), forKey: "x1" as NSCopying)
                    subDic.setObject(Int(points[0].y), forKey: "y1" as NSCopying)
                    subDic.setObject(Int(points[1].x), forKey: "x2" as NSCopying)
                    subDic.setObject(Int(points[1].y), forKey: "y2" as NSCopying)
                    subDic.setObject(Int(points[2].x), forKey: "x3" as NSCopying)
                    subDic.setObject(Int(points[2].y), forKey: "y3" as NSCopying)
                    subDic.setObject(Int(points[3].x), forKey: "x4" as NSCopying)
                    subDic.setObject(Int(points[3].y), forKey: "y4" as NSCopying)
                    subDic.setObject(
                        barcodeItem.angle, forKey: "angle" as NSCopying)
                    subDic.setObject(barcodeItem.bytes, forKey: "barcodeBytes" as NSCopying)
                    outResults.add(subDic)
                }
            }
        }

        return outResults
    }
}
