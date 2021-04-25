import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:io';

import 'package:camera/camera.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:camera_platform_interface/camera_platform_interface.dart';
import 'package:flutter/foundation.dart' show kIsWeb;

String getBarcodeResults(List<BarcodeResult> results) {
  StringBuffer sb = new StringBuffer();
  for (BarcodeResult result in results) {
    sb.write(result.format);
    sb.write("\n");
    sb.write(result.text);
    sb.write("\n\n");
  }
  if (results.length == 0) sb.write("No Barcode Detected");

  return sb.toString();
}

Future<void> main() async {
  if (kIsWeb) {
    runApp(MyApp());
  } else if (Platform.isAndroid) {
    // Ensure that plugin services are initialized so that `availableCameras()`
    // can be called before `runApp()`
    WidgetsFlutterBinding.ensureInitialized();

    // Obtain a list of the available cameras on the device.
    final cameras = await availableCameras();

    // Get a specific camera from the list of available cameras.
    final firstCamera = cameras.first;

    runApp(
      MaterialApp(
        title: 'Dynamsoft Barcode Reader',
        home: Scaffold(
          appBar: AppBar(
            title: Text("Dynamsoft Barcode Reader"),
          ),
          body: HomeScreen(
            camera: firstCamera,
          ),
        ),
      ),
    );
  } else {
    runApp(MyApp());
  }
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  final _controller = TextEditingController();
  String _barcodeResults = '';
  FlutterBarcodeSdk _barcodeReader;

  @override
  void initState() {
    super.initState();
    initPlatformState();

    _barcodeReader = FlutterBarcodeSdk();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    try {
      platformVersion = await FlutterBarcodeSdk.platformVersion;
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
          appBar: AppBar(
            title: const Text('Dynamsoft Barcode Reader'),
          ),
          body: Column(children: [
            Container(
              height: 100,
              child: Row(children: <Widget>[
                Text(
                  _platformVersion,
                  style: TextStyle(fontSize: 14, color: Colors.black),
                )
              ]),
            ),
            TextField(
              controller: _controller,
            ),
            Expanded(
              child: SingleChildScrollView(
                child: Text(
                  _barcodeResults,
                  style: TextStyle(fontSize: 14, color: Colors.black),
                ),
              ),
            ),
            Container(
              height: 100,
              child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: <Widget>[
                    MaterialButton(
                        child: Text('Decode Barcode'),
                        textColor: Colors.white,
                        color: Colors.blue,
                        onPressed: () async {
                          List<BarcodeResult> results =
                              await _barcodeReader.decodeFile(_controller.text);
                          setState(() {
                            _barcodeResults = getBarcodeResults(results);
                          });
                        }),
                  ]),
            ),
          ])),
    );
  }
}

class HomeScreen extends StatefulWidget {
  final CameraDescription camera;

  const HomeScreen({
    Key key,
    @required this.camera,
  }) : super(key: key);

  @override
  HomeScreenState createState() => HomeScreenState();
}

class HomeScreenState extends State<HomeScreen> {
  CameraController _controller;
  Future<void> _initializeControllerFuture;
  FlutterBarcodeSdk _barcodeReader;
  bool _isScanAvailable = true;
  bool _isScanRunning = false;
  String _barcodeResults = '';
  String _buttonText = 'Start Video Scan';

  @override
  void initState() {
    super.initState();
    // To display the current output from the Camera,
    // create a CameraController.
    _controller = CameraController(
      // Get a specific camera from the list of available cameras.
      widget.camera,
      // Define the resolution to use.
      ResolutionPreset.medium,
    );

    // Next, initialize the controller. This returns a Future.
    _initializeControllerFuture = _controller.initialize();
    _initializeControllerFuture.then((_) {
      setState(() {});
    });
    // Initialize Dynamsoft Barcode Reader
    _barcodeReader = FlutterBarcodeSdk();
  }

  void pictureScan() async {
    final image = await _controller.takePicture();
    List<BarcodeResult> results = await _barcodeReader.decodeFile(image?.path);

    // Uint8List bytes = await image.readAsBytes();
    // List<BarcodeResult> results =
    //     await _barcodeReader.decodeFileBytes(bytes);

    // If the picture was taken, display it on a new screen.
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => DisplayPictureScreen(
            // Pass the automatically generated path to
            // the DisplayPictureScreen widget.
            imagePath: image?.path,
            barcodeResults: getBarcodeResults(results)),
      ),
    );
  }

  void videoScan() async {
    if (!_isScanRunning) {
      setState(() {
        _buttonText = 'Stop Video Scan';
      });
      _isScanRunning = true;
      await _controller.startImageStream((CameraImage availableImage) async {
        assert(defaultTargetPlatform == TargetPlatform.android ||
            defaultTargetPlatform == TargetPlatform.iOS);
        int format = FlutterBarcodeSdk.IF_UNKNOWN;

        switch (availableImage.format.group) {
          case ImageFormatGroup.yuv420:
            format = FlutterBarcodeSdk.IF_YUV420;
            break;
          case ImageFormatGroup.bgra8888:
            format = FlutterBarcodeSdk.IF_BRGA8888;
            break;
          default:
            format = FlutterBarcodeSdk.IF_UNKNOWN;
        }

        if (!_isScanAvailable) {
          return;
        }

        _isScanAvailable = false;

        _barcodeReader
            .decodeImageBuffer(
                availableImage.planes[0].bytes,
                availableImage.width,
                availableImage.height,
                availableImage.planes[0].bytesPerRow,
                format)
            .then((results) {
          if (_isScanRunning) {
            setState(() {
              _barcodeResults = getBarcodeResults(results);
            });
          }

          _isScanAvailable = true;
        }).catchError((error) {
          _isScanAvailable = false;
        });
      });
    } else {
      setState(() {
        _buttonText = 'Start Video Scan';
        _barcodeResults = '';
      });
      _isScanRunning = false;
      await _controller.stopImageStream();
    }
  }

  @override
  void dispose() {
    // Dispose of the controller when the widget is disposed.
    _controller?.dispose();
    super.dispose();
  }

  Widget getCameraWidget() {
    if (!_controller.value.isInitialized) {
      return Center(child: CircularProgressIndicator());
    } else {
      // https://stackoverflow.com/questions/49946153/flutter-camera-appears-stretched
      final size = MediaQuery.of(context).size;
      var scale = size.aspectRatio * _controller.value.aspectRatio;

      if (scale < 1) scale = 1 / scale;

      return Transform.scale(
        scale: scale,
        child: Center(
          child: CameraPreview(_controller),
        ),
      );
      // return CameraPreview(_controller);
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(children: [
      Expanded(child: getCameraWidget()),
      Container(
        height: 100,
        child: Row(children: <Widget>[
          Text(
            _barcodeResults,
            style: TextStyle(fontSize: 14, color: Colors.white),
          )
        ]),
      ),
      Container(
        height: 100,
        child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: <Widget>[
              MaterialButton(
                  child: Text(_buttonText),
                  textColor: Colors.white,
                  color: Colors.blue,
                  onPressed: () async {
                    try {
                      // Ensure that the camera is initialized.
                      await _initializeControllerFuture;

                      videoScan();
                      // pictureScan();
                    } catch (e) {
                      // If an error occurs, log the error to the console.
                      print(e);
                    }
                  }),
              MaterialButton(
                  child: Text("Picture Scan"),
                  textColor: Colors.white,
                  color: Colors.blue,
                  onPressed: () async {
                    pictureScan();
                  })
            ]),
      ),
    ]);
  }
}

// A widget that displays the picture taken by the user.
class DisplayPictureScreen extends StatelessWidget {
  final String imagePath;
  final String barcodeResults;

  const DisplayPictureScreen({Key key, this.imagePath, this.barcodeResults})
      : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('Dynamsoft Barcode Reader')),
      // The image is stored as a file on the device. Use the `Image.file`
      // constructor with the given path to display the image.
      body: Stack(
        alignment: const Alignment(0.0, 0.0),
        children: [
          // Show full screen image: https://stackoverflow.com/questions/48716067/show-fullscreen-image-at-flutter
          Image.file(
            File(imagePath),
            fit: BoxFit.cover,
            height: double.infinity,
            width: double.infinity,
            alignment: Alignment.center,
          ),
          Container(
            decoration: BoxDecoration(
              color: Colors.black45,
            ),
            child: Text(
              // 'Dynamsoft Barcode Reader',
              barcodeResults,
              style: TextStyle(
                fontSize: 14,
                color: Colors.white,
              ),
            ),
          ),
        ],
      ),
    );
  }
}
