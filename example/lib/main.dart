import 'package:flutter/material.dart';

import 'barcode_app.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();

  runApp(
    MaterialApp(
      title: 'Dynamsoft Barcode Reader',
      home: const BarcodeApp(),
    ),
  );
}
