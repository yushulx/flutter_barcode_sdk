# flutter_barcode_sdk_macos

macOS platform implementation of [flutter_barcode_sdk](https://pub.dev/packages/flutter_barcode_sdk).

This package provides barcode scanning on macOS using the Dynamsoft
Barcode Reader C++ SDK (v11.6.1000). Universal-2 dylibs for both
Apple Silicon and Intel Macs are bundled.

## Requirements

- macOS 12.0 or later
- Xcode 14 or later
- CocoaPods (`brew install cocoapods`)

## Running the example

```bash
cd packages/flutter_barcode_sdk_macos/example
flutter pub get
flutter run -d macos
```

The first build automatically runs `pod install`, which links the
bundled universal-2 dylibs (`@rpath/` install names) and embeds them
into the app's `Frameworks/` directory.

If the license is not yet activated, the app applies for a temporary
trial license automatically. Replace the demo key in
`example/lib/license.dart` with your own from the
[Dynamsoft License Portal](https://www.dynamsoft.com/customer/license/trialLicense/).

## Permissions in your own app

The example app already contains the required entitlements and Info.plist
entries. If you integrate `flutter_barcode_sdk_macos` into your own macOS
app, make sure to configure the same settings:

**`macos/Runner/DebugProfile.entitlements`** and
**`macos/Runner/Release.entitlements`**:

```xml
<key>com.apple.security.network.client</key>
<true/>
<key>com.apple.security.device.camera</key>
<true/>
<key>com.apple.security.files.user-selected.read-write</key>
<true/>
```

- `network.client` is **required** for Dynamsoft license activation.
- `device.camera` is required for live camera barcode scanning.
- `files.user-selected.read-write` is required to pick image files.

**`macos/Runner/Info.plist`**:

```xml
<key>NSCameraUsageDescription</key>
<string>Camera access is required for barcode scanning.</string>
```

Also set `MACOSX_DEPLOYMENT_TARGET = 12.0` (or later) in
`macos/Runner.xcodeproj/project.pbxproj`, because the Dynamsoft SDK
requires macOS 12+.

## Usage

This package is not intended for direct use by app developers.
It is automatically endorsed by `flutter_barcode_sdk` on the macOS platform.