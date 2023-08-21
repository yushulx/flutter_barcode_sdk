import Cocoa
import FlutterMacOS

public class FlutterBarcodeSdkPlugin: NSObject, FlutterPlugin {
    
    var reader: DynamsoftBarcodeReader? = DynamsoftBarcodeReader()
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "flutter_barcode_sdk", binaryMessenger: registrar.messenger)
        let instance = FlutterBarcodeSdkPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    var callback: Callback = Callback()

    public class Callback: NSObject, DBRLicenseVerificationListener {
        var completionHandlers: [FlutterResult] = []

        public func append(result: @escaping FlutterResult) {
            completionHandlers.append(result)
        }

        public func dbrLicenseVerificationCallback(_ isSuccess: Bool, error: Error?)
        {
            if isSuccess {
                completionHandlers.first?(0)
            } else{
                completionHandlers.first?(-1)
            }
        } 
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "getPlatformVersion":
            result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
        case "init":
            let ret = self.initObj()
            result(ret)
        case "setLicense":
            callback.append(result: result)
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
            result(FlutterMethodNotImplemented)
        }
    }

    func decodeBuffer(arguments:NSDictionary) -> NSArray {
        let buffer:FlutterStandardTypedData = arguments.value(forKey: "bytes") as! FlutterStandardTypedData
        let w:Int = arguments.value(forKey: "width") as! Int
        let h:Int = arguments.value(forKey: "height") as! Int
        let stride:Int = arguments.value(forKey: "stride") as! Int
        let format:Int = arguments.value(forKey: "format") as! Int
        let enumImagePixelFormat = EnumImagePixelFormat(rawValue: format)
        let ret:[iTextResult] = try! reader!.decodeBuffer(buffer.data, withWidth: w, height: h, stride: stride, format: enumImagePixelFormat!, templateName: "")
        return self.wrapResults(results: ret)
    }
    
    public override init() {
        super.init()
        reader = DynamsoftBarcodeReader()
        
        //Best Coverage settings
        //barcodeReader.initRuntimeSettings(with: "{\"ImageParameter\":{\"Name\":\"BestCoverage\",\"DeblurLevel\":9,\"ExpectedBarcodesCount\":512,\"ScaleDownThreshold\":100000,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"},{\"Mode\":\"LM_STATISTICS\"},{\"Mode\":\"LM_LINES\"},{\"Mode\":\"LM_STATISTICS_MARKS\"}],\"GrayscaleTransformationModes\":[{\"Mode\":\"GTM_ORIGINAL\"},{\"Mode\":\"GTM_INVERTED\"}]}}", conflictMode: EnumConflictMode.overwrite, error: nil)
        //Best Speed settings
        //barcodeReader.initRuntimeSettings(with: "{\"ImageParameter\":{\"Name\":\"BestSpeed\",\"DeblurLevel\":3,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_SCAN_DIRECTLY\"}],\"TextFilterModes\":[{\"MinImageDimension\":262144,\"Mode\":\"TFM_GENERAL_CONTOUR\"}]}}", conflictMode: EnumConflictMode.overwrite, error: nil)
        //balance settings
        reader!.initRuntimeSettings(with: "{\"ImageParameter\":{\"Name\":\"Balance\",\"DeblurLevel\":5,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"}]}}", conflictMode: EnumConflictMode.overwrite, error:nil)
        let settings = try! reader!.getRuntimeSettings()
        settings.barcodeFormatIds = Int(EnumBarcodeFormat.ONED.rawValue) | Int(EnumBarcodeFormat.PDF417.rawValue) | Int(EnumBarcodeFormat.QRCODE.rawValue) | Int(EnumBarcodeFormat.DATAMATRIX.rawValue)
        reader!.update(settings, error: nil)
        reader!.setModeArgument("BinarizationModes", index: 0, argumentName: "EnableFillBinaryVacancy", argumentValue: "0", error: nil)
        reader!.setModeArgument("BinarizationModes", index: 0, argumentName: "BlockSizeX", argumentValue: "81", error: nil)
        reader!.setModeArgument("BinarizationModes", index: 0, argumentName: "BlockSizeY", argumentValue: "81", error: nil)
    }

    func initObj() -> Int {
        
        if (reader == nil) {
        reader = DynamsoftBarcodeReader()
        }

        return 0
    }

    func decodeFile(arg:NSDictionary) -> NSArray {
        let path:String = arg.value(forKey: "filename") as! String
        let ret:[iTextResult] = try! self.reader!.decodeFile(withName: path, templateName: "")
        return self.wrapResults(results: ret)
    }

    func setLicense(arg:NSDictionary) {
        let license: String = arg.value(forKey: "license") as! String
        DynamsoftBarcodeReader.initLicense(license, verificationDelegate: callback)     
    }
    
    func setBarcodeFormats(arg:NSDictionary) -> Int {
        let formats:Int = arg.value(forKey: "formats") as! Int
        let settings = try! reader!.getRuntimeSettings()
        settings.barcodeFormatIds = formats
        reader!.update(settings, error: nil)
        return 0
    }
    
    func getParameters() -> String {
        let ret = try! reader!.outputSettings(to: "currentRuntimeSettings")
        return ret
    }
    
    func setParameters(arg:NSDictionary) -> Int{
        let params:String = arg.value(forKey: "params") as! String
        reader!.initRuntimeSettings(with: params, conflictMode: .overwrite, error: nil)
        return 0
    }

    func wrapResults(results:[iTextResult]) -> NSArray {
        let outResults = NSMutableArray(capacity: 8)
        for item in results {
            let subDic = NSMutableDictionary(capacity: 11)
            if item.barcodeFormat_2 != EnumBarcodeFormat2.Null {
                subDic.setObject(item.barcodeFormatString_2 ?? "", forKey: "format" as NSCopying)
            }else{
                subDic.setObject(item.barcodeFormatString ?? "", forKey: "format" as NSCopying)
            }
            // subDic.setObject(item.barcodeText ?? "", forKey: "text" as NSCopying)
            let points = item.localizationResult?.resultPoints as! [CGPoint]
            subDic.setObject(Int(points[0].x), forKey: "x1" as NSCopying)
            subDic.setObject(Int(points[0].y), forKey: "y1" as NSCopying)
            subDic.setObject(Int(points[1].x), forKey: "x2" as NSCopying)
            subDic.setObject(Int(points[1].y), forKey: "y2" as NSCopying)
            subDic.setObject(Int(points[2].x), forKey: "x3" as NSCopying)
            subDic.setObject(Int(points[2].y), forKey: "y3" as NSCopying)
            subDic.setObject(Int(points[3].x), forKey: "x4" as NSCopying)
            subDic.setObject(Int(points[3].y), forKey: "y4" as NSCopying)
            subDic.setObject(item.localizationResult?.angle ?? 0, forKey: "angle" as NSCopying)
            subDic.setObject(item.barcodeBytes ?? "", forKey: "barcodeBytes" as NSCopying)
            outResults.add(subDic)
        }

        return outResults
    }
}
