# flutter_barcode_sdk_platform_interface

A common platform interface for the [flutter_barcode_sdk](https://pub.dev/packages/flutter_barcode_sdk) plugin.

This package defines the contract that all platform implementations
(Android, iOS, Web, Windows, Linux) must follow. It also provides
a default `MethodChannelBarcodeSdk` implementation that communicates
with native code via a `MethodChannel`.

## Usage

This package is not intended for direct use by app developers.
It is used by the platform implementation packages of `flutter_barcode_sdk`.
