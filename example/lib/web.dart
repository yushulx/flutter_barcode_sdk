import 'dart:io';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_barcode_sdk/flutter_barcode_sdk.dart';
import 'package:image_picker/image_picker.dart';

class Web extends StatefulWidget {
  @override
  _WebState createState() => _WebState();
}

class _WebState extends State<Web> {
  String _platformVersion = 'Unknown';
  FlutterBarcodeSdk _barcodeReader = FlutterBarcodeSdk();
  bool _isValid = false;
  String _file = '';
  String _barcodeResults = '';
  final picker = ImagePicker();
  Image _image;

  @override
  void initState() {
    super.initState();
    initPlatformState();
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

  Future getImage() async {
    final pickedFile = await picker.getImage(source: ImageSource.camera);

    setState(() {
      if (pickedFile != null) {
        _image = Image.network(pickedFile.path);
        print(pickedFile.path);
        print(_image);
      } else {
        print('No image selected.');
      }
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
                    _image == null ? Image.asset('images/default.png') : _image,
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
                          getImage();
                        }),
                    MaterialButton(
                        child: Text('Barcode Scanner'),
                        textColor: Colors.white,
                        color: Colors.blue,
                        onPressed: () async {
                          _barcodeReader.decodeVideo();
                        }),
                  ]),
            ),
          ])),
    );
  }
}
