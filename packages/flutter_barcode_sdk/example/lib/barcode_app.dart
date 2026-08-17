import 'dart:async';
import 'dart:ui' as ui;

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:image_picker/image_picker.dart';

import 'license.dart';
import 'overlay_painter.dart';
import 'scanner_screen.dart';
import 'utils.dart';

// Conditional import: on native platforms (Android, iOS, Windows, Linux, macOS)
// we load the real desktop camera implementation; on web we load a no-op stub.
import '_desktop_camera_stub.dart'
    if (dart.library.io) '_desktop_camera.dart';

/// Returns `true` when running on a desktop operating system.
bool get _isDesktopPlatform =>
    !kIsWeb &&
    (defaultTargetPlatform == TargetPlatform.windows ||
        defaultTargetPlatform == TargetPlatform.linux ||
        defaultTargetPlatform == TargetPlatform.macOS);

/// A unified barcode scanner page that adapts its behavior to the current
/// platform (mobile, web, or desktop).
///
/// - **Image picking** uses the `image_picker` package on all platforms.
/// - **Camera scanning** navigates to [ScannerScreen] on mobile/web and
///   uses `flutter_lite_camera` inline on desktop.
class BarcodeApp extends StatefulWidget {
  const BarcodeApp({super.key});

  @override
  State<BarcodeApp> createState() => _BarcodeAppState();
}

class _BarcodeAppState extends State<BarcodeApp> with WidgetsBindingObserver {
  FlutterBarcodeSdk? _barcodeReader;
  bool _isSDKLoaded = false;
  final _picker = ImagePicker();

  // Image preview state
  Uint8List? _imageBytes;
  List<BarcodeResult> _barcodeResultsList = [];
  String _barcodeResults = '';

  // Desktop camera state
  DesktopCameraHelper? _desktopCamera;
  bool _isDesktopCameraActive = false;
  int _textureId = -1;
  int _width = 640;
  int _height = 480;
  bool _isDecoding = false;

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addObserver(this);
    _initBarcodeSDK();
    if (_isDesktopPlatform) {
      _desktopCamera = DesktopCameraHelper();
    }
  }

  @override
  void dispose() {
    WidgetsBinding.instance.removeObserver(this);
    _stopDesktopCamera();
    super.dispose();
  }

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    if (state == AppLifecycleState.paused ||
        state == AppLifecycleState.inactive) {
      _stopDesktopCamera();
    }
  }

  // ---------------------------------------------------------------------------
  // SDK Initialization
  // ---------------------------------------------------------------------------

  Future<void> _initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    // Get a 30-day FREE trial license at:
    // https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform
    await _barcodeReader!.setLicense(LICENSE_KEY);
    await _barcodeReader!.init();

    // Read and re-apply parameters to ensure consistency.
    final params = await _barcodeReader!.getParameters();
    await _barcodeReader!.setParameters(params);

    if (mounted) setState(() => _isSDKLoaded = true);
  }

  // ---------------------------------------------------------------------------
  // Image Picking (all platforms via image_picker)
  // ---------------------------------------------------------------------------

  Future<void> _pickImage() async {
    if (!_isSDKLoaded) {
      _showErrorDialog('Barcode SDK is not loaded yet.');
      return;
    }

    // Stop desktop camera if active.
    await _stopDesktopCamera();

    final pickedFile = await _picker.pickImage(source: ImageSource.gallery);
    if (pickedFile == null) return;

    final bytes = await pickedFile.readAsBytes();

    // Decode the image for barcode reading.
    final image = await decodeImageFromList(bytes);
    final byteData =
        await image.toByteData(format: ui.ImageByteFormat.rawRgba);

    List<BarcodeResult> results = [];
    if (byteData != null) {
      results = await _barcodeReader!.decodeImageBuffer(
        byteData.buffer.asUint8List(),
        image.width,
        image.height,
        byteData.lengthInBytes ~/ image.height,
        ImagePixelFormat.IPF_ARGB_8888.index,
        ImageRotation.rotation0.value,
      );
    }

    setState(() {
      _imageBytes = bytes;
      _barcodeResultsList = results;
      _barcodeResults = getBarcodeResults(results);
      _textureId = -1;
    });
  }

  // ---------------------------------------------------------------------------
  // Camera Scanning
  // ---------------------------------------------------------------------------

  void _startCameraScanning() {
    if (!_isSDKLoaded) {
      _showErrorDialog('Barcode SDK is not loaded yet.');
      return;
    }

    if (_isDesktopPlatform) {
      _startDesktopCamera();
    } else {
      // Mobile & Web: navigate to ScannerScreen (uses the camera package).
      Navigator.push(
        context,
        MaterialPageRoute(
          builder: (_) => ScannerScreen(barcodeReader: _barcodeReader!),
        ),
      );
    }
  }

  // ---------------------------------------------------------------------------
  // Desktop Camera (flutter_lite_camera)
  // ---------------------------------------------------------------------------

  Future<void> _startDesktopCamera() async {
    if (_desktopCamera == null) return;

    try {
      final devices = await _desktopCamera!.getDeviceList();
      if (devices.isEmpty) {
        _showErrorDialog('No camera devices found.');
        return;
      }

      final opened = await _desktopCamera!.open(0);
      if (opened) {
        // Start the native preview stream; the returned texture id is
        // displayed with a [Texture] widget.
        final textureId = await _desktopCamera!.startPreview();
        setState(() {
          _isDesktopCameraActive = true;
          _textureId = textureId;
          _imageBytes = null;
          _barcodeResultsList = [];
          _barcodeResults = '';
        });
        _captureDesktopFrames();
      }
    } catch (e) {
      debugPrint('Error starting desktop camera: $e');
    }
  }

  Future<void> _stopDesktopCamera() async {
    if (!_isDesktopCameraActive) return;

    _isDesktopCameraActive = false;
    _isDecoding = false;
    await _desktopCamera?.stopPreview();
    await _desktopCamera?.release();

    if (mounted) {
      setState(() {
        _textureId = -1;
        _barcodeResultsList = [];
      });
    }
  }

  Future<void> _captureDesktopFrames() async {
    if (!_isDesktopCameraActive) return;

    try {
      final frame = await _desktopCamera!.captureFrame();
      if (frame.containsKey('data')) {
        final Uint8List rgb = frame['data'];
        final int width = frame['width'];
        final int height = frame['height'];
        _width = width;
        _height = height;

        // Decode barcodes from the raw frame. This does not affect the
        // preview stream.
        if (!_isDecoding) {
          _isDecoding = true;
          final results = await _barcodeReader!.decodeImageBuffer(
            rgb,
            width,
            height,
            width * 3,
            ImagePixelFormat.IPF_RGB_888.index,
            ImageRotation.rotation0.value,
          );
          _isDecoding = false;
          if (mounted) {
            setState(() {
              _barcodeResultsList = results;
            });
          }
        }
      }
    } catch (_) {
      // Ignore transient frame-capture errors.
    }

    if (_isDesktopCameraActive) {
      Future.delayed(const Duration(milliseconds: 30), _captureDesktopFrames);
    }
  }

  // ---------------------------------------------------------------------------
  // Helpers
  // ---------------------------------------------------------------------------

  void _showErrorDialog(String message) {
    showDialog(
      context: context,
      builder: (_) => AlertDialog(
        title: const Text('Error'),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: const Text('OK'),
          ),
        ],
      ),
    );
  }

  // ---------------------------------------------------------------------------
  // UI
  // ---------------------------------------------------------------------------

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Dynamsoft Barcode Reader')),
      body: LayoutBuilder(
        builder: (context, constraints) {
          return Column(
            children: [
              Expanded(
                child: _isDesktopCameraActive && _textureId >= 0
                    ? _buildDesktopCameraPreview(constraints)
                    : _buildImagePreview(constraints),
              ),
              _buildControlBar(),
            ],
          );
        },
      ),
    );
  }

  /// Displays the selected image with barcode overlay, or a placeholder.
  Widget _buildImagePreview(BoxConstraints constraints) {
    if (_imageBytes == null) {
      return Center(
        child: Image.asset('images/default.png', fit: BoxFit.contain),
      );
    }

    return SingleChildScrollView(
      child: Column(
        children: [
          SizedBox(
            width: constraints.maxWidth,
            height: constraints.maxHeight * 0.7,
            child: FittedBox(
              fit: BoxFit.contain,
              child: FutureBuilder<ui.Image>(
                future: decodeImageFromList(_imageBytes!),
                builder: (context, snapshot) {
                  if (!snapshot.hasData) {
                    return const Center(child: CircularProgressIndicator());
                  }
                  final img = snapshot.data!;
                  return Stack(
                    children: [
                      SizedBox(
                        width: img.width.toDouble(),
                        height: img.height.toDouble(),
                        child: Image.memory(_imageBytes!),
                      ),
                      Positioned.fill(
                        child: _barcodeResultsList.isEmpty
                            ? Container(
                                color: const Color.fromARGB(26, 0, 0, 0),
                                child: const Center(
                                  child: Text(
                                    'No barcode detected',
                                    style: TextStyle(
                                      color: Colors.white,
                                      fontSize: 20,
                                      fontWeight: FontWeight.bold,
                                    ),
                                  ),
                                ),
                              )
                            : createOverlay(_barcodeResultsList),
                      ),
                    ],
                  );
                },
              ),
            ),
          ),
          const SizedBox(height: 10),
          SelectableText(
            _barcodeResults,
            style: const TextStyle(fontSize: 20, color: Colors.black),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }

  /// Displays the live desktop camera feed (native texture) with barcode
  /// overlay.
  Widget _buildDesktopCameraPreview(BoxConstraints constraints) {
    return Center(
      child: FittedBox(
        fit: BoxFit.contain,
        child: SizedBox(
          width: _width.toDouble(),
          height: _height.toDouble(),
          child: Stack(
            children: [
              // The native layer renders the video feed into this texture;
              // no frame data crosses into Dart for display purposes.
              Texture(textureId: _textureId),
              if (_barcodeResultsList.isNotEmpty)
                // Fill the frame so the overlay painter has the full frame
                // size (otherwise the CustomPaint is laid out 0x0 and the
                // barcode text wraps vertically).
                Positioned.fill(child: createOverlay(_barcodeResultsList)),
            ],
          ),
        ),
      ),
    );
  }

  /// Bottom control bar with Image File and Camera Scan buttons.
  Widget _buildControlBar() {
    return Container(
      padding: const EdgeInsets.symmetric(vertical: 20),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          MaterialButton(
            textColor: Colors.white,
            color: Colors.blue,
            onPressed: _isSDKLoaded ? _pickImage : null,
            child: const Text('Image File'),
          ),
          MaterialButton(
            textColor: Colors.white,
            color: Colors.blue,
            onPressed: _isSDKLoaded
                ? (_isDesktopCameraActive
                    ? _stopDesktopCamera
                    : _startCameraScanning)
                : null,
            child:
                Text(_isDesktopCameraActive ? 'Stop Camera' : 'Camera Scan'),
          ),
        ],
      ),
    );
  }
}
