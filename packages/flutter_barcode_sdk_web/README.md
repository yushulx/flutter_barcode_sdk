# flutter_barcode_sdk_web

Web platform implementation of [flutter_barcode_sdk](https://pub.dev/packages/flutter_barcode_sdk).

The Dynamsoft Barcode Reader JS/WASM bundle
(`dynamsoft-barcode-reader-bundle@11.6.2000`) is **self-hosted** as package
assets: the plugin injects the `<script>` tag automatically at runtime and
points the engine's resource loader at the packaged files. Apps need no CDN
`<script>` tag in `index.html` and no manual asset copying — the whole
engine is served from the app itself.

Apps that already load the JS SDK themselves (for example a CDN `<script>`
tag in `index.html`) keep working: the automatic injection is skipped
whenever `window.Dynamsoft` is already defined.

Serve the app over HTTPS or `localhost`, as required for WASM.

## Usage

This package is not intended for direct use by app developers.
It is automatically endorsed by `flutter_barcode_sdk` on the web platform.
