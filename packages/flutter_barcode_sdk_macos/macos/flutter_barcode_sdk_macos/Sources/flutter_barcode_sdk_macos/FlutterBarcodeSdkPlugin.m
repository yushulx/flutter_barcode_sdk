#import "FlutterBarcodeSdkPlugin.h"

#import "BarcodeManagerBridge.h"

@interface FlutterBarcodeSdkPlugin ()
@property(nonatomic, strong) BarcodeManagerBridge *bridge;
@end

@implementation FlutterBarcodeSdkPlugin

+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  FlutterMethodChannel *channel =
      [FlutterMethodChannel methodChannelWithName:@"flutter_barcode_sdk"
                                  binaryMessenger:registrar.messenger];
  FlutterBarcodeSdkPlugin *instance = [[FlutterBarcodeSdkPlugin alloc] init];
  instance.bridge = [[BarcodeManagerBridge alloc] init];
  [registrar addMethodCallDelegate:instance channel:channel];
}

- (void)handleMethodCall:(FlutterMethodCall *)call result:(FlutterResult)result {
  NSDictionary *args = call.arguments;
  if ([@"init" isEqualToString:call.method]) {
    result(@([self.bridge initSdk]));
  } else if ([@"setLicense" isEqualToString:call.method]) {
    result(@([self.bridge setLicense:args[@"license"]]));
  } else if ([@"decodeFile" isEqualToString:call.method]) {
    result([self.bridge decodeFile:args[@"filename"]]);
  } else if ([@"decodeImageBuffer" isEqualToString:call.method]) {
    // Decode asynchronously via StartCapturing callbacks (same approach as
    // the Windows/Linux implementations). The bridge invokes the completion
    // on the main queue, where the FlutterResult reply is safe.
    FlutterStandardTypedData *bytes = args[@"bytes"];
    [self.bridge decodeImageBuffer:bytes.data
                             width:[args[@"width"] intValue]
                            height:[args[@"height"] intValue]
                            stride:[args[@"stride"] intValue]
                            format:[args[@"format"] intValue]
                          rotation:[args[@"rotation"] intValue]
                        completion:^(NSArray<NSDictionary *> *results) {
                          result(results);
                        }];
  } else if ([@"setBarcodeFormats" isEqualToString:call.method]) {
    result(@([self.bridge setBarcodeFormats:[args[@"formats"] unsignedLongLongValue]]));
  } else if ([@"getParameters" isEqualToString:call.method]) {
    result([self.bridge getParameters]);
  } else if ([@"setParameters" isEqualToString:call.method]) {
    result(@([self.bridge setParameters:args[@"params"]]));
  } else {
    result(FlutterMethodNotImplemented);
  }
}

@end
