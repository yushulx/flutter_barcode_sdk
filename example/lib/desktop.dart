import 'dart:convert';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_example/utils.dart';

class Desktop extends StatefulWidget {
  @override
  _DesktopState createState() => _DesktopState();
}

class _DesktopState extends State<Desktop> {
  String _platformVersion = 'Unknown';
  final _controller = TextEditingController();
  String _barcodeResults = '';
  FlutterBarcodeSdk _barcodeReader;
  bool _isValid = false;
  String _file = '';

  @override
  void initState() {
    super.initState();
    initPlatformState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    // Get 30-day FREEE trial license from https://www.dynamsoft.com/customer/license/trialLicense?product=dbr
    await _barcodeReader.setLicense('LICENSE-KEY');
    await _barcodeReader.setBarcodeFormats(BarcodeFormat.ALL);

    // Get all current parameters.
    // Refer to: https://www.dynamsoft.com/barcode-reader/parameters/reference/image-parameter/?ver=latest
    String params = await _barcodeReader.getParameters();
    // Convert parameters to a JSON object.
    dynamic obj = json.decode(params);
    // Modify parameters.
    if (obj['ImageParameter'] != null) {
      obj['ImageParameter']['DeblurLevel'] = 5;
    } else
      obj['deblurLevel'] = 5;
    // Update the parameters.
    int ret = await _barcodeReader.setParameters(json.encode(obj));
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
      return Image.file(File(_file));
    }
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
                        child: Text('Decode Barcode'),
                        textColor: Colors.white,
                        color: Colors.blue,
                        onPressed: () async {
                          if (_controller.text.isEmpty) {
                            setState(() {
                              _isValid = false;
                              _barcodeResults = '';
                              _file = '';
                            });
                            return;
                          }

                          File file = File(_controller.text);
                          if (!file.existsSync()) {
                            setState(() {
                              _isValid = false;
                              _barcodeResults = '';
                              _file = '';
                            });
                            return;
                          } else {
                            setState(() {
                              _isValid = true;
                              _file = _controller.text;
                            });
                          }
                          // Uint8List bytes = await file.readAsBytes();
                          // List<BarcodeResult> results =
                          //     await _barcodeReader.decodeFileBytes(bytes);

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
