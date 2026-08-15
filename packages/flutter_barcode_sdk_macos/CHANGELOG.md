## 1.0.2

* Fixed missing Dynamsoft `Templates/` and `Models/` resources in the
  built app, which caused `InitSettings error -10030` ("Syntax error:
  value, object or array expected") at startup — the SDK loads them from
  the directory containing its dylibs, but they were buried in a
  `.bundle` inside the plugin framework.
  - Podspec: replaced the misused `resource_bundles` with
    `preserve_paths` for the `Resources/` folder.
  - Example: added a `Copy Dynamsoft Resources` build phase to the
    Runner target that copies the folders into `Contents/Resources/` and
    exposes them in `Contents/Frameworks/` via symlinks (placing data
    files in `Frameworks/` directly breaks code signing).
* README: documented the required resource setup for consumers, with a
  ready-to-paste `post_install` snippet for `macos/Podfile` that injects
  the build phase automatically on `pod install`.

## 1.0.1

* Fixed broken install names of the bundled `libDynamicPdf.dylib` and
  `libDynamsoftNeuralNetwork.dylib` (now `@rpath/...`), which crashed the
  app at launch with "Library not loaded".
* Example: raised the Podfile platform to macOS 12.0.
* Example: disabled the app sandbox — the Dynamsoft license module needs
  IOKit access and a writable license cache, otherwise license activation
  fails with `EC_LICENSE_BUFFER_FAILED (-20002)`.
* Example: skip `setParameters` when `getParameters` returns an empty
  string (avoids `-10030` / `EC_JSON_PARSE_FAILED`).

## 1.0.0

* Initial release.
* macOS implementation of `flutter_barcode_sdk` using the Dynamsoft
  Barcode Reader C++ SDK (11.6.1000, universal-2 dylibs bundled).
* Implements all methods of `BarcodeSdkPlatform` over the
  `flutter_barcode_sdk` MethodChannel.