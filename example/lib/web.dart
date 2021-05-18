import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:flutter_barcode_sdk_example/utils.dart';
import 'package:image_picker/image_picker.dart';

class Web extends StatefulWidget {
  @override
  _WebState createState() => _WebState();
}

class _WebState extends State<Web> {
  String _platformVersion = 'Unknown';
  FlutterBarcodeSdk _barcodeReader;
  String _file;
  String _barcodeResults = '';
  final picker = ImagePicker();

  @override
  void initState() {
    super.initState();
    initPlatformState();
    initBarcodeSDK();
  }

  Future<void> initBarcodeSDK() async {
    _barcodeReader = FlutterBarcodeSdk();
    await _barcodeReader.setBarcodeFormats(BarcodeFormat.ALL);
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
    setState(() {
      _barcodeResults = getBarcodeResults(results);
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
            Expanded(
              child: SingleChildScrollView(
                child: Column(
                  children: [
                    _file == null
                        ? Image.asset('images/default.png')
                        : Image.network(_file),
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
                          final pickedFile =
                              await picker.getImage(source: ImageSource.camera);

                          setState(() {
                            if (pickedFile != null) {
                              _file = pickedFile.path;
                            } else {
                              print('No image selected.');
                            }

                            _barcodeResults = '';
                          });

                          if (_file != null) {
                            List<BarcodeResult> results =
                                await _barcodeReader.decodeFile(_file);
                            updateResults(results);
                          }
                        }),
                    MaterialButton(
                        child: Text('Barcode Scanner'),
                        textColor: Colors.white,
                        color: Colors.blue,
                        onPressed: () async {
                          _barcodeReader.decodeVideo(
                              (results) => {updateResults(results)});
                        }),
                  ]),
            ),
          ])),
    );
  }
}
