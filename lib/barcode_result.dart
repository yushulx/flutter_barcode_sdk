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

  BarcodeResult(this.format, this.text, this.x1, this.y1, this.x2, this.y2,
      this.x3, this.y3, this.x4, this.y4, this.angle);

  BarcodeResult.fromJson(Map<dynamic, dynamic> json)
      : format = json['format'],
        text = json['text'],
        x1 = json['x1'],
        y1 = json['y1'],
        x2 = json['x2'],
        y2 = json['y2'],
        x3 = json['x3'],
        y3 = json['y3'],
        x4 = json['x4'],
        y4 = json['y4'],
        angle = json['angle'];

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
      };
}

/// Convert List<Map<dynamic, dynamic>> to List<BarcodeResult>
List<BarcodeResult> convertResults(List<Map<dynamic, dynamic>> ret) {
  return ret.map((data) => BarcodeResult.fromJson(data)).toList();
}
