#import "BarcodeManagerBridge.h"

#import "DynamsoftCaptureVisionRouter.h"
#import "DynamsoftCore.h"
#import "DynamsoftLicense.h"
#import "DynamsoftUtility.h"

#include <mutex>
#include <vector>

using namespace dynamsoft::basic_structures;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dbr;
using namespace dynamsoft::license;
using namespace dynamsoft::utility;

/// Collects decoded barcode results delivered by the router while
/// StartCapturing is running. Mirrors the Linux implementation.
class CapturedResultReceiver : public CCapturedResultReceiver {
public:
  void OnDecodedBarcodesReceived(CDecodedBarcodesResult *pResult) override {
    pResult->Retain();
    std::lock_guard<std::mutex> lock(mutex_);
    results_.push_back(pResult);
  }

  std::vector<CDecodedBarcodesResult *> takeResults() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<CDecodedBarcodesResult *> out;
    out.swap(results_);
    return out;
  }

private:
  std::vector<CDecodedBarcodesResult *> results_;
  std::mutex mutex_;
};

@interface BarcodeManagerBridge ()
/// Invoked from the SDK worker thread when the image source is exhausted.
- (void)handleCaptureFinished;
@end

/// Notifies the bridge when the file fetcher has no more images, i.e. the
/// current StartCapturing round has finished. Mirrors the Linux
/// implementation.
class ImageSourceStateListener : public CImageSourceStateListener {
public:
  explicit ImageSourceStateListener(BarcodeManagerBridge *bridge)
      : bridge_(bridge) {}

  void OnImageSourceStateReceived(ImageSourceState state) override {
    if (state == ISS_EXHAUSTED) {
      [bridge_ handleCaptureFinished];
    }
  }

private:
  BarcodeManagerBridge *bridge_;
};

@implementation BarcodeManagerBridge {
  CCaptureVisionRouter *_router;
  CFileFetcher *_fileFetcher;
  CapturedResultReceiver *_receiver;
  ImageSourceStateListener *_listener;
  void (^_pendingCompletion)(NSArray<NSDictionary *> *);
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _router = nullptr;
    _fileFetcher = nullptr;
    _receiver = nullptr;
    _listener = nullptr;
    _pendingCompletion = nil;
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

      _fileFetcher = new CFileFetcher();
      _router->SetInput(_fileFetcher);

      _receiver = new CapturedResultReceiver();
      _router->AddResultReceiver(_receiver);

      _listener = new ImageSourceStateListener(self);
      _router->AddImageSourceStateListener(_listener);
    }
    return _router == nullptr ? -1 : 0;
  }
}

- (void)dispose {
  // Swap the pointers out under the lock, then stop and delete the SDK
  // objects outside of it. StopCapturing waits for the capture thread to
  // exit; if a state-listener callback is concurrently blocked on the lock,
  // holding the lock here while joining that thread would deadlock.
  CCaptureVisionRouter *router;
  CFileFetcher *fileFetcher;
  CapturedResultReceiver *receiver;
  ImageSourceStateListener *listener;
  @synchronized(self) {
    router = _router;
    _router = nullptr;
    fileFetcher = _fileFetcher;
    _fileFetcher = nullptr;
    receiver = _receiver;
    _receiver = nullptr;
    listener = _listener;
    _listener = nullptr;
    _pendingCompletion = nil;
  }
  if (router != nullptr) {
    router->StopCapturing();
    delete router;
  }
  delete fileFetcher;
  delete receiver;
  delete listener;
}

#pragma mark - License

- (int)setLicense:(NSString *)license {
  char errorMsgBuffer[512] = {0};
  int ret = CLicenseManager::InitLicense(license.UTF8String, errorMsgBuffer, 512);
  NSLog(@"Dynamsoft InitLicense returned %d: %s", ret, errorMsgBuffer);
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

- (void)decodeImageBuffer:(NSData *)bytes
                    width:(int)width
                   height:(int)height
                   stride:(int)stride
                   format:(int)format
                 rotation:(int)rotation
               completion:(void (^)(NSArray<NSDictionary *> *))completion {
  @synchronized(self) {
    if (_router == nullptr) {
      [self respondAsync:[self wrapError:-1 msg:@"SDK not initialized"]
              completion:completion];
      return;
    }
    if (bytes.length == 0 || width <= 0 || height <= 0) {
      [self respondAsync:[self wrapError:-2 msg:@"Invalid image buffer"]
              completion:completion];
      return;
    }

    // If a previous frame is still being processed, release its caller with
    // an empty result so the Dart future does not hang; only the latest
    // frame matters in a video stream.
    if (_pendingCompletion != nil) {
      [self respondAsync:@[ ] completion:_pendingCompletion];
      _pendingCompletion = nil;
    }

    ImagePixelFormat pixelFormat = [self pixelFormatForIndex:format];
    CImageData imageData(bytes.length, (const unsigned char *)bytes.bytes,
                         width, height, stride, pixelFormat, rotation);
    // CFileFetcher copies the image data internally.
    _fileFetcher->SetFile(&imageData);

    _pendingCompletion = [completion copy];

    char errorMsgBuffer[512] = {0};
    int errorCode =
        _router->StartCapturing("", false, errorMsgBuffer, 512);
    if (errorCode != 0) {
      NSLog(@"StartCapturing error %d: %s", errorCode, errorMsgBuffer);
      void (^pending)(NSArray<NSDictionary *> *) = _pendingCompletion;
      _pendingCompletion = nil;
      [self respondAsync:[self wrapError:errorCode
                                     msg:[NSString stringWithUTF8String:
                                                       errorMsgBuffer]]
              completion:pending];
    }
  }
}

/// Called by ImageSourceStateListener on an SDK worker thread once the
/// current capture round is exhausted. Stops capturing, wraps whatever
/// barcode results were received and answers the pending Dart call on the
/// main queue.
- (void)handleCaptureFinished {
  void (^completion)(NSArray<NSDictionary *> *) = nil;
  NSArray<NSDictionary *> *out = nil;
  @synchronized(self) {
    if (_router != nullptr) {
      _router->StopCapturing();
    }
    if (_receiver != nullptr) {
      out = [self wrapCapturedResults:_receiver->takeResults()];
    } else {
      out = @[ ];
    }
    completion = _pendingCompletion;
    _pendingCompletion = nil;
  }
  if (completion != nil) {
    dispatch_async(dispatch_get_main_queue(), ^{
      completion(out);
    });
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
      NSLog(@"UpdateSettings error %d: %s", ret, errorMsgBuffer);
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
      NSLog(@"InitSettings error %d: %s", ret, errorMsgBuffer);
    }
    return ret;
  }
}

#pragma mark - Result wrapping

/// Wraps the asynchronous capture-round results collected by the receiver.
/// Releases every retained CDecodedBarcodesResult.
- (NSArray<NSDictionary *> *)wrapCapturedResults:
    (std::vector<CDecodedBarcodesResult *>)capturedResults {
  NSMutableArray<NSDictionary *> *results = [NSMutableArray array];
  for (CDecodedBarcodesResult *barcodes : capturedResults) {
    if (barcodes == nullptr) {
      continue;
    }
    if (barcodes->GetErrorCode()) {
      NSLog(@"Capture error %d: %s", barcodes->GetErrorCode(),
            barcodes->GetErrorString());
      NSArray<NSDictionary *> *error = [self
          wrapError:barcodes->GetErrorCode()
                msg:[NSString stringWithUTF8String:
                                  barcodes->GetErrorString() ?: ""]];
      barcodes->Release();
      return error;
    }
    int count = barcodes->GetItemsCount();
    for (int i = 0; i < count; i++) {
      NSDictionary *entry = [self wrapBarcodeItem:barcodes->GetItem(i)];
      if (entry != nil) {
        [results addObject:entry];
      }
    }
    barcodes->Release();
  }
  return results;
}

- (NSArray<NSDictionary *> *)wrapResult:(CCapturedResult *)captured {
  if (captured == nullptr) {
    return [self wrapError:-1 msg:@"No capture result"];
  }

  if (captured->GetErrorCode()) {
    NSLog(@"Capture error %d: %s", captured->GetErrorCode(),
          captured->GetErrorString());
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
    int count = barcodes->GetItemsCount();
    for (int i = 0; i < count; i++) {
      NSDictionary *entry = [self wrapBarcodeItem:barcodes->GetItem(i)];
      if (entry != nil) {
        [results addObject:entry];
      }
    }
    barcodes->Release();
  }

  captured->Release();
  return results;
}

- (NSDictionary *)wrapBarcodeItem:(const CBarcodeResultItem *)item {
  if (item == nullptr) {
    return nil;
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
  return entry;
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

/// Delivers a result on the main queue. FlutterResult callbacks must be
/// invoked on the platform thread; calling them from a background queue can
/// drop or corrupt the reply.
- (void)respondAsync:(NSArray<NSDictionary *> *)results
          completion:(void (^)(NSArray<NSDictionary *> *))completion {
  if (completion == nil) {
    return;
  }
  dispatch_async(dispatch_get_main_queue(), ^{
    completion(results);
  });
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
