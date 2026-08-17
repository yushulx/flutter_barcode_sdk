import 'dart:async';
import 'dart:io';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_macos_example/utils.dart';
import 'package:file_selector_platform_interface/file_selector_platform_interface.dart';
import 'package:flutter_lite_camera/flutter_lite_camera.dart';
import 'license.dart';
import 'overlay_painter.dart';

class Desktop extends StatefulWidget {
  @override
  _DesktopState createState() => _DesktopState();
}

class _DesktopState extends State<Desktop> {
  final FlutterLiteCamera _flutterLiteCameraPlugin = FlutterLiteCamera();
  final _controller = TextEditingController();
  String _barcodeResults = '';
  late FlutterBarcodeSdk _barcodeReader;
  List<BarcodeResult> _barcodeResultsList = [];
  bool _isCameraOpened = false;
  bool _isCapturing = false;
  int _width = 640;
  int _height = 480;
  bool _shouldCapture = false;
  int _textureId = -1;
  bool isDecoding = false;

  @override
  void initState() {
    super.initState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    // Get 30-day FREEE trial license from https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform
    int licenseRet = await _barcodeReader.setLicense(LICENSE_KEY);
    print('License activation returned: $licenseRet');
    await _barcodeReader.init();
    // await _barcodeReader.setBarcodeFormats(BarcodeFormat.CODE_39 |
    //     BarcodeFormat.CODABAR |
    //     BarcodeFormat.QR_CODE |
    //     BarcodeFormat.DATAMATRIX);

    // Get all current parameters.
    String params = await _barcodeReader.getParameters();
    // Update the parameters.
    int ret = await _barcodeReader.setParameters(params);
    print('Parameter update: $ret');
  }

  Widget getDefaultImageWithOverlay() {
    if (_controller.text.isEmpty) {
      return Center(
        child: Text(
          'No image loaded',
          style: TextStyle(fontSize: 18, color: Colors.grey),
        ),
      );
    } else {
      final File imageFile = File(_controller.text);

      return LayoutBuilder(
        builder: (context, constraints) {
          return FutureBuilder<ui.Image>(
            future: _loadImage(imageFile), // Load the image as a ui.Image
            builder: (context, snapshot) {
              if (!snapshot.hasData) {
                return Center(child: CircularProgressIndicator());
              }

              final ui.Image image = snapshot.data!;

              return Stack(
                children: [
                  SizedBox(
                      width: MediaQuery.of(context).size.width,
                      height: MediaQuery.of(context).size.height -
                          MediaQuery.of(context).padding.top,
                      child: FittedBox(
                        fit: BoxFit.contain,
                        child: Stack(
                          children: [
                            SizedBox(
                              width: image.width.toDouble(),
                              height: image.height.toDouble(),
                              child: Image.file(imageFile),
                            ),
                            Positioned(
                              top: 0.0,
                              right: 0.0,
                              bottom: 0.0,
                              left: 0.0,
                              child: _barcodeResultsList.isEmpty
                                  ? Container(
                                      color: Color.fromARGB(26, 0, 0, 0),
                                      child: const Center(
                                        child: Text(
                                          'No barcode detected',
                                          style: TextStyle(
                                            color: Colors.white,
                                            fontSize: 20.0,
                                            fontWeight: FontWeight.bold,
                                          ),
                                        ),
                                      ))
                                  : createOverlay(_barcodeResultsList),
                            ),
                          ],
                        ),
                      )),
                ],
              );
            },
          );
        },
      );
    }
  }

  Future<ui.Image> _loadImage(File imageFile) async {
    final Uint8List imageBytes = await imageFile.readAsBytes();
    final Completer<ui.Image> completer = Completer();
    ui.decodeImageFromList(imageBytes, (ui.Image img) {
      completer.complete(img);
    });
    return completer.future;
  }

  Future<void> _decodeFrame(Uint8List rgb, int width, int height) async {
    if (isDecoding) return;

    isDecoding = true;
    final results = await _barcodeReader.decodeImageBuffer(
        rgb,
        width,
        height,
        width * 3,
        ImagePixelFormat.IPF_RGB_888.index,
        ImageRotation.rotation0.value);

    if (mounted) {
      setState(() {
        _barcodeResultsList = results;
      });
    }
    // _barcodeResults = getBarcodeResults(_barcodeResultsList);

    isDecoding = false;
  }

  Future<void> _captureFrames() async {
    if (!_isCameraOpened || !_shouldCapture) return;

    try {
      Map<String, dynamic> frame =
          await _flutterLiteCameraPlugin.captureFrame();
      if (frame.containsKey('data')) {
        Uint8List rgbBuffer = frame['data'];
        _width = frame['width'];
        _height = frame['height'];
        // Decode barcodes only; the preview stream is unaffected.
        _decodeFrame(rgbBuffer, frame['width'], frame['height']);
      }
    } catch (e) {
      // print("Error capturing frame: $e");
    }

    // Schedule the next frame
    if (_shouldCapture) {
      Future.delayed(const Duration(milliseconds: 30), _captureFrames);
    }
  }

  Future<void> _startCamera() async {
    try {
      List<String> devices = await _flutterLiteCameraPlugin.getDeviceList();
      if (devices.isNotEmpty) {
        print("Available Devices: $devices");
        print("Opening camera 0");
        bool opened = await _flutterLiteCameraPlugin.open(0);
        if (opened) {
          // Start the native preview stream; the returned texture id is
          // displayed with a [Texture] widget.
          int textureId = await _flutterLiteCameraPlugin.startPreview();
          setState(() {
            _textureId = textureId;
            _isCameraOpened = true;
            _shouldCapture = true;
            _isCapturing = true;
          });
          _captureFrames();
        } else {
          print("Failed to open the camera.");
        }
      }
    } catch (e) {
      // print("Error initializing camera: $e");
    }
  }

  Future<void> _stopCamera() async {
    _shouldCapture = false;

    if (_isCameraOpened) {
      await _flutterLiteCameraPlugin.stopPreview();
      await _flutterLiteCameraPlugin.release();
      _isCameraOpened = false;
      _textureId = -1;
      isDecoding = false;
      Future.delayed(const Duration(milliseconds: 100), () {
        setState(() {
          _barcodeResultsList = [];
        });
      });
    }

    _isCapturing = false;
  }

  @override
  void dispose() {
    _stopCamera();
    super.dispose();
  }

  @override
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Dynamsoft Barcode Reader'),
      ),
      body: LayoutBuilder(
        builder: (context, constraints) {
          return Column(
            children: [
              // Display camera stream or image
              Expanded(
                child: _textureId >= 0
                    ? _buildCameraStream(constraints)
                    : _buildImageWithOverlay(constraints),
              ),

              // Control Buttons
              Container(
                padding: const EdgeInsets.symmetric(vertical: 20),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: <Widget>[
                    MaterialButton(
                      child: Text('Image File'),
                      textColor: Colors.white,
                      color: Colors.blue,
                      onPressed: () async {
                        await _stopCamera();
                        final typeGroup = XTypeGroup(
                          label: 'images',
                          extensions: ['jpg', 'png'],
                        );
                        try {
                          final files = await FileSelectorPlatform.instance
                              .openFiles(acceptedTypeGroups: [typeGroup]);
                          if (files.isNotEmpty) {
                            final file = files[0];
                            _barcodeResultsList =
                                await _barcodeReader.decodeFile(file.path);

                            _barcodeResults =
                                getBarcodeResults(_barcodeResultsList);
                            _controller.text = file.path;
                            setState(() {});
                          }
                        } catch (err) {
                          print('Error: $err');
                        }
                      },
                    ),
                    MaterialButton(
                      child: Text('Camera Stream'),
                      textColor: Colors.white,
                      color: Colors.blue,
                      onPressed: _isCapturing
                          ? null
                          : () {
                              _barcodeResultsList = [];
                              _startCamera();
                            },
                    ),
                  ],
                ),
              ),
            ],
          );
        },
      ),
    );
  }

  Widget _buildCameraStream(BoxConstraints constraints) {
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

  Widget _buildImageWithOverlay(BoxConstraints constraints) {
    return SingleChildScrollView(
      child: Column(
        children: [
          Container(
            width: constraints.maxWidth,
            height: constraints.maxHeight * 0.7,
            child: getDefaultImageWithOverlay(),
          ),
          SizedBox(height: 10),
          SelectableText(
            _barcodeResults,
            style: TextStyle(
              fontSize: 20,
              color: Colors.black,
            ),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }
}
