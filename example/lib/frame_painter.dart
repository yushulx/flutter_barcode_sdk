import 'package:flutter/material.dart';

import 'package:flutter_barcode_sdk/dynamsoft_barcode.dart';
import 'dart:ui' as ui;

class FramePainter extends CustomPainter {
  final ui.Image image;
  final List<BarcodeResult> results;

  FramePainter(this.image, this.results);

  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint();

    // Calculate the scaling factor (minimum of X and Y ratios)
    final double scale =
        (size.width / image.width).compareTo(size.height / image.height) < 0
            ? size.width / image.width
            : size.height / image.height;

    // Center the image in the canvas
    final double dx = (size.width - image.width * scale) / 2;
    final double dy = (size.height - image.height * scale) / 2;

    // Draw the image
    canvas.drawImageRect(
      image,
      Rect.fromLTWH(0, 0, image.width.toDouble(), image.height.toDouble()),
      Rect.fromLTWH(dx, dy, image.width * scale, image.height * scale),
      paint,
    );

    // Draw barcode results
    if (results.isNotEmpty) {
      final textPaint = Paint()
        ..color = Colors.blue
        ..style = PaintingStyle.stroke
        ..strokeWidth = 2;

      for (var result in results) {
        // Scale the coordinates and offset by dx and dy
        final path = Path()
          ..moveTo(dx + result.x1.toDouble() * scale,
              dy + result.y1.toDouble() * scale)
          ..lineTo(dx + result.x2.toDouble() * scale,
              dy + result.y2.toDouble() * scale)
          ..lineTo(dx + result.x3.toDouble() * scale,
              dy + result.y3.toDouble() * scale)
          ..lineTo(dx + result.x4.toDouble() * scale,
              dy + result.y4.toDouble() * scale)
          ..close();

        canvas.drawPath(path, textPaint);

        // Scale text position
        final double textX = dx + result.x1.toDouble() * scale;
        final double textY = dy + result.y1.toDouble() * scale;

        final textPainter = TextPainter(
          text: TextSpan(
            text: result.text,
            style: const TextStyle(
              color: Colors.red,
              fontSize: 16,
            ),
          ),
          textDirection: TextDirection.ltr,
        );

        textPainter.layout();
        textPainter.paint(
          canvas,
          Offset(textX, textY),
        );
      }
    }
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => true;
}
