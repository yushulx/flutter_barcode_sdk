/// macOS platform implementation of `flutter_barcode_sdk`.
///
/// This package provides the native macOS implementation of the
/// `flutter_barcode_sdk` federated plugin. It uses a Swift plugin class
/// backed by an ObjC++ bridge to the Dynamsoft C++ CaptureVisionRouter
/// SDK (universal-2 dylibs are bundled in the pod).
///
/// The Dart side uses the default `MethodChannelBarcodeSdk` from
/// `flutter_barcode_sdk_platform_interface`; native code registers
/// the `flutter_barcode_sdk` MethodChannel handler.
library flutter_barcode_sdk_macos;