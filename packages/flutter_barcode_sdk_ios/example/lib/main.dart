import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'mobile.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  final cameras = await availableCameras();
  final firstCamera = cameras.first;
  runApp(
    MaterialApp(
      title: 'Dynamsoft Barcode Reader',
      home: Scaffold(body: Mobile(camera: firstCamera)),
    ),
  );
}
