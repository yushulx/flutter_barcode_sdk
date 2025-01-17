import 'dart:convert';
import 'dart:io';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_example/utils.dart';
import 'package:file_selector_platform_interface/file_selector_platform_interface.dart';
import 'dart:typed_data';

class Desktop extends StatefulWidget {
  @override
  _DesktopState createState() => _DesktopState();
}

class _DesktopState extends State<Desktop> {
  String _platformVersion = 'Unknown';
  final _controller = TextEditingController();
  String _barcodeResults = '';
  late FlutterBarcodeSdk _barcodeReader;
  bool _isValid = false;

  @override
  void initState() {
    super.initState();
    initPlatformState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    // Get 30-day FREEE trial license from https://www.dynamsoft.com/customer/license/trialLicense/?product=dcv&package=cross-platform
    await _barcodeReader.setLicense(
        'DLS2eyJoYW5kc2hha2VDb2RlIjoiMjAwMDAxLTE2NDk4Mjk3OTI2MzUiLCJvcmdhbml6YXRpb25JRCI6IjIwMDAwMSIsInNlc3Npb25QYXNzd29yZCI6IndTcGR6Vm05WDJrcEQ5YUoifQ==');
    await _barcodeReader.init();
    await _barcodeReader.setBarcodeFormats(BarcodeFormat.ALL);

    // Get all current parameters.
    // Refer to: https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest
    String params = await _barcodeReader.getParameters();
    // Update the parameters.
    int ret = await _barcodeReader.setParameters(params);
    print('Parameter update: $ret');
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

  Widget getDefaultImage() {
    if (_controller.text.isEmpty || !_isValid) {
      return Image.asset('images/default.png');
    } else {
      return Image.file(File(_controller.text));
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
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
          decoration: InputDecoration(
            labelText: 'Input an image path',
            errorText: _isValid ? null : 'File not exists',
          ),
        ),
        Expanded(
          child: SingleChildScrollView(
            child: Column(
              children: [
                getDefaultImage(),
                Text(
                  _barcodeResults,
                  style: TextStyle(fontSize: 14, color: Colors.black),
                ),
              ],
            ),
          ),
        ),
        Container(
          height: 100,
          child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                MaterialButton(
                    child: Text('Open an Image'),
                    textColor: Colors.white,
                    color: Colors.blue,
                    onPressed: () async {
                      final typeGroup = XTypeGroup(
                        label: 'images',
                        extensions: ['jpg', 'png'],
                      );
                      try {
                        final files = await FileSelectorPlatform.instance
                            .openFiles(acceptedTypeGroups: [typeGroup]);
                        final file = files[0];
                        List<BarcodeResult> results =
                            await _barcodeReader.decodeFile(file.path);

                        _isValid = true;
                        setState(() {
                          _controller.text = file.path;
                          _barcodeResults = getBarcodeResults(results);
                        });
                      } catch (err) {
                        print('Error: $err');
                      }
                    }),
                MaterialButton(
                    child: Text('Decode Barcode'),
                    textColor: Colors.white,
                    color: Colors.blue,
                    onPressed: () async {
                      if (_controller.text.isEmpty) {
                        _isValid = false;
                        setState(() {
                          _barcodeResults = '';
                        });
                        return;
                      }

                      File file = File(_controller.text);
                      if (!file.existsSync()) {
                        _isValid = false;
                        setState(() {
                          _barcodeResults = '';
                        });
                        return;
                      } else {
                        _isValid = true;
                      }
                      Uint8List fileBytes = await file.readAsBytes();

                      ui.Image image = await decodeImageFromList(fileBytes);

                      ByteData? byteData = await image.toByteData(
                          format: ui.ImageByteFormat.rawRgba);

                      if (byteData == null) return;
                      List<BarcodeResult> results =
                          await _barcodeReader.decodeImageBuffer(
                              byteData.buffer.asUint8List(),
                              image.width,
                              image.height,
                              byteData.lengthInBytes ~/ image.height,
                              ImagePixelFormat.IPF_ARGB_8888.index);
                      // List<BarcodeResult> results =
                      //     await _barcodeReader.decodeFile(_controller.text);

                      setState(() {
                        _barcodeResults = getBarcodeResults(results);
                      });
                    }),
              ]),
        ),
      ]),
    );
  }
}
