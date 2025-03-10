import 'dart:typed_data';

/// Represents the result of a barcode scan, including format, text, coordinates, and angle.
///
/// This class stores barcode details such as the decoded text, format, bounding box coordinates,
/// rotation angle, and raw barcode bytes.
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
      };
}

/// Converts a list of barcode scan results from JSON format to [BarcodeResult] objects.
///
/// This function is used to transform the decoded barcode results into a list of
/// [BarcodeResult] instances.
///
/// ### Example Usage:
/// ```dart
/// List<BarcodeResult> results = convertResults(jsonData);
/// print(results[0].text);
/// ```
List<BarcodeResult> convertResults(List<Map<dynamic, dynamic>> jsonResults) {
  return jsonResults.map((data) => BarcodeResult.fromJson(data)).toList();
}

/// Defines supported barcode formats.
///
class BarcodeFormat {
  static const int ONED = 0x003007FF;

  static const int GS1_DATABAR = 0x0003F800;

  static const int CODE_39 = 0x1;

  static const int CODE_128 = 0x2;

  static const int CODE_93 = 0x4;

  static const int CODABAR = 0x8;

  static const int ITF = 0x10;

  static const int EAN_13 = 0x20;

  static const int EAN_8 = 0x40;

  static const int UPC_A = 0x80;

  static const int UPC_E = 0x100;

  static const int INDUSTRIAL_25 = 0x200;

  static const int CODE_39_EXTENDED = 0x400;

  static const int GS1_DATABAR_OMNIDIRECTIONAL = 0x800;

  static const int GS1_DATABAR_TRUNCATED = 0x1000;

  static const int GS1_DATABAR_STACKED = 0x2000;

  static const int GS1_DATABAR_STACKED_OMNIDIRECTIONAL = 0x4000;

  static const int GS1_DATABAR_EXPANDED = 0x8000;

  static const int GS1_DATABAR_EXPANDED_STACKED = 0x10000;

  static const int GS1_DATABAR_LIMITED = 0x20000;

  static const int PATCHCODE = 0x00040000;

  static const int CODE_32 = 0x1000000;

  static const int PDF417 = 0x02000000;

  static const int QR_CODE = 0x04000000;

  static const int DATAMATRIX = 0x08000000;

  static const int AZTEC = 0x10000000;

  static const int MAXICODE = 0x20000000;

  static const int MICRO_QR = 0x40000000;

  static const int MICRO_PDF417 = 0x00080000;

  static const int GS1_COMPOSITE = 0x80000000;

  static const int MSI_CODE = 0x100000;

  static const int CODE_11 = 0x200000;

  static const int TWO_DIGIT_ADD_ON = 0x400000;

  static const int FIVE_DIGIT_ADD_ON = 0x800000;

  static const int MATRIX_25 = 0x1000000000;

  static const int POSTALCODE = 0x3F0000000000000;

  static const int NONSTANDARD_BARCODE = 0x100000000;

  static const int USPSINTELLIGENTMAIL = 0x10000000000000;

  static const int POSTNET = 0x20000000000000;

  static const int PLANET = 0x40000000000000;

  static const int AUSTRALIANPOST = 0x80000000000000;

  static const int RM4SCC = 0x100000000000000;

  static const int KIX = 0x200000000000000;

  static const int DOTCODE = 0x200000000;

  static const int PHARMACODE_ONE_TRACK = 0x400000000;

  static const int PHARMACODE_TWO_TRACK = 0x800000000;

  static const int PHARMACODE = 0xC00000000;
}
