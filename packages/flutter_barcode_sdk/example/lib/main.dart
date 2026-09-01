// A unified cross-platform barcode scanner example for flutter_barcode_sdk.
//
// The camera preview and RGB frame capture are provided by
// `flutter_lite_camera`, so this single code base runs on Android, iOS, web,
// Windows, macOS and Linux without any platform-specific files.
//
// Features:
// - Live camera scanning with a real-time annotation overlay.
// - Resolution preset selection.
// - Image file scanning (file loading + decoding + annotated result view).

import 'dart:async';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_lite_camera/flutter_lite_camera.dart';
import 'package:image_picker/image_picker.dart';

void main() {
  runApp(const BarcodeScannerApp());
}

class BarcodeScannerApp extends StatelessWidget {
  const BarcodeScannerApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Dynamsoft Barcode Reader',
      theme: ThemeData(colorSchemeSeed: Colors.blue, useMaterial3: true),
      home: const ScannerHomePage(),
    );
  }
}

class ScannerHomePage extends StatefulWidget {
  const ScannerHomePage({super.key});

  @override
  State<ScannerHomePage> createState() => _ScannerHomePageState();
}

class _ScannerHomePageState extends State<ScannerHomePage> {
  final FlutterLiteCamera _camera = FlutterLiteCamera();

  // To read barcodes, get a 30-day FREE trial license for Dynamsoft Barcode
  // Reader:
  // https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform
  static const String _licenseKey =
      'DLS2eyJoYW5kc2hha2VDb2RlIjoiMjAwMDAxLTE2NDk4Mjk3OTI2MzUiLCJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSIsInNlc3Npb25QYXNzd29yZCI6IndTcGR6Vm05WDJrcEQ5YUoifQ==';

  FlutterBarcodeSdk? _barcodeReader;
  bool _isSdkReady = false;

  bool _isCameraOpened = false;
  int _textureId = -1;
  // Actual frame size negotiated with the device. Filled in after the camera
  // opens (and after every resolution change) — never hard-coded.
  int _width = 0;
  int _height = 0;
  // Clockwise degrees to rotate the preview/result so it appears upright.
  // Phones report 90 (or 180/270) depending on how the device is held;
  // desktop/web report 0.
  int _rotation = 0;
  ResolutionPreset _preset = ResolutionPreset.medium;

  bool _shouldDecode = false;
  bool _isDecoding = false;
  List<BarcodeResult> _results = [];
  bool _isPickingFile = false;

  @override
  void initState() {
    super.initState();
    _initBarcodeSdk();
  }

  @override
  void dispose() {
    _stopCamera();
    super.dispose();
  }

  Future<void> _initBarcodeSdk() async {
    final reader = FlutterBarcodeSdk();
    await reader.setLicense(_licenseKey);
    await reader.init();
    if (mounted) {
      setState(() {
        _barcodeReader = reader;
        _isSdkReady = true;
      });
    }
  }

  // ---------------------------------------------------------------------------
  // Camera (flutter_lite_camera)
  // ---------------------------------------------------------------------------

  Future<void> _startCamera() async {
    try {
      List<String> devices = await _camera.getDeviceList();
      if (devices.isEmpty) {
        debugPrint('No camera devices found.');
        return;
      }
      debugPrint('Available devices: $devices');

      // false = permission denied or the device is busy/unavailable.
      bool opened = await _camera.open(0);
      if (!opened) {
        debugPrint('Failed to open the camera.');
        return;
      }

      // The native layer renders the video feed into this texture; no
      // frame data crosses into Dart for display purposes.
      int textureId = await _camera.startPreview();
      int rotation = 0;
      try {
        rotation = await _camera.getRotation();
      } catch (e) {
        debugPrint('getRotation failed, assuming 0: $e');
      }
      // The plugin negotiates the actual frame size with the device; read it
      // back instead of assuming a fixed resolution.
      int width = await _camera.getWidth();
      int height = await _camera.getHeight();
      debugPrint('Negotiated frame size: ${width}x$height, rotation $rotation');

      setState(() {
        _isCameraOpened = true;
        _textureId = textureId;
        if (width > 0 && height > 0) {
          _width = width;
          _height = height;
        }
        _rotation = rotation;
        _shouldDecode = true;
      });

      // Start pulling frames for barcode decoding only. This does not
      // affect the preview stream.
      _decodeFrames();

      // Honor a preset chosen before the camera was opened (open()
      // negotiates the medium target by default).
      if (_preset != ResolutionPreset.medium) {
        await _applyResolution(_preset);
      }
    } catch (e) {
      debugPrint('Error initializing camera: $e');
    }
  }

  Future<void> _stopCamera() async {
    _shouldDecode = false;

    if (_isCameraOpened) {
      try {
        await _camera.stopPreview();
        await _camera.release();
      } catch (e) {
        debugPrint('Failed to release the camera: $e');
      }
      if (mounted) {
        setState(() {
          _isCameraOpened = false;
          _textureId = -1;
          _results = [];
        });
      }
    }
  }

  Future<void> _applyResolution(ResolutionPreset preset) async {
    setState(() => _preset = preset);
    bool ok = await _camera.setResolutionPreset(preset);
    if (!ok) {
      debugPrint('setResolutionPreset($preset) failed.');
      return;
    }
    // The device may have fallen back to a different size; read the actual
    // one so the preview aspect ratio and overlay stay correct.
    int width = await _camera.getWidth();
    int height = await _camera.getHeight();
    debugPrint('Resolution after $preset: ${width}x$height');
    if (width > 0 && height > 0) {
      setState(() {
        _width = width;
        _height = height;
      });
    }
  }

  Future<void> _decodeFrames() async {
    if (!_isCameraOpened || !_shouldDecode) return;

    if (!_isDecoding && _barcodeReader != null) {
      _isDecoding = true;
      try {
        Map<String, dynamic> frame = await _camera.captureFrame();
        if (frame.containsKey('data')) {
          final int frameWidth = frame['width'];
          final int frameHeight = frame['height'];
          // Keep the preview aspect ratio and the overlay in sync with the
          // real frame size: the device may deliver frames at a different
          // size than the one negotiated at open() time.
          _width = frameWidth;
          _height = frameHeight;
          Uint8List rgbBuffer = frame['data'];

          List<BarcodeResult> ret = await _barcodeReader!.decodeImageBuffer(
            rgbBuffer,
            frameWidth,
            frameHeight,
            frameWidth * 3,
            ImagePixelFormat.IPF_RGB_888.index,
            ImageRotation.rotation0.value,
          );

          if (_shouldDecode && mounted) {
            setState(() {
              _results = ret;
            });
          }
        }
      } catch (e) {
        // No frame available yet.
      }
      _isDecoding = false;
    }

    if (_shouldDecode) {
      Future.delayed(const Duration(milliseconds: 30), _decodeFrames);
    }
  }

  // ---------------------------------------------------------------------------
  // Image file scanning (file loading + decoding + annotated result view)
  // ---------------------------------------------------------------------------

  Future<void> _scanImageFile() async {
    if (!_isSdkReady || _barcodeReader == null) {
      _showMessage('The barcode SDK is not ready yet.');
      return;
    }
    if (_isPickingFile) return;

    // Release the camera first: mobile platforms only allow one camera
    // client at a time.
    await _stopCamera();

    // image_picker opens the system photo library on iOS/Android (PHPicker /
    // Photo Picker) and falls back to a file dialog on desktop and web.
    final ImagePicker picker = ImagePicker();

    setState(() => _isPickingFile = true);
    try {
      final XFile? file =
          await picker.pickImage(source: ImageSource.gallery);
      if (file == null) return; // The user canceled the dialog.

      List<BarcodeResult> results;
      if (kIsWeb) {
        // The web SDK cannot read from a file path, so decode the raw RGBA
        // pixels instead.
        final Uint8List bytes = await file.readAsBytes();
        final ui.Image image = await decodeImageFromList(bytes);
        final ByteData? byteData =
            await image.toByteData(format: ui.ImageByteFormat.rawRgba);
        if (byteData == null) {
          throw StateError('Failed to read raw pixel data of the image.');
        }
        results = await _barcodeReader!.decodeImageBuffer(
          byteData.buffer.asUint8List(),
          image.width,
          image.height,
          image.width * 4,
          ImagePixelFormat.IPF_ARGB_8888.index,
          ImageRotation.rotation0.value,
        );
      } else {
        results = await _barcodeReader!.decodeFile(file.path);
      }

      if (!mounted) return;
      await Navigator.of(context).push(MaterialPageRoute<void>(
        builder: (_) => ScanResultScreen(file: file, results: results),
      ));
    } catch (e) {
      _showMessage('Failed to scan the image file: $e');
    } finally {
      if (mounted) setState(() => _isPickingFile = false);
    }
  }

  void _showMessage(String message) {
    if (!mounted) return;
    ScaffoldMessenger.of(context)
      ..hideCurrentSnackBar()
      ..showSnackBar(SnackBar(content: Text(message)));
  }

  // ---------------------------------------------------------------------------
  // UI
  // ---------------------------------------------------------------------------

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          if (_textureId >= 0)
            LayoutBuilder(
              builder: (context, constraints) {
                final double screenWidth = constraints.maxWidth;
                final double screenHeight = constraints.maxHeight;

                // When the preview is rotated 90/270 degrees the displayed
                // frame becomes portrait, so the aspect ratio flips. Fall
                // back to 4:3 until the first frame reports its real size.
                final int frameWidth = _width > 0 ? _width : 4;
                final int frameHeight = _height > 0 ? _height : 3;
                final bool rotated = _rotation % 180 == 90;
                final double imageAspectRatio = rotated
                    ? frameHeight / frameWidth
                    : frameWidth / frameHeight;
                final double screenAspectRatio = screenWidth / screenHeight;

                double drawWidth, drawHeight;
                if (imageAspectRatio > screenAspectRatio) {
                  drawWidth = screenWidth;
                  drawHeight = screenWidth / imageAspectRatio;
                } else {
                  drawHeight = screenHeight;
                  drawWidth = screenHeight * imageAspectRatio;
                }

                return Center(
                  child: SizedBox(
                    width: drawWidth,
                    height: drawHeight,
                    child: Stack(
                      children: [
                        // Rotate only the widget — never the native pixels —
                        // so the preview appears upright without any
                        // per-frame buffer processing.
                        RotatedBox(
                          quarterTurns: (_rotation ~/ 90) % 4,
                          child: _camera.buildPreview(_textureId),
                        ),
                        CustomPaint(
                          painter: ResultPainter(
                            _results,
                            srcWidth: frameWidth,
                            srcHeight: frameHeight,
                            drawWidth: drawWidth,
                            drawHeight: drawHeight,
                            rotation: _rotation,
                          ),
                          child: Container(),
                        ),
                      ],
                    ),
                  ),
                );
              },
            )
          else
            Center(
              child: _isCameraOpened
                  ? const CircularProgressIndicator()
                  : const Text('Camera not started'),
            ),
          Positioned(
            left: 20,
            right: 20,
            bottom: 20,
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: [
                    ElevatedButton(
                      onPressed: _isCameraOpened ? null : _startCamera,
                      child: const Text('Start'),
                    ),
                    Expanded(
                      child: Center(
                        child: Container(
                          padding: const EdgeInsets.symmetric(horizontal: 12),
                          decoration: BoxDecoration(
                            color: Colors.white.withValues(alpha: 0.9),
                            borderRadius: BorderRadius.circular(8),
                          ),
                          child: DropdownButton<ResolutionPreset>(
                            value: _preset,
                            underline: const SizedBox.shrink(),
                            isExpanded: true,
                            onChanged: (preset) {
                              if (preset == null) return;
                              if (_isCameraOpened) {
                                _applyResolution(preset);
                              } else {
                                setState(() => _preset = preset);
                              }
                            },
                            items: ResolutionPreset.values
                                .map((p) => DropdownMenuItem(
                                      value: p,
                                      child: Text(
                                        '${p.name} (${p.width}x${p.height})',
                                        overflow: TextOverflow.ellipsis,
                                      ),
                                    ))
                                .toList(),
                          ),
                        ),
                      ),
                    ),
                    ElevatedButton(
                      onPressed: !_isCameraOpened ? null : _stopCamera,
                      child: const Text('Stop'),
                    ),
                  ],
                ),
                const SizedBox(height: 12),
                ElevatedButton.icon(
                  onPressed: _isPickingFile ? null : _scanImageFile,
                  icon: const Icon(Icons.image_outlined),
                  label: const Text('Scan Image File'),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

/// An image file and its decoded in-memory representation.
class _LoadedImage {
  final Uint8List bytes;
  final ui.Image image;

  const _LoadedImage(this.bytes, this.image);
}

/// The result page for image file scanning: shows the loaded image with an
/// annotation overlay drawn on every detected barcode, plus the decoded
/// text below.
class ScanResultScreen extends StatefulWidget {
  final XFile file;
  final List<BarcodeResult> results;

  const ScanResultScreen({
    super.key,
    required this.file,
    required this.results,
  });

  @override
  State<ScanResultScreen> createState() => _ScanResultScreenState();
}

class _ScanResultScreenState extends State<ScanResultScreen> {
  late Future<_LoadedImage> _loadFuture;

  @override
  void initState() {
    super.initState();
    _loadFuture = _loadImage();
  }

  Future<_LoadedImage> _loadImage() async {
    final Uint8List bytes = await widget.file.readAsBytes();
    final ui.Image image = await decodeImageFromList(bytes);
    return _LoadedImage(bytes, image);
  }

  String get _title {
    final String name = widget.file.name;
    return name.isEmpty ? 'Scan Result' : name;
  }

  String get _summary {
    final List<BarcodeResult> results = widget.results;
    if (results.isEmpty) return 'No barcode detected';

    final StringBuffer sb = StringBuffer();
    for (var result in results) {
      if (result.errorCode != 0) {
        sb.write('Error ${result.errorCode}: ${result.errorMsg}\n');
        continue;
      }
      sb
        ..write('Format: ${result.format}\n')
        ..write('Text: ${result.text}\n\n');
    }
    return sb.toString().trim();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(_title)),
      body: SafeArea(
        child: Column(
          children: [
            // The image area keeps a fixed size regardless of how many
            // results there are; the results list below scrolls instead of
            // squeezing the image.
            SizedBox(
              width: double.infinity,
              height: MediaQuery.sizeOf(context).height * 0.55,
              child: FutureBuilder<_LoadedImage>(
                future: _loadFuture,
                builder: (context, snapshot) {
                  if (snapshot.connectionState != ConnectionState.done) {
                    return const Center(child: CircularProgressIndicator());
                  }
                  if (snapshot.hasError) {
                    return Center(
                        child: Text('Failed to load image: ${snapshot.error}'));
                  }

                  final _LoadedImage loaded = snapshot.data!;
                  // The SizedBox uses the natural image size, so the painter
                  // works in image pixel coordinates and FittedBox scales
                  // image and overlay together.
                  return Center(
                    child: FittedBox(
                      fit: BoxFit.contain,
                      child: SizedBox(
                        width: loaded.image.width.toDouble(),
                        height: loaded.image.height.toDouble(),
                        child: Stack(
                          children: [
                            Image.memory(loaded.bytes, fit: BoxFit.fill),
                            Positioned.fill(
                              child: CustomPaint(
                                painter: ImageResultPainter(widget.results),
                              ),
                            ),
                          ],
                        ),
                      ),
                    ),
                  );
                },
              ),
            ),
            Expanded(
              child: Container(
                width: double.infinity,
                color: Theme.of(context).colorScheme.surfaceContainerHighest,
                child: SingleChildScrollView(
                  padding: const EdgeInsets.all(16),
                  child: SelectableText(
                    _summary,
                    style: Theme.of(context).textTheme.bodyMedium,
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

/// Draws annotation overlays (quadrilateral + text) for barcodes found in a
/// camera frame. Points are mapped from source-frame coordinates to the
/// displayed (possibly rotated) canvas.
class ResultPainter extends CustomPainter {
  final List<BarcodeResult> results;
  final int srcWidth;
  final int srcHeight;
  final double drawWidth;
  final double drawHeight;
  final int rotation;

  ResultPainter(
    this.results, {
    required this.srcWidth,
    required this.srcHeight,
    required this.drawWidth,
    required this.drawHeight,
    required this.rotation,
  });

  /// Maps a point from source-frame coordinates to the displayed (possibly
  /// rotated) canvas, so overlay boxes align with the upright preview even
  /// when the texture is rotated by [rotation] degrees clockwise.
  Offset _transform(double x, double y) {
    final int q = (rotation ~/ 90) % 4;
    switch (q) {
      case 1: // 90° clockwise
        final double nx = srcHeight - y;
        final double ny = x;
        return Offset(nx * drawWidth / srcHeight, ny * drawHeight / srcWidth);
      case 2: // 180°
        final double nx = srcWidth - x;
        final double ny = srcHeight - y;
        return Offset(nx * drawWidth / srcWidth, ny * drawHeight / srcHeight);
      case 3: // 270° clockwise (90° counter-clockwise)
        final double nx = y;
        final double ny = srcWidth - x;
        return Offset(nx * drawWidth / srcHeight, ny * drawHeight / srcWidth);
      default: // 0°
        return Offset(x * drawWidth / srcWidth, y * drawHeight / srcHeight);
    }
  }

  @override
  void paint(Canvas canvas, Size size) {
    if (results.isEmpty) return;

    final Paint boxPaint = Paint()
      ..color = Colors.blue
      ..style = PaintingStyle.stroke
      ..strokeWidth = 2;

    for (var result in results) {
      final Offset a = _transform(result.x1.toDouble(), result.y1.toDouble());
      final Offset b = _transform(result.x2.toDouble(), result.y2.toDouble());
      final Offset c = _transform(result.x3.toDouble(), result.y3.toDouble());
      final Offset d = _transform(result.x4.toDouble(), result.y4.toDouble());

      final Path path = Path()
        ..moveTo(a.dx, a.dy)
        ..lineTo(b.dx, b.dy)
        ..lineTo(c.dx, c.dy)
        ..lineTo(d.dx, d.dy)
        ..close();

      canvas.drawPath(path, boxPaint);

      final TextPainter textPainter = TextPainter(
        text: TextSpan(
          text: result.text,
          style: const TextStyle(
            color: Colors.red,
            fontSize: 16,
          ),
        ),
        textDirection: TextDirection.ltr,
      );

      textPainter.layout();
      textPainter.paint(canvas, a);
    }
  }

  @override
  bool shouldRepaint(covariant ResultPainter oldDelegate) =>
      results != oldDelegate.results;
}

/// Draws annotation overlays for barcodes found in a loaded image file.
/// All coordinates are image pixel coordinates (1:1 with the canvas).
class ImageResultPainter extends CustomPainter {
  final List<BarcodeResult> results;

  ImageResultPainter(this.results);

  @override
  void paint(Canvas canvas, Size size) {
    final double scale = size.width / 1000;
    final double strokeWidth = (2 * scale).clamp(1.0, 8.0);
    final double fontSize = (24 * scale).clamp(12.0, 48.0);

    final Paint boxPaint = Paint()
      ..color = Colors.red
      ..style = PaintingStyle.stroke
      ..strokeWidth = strokeWidth;

    for (var result in results) {
      if (result.errorCode != 0) continue;

      double minX = result.x1.toDouble();
      double minY = result.y1.toDouble();
      for (final (int x, int y) in <(int, int)>[
        (result.x2, result.y2),
        (result.x3, result.y3),
        (result.x4, result.y4),
      ]) {
        if (x < minX) minX = x.toDouble();
        if (y < minY) minY = y.toDouble();
      }

      final Path path = Path()
        ..moveTo(result.x1.toDouble(), result.y1.toDouble())
        ..lineTo(result.x2.toDouble(), result.y2.toDouble())
        ..lineTo(result.x3.toDouble(), result.y3.toDouble())
        ..lineTo(result.x4.toDouble(), result.y4.toDouble())
        ..close();

      canvas.drawPath(path, boxPaint);

      final TextPainter textPainter = TextPainter(
        text: TextSpan(
          text: result.text,
          style: TextStyle(
            color: Colors.blue,
            fontSize: fontSize,
            backgroundColor: Colors.white.withValues(alpha: 0.7),
          ),
        ),
        textDirection: TextDirection.ltr,
      );
      textPainter.layout(maxWidth: size.width);
      textPainter.paint(
        canvas,
        Offset(minX, (minY - fontSize).clamp(0.0, size.height)),
      );
    }
  }

  @override
  bool shouldRepaint(covariant ImageResultPainter oldDelegate) =>
      results != oldDelegate.results;
}
