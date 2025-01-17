import 'dart:convert';
import 'dart:ui' as ui;

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_example/utils.dart';
import 'package:image_picker/image_picker.dart';

import 'scanner_screen.dart';

class Web extends StatefulWidget {
  @override
  _WebState createState() => _WebState();
}

class _WebState extends State<Web> {
  String _platformVersion = 'Unknown';
  FlutterBarcodeSdk? _barcodeReader;
  String? _file;
  String _barcodeResults = '';
  final picker = ImagePicker();
  bool _isSDKLoaded = false;

  @override
  void initState() {
    super.initState();
    initPlatformState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    await _barcodeReader!.setLicense('LICENSE-KEY');
    await _barcodeReader!.init();
    await _barcodeReader!.setBarcodeFormats(BarcodeFormat.ALL);

    setState(() {
      _isSDKLoaded = true;
    });
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

  void updateResults(List<BarcodeResult> results) {
    // if (results.length > 0) _barcodeReader.closeVideo();

    setState(() {
      _barcodeResults = getBarcodeResults(results);
    });
  }

  _showDialog(String title, String message) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text(title),
          content: Text(message),
          actions: <Widget>[
            TextButton(
              child: const Text('OK'),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
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
        Expanded(
          child: SingleChildScrollView(
            child: Column(
              children: [
                _file == null
                    ? Image.asset('images/default.png')
                    : Image.network(_file!),
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
                    child: Text('Barcode Reader'),
                    textColor: Colors.white,
                    color: Colors.blue,
                    onPressed: () async {
                      if (_isSDKLoaded == false) {
                        _showDialog('Error', 'Barcode SDK is not loaded.');
                        return;
                      }

                      final pickedFile =
                          await picker.pickImage(source: ImageSource.gallery);

                      setState(() {
                        if (pickedFile != null) {
                          _file = pickedFile.path;
                        } else {
                          print('No image selected.');
                        }

                        _barcodeResults = '';
                      });

                      if (_file != null) {
                        // Uint8List fileBytes =
                        //     await pickedFile.readAsBytes();

                        // ui.Image image =
                        //     await decodeImageFromList(fileBytes);

                        // ByteData byteData = await image.toByteData(
                        //     format: ui.ImageByteFormat.rawRgba);
                        // List<BarcodeResult> results =
                        //     await _barcodeReader.decodeImageBuffer(
                        //         byteData.buffer.asUint8List(),
                        //         image.width,
                        //         image.height,
                        //         byteData.lengthInBytes ~/ image.height,
                        //         ImagePixelFormat.IPF_ARGB_8888.index);

                        List<BarcodeResult> results =
                            await _barcodeReader!.decodeFile(_file!);
                        updateResults(results);
                      }
                    }),
                MaterialButton(
                    child: Text('Barcode Scanner'),
                    textColor: Colors.white,
                    color: Colors.blue,
                    onPressed: () async {
                      if (_isSDKLoaded == false) {
                        _showDialog('Error', 'Barcode SDK is not loaded.');
                        return;
                      }

                      Navigator.push(
                        context,
                        MaterialPageRoute(
                            builder: (context) => ScannerScreen(
                                  barcodeReader: _barcodeReader!,
                                )),
                      );
                    }),
              ]),
        ),
      ]),
    );
  }
}
