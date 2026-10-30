/// Native / non-web fallback for [resolveDemoLicenseKey] and the analytics hooks.
///
/// A desktop or mobile build has no hosting page to inject a key, so it uses the
/// public Dynamsoft Barcode Reader trial key. Replace it with your own license
/// for anything beyond local experiments.
const String kTrialLicenseKey =
    'DLS2eyJoYW5kc2hha2VDb2RlIjoiMjAwMDAxLTE2NDk4Mjk3OTI2MzUiLCJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSIsInNlc3Npb25QYXNzd29yZCI6IndTcGR6Vm05WDJrcEQ5YUoifQ==';

/// Returns the license key used by [initBarcodeSDK].
String resolveDemoLicenseKey() => kTrialLicenseKey;

/// No-op on native platforms: analytics only exists in the hosted web demo.
void reportDemoReady(int loadMs) {}

/// No-op on native platforms.
void reportDemoScan(String inputSource) {}

/// No-op on native platforms.
void reportDemoAction(String name, [Map<String, Object?>? params]) {}
