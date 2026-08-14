import 'package:flutter/material.dart';
import 'web.dart';

void main() {
  runApp(
    MaterialApp(
      title: 'Dynamsoft Barcode Reader',
      home: Scaffold(body: Web()),
    ),
  );
}
