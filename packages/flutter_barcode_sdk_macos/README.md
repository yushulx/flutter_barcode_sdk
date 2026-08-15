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

## Dynamsoft resources in your own app (required)

The Dynamsoft SDK loads its preset templates and recognition models from
`Templates/` and `Models/` directories located **next to its dylibs**
(i.e. `Contents/Frameworks/` of the built app). If they are missing,
`OutputSettings`/`Capture` fail — typically surfacing as
`InitSettings error -10030` ("Syntax error: value, object or array
expected") at startup.

Data files cannot be placed in `Contents/Frameworks/` directly (code
signing treats everything there as code, and the build fails with
"code object is not signed at all"), so the folders must be copied into
`Contents/Resources/` and exposed in `Contents/Frameworks/` through
symlinks. A Flutter plugin cannot add an app-level build phase by itself
(CocoaPods integrates pods into the *pod* target, never into your app
target), so pick **one** of the two options below. The example app
already ships the resulting build phase, so you can also just copy it
from `example/macos/Runner.xcodeproj`.

### Option 1 — automatic via `macos/Podfile` (recommended)

Paste this into the existing `post_install` block of your app's
`macos/Podfile`. On every `pod install` it injects a
`Copy Dynamsoft Resources` build phase into your Runner target (and only
once — re-running `pod install` does not duplicate it):

```ruby
post_install do |installer|
  installer.pods_project.targets.each do |target|
    flutter_additional_macos_build_settings(target)
  end

  # flutter_barcode_sdk_macos: bundle the Dynamsoft Templates/ and
  # Models/ folders next to the Dynamsoft dylibs on every build.
  installer.aggregate_targets.each do |aggregate_target|
    aggregate_target.user_targets.each do |target|
      next if target.build_phases.any? { |phase| phase.respond_to?(:name) && phase.name == 'Copy Dynamsoft Resources' }

      phase = target.new_shell_script_build_phase('Copy Dynamsoft Resources')
      phase.input_paths = [
        '${PODS_ROOT}/../Flutter/ephemeral/.symlinks/plugins/flutter_barcode_sdk_macos/macos/Resources/Templates',
        '${PODS_ROOT}/../Flutter/ephemeral/.symlinks/plugins/flutter_barcode_sdk_macos/macos/Resources/Models',
      ]
      phase.output_paths = [
        '${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/Templates',
        '${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/Models',
        '${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Templates',
        '${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Models',
      ]
      phase.shell_script = <<~'SH'
        DYNAMSOFT_RESOURCES="${PODS_ROOT}/../Flutter/ephemeral/.symlinks/plugins/flutter_barcode_sdk_macos/macos/Resources"
        if [ -d "$DYNAMSOFT_RESOURCES" ]; then
          rsync -a --delete "$DYNAMSOFT_RESOURCES/Templates" "${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/"
          rsync -a --delete "$DYNAMSOFT_RESOURCES/Models" "${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/"
          ln -sfn ../Resources/Templates "${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Templates"
          ln -sfn ../Resources/Models "${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Models"
        fi
      SH
      aggregate_target.user_project.save
    end
  end
end
```

Then run `pod install` (or just `flutter run -d macos`, which runs it
automatically). Note that this modifies your `Runner.xcodeproj` — commit
that change like any other Xcode project edit.

### Option 2 — manual via Xcode

1. Open `macos/Runner.xcworkspace` in Xcode.
2. Select the **Runner** target → **Build Phases**.
3. Click **+** → **New Run Script Phase**, rename it to
   `Copy Dynamsoft Resources`, and drag it below
   `[CP] Embed Pods Frameworks`.
4. Paste this script:

   ```sh
   DYNAMSOFT_RESOURCES="${PODS_ROOT}/../Flutter/ephemeral/.symlinks/plugins/flutter_barcode_sdk_macos/macos/Resources"
   if [ -d "$DYNAMSOFT_RESOURCES" ]; then
     rsync -a --delete "$DYNAMSOFT_RESOURCES/Templates" "${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/"
     rsync -a --delete "$DYNAMSOFT_RESOURCES/Models" "${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/"
     ln -sfn ../Resources/Templates "${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Templates"
     ln -sfn ../Resources/Models "${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/Models"
   fi
   ```

5. Under **Input Files** add the two source folders
   (`.../Resources/Templates`, `.../Resources/Models`) and under
   **Output Files** the four destinations
   (`$(TARGET_BUILD_DIR)/$(UNLOCALIZED_RESOURCES_FOLDER_PATH)/Templates`,
   `$(TARGET_BUILD_DIR)/$(UNLOCALIZED_RESOURCES_FOLDER_PATH)/Models`,
   `$(TARGET_BUILD_DIR)/$(FRAMEWORKS_FOLDER_PATH)/Templates`,
   `$(TARGET_BUILD_DIR)/$(FRAMEWORKS_FOLDER_PATH)/Models`).
   Declaring them keeps the phase compatible with Xcode's user script
   sandboxing.

### Verify

Run the app; the console should show `Parameter update: 0`. If you still
see `-10030`, confirm the built app contains
`Contents/Frameworks/Templates -> ../Resources/Templates` and
`Contents/Resources/Templates/DBR-PresetTemplates.json`.

## Permissions in your own app

The example app already contains the required entitlements and Info.plist
entries. If you integrate `flutter_barcode_sdk_macos` into your own macOS
app, make sure to configure the same settings:

**`macos/Runner/DebugProfile.entitlements`** and
**`macos/Runner/Release.entitlements`**:

```xml
<key>com.apple.security.app-sandbox</key>
<false/>
<key>com.apple.security.network.client</key>
<true/>
<key>com.apple.security.device.camera</key>
<true/>
<key>com.apple.security.files.user-selected.read-write</key>
<true/>
```

- `app-sandbox` must be **disabled**. The Dynamsoft license module reads
  hardware info via IOKit and writes a local license cache; the sandbox
  denies both, so license activation fails with
  `EC_LICENSE_BUFFER_FAILED (-20002)` ("Failed to read or write license
  buffer").
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