import 'dart:typed_data';

/// Represents the result of a barcode scan, including format, text,
/// coordinates, and angle.
///
/// This class stores barcode details such as the decoded text, format,
/// bounding box coordinates, rotation angle, and raw barcode bytes.
///
/// ### Example Usage:
/// ```dart
/// BarcodeResult result = BarcodeResult.fromJson(jsonData);
/// print(result.text);
/// ```
class BarcodeResult {
  /// Barcode format (e.g., "QR Code", "Code 128").
  final String format;

  /// Decoded text from the barcode.
  final String text;

  /// X-coordinate of the first corner point.
  final int x1;

  /// Y-coordinate of the first corner point.
  final int y1;

  /// X-coordinate of the second corner point.
  final int x2;

  /// Y-coordinate of the second corner point.
  final int y2;

  /// X-coordinate of the third corner point.
  final int x3;

  /// Y-coordinate of the third corner point.
  final int y3;

  /// X-coordinate of the fourth corner point.
  final int x4;

  /// Y-coordinate of the fourth corner point.
  final int y4;

  /// Rotation angle of the barcode in degrees.
  final int angle;

  /// Raw barcode data in bytes.
  final Uint8List barcodeBytes;

  /// Error code.
  final int errorCode;

  /// Error message.
  final String errorMsg;

  /// Constructs a [BarcodeResult] with the given properties.
  BarcodeResult(
    this.format,
    this.text,
    this.x1,
    this.y1,
    this.x2,
    this.y2,
    this.x3,
    this.y3,
    this.x4,
    this.y4,
    this.angle,
    this.barcodeBytes,
    this.errorCode,
    this.errorMsg,
  );

  /// Creates a [BarcodeResult] instance from a JSON object.
  factory BarcodeResult.fromJson(Map<dynamic, dynamic> json) {
    String format = json['format'];

    int x1 = json['x1'];
    int y1 = json['y1'];
    int x2 = json['x2'];
    int y2 = json['y2'];
    int x3 = json['x3'];
    int y3 = json['y3'];
    int x4 = json['x4'];
    int y4 = json['y4'];
    int angle = json['angle'];
    int errorCode = json['errorCode'];
    String errorMsg = json['errorMsg'];

    // Convert barcode byte data
    List<Object?> rawBytes = json['barcodeBytes'];
    Uint8List barcodeBytes =
        Uint8List.fromList(rawBytes.map((e) => e as int).toList());

    String text = String.fromCharCodes(barcodeBytes);

    return BarcodeResult(
      format,
      text,
      x1,
      y1,
      x2,
      y2,
      x3,
      y3,
      x4,
      y4,
      angle,
      barcodeBytes,
      errorCode,
      errorMsg,
    );
  }

  /// Converts this object to a JSON-compatible [Map].
  Map<String, dynamic> toJson() => {
        'format': format,
        'text': text,
        'x1': x1,
        'y1': y1,
        'x2': x2,
        'y2': y2,
        'x3': x3,
        'y3': y3,
        'x4': x4,
        'y4': y4,
        'angle': angle,
        'barcodeBytes': barcodeBytes,
        'errorCode': errorCode,
        'errorMsg': errorMsg,
      };
}

/// Converts a list of barcode scan results from JSON format to
/// [BarcodeResult] objects.
///
/// This function is used to transform the decoded barcode results into
/// a list of [BarcodeResult] instances.
///
/// ### Example Usage:
/// ```dart
/// List<BarcodeResult> results = convertResults(jsonData);
/// print(results[0].text);
/// ```
List<BarcodeResult> convertResults(List<Map<dynamic, dynamic>> jsonResults) {
  return jsonResults.map((data) => BarcodeResult.fromJson(data)).toList();
}
