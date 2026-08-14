# flutter_barcode_sdk

[![pub package](https://img.shields.io/pub/v/flutter_barcode_sdk.svg)](https://pub.dev/packages/flutter_barcode_sdk)

A cross-platform Flutter plugin for barcode reading and scanning, powered by
the [Dynamsoft Barcode Reader SDK](https://www.dynamsoft.com/barcode-reader/overview/).
Supports **Android**, **iOS**, **Web**, **Windows**, and **Linux**.

## Repository Structure

This is a **monorepo** for the `flutter_barcode_sdk` federated plugin,
following the [Flutter camera package](https://github.com/flutter/packages/tree/main/packages/camera)
pattern. All packages are siblings under [`packages/`](packages/) and are
published to [pub.dev](https://pub.dev) independently:

| Package | Description |
|---------|-------------|
| [`flutter_barcode_sdk`](packages/flutter_barcode_sdk) | App-facing package. Add this to your app. |
| [`flutter_barcode_sdk_platform_interface`](packages/flutter_barcode_sdk_platform_interface) | Platform interface contract and shared data models. |
| [`flutter_barcode_sdk_android`](packages/flutter_barcode_sdk_android) | Android implementation. |
| [`flutter_barcode_sdk_macos`](packages/flutter_barcode_sdk_macos) | macOS implementation (universal-2). || [`flutter_barcode_sdk_ios`](packages/flutter_barcode_sdk_ios) | iOS implementation. |
| [`flutter_barcode_sdk_windows`](packages/flutter_barcode_sdk_windows) | Windows implementation. |
| [`flutter_barcode_sdk_linux`](packages/flutter_barcode_sdk_linux) | Linux implementation. |
| [`flutter_barcode_sdk_web`](packages/flutter_barcode_sdk_web) | Web implementation. |

The platform packages are **endorsed** by the app-facing package, so app
developers only need to depend on `flutter_barcode_sdk`.

## Development

The repository uses a [pub workspace](https://dart.dev/tools/pub/workspaces) —
run `flutter pub get` once at the repository root and all packages resolve
local dependencies automatically.

```bash
flutter pub get        # once at the repo root
flutter analyze        # per package: cd packages/<pkg> && flutter analyze
flutter test
```

Each package has its own `example/` app for standalone testing:

```bash
cd packages/flutter_barcode_sdk_windows/example
flutter run -d windows

# macOS (requires Xcode + CocoaPods, macOS 12+)
cd packages/flutter_barcode_sdk_macos/example
flutter run -d macos
```

## Publishing

Each package is published independently from its own directory,
with its own version:

```bash
cd packages/flutter_barcode_sdk_web
dart pub publish       # dry run: dart pub publish --dry-run
```

The GitHub Actions workflow [`.github/workflows/publish.yml`](.github/workflows/publish.yml)
publishes automatically when a tag of the form
`<package-name>-v<version>` is pushed, e.g.
`flutter_barcode_sdk_web-v1.2.3`, or manually via
**Actions → Publish to pub.dev → Run workflow**.

Because each platform package ships its own native binaries, each package
gets the full 100 MB pub.dev size limit independently.

## License

See [LICENSE](LICENSE).