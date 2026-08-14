/// Defines the supported image pixel formats.
enum ImagePixelFormat {
  /// Binary format (0: Black, 1: White).
  IPF_BINARY,

  /// Inverted binary format (0: White, 1: Black).
  IPF_BINARYINVERTED,

  /// 8-bit grayscale format.
  IPF_GRAYSCALED,

  /// NV21 format (commonly used in Android camera preview).
  IPF_NV21,

  /// 16-bit RGB format with 5 bits for red, 6 bits for green, and 5 bits for blue.
  IPF_RGB_565,

  /// 16-bit RGB format with 5 bits for each color channel.
  IPF_RGB_555,

  /// 24-bit RGB format with 8 bits for each color channel.
  IPF_RGB_888,

  /// 32-bit ARGB format (Alpha, Red, Green, Blue) stored from high to low memory address.
  IPF_ARGB_8888,

  /// 48-bit RGB format with 16 bits for each color channel.
  IPF_RGB_161616,

  /// 64-bit ARGB format with 16 bits for each channel.
  IPF_ARGB_16161616,

  /// 32-bit ABGR format (Alpha, Blue, Green, Red) stored from high to low memory address.
  IPF_ABGR_8888,

  /// 64-bit ABGR format with 16 bits for each channel.
  IPF_ABGR_16161616,

  /// 24-bit BGR format with 8 bits for each channel.
  IPF_BGR_888,
}
