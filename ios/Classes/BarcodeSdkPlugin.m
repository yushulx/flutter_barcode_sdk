#import "BarcodeSdkPlugin.h"
#if __has_include(<barcode_sdk/barcode_sdk-Swift.h>)
#import <barcode_sdk/barcode_sdk-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "barcode_sdk-Swift.h"
#endif

@implementation BarcodeSdkPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftBarcodeSdkPlugin registerWithRegistrar:registrar];
}
@end
