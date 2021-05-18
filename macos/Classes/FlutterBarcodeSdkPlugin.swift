import Cocoa
import FlutterMacOS

public class FlutterBarcodeSdkPlugin: NSObject, FlutterPlugin {
    
    var reader: DynamsoftBarcodeReader? = DynamsoftBarcodeReader()
    
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "flutter_barcode_sdk", binaryMessenger: registrar.messenger)
        let instance = FlutterBarcodeSdkPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "getPlatformVersion":
            result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
        case "setLicense":
            self.setLicense(arg: call.arguments as! NSDictionary)
            result(.none)
        case "setBarcodeFormats":
            self.setBarcodeFormats(arg: call.arguments as! NSDictionary)
            result(.none)
        case "decodeFile":
            let res = self.decodeFile(arg: call.arguments as! NSDictionary)
            result(res)
        default:
            result(FlutterMethodNotImplemented)
        }
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

    func decodeFile(arg:NSDictionary) -> NSArray {
        let path:String = arg.value(forKey: "filename") as! String
        let ret:[iTextResult] = try! self.reader!.decodeFile(withName: path, templateName: "")
        return self.wrapResults(results: ret)
    }

    func setLicense(arg:NSDictionary) {
        let lic:String = arg.value(forKey: "license") as! String
        reader = DynamsoftBarcodeReader(license: lic)
    }
    
    func setBarcodeFormats(arg:NSDictionary) {
        let formats:Int = arg.value(forKey: "formats") as! Int
        let settings = try! reader!.getRuntimeSettings()
        settings.barcodeFormatIds = formats
        reader!.update(settings, error: nil)
    }

    func wrapResults(results:[iTextResult]) -> NSArray {
        let outResults = NSMutableArray(capacity: 8)
        for item in results {
            let subDic = NSMutableDictionary(capacity: 8)
            if item.barcodeFormat_2 != EnumBarcodeFormat2.Null {
                subDic.setObject(item.barcodeFormatString_2 ?? "", forKey: "format" as NSCopying)
            }else{
                subDic.setObject(item.barcodeFormatString ?? "", forKey: "format" as NSCopying)
            }
            subDic.setObject(item.barcodeText ?? "", forKey: "text" as NSCopying)
            let points = item.localizationResult?.resultPoints as! [CGPoint]
            subDic.setObject(Int(points[0].x), forKey: "x1" as NSCopying)
            subDic.setObject(Int(points[0].x), forKey: "y1" as NSCopying)
            subDic.setObject(Int(points[1].x), forKey: "x2" as NSCopying)
            subDic.setObject(Int(points[1].x), forKey: "y2" as NSCopying)
            subDic.setObject(Int(points[2].x), forKey: "x3" as NSCopying)
            subDic.setObject(Int(points[2].x), forKey: "y3" as NSCopying)
            subDic.setObject(Int(points[3].x), forKey: "x4" as NSCopying)
            subDic.setObject(Int(points[3].x), forKey: "y4" as NSCopying)
            subDic.setObject(item.localizationResult?.angle ?? 0, forKey: "angle" as NSCopying)
            outResults.add(subDic)
        }

        return outResults
    }
}
