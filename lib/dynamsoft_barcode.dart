import 'dart:typed_data';

/// Barcode result contains barcode format, result, and coordinate points
///
/// https://flutter.dev/docs/development/data-and-backend/json
class BarcodeResult {
  final String format;
  final String text;
  final int x1;
  final int y1;
  final int x2;
  final int y2;
  final int x3;
  final int y3;
  final int x4;
  final int y4;
  final int angle;
  final Uint8List barcodeBytes;

  BarcodeResult(this.format, this.text, this.x1, this.y1, this.x2, this.y2,
      this.x3, this.y3, this.x4, this.y4, this.angle, this.barcodeBytes);

  static BarcodeResult fromJson(Map<dynamic, dynamic> json) {
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
    List<Object?> lobjects = json['barcodeBytes'];
    List<int> lint = lobjects.map((e) => e as int).toList();
    Uint8List barcodeBytes = Uint8List.fromList(lint);

    String text = String.fromCharCodes(barcodeBytes);
    return BarcodeResult(
        format, text, x1, y1, x2, y2, x3, y3, x4, y4, angle, barcodeBytes);
  }

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

/// Convert List<Map<dynamic, dynamic>> to List<BarcodeResult>
List<BarcodeResult> convertResults(List<Map<dynamic, dynamic>> ret) {
  return ret.map((data) => BarcodeResult.fromJson(data)).toList();
}

/// Supported barcode formats: https://www.dynamsoft.com/barcode-reader/parameters/enum/format-enums.html?ver=latest#barcodeformat
class BarcodeFormat {
  static const int ALL = -31457281;
  static const int ONED = 0x001007FF;
  static const int GS1_DATABAR = 0x0003F800;
  static const int NULL = 0x00;
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
  static const int PDF417 = 0x02000000;
  static const int QR_CODE = 0x04000000;
  static const int DATAMATRIX = 0x08000000;
  static const int AZTEC = 0x10000000;
  static const int MAXICODE = 0x20000000;
  static const int MICRO_QR = 0x40000000;
  static const int MICRO_PDF417 = 0x00080000;
  static const int GS1_COMPOSITE = -2147483648;
  static const int MSI_CODE = 0x100000;
}
