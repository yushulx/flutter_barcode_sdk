@JS('Dynamsoft')
library dynamsoft;

import 'dart:convert';
import 'dart:js';
import 'package:js/js.dart';
import 'utils.dart';

@JS('DBR')
class DBR {
  external static Object get EnumBarcodeFormat;
}

@JS('DBR.BarcodeScanner')
class BarcodeScanner {
  external static PromiseJsImpl<BarcodeScanner> createInstance();
  external void show();
  external set onFrameRead(Function func);
}

@JS('DBR.BarcodeReader')
class BarcodeReader {
  external static PromiseJsImpl<BarcodeReader> createInstance();
  external PromiseJsImpl<List<dynamic>> decode(dynamic file);
}

class BarcodeManager {
  late BarcodeScanner _barcodeScanner;
  late BarcodeReader _barcodeReader;

  void initBarcodeScanner(BarcodeScanner scanner) {
    _barcodeScanner = scanner;
    _barcodeScanner.onFrameRead =
        allowInterop((results) => {print(_resultWrapper(results))});
  }

  void initBarcodeReader(BarcodeReader reader) {
    _barcodeReader = reader;
  }

  BarcodeManager() {
    handleThenable(BarcodeScanner.createInstance())
        .then((scanner) => {initBarcodeScanner(scanner)});

    handleThenable(BarcodeReader.createInstance())
        .then((reader) => {initBarcodeReader(reader)});
  }

  void decodeVideo() {
    _barcodeScanner.show();
  }

  List<Map<dynamic, dynamic>> _resultWrapper(List<dynamic> barcodeResults) {
    List<Map<dynamic, dynamic>> results = [];

    for (dynamic result in barcodeResults) {
      Map value = json.decode(stringify(result));

      var tmp = Map<dynamic, dynamic>();
      tmp['format'] = value['barcodeFormatString'];
      tmp['text'] = value['barcodeText'];
      tmp['x1'] = value['localizationResult']['x1'];
      tmp['y1'] = value['localizationResult']['y1'];
      tmp['x2'] = value['localizationResult']['x2'];
      tmp['y2'] = value['localizationResult']['y2'];
      tmp['x3'] = value['localizationResult']['x3'];
      tmp['y3'] = value['localizationResult']['y3'];
      tmp['x4'] = value['localizationResult']['x4'];
      tmp['y4'] = value['localizationResult']['y4'];
      tmp['angle'] = value['localizationResult']['angle'];
      results.add(tmp);
    }

    return results;
  }

  Future<List<Map<dynamic, dynamic>>> decodeFile(String filename) async {
    List<dynamic> barcodeResults =
        await handleThenable(_barcodeReader.decode(filename));

    return _resultWrapper(barcodeResults);
  }
}
