#import "BarcodeManagerBridge.h"

#import "DynamsoftCaptureVisionRouter.h"
#import "DynamsoftCore.h"
#import "DynamsoftLicense.h"

using namespace dynamsoft::basic_structures;
using namespace dynamsoft::cvr;
using namespace dynamsoft::license;

@implementation BarcodeManagerBridge {
  CCaptureVisionRouter *_router;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _router = nullptr;
  }
  return self;
}

- (void)dealloc {
  [self dispose];
}

#pragma mark - Lifecycle

- (int)initSdk {
  @synchronized(self) {
    if (_router == nullptr) {
      _router = new CCaptureVisionRouter();
    }
    return _router == nullptr ? -1 : 0;
  }
}

- (void)dispose {
  @synchronized(self) {
    if (_router != nullptr) {
      delete _router;
      _router = nullptr;
    }
  }
}

#pragma mark - License

- (int)setLicense:(NSString *)license {
  char errorMsgBuffer[512] = {0};
  int ret = CLicenseManager::InitLicense(license.UTF8String, errorMsgBuffer, 512);
  if (ret) {
    NSLog(@"Dynamsoft license error: %s", errorMsgBuffer);
  }
  return ret;
}

#pragma mark - Decoding

- (NSArray<NSDictionary *> *)decodeFile:(NSString *)path {
  @synchronized(self) {
    if (_router == nullptr) {
      return [self wrapError:-1 msg:@"SDK not initialized"];
    }
    CCapturedResult *captured = _router->Capture(path.UTF8String, "");
    return [self wrapResult:captured];
  }
}

- (NSArray<NSDictionary *> *)decodeImageBuffer:(NSData *)bytes
                                         width:(int)width
                                        height:(int)height
                                        stride:(int)stride
                                        format:(int)format
                                      rotation:(int)rotation {
  @synchronized(self) {
    if (_router == nullptr) {
      return [self wrapError:-1 msg:@"SDK not initialized"];
    }
    if (bytes.length == 0 || width <= 0 || height <= 0) {
      return [self wrapError:-2 msg:@"Invalid image buffer"];
    }

    ImagePixelFormat pixelFormat = [self pixelFormatForIndex:format];
    CImageData *imageData =
        new CImageData(bytes.length, (const unsigned char *)bytes.bytes, width,
                       height, stride, pixelFormat, rotation);
    CCapturedResult *captured = _router->Capture(imageData);
    delete imageData;
    return [self wrapResult:captured];
  }
}

#pragma mark - Settings

- (int)setBarcodeFormats:(unsigned long long)formats {
  @synchronized(self) {
    if (_router == nullptr) {
      return -1;
    }
    SimplifiedCaptureVisionSettings settings = {};
    _router->GetSimplifiedSettings("", &settings);
    settings.barcodeSettings.barcodeFormatIds = formats;
    char errorMsgBuffer[256] = {0};
    int ret = _router->UpdateSettings("", &settings, errorMsgBuffer, 256);
    if (ret) {
      NSLog(@"UpdateSettings error: %s", errorMsgBuffer);
    }
    return ret;
  }
}

- (NSString *)getParameters {
  @synchronized(self) {
    if (_router == nullptr) {
      return @"";
    }
    char *content = _router->OutputSettings("", false, nullptr);
    if (content == nullptr) {
      return @"";
    }
    NSString *params = [NSString stringWithUTF8String:content];
    CCaptureVisionRouter::FreeString(content);
    return params;
  }
}

- (int)setParameters:(NSString *)params {
  @synchronized(self) {
    if (_router == nullptr) {
      return -1;
    }
    char errorMsgBuffer[256] = {0};
    int ret = _router->InitSettings(params.UTF8String, errorMsgBuffer, 256);
    if (ret) {
      NSLog(@"InitSettings error: %s", errorMsgBuffer);
    }
    return ret;
  }
}

#pragma mark - Result wrapping

- (NSArray<NSDictionary *> *)wrapResult:(CCapturedResult *)captured {
  if (captured == nullptr) {
    return [self wrapError:-1 msg:@"No capture result"];
  }

  if (captured->GetErrorCode()) {
    NSArray<NSDictionary *> *result =
        [self wrapError:captured->GetErrorCode()
                    msg:[NSString stringWithUTF8String:captured->GetErrorString()
                                                     ?: ""]];
    captured->Release();
    return result;
  }

  CDecodedBarcodesResult *barcodes = captured->GetDecodedBarcodesResult();
  NSMutableArray<NSDictionary *> *results = [NSMutableArray array];

  if (barcodes != nullptr) {
    int count = barcodes->GetCount();
    for (int i = 0; i < count; i++) {
      const CBarcodeResultItem *item = barcodes->GetItem(i);
      if (item == nullptr) {
        continue;
      }

      CQuadrilateral location = item->GetLocation();
      NSMutableDictionary *entry = [NSMutableDictionary dictionary];
      entry[@"format"] = [NSString stringWithUTF8String:item->GetFormatString()
                                                      ?: ""];
      entry[@"text"] = [NSString stringWithUTF8String:item->GetText() ?: ""];

      // The quadrilateral contains 4 corner points; coordinates are
      // accessed via the point's coordinate array (index 0 = x, 1 = y).
      entry[@"x1"] = @(location.points[0][0]);
      entry[@"y1"] = @(location.points[0][1]);
      entry[@"x2"] = @(location.points[1][0]);
      entry[@"y2"] = @(location.points[1][1]);
      entry[@"x3"] = @(location.points[2][0]);
      entry[@"y3"] = @(location.points[2][1]);
      entry[@"x4"] = @(location.points[3][0]);
      entry[@"y4"] = @(location.points[3][1]);

      entry[@"angle"] = @(item->GetAngle());

      NSData *rawBytes = [NSData dataWithBytes:item->GetBytes()
                                        length:item->GetBytesLength()];
      entry[@"barcodeBytes"] = rawBytes;
      entry[@"errorCode"] = @0;
      entry[@"errorMsg"] = @"";
      [results addObject:entry];
    }
    barcodes->Release();
  }

  captured->Release();
  return results;
}

- (NSArray<NSDictionary *> *)wrapError:(int)errorCode msg:(NSString *)msg {
  NSMutableDictionary *entry = [NSMutableDictionary dictionary];
  entry[@"format"] = @"";
  entry[@"text"] = @"";
  [self fillZeroCoordinates:entry];
  entry[@"angle"] = @0;
  entry[@"barcodeBytes"] = [NSData data];
  entry[@"errorCode"] = @(errorCode);
  entry[@"errorMsg"] = msg;
  return @[ entry ];
}

- (void)fillZeroCoordinates:(NSMutableDictionary *)entry {
  entry[@"x1"] = @0;
  entry[@"y1"] = @0;
  entry[@"x2"] = @0;
  entry[@"y2"] = @0;
  entry[@"x3"] = @0;
  entry[@"y3"] = @0;
  entry[@"x4"] = @0;
  entry[@"y4"] = @0;
}

- (ImagePixelFormat)pixelFormatForIndex:(int)format {
  switch (format) {
    case 0:
      return IPF_BINARY;
    case 1:
      return IPF_BINARYINVERTED;
    case 2:
      return IPF_GRAYSCALED;
    case 3:
      return IPF_NV21;
    case 4:
      return IPF_RGB_565;
    case 5:
      return IPF_RGB_555;
    case 6:
      return IPF_RGB_888;
    case 7:
      return IPF_ARGB_8888;
    case 8:
      return IPF_RGB_161616;
    case 9:
      return IPF_ARGB_16161616;
    case 10:
      return IPF_ABGR_8888;
    case 11:
      return IPF_ABGR_16161616;
    case 12:
      return IPF_BGR_888;
    default:
      return IPF_BGR_888;
  }
}

@end