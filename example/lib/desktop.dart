import 'dart:async';
import 'dart:io';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_example/utils.dart';
import 'package:file_selector_platform_interface/file_selector_platform_interface.dart';
import 'package:flutter_lite_camera/flutter_lite_camera.dart';
import 'frame_painter.dart';
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
  ui.Image? _latestFrame;
  bool isDecoding = false;

  @override
  void initState() {
    super.initState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    // Get 30-day FREEE trial license from https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform
    await _barcodeReader.setLicense(LICENSE_KEY);
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
    _barcodeResultsList = await _barcodeReader.decodeImageBuffer(
      rgb,
      width,
      height,
      width * 3,
      ImagePixelFormat.IPF_RGB_888.index,
    );

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
        _decodeFrame(rgbBuffer, frame['width'], frame['height']);
        await _convertBufferToImage(rgbBuffer, frame['width'], frame['height']);
      }
    } catch (e) {
      // print("Error capturing frame: $e");
    }

    // Schedule the next frame
    if (_shouldCapture) {
      Future.delayed(const Duration(milliseconds: 30), _captureFrames);
    }
  }

  Future<void> _convertBufferToImage(
      Uint8List rgbBuffer, int width, int height) async {
    final pixels = Uint8List(width * height * 4); // RGBA buffer

    for (int i = 0; i < width * height; i++) {
      int r = rgbBuffer[i * 3];
      int g = rgbBuffer[i * 3 + 1];
      int b = rgbBuffer[i * 3 + 2];

      // Populate RGBA buffer
      pixels[i * 4] = b;
      pixels[i * 4 + 1] = g;
      pixels[i * 4 + 2] = r;
      pixels[i * 4 + 3] = 255; // Alpha channel
    }

    final completer = Completer<ui.Image>();
    ui.decodeImageFromPixels(
      pixels,
      width,
      height,
      ui.PixelFormat.rgba8888,
      completer.complete,
    );

    final image = await completer.future;
    setState(() {
      _latestFrame = image;
    });
  }

  Future<void> _startCamera() async {
    try {
      List<String> devices = await _flutterLiteCameraPlugin.getDeviceList();
      if (devices.isNotEmpty) {
        print("Available Devices: $devices");
        print("Opening camera 0");
        bool opened = await _flutterLiteCameraPlugin.open(0);
        if (opened) {
          _isCameraOpened = true;
          _shouldCapture = true;
          _isCapturing = true;
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
      await _flutterLiteCameraPlugin.release();
      _isCameraOpened = false;
      _latestFrame = null;
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
                child: _latestFrame != null
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
    final screenWidth = constraints.maxWidth;
    final screenHeight = constraints.maxHeight;
    final imageAspectRatio = _width / _height;
    final screenAspectRatio = screenWidth / screenHeight;

    double drawWidth, drawHeight;
    if (imageAspectRatio > screenAspectRatio) {
      drawWidth = screenWidth;
      drawHeight = screenWidth / imageAspectRatio;
    } else {
      drawHeight = screenHeight;
      drawWidth = screenHeight * imageAspectRatio;
    }

    return Column(
      children: [
        // Constrain the camera stream height to avoid overflow
        Container(
          width: drawWidth,
          height: drawHeight.clamp(
              0.0, constraints.maxHeight * 0.8), // Use up to 80% of height
          alignment: Alignment.center,
          child: CustomPaint(
            painter: FramePainter(_latestFrame!, _barcodeResultsList),
            child: SizedBox(
              width: drawWidth,
              height: drawHeight,
            ),
          ),
        ),

        // Add spacing below the camera stream
        SizedBox(height: 10),

        // Display barcode results
        // Expanded(
        //   child: SingleChildScrollView(
        //     child: SelectableText(
        //       _barcodeResults.isEmpty ? 'No barcode detected' : _barcodeResults,
        //       style: TextStyle(
        //         fontSize: 20,
        //         color: Colors.black,
        //       ),
        //       textAlign: TextAlign.center,
        //     ),
        //   ),
        // ),
      ],
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
