/// A common platform interface for the flutter_barcode_sdk plugin.
///
/// This library defines the contract that all platform implementations
/// (Android, iOS, Web, Windows, Linux) must follow. It also provides
/// a default [MethodChannelBarcodeSdk] implementation that communicates
/// with native code via a [MethodChannel].
library flutter_barcode_sdk_platform_interface;

export 'src/barcode_sdk_platform.dart';
export 'src/method_channel_barcode_sdk.dart';
export 'src/barcode_result.dart';
export 'src/barcode_format.dart';
export 'src/image_pixel_format.dart';
export 'src/image_rotation.dart';
