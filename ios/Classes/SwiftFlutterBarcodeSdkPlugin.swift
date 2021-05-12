import Flutter
import UIKit
import DynamsoftBarcodeReader

public class SwiftFlutterBarcodeSdkPlugin: NSObject, FlutterPlugin, DMLTSLicenseVerificationDelegate{
    
    var reader: DynamsoftBarcodeReader? = DynamsoftBarcodeReader()
    
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "flutter_barcode_sdk", binaryMessenger: registrar.messenger())
        let instance = SwiftFlutterBarcodeSdkPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }
    
    public override init() {
        super.init()
        let lts = iDMLTSConnectionParameters()
        // The organization id 200001 here will grant you a public trial license good for 7 days. After that, you can send an email to trial@dynamsoft.com (make sure to include the keyword privateTrial in the email title) to obtain a 30-day free private trial license which will also come in the form of an organization id.
        lts.organizationID = "200001"
        reader = DynamsoftBarcodeReader(licenseFromLTS: lts, verificationDelegate: self)
        
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
    
    public func ltsLicenseVerificationCallback(_ isSuccess: Bool, error: Error?) {
        if (error != nil) {
            let err: NSError = error! as NSError
            print(err.userInfo)
        }
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "getPlatformVersion":
            result("iOS " + UIDevice.current.systemVersion)
        case "setLicense":
            self.setLicense(arg: call.arguments as! NSDictionary)
            result(.none)
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
    
    func decodeBuffer(arguments:NSDictionary) -> NSArray {
        let buffer:FlutterStandardTypedData = arguments.value(forKey: "bytes") as! FlutterStandardTypedData
        let w:Int = arguments.value(forKey: "width") as! Int
        let h:Int = arguments.value(forKey: "height") as! Int
        let stride:Int = arguments.value(forKey: "stride") as! Int
        let ret:[iTextResult] = try! reader!.decodeBuffer(buffer.data, withWidth: w, height: h, stride: stride, format:.ARGB_8888, templateName: "")
        return self.wrapResults(results: ret)
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
