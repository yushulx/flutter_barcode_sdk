/// Defines supported barcode format constants for use with
/// [FlutterBarcodeSdk.setBarcodeFormats].
///
/// These constants can be combined using the bitwise OR operator (`|`) to
/// specify multiple barcode formats simultaneously.
///
/// ### Example
/// ```dart
/// // Detect only QR Code and Code 128
/// await barcodeReader.setBarcodeFormats(
///   BarcodeFormat.QR_CODE | BarcodeFormat.CODE_128,
/// );
/// ```
///
/// ### Web Platform Support
///
/// On web, barcode format values are automatically converted to JavaScript
/// `BigInt` to preserve their full 64-bit precision. This allows formats
/// with values exceeding `0x7FFFFFFF` (such as postal codes, DotCode, and
/// Pharmacode) to work correctly.
///
/// **Note:** When combining formats using Dart's bitwise OR operator (`|`)
/// in your code, the operation is performed in Dart before being sent to
/// JavaScript, so all formats can be safely combined regardless of their
/// magnitude.
class BarcodeFormat {
  BarcodeFormat._();

  // ---------------------------------------------------------------------------
  // Special / Aggregate Values
  // ---------------------------------------------------------------------------

  /// No barcode format.
  static const int NULL = 0x00;

  /// All supported barcode formats (`0xFFFFFFFEFFFFFFFF` unsigned).
  ///
  /// This uses the signed 64-bit equivalent so that the constant compiles
  /// on both native and web platforms.
  static const int ALL = -4294967297; // 0xFFFFFFFEFFFFFFFF

  /// Default barcode format set covering the most commonly used symbologies.
  static const int DEFAULT = 0xFE3BFFFF;

  /// Combined value of all 1D (linear) barcode formats: [CODE_39],
  /// [CODE_128], [CODE_93], [CODABAR], [ITF], [EAN_13], [EAN_8], [UPC_A],
  /// [UPC_E], [INDUSTRIAL_25], [CODE_39_EXTENDED], [MSI_CODE], [CODE_11].
  static const int ONED = 0x003007FF;

  /// Combined value of all GS1 DataBar formats: [GS1_DATABAR_OMNIDIRECTIONAL],
  /// [GS1_DATABAR_TRUNCATED], [GS1_DATABAR_STACKED],
  /// [GS1_DATABAR_STACKED_OMNIDIRECTIONAL], [GS1_DATABAR_EXPANDED],
  /// [GS1_DATABAR_EXPANDED_STACKED], [GS1_DATABAR_LIMITED].
  static const int GS1_DATABAR = 0x0003F800;

  /// Combined value of all postal code formats: [USPSINTELLIGENTMAIL],
  /// [POSTNET], [PLANET], [AUSTRALIANPOST], [RM4SCC], [KIX].
  static const int POSTALCODE = 0x3F0000000000000;

  /// Combined value of [PHARMACODE_ONE_TRACK] and [PHARMACODE_TWO_TRACK].
  static const int PHARMACODE = 0xC00000000;

  // ---------------------------------------------------------------------------
  // 1D / Linear Barcodes
  // ---------------------------------------------------------------------------

  /// Code 39 barcode format.
  static const int CODE_39 = 0x1;

  /// Code 128 barcode format.
  static const int CODE_128 = 0x2;

  /// Code 93 barcode format.
  static const int CODE_93 = 0x4;

  /// Codabar barcode format.
  static const int CODABAR = 0x8;

  /// Interleaved 2 of 5 (ITF) barcode format.
  static const int ITF = 0x10;

  /// EAN-13 barcode format.
  static const int EAN_13 = 0x20;

  /// EAN-8 barcode format.
  static const int EAN_8 = 0x40;

  /// UPC-A barcode format.
  static const int UPC_A = 0x80;

  /// UPC-E barcode format.
  static const int UPC_E = 0x100;

  /// Industrial 2 of 5 barcode format.
  static const int INDUSTRIAL_25 = 0x200;

  /// Code 39 Extended barcode format.
  static const int CODE_39_EXTENDED = 0x400;

  /// Code 32 barcode format (Italian Pharmacode).
  static const int CODE_32 = 0x1000000;

  /// MSI (Modified Plessey) barcode format.
  static const int MSI_CODE = 0x100000;

  /// Code 11 barcode format.
  static const int CODE_11 = 0x200000;

  /// Two-digit add-on supplement for UPC/EAN barcodes.
  static const int TWO_DIGIT_ADD_ON = 0x400000;

  /// Five-digit add-on supplement for UPC/EAN barcodes.
  static const int FIVE_DIGIT_ADD_ON = 0x800000;

  /// Matrix 2 of 5 barcode format.
  static const int MATRIX_25 = 0x1000000000;

  /// Telepen barcode format.
  ///
  /// Designed primarily for library and membership systems, Telepen can
  /// encode the full ASCII character set.
  static const int TELEPEN = 0x2000000000;

  /// Telepen Numeric barcode format.
  ///
  /// A variation of Telepen optimized for encoding numeric data only.
  static const int TELEPEN_NUMERIC = 0x4000000000;

  // ---------------------------------------------------------------------------
  // GS1 DataBar Formats
  // ---------------------------------------------------------------------------

  /// GS1 DataBar Omnidirectional barcode format.
  static const int GS1_DATABAR_OMNIDIRECTIONAL = 0x800;

  /// GS1 DataBar Truncated barcode format.
  static const int GS1_DATABAR_TRUNCATED = 0x1000;

  /// GS1 DataBar Stacked barcode format.
  static const int GS1_DATABAR_STACKED = 0x2000;

  /// GS1 DataBar Stacked Omnidirectional barcode format.
  static const int GS1_DATABAR_STACKED_OMNIDIRECTIONAL = 0x4000;

  /// GS1 DataBar Expanded barcode format.
  static const int GS1_DATABAR_EXPANDED = 0x8000;

  /// GS1 DataBar Expanded Stacked barcode format.
  static const int GS1_DATABAR_EXPANDED_STACKED = 0x10000;

  /// GS1 DataBar Limited barcode format.
  static const int GS1_DATABAR_LIMITED = 0x20000;

  // ---------------------------------------------------------------------------
  // 2D Barcodes
  // ---------------------------------------------------------------------------

  /// PDF417 barcode format.
  static const int PDF417 = 0x02000000;

  /// QR Code barcode format.
  static const int QR_CODE = 0x04000000;

  /// DataMatrix barcode format.
  static const int DATAMATRIX = 0x08000000;

  /// Aztec Code barcode format.
  static const int AZTEC = 0x10000000;

  /// MaxiCode barcode format (modes 2-5).
  static const int MAXICODE = 0x20000000;

  /// Micro QR Code barcode format.
  static const int MICRO_QR = 0x40000000;

  /// Micro PDF417 barcode format.
  static const int MICRO_PDF417 = 0x00080000;

  /// GS1 Composite Code barcode format.
  static const int GS1_COMPOSITE = 0x80000000;

  /// DotCode barcode format.
  static const int DOTCODE = 0x200000000;

  // ---------------------------------------------------------------------------
  // Patch / Special Codes
  // ---------------------------------------------------------------------------

  /// Patch Code barcode format.
  static const int PATCHCODE = 0x00040000;

  /// Non-standard barcode format.
  static const int NONSTANDARD_BARCODE = 0x100000000;

  // ---------------------------------------------------------------------------
  // Postal Codes
  // ---------------------------------------------------------------------------

  /// USPS Intelligent Mail barcode format.
  static const int USPSINTELLIGENTMAIL = 0x10000000000000;

  /// POSTNET barcode format.
  static const int POSTNET = 0x20000000000000;

  /// PLANET barcode format.
  static const int PLANET = 0x40000000000000;

  /// Australia Post barcode format.
  static const int AUSTRALIANPOST = 0x80000000000000;

  /// Royal Mail 4-State Customer Code (RM4SCC) barcode format.
  static const int RM4SCC = 0x100000000000000;

  /// KIX (Klant Index) barcode format used by Royal Dutch TPG Post.
  static const int KIX = 0x200000000000000;

  // ---------------------------------------------------------------------------
  // Pharmacode
  // ---------------------------------------------------------------------------

  /// Pharmacode (one-track) barcode format.
  static const int PHARMACODE_ONE_TRACK = 0x400000000;

  /// Pharmacode (two-track) barcode format.
  static const int PHARMACODE_TWO_TRACK = 0x800000000;
}
