#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Objective-C bridge between the Swift plugin and the Dynamsoft
/// C++ CaptureVisionRouter SDK.
///
/// The Dynamsoft desktop SDK exposes a C++ API which cannot be called
/// directly from Swift, so this ObjC++ class wraps it and returns
/// Foundation objects that Flutter can encode over the method channel.
@interface BarcodeManagerBridge : NSObject

/// Creates the CaptureVisionRouter instance and wires up the async
/// capture pipeline (file fetcher, result receiver, state listener).
/// Returns 0 on success, a negative value on failure.
- (int)initSdk;

/// Sets the Dynamsoft license key.
/// Returns 0 on success, or an error code on failure.
- (int)setLicense:(NSString *)license;

/// Decodes barcodes from an image file.
/// Returns an array of dictionaries with keys:
/// format, text, x1..y4, angle, barcodeBytes, errorCode, errorMsg.
- (NSArray<NSDictionary *> *)decodeFile:(NSString *)path;

/// Decodes barcodes from a raw image buffer asynchronously.
///
/// Mirrors the Windows/Linux implementations: the buffer is fed to a
/// CFileFetcher input source and CCaptureVisionRouter::StartCapturing is
/// invoked. The completion block is invoked on the main queue once the
/// SDK signals that the source is exhausted, with the same dictionary
/// format as [decodeFile:]. This keeps the decode call non-blocking in
/// video stream scenarios.
- (void)decodeImageBuffer:(NSData *)bytes
                    width:(int)width
                   height:(int)height
                   stride:(int)stride
                   format:(int)format
                 rotation:(int)rotation
               completion:(void (^)(NSArray<NSDictionary *> *results))completion;

/// Sets the barcode formats to be detected.
/// Returns 0 on success, or an error code on failure.
- (int)setBarcodeFormats:(unsigned long long)formats;

/// Retrieves the current settings as a JSON string.
- (NSString *)getParameters;

/// Applies settings from a JSON string.
/// Returns 0 on success, or an error code on failure.
- (int)setParameters:(NSString *)params;

/// Releases the underlying SDK instance.
- (void)dispose;

@end

NS_ASSUME_NONNULL_END
