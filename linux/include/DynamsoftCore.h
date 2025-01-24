#pragma once
#define DYNAMSOFT_CORE_VERSION "3.4.20.2248"

/**Enumeration section*/

/**
* @enum ErrorCode
*
* Describes the error code that can be output by the library.
*/
typedef enum ErrorCode {
	/**Successful. */
	EC_OK = 0,

	/** -10000~-19999: Common error code. */
	/**Unknown error. */
	EC_UNKNOWN = -10000,

	/**Not enough memory to perform the operation. */
	EC_NO_MEMORY = -10001,

	/**Null pointer */
	EC_NULL_POINTER = -10002,

	/**License invalid*/
	EC_LICENSE_INVALID = -10003,

	/**License expired*/
	EC_LICENSE_EXPIRED = -10004,

	/**File not found*/
	EC_FILE_NOT_FOUND = -10005,

	/**The file type is not supported. */
	EC_FILE_TYPE_NOT_SUPPORTED = -10006,

	/**The BPP (Bits Per Pixel) is not supported. */
	EC_BPP_NOT_SUPPORTED = -10007,

	/**The index is invalid.*/
	EC_INDEX_INVALID = -10008,

	/**The input region value parameter is invalid.*/
	EC_CUSTOM_REGION_INVALID = -10010,

	/**Failed to read the image. */
	EC_IMAGE_READ_FAILED = -10012,

	/**Failed to read the TIFF image. */
	EC_TIFF_READ_FAILED = -10013,

	/**The DIB (Device-Independent Bitmaps) buffer is invalid. */
	EC_DIB_BUFFER_INVALID = -10018,

	/**Failed to read the PDF image. */
	EC_PDF_READ_FAILED = -10021,

	/**The PDF DLL is missing. */
	EC_PDF_DLL_MISSING = -10022,

	/**The page number is invalid. */
	EC_PAGE_NUMBER_INVALID = -10023,

	/**The custom size is invalid. */
	EC_CUSTOM_SIZE_INVALID = -10024,

	/** timeout. */
	EC_TIMEOUT = -10026,

	/**Json parse failed*/
	EC_JSON_PARSE_FAILED = -10030,

	/**Json type invalid*/
	EC_JSON_TYPE_INVALID = -10031,

	/**Json key invalid*/
	EC_JSON_KEY_INVALID = -10032,

	/**Json value invalid*/
	EC_JSON_VALUE_INVALID = -10033,

	/**Json name key missing*/
	EC_JSON_NAME_KEY_MISSING = -10034,

	/**The value of the key "Name" is duplicated.*/
	EC_JSON_NAME_VALUE_DUPLICATED = -10035,

	/**Template name invalid*/
	EC_TEMPLATE_NAME_INVALID = -10036,

	/**The name reference is invalid.*/
	EC_JSON_NAME_REFERENCE_INVALID = -10037,

	/**Parameter value invalid*/
	EC_PARAMETER_VALUE_INVALID = -10038,

	/**The domain of your current site does not match the domain bound in the current product key.*/
	EC_DOMAIN_NOT_MATCH = -10039,

	/**The reserved info does not match the reserved info bound in the current product key.*/
	EC_RESERVED_INFO_NOT_MATCH = -10040,

	/**The license key does not match the license content.*/
	EC_LICENSE_KEY_NOT_MATCH = -10043,

	/**Failed to request the license content.*/
	EC_REQUEST_FAILED = -10044,

	/**Failed to init the license.*/
	EC_LICENSE_INIT_FAILED = -10045,

	/**Failed to set mode's argument.*/
	EC_SET_MODE_ARGUMENT_ERROR = -10051,

	/**The license content is invalid.*/
	EC_LICENSE_CONTENT_INVALID = -10052,

	/**The license key is invalid.*/
	EC_LICENSE_KEY_INVALID = -10053,

	/**The license key has no remaining quota.*/
	EC_LICENSE_DEVICE_RUNS_OUT = -10054,

	/**Failed to get mode's argument.*/
	EC_GET_MODE_ARGUMENT_ERROR = -10055,

	/**The Intermediate Result Types license is invalid.*/
	EC_IRT_LICENSE_INVALID = -10056,

	/**Failed to save file.*/
	EC_FILE_SAVE_FAILED = -10058,

	/**The stage type is invalid.*/
	EC_STAGE_TYPE_INVALID = -10059,

	/**The image orientation is invalid.*/
	EC_IMAGE_ORIENTATION_INVALID = -10060,

	/**Complex template can't be converted to simplified settings.*/
	EC_CONVERT_COMPLEX_TEMPLATE_ERROR = -10061,

	/**Reject function call while capturing in progress.*/
	EC_CALL_REJECTED_WHEN_CAPTURING = -10062,

	/**The input image source was not found.*/
	EC_NO_IMAGE_SOURCE = -10063,

	/**Failed to read directory.*/
	EC_READ_DIRECTORY_FAILED = -10064,

	/**[Name] Module not found.*/
	/**Name: */
	/**DynamsoftBarcodeReader*/
	/**DynamsoftLabelRecognizer*/
	/**DynamsoftDocumentNormalizer*/
	EC_MODULE_NOT_FOUND = -10065,
	
	/**The api does not support multi-page files. Please use FileFetcher instead.*/
	EC_MULTI_PAGES_NOT_SUPPORTED = -10066,

	/**The file already exists but overwriting is disabled.*/
	EC_FILE_ALREADY_EXISTS = -10067,

	/**The file path does not exist but cannot be created, or the file 
	cannot be created for any other reason.*/
	EC_CREATE_FILE_FAILED = -10068,

	/**The input ImageData object contains invalid parameter(s).*/
	EC_IMAGE_DATA_INVALID = -10069,

	/**The size of the input image does not meet the requirements.*/
	EC_IMAGE_SIZE_NOT_MATCH = -10070,

	/**The pixel format of the input image does not meet the requirements.*/
	EC_IMAGE_PIXEL_FORMAT_NOT_MATCH = -10071,

	/**The section level result is irreplaceable.*/
	EC_SECTION_LEVEL_RESULT_IRREPLACEABLE = -10072,

	/**The axis definition is incorrect.*/
	EC_AXIS_DEFINITION_INCORRECT = -10073,

	/**The result is not replaceable due to type mismatch.*/
	EC_RESULT_TYPE_MISMATCH_IRREPLACEABLE = -10074,

	/**Failed to load the PDF library.*/
	EC_PDF_LIBRARY_LOAD_FAILED = -10075,

	/**The license is initialized successfully but detected invalid content in your key.*/
	EC_LICENSE_WARNING = -10076,
	/** -20000~-29999: DLS license error code. */
	/**No license.*/
	EC_NO_LICENSE = -20000,

	/**The Handshake Code is invalid.*/
	EC_HANDSHAKE_CODE_INVALID = -20001,

	/**Failed to read or write license buffer. */
	EC_LICENSE_BUFFER_FAILED = -20002,

	/**Failed to synchronize license info with license server. */
	EC_LICENSE_SYNC_FAILED = -20003,

	/**Device dose not match with buffer. */
	EC_DEVICE_NOT_MATCH = -20004,

	/**Failed to bind device. */
	EC_BIND_DEVICE_FAILED = -20005,

	/**License Client dll is missing.*/
	EC_LICENSE_CLIENT_DLL_MISSING = -20007,

	/**Instance count is over limit.*/
	EC_INSTANCE_COUNT_OVER_LIMIT = -20008,

	/**Interface InitLicense has to be called before creating any SDK objects.*/
	EC_LICENSE_INIT_SEQUENCE_FAILED = -20009,

	/**Trial License*/
	EC_TRIAL_LICENSE = -20010,

	/**The license is not valid for current version*/
	EC_LICENSE_VERSION_NOT_MATCH = -20011,

	/**Online license validation failed due to network issues.Using cached license information for validation.*/
	EC_LICENSE_CACHE_USED = -20012,

	/**Failed to reach License Server.*/
	EC_FAILED_TO_REACH_DLS = -20200,


	/**-30000~-39999: DBR error code*/
	/**The barcode format is invalid.*/
	EC_BARCODE_FORMAT_INVALID = -30009,

	/**The QR Code license is invalid.*/
	EC_QR_LICENSE_INVALID = -30016,

	/**The 1D Barcode license is invalid.*/
	EC_1D_LICENSE_INVALID = -30017,

	/**The PDF417 license is invalid.*/
	EC_PDF417_LICENSE_INVALID = -30019,

	/**The DATAMATRIX license is invalid. */
	EC_DATAMATRIX_LICENSE_INVALID = -30020,

	/**The custom module size is invalid. */
	EC_CUSTOM_MODULESIZE_INVALID = -30025,

	/**The AZTEC license is invalid.*/
	EC_AZTEC_LICENSE_INVALID = -30041,

	/**The Patchcode license is invalid.*/
	EC_PATCHCODE_LICENSE_INVALID = -30046,

	/**The Postal code license is invalid.*/
	EC_POSTALCODE_LICENSE_INVALID = -30047,

	/**The DPM license is invalid.*/
	EC_DPM_LICENSE_INVALID = -30048,

	/**The frame decoding thread already exists.*/
	EC_FRAME_DECODING_THREAD_EXISTS = -30049,

	/**Failed to stop the frame decoding thread.*/
	EC_STOP_DECODING_THREAD_FAILED = -30050,

	/**The Maxicode license is invalid.*/
	EC_MAXICODE_LICENSE_INVALID = -30057,

	/**The GS1 Databar license is invalid.*/
	EC_GS1_DATABAR_LICENSE_INVALID = -30058,

	/**The GS1 Composite code license is invalid.*/
	EC_GS1_COMPOSITE_LICENSE_INVALID = -30059,

	/**The DotCode license is invalid.*/
	EC_DOTCODE_LICENSE_INVALID = -30061,

	/**The Pharmacode license is invalid.*/
	EC_PHARMACODE_LICENSE_INVALID = -30062,

	/**The barcode license is not found.*/
	EC_BARCODE_READER_LICENSE_NOT_FOUND = -30063,

	/**-40000~-49999: DLR error code*/
	/**Character Model file is not found*/
	EC_CHARACTER_MODEL_FILE_NOT_FOUND = -40100,

	/**There is a conflict in the layout of TextLineGroup. */
	EC_TEXT_LINE_GROUP_LAYOUT_CONFLICT = -40101,
	/**There is a conflict in the regex of TextLineGroup. */
	EC_TEXT_LINE_GROUP_REGEX_CONFLICT = -40102,

	/**The label recognizer license is not found.*/
	EC_LABEL_RECOGNIZER_LICENSE_NOT_FOUND = -40103,

	/**-50000~-59999: DDN error code*/
	/*The quardrilateral is invalid*/
	EC_QUADRILATERAL_INVALID = -50057,

	/**The document normalizer license is not found.*/
	EC_DOCUMENT_NORMALIZER_LICENSE_NOT_FOUND = -50058,

	/**-60000~-69999: DCE error code*/
	/**-70000~-79999: Panorama error code*/
	/**The panorama license is invalid.*/
	EC_PANORAMA_LICENSE_INVALID = -70060,

	/**-80000~-89999: Reserved error code*/
	/**-90000~-99999: DCP error code*/
	/*The resource path does not exist.*/
	EC_RESOURCE_PATH_NOT_EXIST = -90001,

	/*Failed to load resource.*/
	EC_RESOURCE_LOAD_FAILED = -90002,

	/*The code specification is not found.*/
	EC_CODE_SPECIFICATION_NOT_FOUND = -90003,

	/*The full code string is empty.*/
	EC_FULL_CODE_EMPTY = -90004,

	/*Failed to preprocess the full code string.*/
	EC_FULL_CODE_PREPROCESS_FAILED = -90005,

	/*The license for parsing South Africa Driver License is invalid.*/
	EC_ZA_DL_LICENSE_INVALID = -90006,

	/*The license for parsing North America DL/ID is invalid.*/
	EC_AAMVA_DL_ID_LICENSE_INVALID = -90007,

	/*The license for parsing Aadhaar is invalid.*/
	EC_AADHAAR_LICENSE_INVALID = -90008,

	/*The license for parsing Machine Readable Travel Documents is invalid.*/
	EC_MRTD_LICENSE_INVALID = -90009,

	/*The license for parsing Vehicle Identification Number is invalid.*/
	EC_VIN_LICENSE_INVALID = -90010,

	/*The license for parsing customized code type is invalid.*/
	EC_CUSTOMIZED_CODE_TYPE_LICENSE_INVALID = -90011,

	/**The code parser license is not found.*/
	EC_CODE_PARSER_LICENSE_NOT_FOUND = -90012

} ErrorCode;

/**
* @enum ImagePixelFormat
*
* Describes all the supported image pixel formats.
*/
typedef enum ImagePixelFormat
{
	/**0:Black, 1:White */
	IPF_BINARY,

	/**0:White, 1:Black */
	IPF_BINARYINVERTED,

	/**8bit gray */
	IPF_GRAYSCALED,

	/**NV21 */
	IPF_NV21,

	/**16bit with RGB channel order stored in memory from high to low address*/
	IPF_RGB_565,

	/**16bit with RGB channel order stored in memory from high to low address*/
	IPF_RGB_555,

	/**24bit with RGB channel order stored in memory from high to low address*/
	IPF_RGB_888,

	/**32bit with ARGB channel order stored in memory from high to low address*/
	IPF_ARGB_8888,

	/**48bit with RGB channel order stored in memory from high to low address*/
	IPF_RGB_161616,

	/**64bit with ARGB channel order stored in memory from high to low address*/
	IPF_ARGB_16161616,

	/**32bit with ABGR channel order stored in memory from high to low address*/
	IPF_ABGR_8888,

	/**64bit with ABGR channel order stored in memory from high to low address*/
	IPF_ABGR_16161616,

	/**24bit with BGR channel order stored in memory from high to low address*/
	IPF_BGR_888,

	/**0:Black, 255:White */
	IPF_BINARY_8,	

	/**NV12 */
	IPF_NV12,

	/**0:White, 255:Black */
	IPF_BINARY_8_INVERTED

} ImagePixelFormat;

/**
* @enum GrayscaleTransformationMode
*
* Describes the grayscale transformation mode.
*/
typedef enum GrayscaleTransformationMode
{
	/**Transforms to inverted grayscale. Recommended for light on dark images. */
	GTM_INVERTED = 0x01,

	/**Keeps the original grayscale. Recommended for dark on light images. */
	GTM_ORIGINAL = 0x02,

	/**Lets the library choose an algorithm automatically for grayscale transformation.*/
	GTM_AUTO = 0x04,

	/**Reserved setting for grayscale transformation mode.*/
#if defined(_WIN32) || defined(_WIN64)
	GTM_REV = 0x80000000,
#else
	GTM_REV = -2147483648,
#endif

	/**Skips grayscale transformation. */
	GTM_SKIP = 0x00

} GrayscaleTransformationMode;

/**
* @enum GrayscaleEnhancementMode
*
* Describes the grayscaleEnhancementMode.
*/
typedef enum GrayscaleEnhancementMode
{
	/**Not supported yet. */
	GEM_AUTO = 0x01,

	/**Takes the unpreprocessed image for following operations. */
	GEM_GENERAL = 0x02,

	/**Preprocesses the image using the gray equalization algorithm. Check @ref IPM for available argument settings.*/
	GEM_GRAY_EQUALIZE = 0x04,

	/**Preprocesses the image using the gray smoothing algorithm. Check @ref IPM for available argument settings.*/
	GEM_GRAY_SMOOTH = 0x08,

	/**Preprocesses the image using the sharpening and smoothing algorithm. Check @ref IPM for available argument settings.*/
	GEM_SHARPEN_SMOOTH = 0x10,

	/**Reserved setting for image preprocessing mode.*/
#if defined(_WIN32) || defined(_WIN64)
	GEM_REV = 0x80000000,
#else
	GEM_REV = -2147483648,
#endif

	/**Skips image preprocessing. */
	GEM_SKIP = 0x00
} GrayscaleEnhancementMode;

/**
* @enum PDFReadingMode
*
* Describes the PDF reading mode.
*/
typedef enum PDFReadingMode
{
	/** Outputs vector data found in the PDFs.*/
	PDFRM_VECTOR = 0x01,

	/**The default value.
	 * Outputs raster data found in the PDFs.
	 * Depending on the argument Resolution,
	 * the SDK may rasterize the PDF pages.
	 * Check the template for available argument settings.*/
	PDFRM_RASTER = 0x02,

	/**Reserved setting for PDF reading mode.*/
#if defined(_WIN32) || defined(_WIN64)
	PDFRM_REV = 0x80000000,
#else
	PDFRM_REV = -2147483648,
#endif
} PDFReadingMode;

/**
* @enum RasterDataSource
*
* Describes the raster data sources.
*/
typedef enum RasterDataSource
{
	/**The target type of the PDF file is "page". Only available for PDFReadingMode PDFRM_RASTER.*/
	RDS_RASTERIZED_PAGES,

	/**The target type of the PDF file is "image".*/
	RDS_EXTRACTED_IMAGES
} RasterDataSource;

/**
* @enum CapturedResultItemType
*
* Describes all types of captured result item.
*/
typedef enum CapturedResultItemType
{
	/** The type of the CapturedResultItem is "original image". */
	CRIT_ORIGINAL_IMAGE = 1,

	/** The type of the CapturedResultItem is "barcode". */
	CRIT_BARCODE = 2,

	/** The type of the CapturedResultItem is "text line". */
	CRIT_TEXT_LINE = 4,

	/** The type of the CapturedResultItem is "detected quad". */
	CRIT_DETECTED_QUAD = 8,

	/** The type of the CapturedResultItem is "normalized image". */
	CRIT_NORMALIZED_IMAGE = 16,

	/** The type of the CapturedResultItem is "parsed result". */
	CRIT_PARSED_RESULT = 32
} CapturedResultItemType;

/**
* @enum BufferOverflowProtectionMode
*
* Describes the protection modes when the buffer of ImageSourceAdapter is overflow.
*/
typedef enum BufferOverflowProtectionMode 
{
	/** New images are blocked when the buffer is full. */
	BOPM_BLOCK = 0x00,

	/** New images are appended at the end, and the oldest images are pushed out from the beginning if the buffer is full. */
	BOPM_UPDATE = 0x01,

} BufferOverflowProtectionMode;

/**
* @enum ImageTagType
*
* Describes the type of the image tag, which is used to distinguish video frame and file images.
*/
typedef enum ImageTagType 
{
	/**The image is a file image.*/
	ITT_FILE_IMAGE,

	/**The image is a video frame.*/
	ITT_VIDEO_FRAME
} ImageTagType;

/**
* @enum VideoFrameQuality
*
* Describes the quality of video frames.
*/
typedef enum VideoFrameQuality 
{
	/**The frame quality is measured to be high.*/
	VFQ_HIGH,

	/**The frame quality is measured to be low.*/
	VFQ_LOW,

	/**The frame quality is unknown.*/
	VFQ_UNKNOWN
} VideoFrameQuality;

/**
* @enum CornerType
*
* Describes how the corner is formed by its sides.
*/
typedef enum CornerType 
{
	/**The sides of the corner is normally intersected.*/
	CT_NORMAL_INTERSECTED = 0,

	/**The sides of the corner is T-intersected.*/
	CT_T_INTERSECTED = 1,

	/**The sides of the corner is cross-intersected.*/
	CT_CROSS_INTERSECTED = 2,

	/**The sides are not intersected but they definitely make up a corner.*/
	CT_NOT_INTERSECTED = 3
} CornerType;

/**
* @enum SectionType
*
* Describes the section of the algorithm. 
* In the IntermediateResultReceiver, the SectionType indicate the algorithm section that produced the IntermediateResult.
*/
typedef enum SectionType
{
	ST_NULL,

	/**The result is output by "region prediction" section.*/
	ST_REGION_PREDETECTION,

	/**The result is output by "barcode localization" section.*/
	ST_BARCODE_LOCALIZATION,

	/**The result is output by "barcode decoding" section.*/
	ST_BARCODE_DECODING,

	/**The result is output by "text line localization" section.*/
	ST_TEXT_LINE_LOCALIZATION,

	/**The result is output by "text line recognition" section.*/
	ST_TEXT_LINE_RECOGNITION,

	/**The result is output by "document detection" section.*/
	ST_DOCUMENT_DETECTION,

	/**The result is output by "document normalization" section.*/
	ST_DOCUMENT_NORMALIZATION,
} SectionType;

/**
* @enum IntermediateResultUnitType
*
* IntermediateResultUnitType is used in each subclass of IntermediateResult to indicate the type of the result. 
* It is also used to declare which kinds IntermediateResult should be output by the library.
*/
enum IntermediateResultUnitType : unsigned long long
{
	/**No IntermediateResult type is specified.*/
	IRUT_NULL = 0,

	/**The type of the IntermediateResult is "colour image".*/
	IRUT_COLOUR_IMAGE = 1,

	/**The type of the IntermediateResult is "scaled down colour image".*/
	IRUT_SCALED_DOWN_COLOUR_IMAGE = 1 << 1,

	/**The type of the IntermediateResult is "grayscale image".*/
	IRUT_GRAYSCALE_IMAGE = 1 << 2,

	/**The type of the IntermediateResult is "transformed grayscale image".*/
	IRUT_TRANSFORMED_GRAYSCALE_IMAGE = 1 << 3,

	/**The type of the IntermediateResult is "enhanced grayscale image".*/
	IRUT_ENHANCED_GRAYSCALE_IMAGE = 1 << 4,

	/**The type of the IntermediateResult is "predected regions".*/
	IRUT_PREDETECTED_REGIONS = 1 << 5,

	/**The type of the IntermediateResult is "binary image".*/
	IRUT_BINARY_IMAGE = 1 << 6,

	/**The type of the IntermediateResult is "texture detection result".*/
	IRUT_TEXTURE_DETECTION_RESULT = 1 << 7,

	/**The type of the IntermediateResult is "texture-removed grayscale image".*/
	IRUT_TEXTURE_REMOVED_GRAYSCALE_IMAGE = 1 << 8,

	/**The type of the IntermediateResult is "texture-removed binary image".*/
	IRUT_TEXTURE_REMOVED_BINARY_IMAGE = 1 << 9,

	/**The type of the IntermediateResult is "contours".*/
	IRUT_CONTOURS = 1 << 10,

	/**The type of the IntermediateResult is "line segments".*/
	IRUT_LINE_SEGMENTS = 1 << 11,

	/**The type of the IntermediateResult is "text zones".*/
	IRUT_TEXT_ZONES = 1 << 12,

	/**The type of the IntermediateResult is "text removed binary image".*/
	IRUT_TEXT_REMOVED_BINARY_IMAGE = 1 << 13,

	/**The type of the IntermediateResult is "candidate barcode zones".*/
	IRUT_CANDIDATE_BARCODE_ZONES = 1 << 14,

	/**The type of the IntermediateResult is "localized barcodes".*/
	IRUT_LOCALIZED_BARCODES = 1 << 15,

	/**The type of the IntermediateResult is "scale up barcode image".*/
	IRUT_SCALED_UP_BARCODE_IMAGE = 1 << 16,

	/**The type of the IntermediateResult is "deformation resisted barcode image".*/
	IRUT_DEFORMATION_RESISTED_BARCODE_IMAGE = 1 << 17,

	/**The type of the IntermediateResult is "complemented barcode image".*/
	IRUT_COMPLEMENTED_BARCODE_IMAGE = 1 << 18,

	/**The type of the IntermediateResult is "decoded barcodes".*/
	IRUT_DECODED_BARCODES = 1 << 19,

	/**The type of the IntermediateResult is "long lines".*/
	IRUT_LONG_LINES = 1 << 20,

	/**The type of the IntermediateResult is "corners".*/
	IRUT_CORNERS = 1 << 21,

	/**The type of the IntermediateResult is "candidate quad edges".*/
	IRUT_CANDIDATE_QUAD_EDGES = 1 << 22,

	/**The type of the IntermediateResult is "detected quads".*/
	IRUT_DETECTED_QUADS = 1 << 23,

	/**The type of the IntermediateResult is "localized text lines".*/
	IRUT_LOCALIZED_TEXT_LINES = 1 << 24,

	/**The type of the IntermediateResult is "recognized text lines".*/
	IRUT_RECOGNIZED_TEXT_LINES = 1 << 25,

	/**The type of the IntermediateResult is "normalized image".*/
	IRUT_NORMALIZED_IMAGES = 1 << 26,

	/**The type of the IntermediateResult is "short lines".*/
	IRUT_SHORT_LINES = 1 << 27,

	/**The type of the IntermediateResult is "text line groups".*/
	IRUT_RAW_TEXT_LINES = 1LL << 28,

	/**The type of the IntermediateResult is "all".*/
	IRUT_ALL = 0xFFFFFFFFFFFFFFFF
};

/**
* @enum RegionObjectElementType
*
* RegionObjectElementType is used to determine the particular type of the subclassed of RegionObjectElement.
*/
typedef enum RegionObjectElementType
{
	/**The type of subclass PredetectedRegionElement.*/
	ROET_PREDETECTED_REGION,

	/**The type of subclass LocalizedBarcodeElement.*/
	ROET_LOCALIZED_BARCODE,

	/**The type of subclass DecodedBarcodeElement.*/
	ROET_DECODED_BARCODE,

	/**The type of subclass LocalizedTextLineElement.*/
	ROET_LOCALIZED_TEXT_LINE,

	/**The type of subclass RecognizedTextLineElement.*/
	ROET_RECOGNIZED_TEXT_LINE,

	/**The type of subclass DetectedQuadElement.*/
	ROET_DETECTED_QUAD,

	/**The type of subclass NormalizedImageElement.*/
	ROET_NORMALIZED_IMAGE,

	/**The type of subclass SourceImageElement.*/
	ROET_SOURCE_IMAGE,

	/**The type of subclass TargetROIElement.*/
	ROET_TARGET_ROI,
} RegionObjectElementType;

/**
* @enum ImageCaptureDistanceMode
*
* Describes the shooting mode of the image. It is used in the overlap mode of Panorama.
*/
typedef enum ImageCaptureDistanceMode
{
	/**The image is taken by a camera from a close distance.*/
	ICDM_NEAR,

	/**The image is taken by a camera from a distance.*/
	ICDM_FAR
} ImageCaptureDistanceMode;

/**
* @enum ColourChannelUsageType
*
* Describes how the color channel is used in the image.
*/
typedef enum ColourChannelUsageType
{
	/**Automatic color channel usage determination based on image pixel format and scene.*/
	CCUT_AUTO,

	/**Use all available color channels for processing.*/
	CCUT_FULL_CHANNEL,

	/**Use only the Y(luminance) channel for processing in images represented in the NV21 format.*/
	CCUT_Y_CHANNEL_ONLY,

	/**Use only the red channel for processing in RGB images.*/
	CCUT_RGB_R_CHANNEL_ONLY,

	/**Use only the green channel for processing in RGB images.*/
	CCUT_RGB_G_CHANNEL_ONLY,

	/**Use only the blue channel for processing in RGB images.*/
	CCUT_RGB_B_CHANNEL_ONLY

}ColourChannelUsageType;

/**
* @enum TransformMatrixType
*
* Describes the type of the transformation matrix.
*/
typedef enum TransformMatrixType
{
	/**Represents a transformation matrix that converts coordinates from the local image to the original image.*/
	TMT_LOCAL_TO_ORIGINAL_IMAGE,

	/**Represents a transformation matrix that converts coordinates from the original image to the local image.*/
	TMT_ORIGINAL_TO_LOCAL_IMAGE,

	/**Represents a transformation matrix that converts coordinates from the rotated image to the original image.*/
	TMT_ROTATED_TO_ORIGINAL_IMAGE,

	/**Represents a transformation matrix that converts coordinates from the original image to the rotated image.*/
	TMT_ORIGINAL_TO_ROTATED_IMAGE

}TransformMatrixType;

/**Structures section*/
#pragma pack(push)
#pragma pack(1)

/**
* @struct IntermediateResultExtraInfo
*
* The IntermediateResultExtraInfo structure represents the extra information for generating an intermediate result unit.
*/
typedef struct IntermediateResultExtraInfo
{
	/**Specifies the name of the TargetROIDef object that generates the intermediate result.*/
	const char* targetROIDefName;

	/**Specifies the name of the task that generates the intermediate result.*/
	const char* taskName;

	/**Specifies whether the intermediate result is section-level result.*/
	bool isSectionLevelResult;

	/**Specifies the SectionType that generates the intermediate result.*/
	SectionType sectionType;

	/**Reserved for future use.*/
	char reserved[64];
}IntermediateResultExtraInfo;

#pragma pack(pop)


#if !defined(_WIN32) && !defined(_WIN64)
#define DS_API __attribute__((visibility("default")))
#include <stddef.h> 
#else
#if defined(ANDROID) || defined(__APPLE__) || defined(__linux__) || ((defined(RELEASE_LIB) || defined(DEBUG_LIB)) && !defined(MULTI_MODULE_FLAG))
#define DS_API
#else
#ifdef DS_EXPORTS
#define DS_API __declspec(dllexport)
#else
#define DS_API __declspec(dllimport)
#endif
#endif
#include <windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif	
	/**
	 * Gets error message by error code.
	 *
	 * @param [in] errorCode The error code.
	 *
	 * @return Returns the error message.
	 *
	 */
	DS_API  const char* DC_GetErrorString(int errorCode);

	/**
	 * Calculates the square root of a number.
	 *
	 * @param [in] x A non-negative number whose square root is to be computed.
	 *
	 * @return Returns the square root of the given argument.
	 *
	 */
	DS_API  double DC_Sqrt(double x);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace dynamsoft
{
	namespace basic_structures
	{

#pragma pack(push)
#pragma pack(1)

		/**
		 * A 3x3 matrix that represents an identity matrix.
		 */
		const double IDENTITY_MATRIX[9] = { 
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0 };

		/**
		 * A function pointer to free bytes.
		 *
		 */
		typedef void(*FreeBytesFunc)(const unsigned char*);

		/**
		 * The CCoreModule class defines general functions in the core module.
		 */
		class DS_API CCoreModule
		{
		public:
			/**
			 * Returns the version of the core module.
			 *
			 * @return Returns a const char pointer representing the version of the core module.
			 */
			static const char* GetVersion();
		};

		/**
		 * The DMPoint_ class template represents a point in 2D space.
		 */
		template<typename _Tp>
		class DMPoint_
		{
		public:
			/**
			* Constructor
			*/
			DMPoint_(void) {
				coordinate[0] = 0;
				coordinate[1] = 0;
			};

			/**
			* Constructor
			*
			* @param [in] _x The x coordinate of a point.
			* @param [in] _y The y coordinate of a point.
			*
			*/
			DMPoint_(_Tp _x, _Tp _y)
			{
				this->coordinate[0] = _x;
				this->coordinate[1] = _y;
			}

			/**
			* Constructor
			*
			* @param [in] pt The given point.
			*
			*/
			DMPoint_(const DMPoint_ &pt)
			{
				this->coordinate[0] = pt.coordinate[0];
				this->coordinate[1] = pt.coordinate[1];
			}

			/**
			* Destructor.
			*/
			~DMPoint_(void) {};

		public:
			/**
			* Sets the coordinates of a point.
			*
			* @param [in] _x The x coordinate of a point.
			* @param [in] _y The y coordinate of a point.
			*
			*/
			void Set(_Tp _x, _Tp _y)
			{
				this->coordinate[0] = _x;
				this->coordinate[1] = _y;
			}

			DMPoint_& operator=(const DMPoint_&pt)
			{
				this->coordinate[0] = pt.coordinate[0];
				this->coordinate[1] = pt.coordinate[1];
				return *this;
			}

			/**
			* Gets the element at the specified index in the DMPoint_.
			*
			* @param [in] i An integer index used to access the element of the DMPoint_.
			*
			* @return Returns a reference to the element at the specified index in the DMPoint_.
			*
			*/
			const _Tp& operator[](int i) const
			{
				return this->coordinate[i];
			}

			/**
			* Gets the element at the specified index in the DMPoint_.
			*
			* @param [in] i An integer index used to access the element of the DMPoint_.
			*
			* @return Returns a reference to the element at the specified index in the DMPoint_.
			*
			*/
			_Tp& operator[](int i)
			{
				return this->coordinate[i];
			}

			template<typename _Tp2> operator DMPoint_<_Tp2>() const
			{
				return DMPoint_<_Tp2>((_Tp2)(this->coordinate[0]), (_Tp2)(this->coordinate[1]));
			}

			/**
			* Calculates the distance between the current point and the specified target point.
			*
			* @param [in] pt The target point to which the distance is calculated.
			*
			* @return Returns a floating-point value representing the distance between the current point and the specified target point.
			*
			*/
			double DistanceTo(const DMPoint_&pt) const
			{
				double a = double(pt[0] - this->coordinate[0])*double(pt[0] - this->coordinate[0]);
				double b = double(pt[1] - this->coordinate[1])*double(pt[1] - this->coordinate[1]);
				return DC_Sqrt(a + b);
			}

			/**
			* Transforms the coordinates of a point using a given transformation matrix.
			*
			* @param [in] originalPoint The original point to transform.
			* @param [in] transformationMatrix The transformation matrix to apply to the coordinates.
			*
			* @return Returns a new point with the transformed coordinates.
			*
			*/
			static DMPoint_ TransformCoordinates(const DMPoint_& originalPoint, const double transformationMatrix[9])
			{
				DMPoint_<_Tp> dst;
				double w = originalPoint.coordinate[0] * transformationMatrix[6] + originalPoint.coordinate[1] * transformationMatrix[7] + transformationMatrix[8];
				w = 1. / w;
				dst.coordinate[0] = static_cast<_Tp>((originalPoint.coordinate[0] * transformationMatrix[0] +
					originalPoint.coordinate[1] * transformationMatrix[1] + transformationMatrix[2])*w);
				dst.coordinate[1] = static_cast<_Tp>((originalPoint.coordinate[0] * transformationMatrix[3] +
					originalPoint.coordinate[1] * transformationMatrix[4] + transformationMatrix[5])*w);
				return dst;
			}

		private:
			/**
			* The coordinate array of the point.
			*/
			_Tp coordinate[2];
		};

		/**
		 * The CPoint class represents a point in 2D space. It contains an array of two integers, which represent the coordinates of the point.
		 */
		//template class DS_API DMPoint_<int>;
		typedef DMPoint_<int> CPoint;

		/**
		 * A function pointer to free points.
		 *
		 */
		typedef void(*FreePointsFunc)(const CPoint*);

		/**
		* The CContour class represents a contour in 2D space. It contains an array of CPoint objects, which represent the vertices of the contour.
		*/
		class DS_API CContour
		{
		protected:
			/**
			* The number of points in the contour.
			*/
			int pointsCount;

			/**
			* The point array of the contour. The memory will be released by CContour.
			*/
			const CPoint* points;

			FreePointsFunc freePointsFunc;

		public:
			/**
			* Destructor. It releases the memory of the point array.
			*/
			~CContour();

			/**
			* Constructor
			*/
			CContour();

			/**
			 * Copy constructor for CContour.
			 *
			 * @param contour The reference to another CContour object.
			*/
			CContour(const CContour& contour);

			/**
			 * Move constructor for CContour.
			 *
			 * @param contour The rvalue reference to another CContour object.
			 */
			CContour(CContour&& contour);

			/**
			 * Copy assignment operator for CContour.
			 *
			 * @param contour The reference to another CContour object.
			 * @return A reference to the copied CContour object.
			 */
			CContour& operator=(const CContour& contour);

			/**
			 * Move assignment operator for CContour.
			 *
			 * @param barcode The rvalue reference to another CContour object.
			 * @return A reference to the moved CContour object.
			 */
			CContour& operator=(CContour&& contour);

			/**
			 * Sets the point array and the freePointsFunc function pointer.
			 *
			 * @param pointsCount The number of points in the point array.
			 * @param points The point array.
			 * @param freePointsFunc The freePointsFunc function pointer.
			 */
			void SetPoints(int count, const CPoint* points, FreePointsFunc freePointsFunc);

			/**
			 * Gets the number of points in the point array.
			 *
			 * @return The number of points in the point array.
			 */
			int GetPointsCount() const;

			/**
			 * Gets the point array.
			 *
			 * @return The point array.
			 */
			const CPoint* GetPoints() const;

		};

		/**
		* The CVector4 class represents a four-dimensional vector.
		*/
		class DS_API CVector4
		{
		public:
			int value[4];

			/**
			* Constructor
			*/
			CVector4(void);

			/**
			* Constructor
			*
			* @param [in] v1 The first component value of the four-dimensional vector.
			* @param [in] v2 The second component value of the four-dimensional vector.
			* @param [in] v3 The third component value of the four-dimensional vector.
			* @param [in] v4 The fourth component value of the four-dimensional vector.
			*
			*/
			CVector4(int v1, int v2, int v3, int v4);

			/**
			* Constructor
			*
			* @param [in] v The given CVector4 object.
			*
			*/
			CVector4(const CVector4& v);

			/**
			* Destructor.
			*/
			~CVector4(void);

			/**
			* Sets the components value of a four-dimensional vector.
			*
			* @param [in] v1 The first component value of the four-dimensional vector.
			* @param [in] v2 The second component value of the four-dimensional vector.
			* @param [in] v3 The third component value of the four-dimensional vector.
			* @param [in] v4 The fourth component value of the four-dimensional vector.
			*
			*/
			void Set(int v1, int v2, int v3, int v4);

			/**
			 * Copy assignment operator for CVector4.
			 *
			 * @param v The reference to another CVector4 object.
			 * @return A reference to the copied CVector4 object.
			 */
			CVector4& operator=(const CVector4& v);

			/**
			* Gets the component value at the specified index in the CVector4.
			*
			* @param [in] i An integer index used to access the component value of the CVector4.
			*
			* @return Returns the component value at the specified index in the CVector4.
			*
			*/
			int operator[](int i) const;

			/**
			* Gets the component value at the specified index in the CVector4.
			*
			* @param [in] i An integer index used to access the component value of the CVector4.
			*
			* @return Returns a reference to the component value at the specified index in the CVector4.
			*
			*/
			int& operator[](int i);
		};

		/**
		* The CLineSegment class represents a line segment in 2D space. It contains two CPoint objects, which represent the start point and end point of the line segment.
		*/
		class DS_API CLineSegment
		{
		protected:
			/**
			* The start point of the line segment.
			*/
			CPoint startPoint;

			/**
			* The end point of the line segment.
			*/
			CPoint endPoint;

			virtual void Init();

		public:
			/**
			* Constructor
			*/
			CLineSegment();

			/**
			* Constructor
			*
			* @param [in] p1 The start point of the line segment.
			* @param [in] p2 The end point of the line segment.
			*
			*/
			CLineSegment(const CPoint& p1, const CPoint& p2);

			/**
			* Destructor.
			*/
			virtual ~CLineSegment();

			/**
			 * Copies the CLineSegment object.
			 *
			 * @param line The CLineSegment object to be copied.
			 * @return The copied CLineSegment object.
			 */
			CLineSegment& operator = (const CLineSegment& line);

			/**
			* Gets the start point of the line segment.
			*
			* @return Returns the start point of the line segment.
			*
			*/
			const CPoint& GetStartPoint() const { return startPoint; }

			/**
			* Gets the end point of the line segment.
			*
			* @return Returns the end point of the line segment.
			*
			*/
			const CPoint& GetEndPoint() const { return endPoint; }

			/**
			* Sets the start point of the line segment.
			*
			* @param [in] pt The start point of the line segment.
			*
			*/
			void SetStartPoint(const CPoint& pt);

			/**
			* Sets the end point of the line segment.
			*
			* @param [in] pt The end point of the line segment.
			*
			*/
			void SetEndPoint(const CPoint& pt);
		};

		/**
		* CCorner is a structure in an image consisting of two line segments and intersection point. A Corner represents a point at which the image's brightness or color sharply changes.
		*/
		class DS_API CCorner
		{
		public:
			/**
			* The type of the corner.
			*/
			CornerType type;

			/**
			* The intersection point of the corner.
			*/
			CPoint intersection;

			/**
			* The first line connected to the corner.
			*/
			CLineSegment line1;

			/**
			* The second line connected to the corner.
			*/
			CLineSegment line2;
		};

		/**
		* CEdge is a structure composed of two Corner points in an image. 
		* A Corner represents a point at which the image's brightness or color sharply changes. Therefore, a CEdge is a line segment connecting two such points that have been identified as Corners.
		*/
		class DS_API CEdge
		{
		public:
			/**
			* The start corner point of the edge.
			*/
			CCorner startCorner;

			/**
			* The end corner point of the edge.
			*/
			CCorner endCorner;
		};

		/**
		* The CQuadrilateral class represents a quadrilateral shape in 2D space. It contains an array of four CPoint objects, which represent the vertices of the quadrilateral.
		*/
		class DS_API CQuadrilateral
		{
		public:
			/**
			* The point array of the quadrilateral.
			*/
			CPoint points[4];

			/**
			* Determines whether a point is inside the quadrilateral.
			*
			* @param [in] point The point to test.
			*
			* @return Returns true if the point inside the quadrilateral, false otherwise.
			*
			*/
			bool Contains(const CPoint* point) const;

			/**
			* Gets the area of the quadrilateral.
			*
			* @return Returns the area of the quadrilateral.
			*
			*/
			int GetArea() const;
		};

		/**
		* The CRect class represents a rectangle in 2D space. It contains four integer values that specify the top, left, right, and bottom edges of the rectangle.
		*/
		class DS_API CRect
		{
		public:
			/**
			* The top edge of the rectangle.
			*/
			int top;

			/**
			* The left edge of the rectangle.
			*/
			int left;

			/**
			* The right edge of the rectangle.
			*/
			int right;

			/**
			* The bottom edge of the rectangle.
			*/
			int bottom;
		};

		/**
		* The CImageTag class represents an image tag that can be attached to an image in a system. It contains information about the image, such as the image ID and the image capture distance mode.
		*/
		class DS_API CImageTag 
		{
		private:
			int imageId;
			ImageCaptureDistanceMode mode;
		protected:
			CImageTag();
		public:
			/**
			* Destructor
			*/
			virtual ~CImageTag();

			/**
			* Gets the type of the image tag.
			*
			* @return Returns the type of the image tag.
			*
			*/
			virtual ImageTagType GetType() const = 0;

			/**
			* Creates a copy of the image tag.
			*
			* @return Returns a pointer to a copy of the image tag.
			*
			*/
			virtual CImageTag* Clone() const = 0;

			/**
			* Gets the ID of the image.
			*
			* @return Returns the ID of the image.
			*
			*/
			int GetImageId()const;

			/**
			* Sets the ID of the image.
			*
			* @param [in] imgId The ID of the image.
			*
			*/
			void SetImageId(int imgId);

			/**
			* Gets the capture distance mode of the image.
			*
			* @return Returns the capture distance mode of the image.
			*
			*/
			ImageCaptureDistanceMode GetImageCaptureDistanceMode()const;

			/**
			* Sets the capture distance mode of the image.
			*
			* @param [in] mode The capture distance mode of the image.
			*
			*/
			void SetImageCaptureDistanceMode(ImageCaptureDistanceMode mode);
		};

		/**
		* The CFileImageTag class represents an image tag that is associated with a file. It inherits from the CImageTag class and adds two attributes, a file path and a page number.
		*/
		class DS_API CFileImageTag : public CImageTag 
		{
		public:
			/**
			* The constructor of the CFileImageTag class.
			*
			* @param [in] _filePath The file path.
			* @param [in] _pageNumber The page number of the file image.
			* @param [in] _totalPages The total pages of the file image.
			*
			*/
			CFileImageTag(const char* _filePath, int _pageNumber, int _totalPages);

			/**
			* The destructor of the CFileImageTag class. It frees the memory allocated for the file path.
			*/
			~CFileImageTag();

			/**
			* Gets the type of the image tag.
			*
			* @return Returns the type of the image tag.
			*
			*/
			ImageTagType GetType() const override;

			/**
			* Creates a copy of the image tag.
			*
			* @return Returns a pointer to a copy of the image tag.
			*
			*/
			CImageTag* Clone() const override;

			/**
			* Gets the file path of the image tag.
			*
			* @return Returns the file path of the image tag.
			*
			*/
			const char* GetFilePath()const;

			/**
			* Gets the page number of the image tag.
			*
			* @return Returns the page number of the image tag.
			*
			*/
			int GetPageNumber()const;
			/**
			* Gets the total pages of the image tag.
			*
			* @return Returns the total pages of the image tag.
			*
			*/
			int GetTotalPages()const;

		protected:
			const char* filePath;
			int pageNumber;
			int totalPages;
		private:
			CFileImageTag(const CFileImageTag&);
			CFileImageTag& operator=(const CFileImageTag&);
		};

		/**
		* The CVideoFrameTag class represents a video frame tag, which is a type of image tag that is used to store additional information about a video frame. It inherits from the CImageTag class and adds additional attributes and methods specific to video frames.
		*/
		class DS_API CVideoFrameTag :public CImageTag
		{
		protected:
			VideoFrameQuality quality;
			bool isCropped;
			CRect* cropRegion;
			int originalWidth;
			int originalHeight;

		public:
			/**
			* Gets the quality of the video frame.
			*
			* @return Returns the quality of the video frame.
			*
			*/
			VideoFrameQuality GetVideoFrameQuality()const;

			/**
			* Determines whether the video frame is cropped.
			*
			* @return Returns true if the video frame is cropped, false otherwise.
			*
			*/
			bool IsCropped()const;

			/**
			* Gets the crop region of the video frame.
			*
			* @return Returns a pointer to a CRect object that represents the crop region of the video frame. It may be NULL.
			*
			*/
			const CRect* GetCropRegion()const;

			/**
			* Gets the original width of the video frame.
			*
			* @return Returns the original width of the video frame.
			*
			*/
			int GetOriginalWidth()const;

			/**
			* Gets the original height of the video frame.
			*
			* @return Returns the original height of the video frame.
			*
			*/
			int GetOriginalHeight()const;

			/**
			* Gets the type of the image tag.
			*
			* @return Returns the type of the image tag.
			*
			*/
			ImageTagType GetType()const override;

			/**
			* Creates a copy of the image tag.
			*
			* @return Returns a pointer to a copy of the image tag.
			*
			*/
			CImageTag* Clone()const override;

			/**
			* The constructor of the CVideoFrameTag class.
			*
			* @param [in] quality The quality of the video frame.
			* @param [in] isCropped A boolean value indicating whether the video frame is cropped.
			* @param [in] cropRegion A pointer to a CRect object that represents the crop region of the video frame.
			* @param [in] originalWidth The original width of the video frame.
			* @param [in] originalHeight The original height of the video frame.
			*
			*/
			CVideoFrameTag(VideoFrameQuality quality, bool isCropped, const CRect* cropRegion,
				int originalWidth, int originalHeight);

			/**
			* The destructor of the CVideoFrameTag class.
			*/
			virtual ~CVideoFrameTag();

		};

		/**
		* The CImageData class represents image data, which contains the image bytes, width, height, stride, pixel format, orientation and a tag.
		*/
		class DS_API CImageData
		{
		protected:
			unsigned long long bytesLength;
			const unsigned char* bytes;
			int width;
			int height;
			int stride;
			ImagePixelFormat format;
			int orientation;
			CImageTag* tag;
			FreeBytesFunc freeBytesFunc;
		public:
			/**
			* Constructs an empty image data object.
			*/
			CImageData();

			/**
			* Constructs an image data object with the specified parameters.
			*
			* @param [in] _bytesLength The length of the image byte array.
			* @param [in] _bytes The image byte array.
			* @param [in] _width The width of the image.
			* @param [in] _height The height of the image.
			* @param [in] _stride The stride of the image.
			* @param [in] _format The pixel format of the image.
			* @param [in] _orientation The orientation of the image.
			* @param [in] _tag The tag of the image.
			*
			*/
			CImageData(unsigned long long _bytesLength, const unsigned char* _bytes, int _width, int _height, int _stride,
				ImagePixelFormat _format, int _orientation = 0, const CImageTag* _tag = NULL);

			/**
			* Constructs an image data object with the specified parameters.
			*
			* @param [in] _bytesLength The length of the image byte array.
			* @param [in] _bytes The image byte array.
			* @param [in] _freeBytesFunc The function to free the image byte array.
			* @param [in] _width The width of the image.
			* @param [in] _height The height of the image.
			* @param [in] _stride The stride of the image.
			* @param [in] _format The pixel format of the image.
			* @param [in] _orientation The orientation of the image.
			* @param [in] _tag The tag of the image.
			*
			*/
			CImageData(unsigned long long _bytesLength, const unsigned char* _bytes, FreeBytesFunc _freeBytesFunc, int _width, int _height, int _stride,
				ImagePixelFormat _format, int _orientation = 0, const CImageTag* _tag = NULL);

			/**
			* Destructs the image data object and frees the allocated memory.
			*/
			virtual ~CImageData();

			/**
			* Gets the image byte array.
			*
			* @return Returns a pointer to the image byte array.
			*
			*/
			const unsigned char* GetBytes() const;

			/**
			* Gets the length of the image byte array.
			*
			* @return Returns the length of the image byte array.
			*
			*/
			unsigned long long GetBytesLength() const;

			/**
			* Gets the width of the image.
			*
			* @return Returns the width of the image.
			*
			*/
			int GetWidth() const;

			/**
			* Gets the height of the image.
			*
			* @return Returns the height of the image.
			*
			*/
			int GetHeight() const;

			/**
			* Gets the stride of the image.
			*
			* @return Returns the stride of the image.
			*
			*/
			int GetStride() const;

			/**
			* Gets the pixel format of the image.
			*
			* @return Returns the pixel format of the image.
			*
			*/
			ImagePixelFormat GetImagePixelFormat() const;

			/**
			* Gets the orientation of the image.
			*
			* @return Returns the orientation of the image.
			*
			*/
			int GetOrientation() const;

			/**
			* Gets the tag of the image.
			*
			* @return Returns a pointer to the tag of the image.
			*
			*/
			const CImageTag* GetImageTag()const;

			/**
			* Sets the tag of the image.
			*
			* @param [in] _tag The tag of the image.
			*
			*/
			void SetImageTag(const CImageTag* _tag);
			
		private:
			//CImageData(const CImageData&);
			//CImageData& operator=(const CImageData&);
		};

		/**
		* The CCapturedResultItem class represents an item in a captured result. It is an abstract base class with multiple subclasses, each representing a different type of captured item such as barcode, text line, detected quad, normalized image, original image, parsed item, etc.
		*/
		class DS_API CCapturedResultItem
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CCapturedResultItem() {};

		public:
			/**
			* Gets the type of the captured result item.
			*
			* @return Returns the type of the captured result item.
			*
			*/
			virtual CapturedResultItemType GetType() const = 0;

			/**
			* Gets a pointer to the referenced item in the captured result item.
			*
			* @return Returns a pointer to the referenced item in the captured result item. You are not required to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CCapturedResultItem* GetReferenceItem() const = 0;

			/**
			 * Gets the name of the target ROI definition.
			 *
			 * @return Returns the name of the target ROI definition.
			 */
			virtual const char* GetTargetROIDefName() const = 0;

			/**
			* Gets the name of the task.
			*
			* @return Returns the name of the task.
			*
			*/
			virtual const char* GetTaskName() const = 0;

			/**
			 * Increases the reference count of the CCapturedResultItem object.
			 *
			 * @return An object of CCapturedResultItem.
			 */
			virtual CCapturedResultItem* Retain() = 0;
			
			/**
			* Decreases the reference count of the CCapturedResultItem object.
			*
			*/
			virtual void Release() = 0;

			/**
			 * Clone the captured result item.
			 *
			 * @return Returns a pointer to a copy of the captured result item.
			 */
			virtual CCapturedResultItem* Clone() const = 0;
		};

		/**
		* The `COriginalImageResultItem` class represents a captured original image result item. It is a derived class of `CCapturedResultItem` and provides an interface to get the image data.
		*/
		class DS_API COriginalImageResultItem : public CCapturedResultItem
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~COriginalImageResultItem() {};

		public:
			/**
			* Gets the image data for the COriginalImageResultItem.
			*
			* @return Returns a const pointer to the CImageData object that contains the image data for the COriginalImageResultItem.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;
		};

		/**
		* The CImageSourceErrorListener class is an abstract base class for receiving error notifications from an image source.
		*
		* Subclasses of this class can be implemented to handle specific error conditions that may occur during image source operations.
		* Error information, such as error code and error maessage, is passed to the "OnErrorReceived" method when an error occurs.
		*/
		class DS_API CImageSourceErrorListener {
		public:

			/**
			* Called when an error is received from the image source.
			*
			* @param [in] errorCode The integer error code indicating the type of error.
			* @param [in] errorMessage A C-style string containing the error message providing additional information about the error.
			*
			*/
			virtual void OnErrorReceived(int errorCode, const char* errorMessage) = 0;
		};

		/**
		* The CImageSourceAdapter class provides an interface for fetching and buffering images. It is an abstract class that needs to be implemented by a concrete class to provide actual functionality.
		*/
		class DS_API CImageSourceAdapter 
		{
		private:
			class CImageSourceAdapterInner;
			CImageSourceAdapter(const CImageSourceAdapter&);
			CImageSourceAdapter& operator=(const CImageSourceAdapter&);
			CImageSourceAdapterInner* m_inner;
		protected:
			CImageSourceErrorListener* m_listener;

			/**
			* Constructor
			*/
			CImageSourceAdapter();
			
			/**
			* Adds an image to the buffer of the adapter.
			*
			* @param [in] img The image to add to the buffer.
			* @param [in] bClone Whether the image should be cloned before being added to the buffer.
			*
			*/
			void AddImageToBuffer(const CImageData* img, bool bClone = true);

		public:
			/**
			* Destructor
			*/
			virtual ~CImageSourceAdapter();

			/**
			* Determines whether there are more images left to fetch.
			*
			* @return Returns true if there are more images left to fetch, false otherwise. This function must be implemented in the subclass.
			*
			*/
			virtual bool HasNextImageToFetch()const = 0;
			
			/**
			* Starts fetching images.
			*/
			virtual void StartFetching();

			/**
			* Stops fetching images.
			*/
			virtual void StopFetching();
			
			/**
			* Returns a buffered image.
			*
			* @return Returns a pointer to the image if it exists in the buffer, NULL otherwise.
			*
			*/
			virtual CImageData* GetImage();
			
			/**
			* Sets how many images are allowed to be buffered.
			*
			* @param [in] count The maximum number of images that can be buffered.
			*
			*/
			void SetMaxImageCount(int count);

			/**
			* Returns how many images can be buffered.
			*
			* @return Returns the maximum number of images that can be buffered.
			*
			*/
			int GetMaxImageCount()const;

			/**
			* Sets a mode that determines the action to take when there is a new incoming image and the buffer is full.
			*
			* @param [in] mode The buffer overflow protection mode to set.
			*
			*/
			void SetBufferOverflowProtectionMode(BufferOverflowProtectionMode mode);

			/**
			* Returns the current buffer overflow protection mode.
			*
			* @return Returns the current buffer overflow protection mode.
			*
			*/
			BufferOverflowProtectionMode GetBufferOverflowProtectionMode() const;

			/**
			* Determines whether the image is in the buffer or not.
			*
			* @param [in] imageId The ID of the image to check.
			*
			* @return Returns true if the image is in the buffer, false otherwise.
			*
			*/
			bool HasImage(int imageId)const;
			
			/**
			* Sets the next image to return.
			*
			* @param [in] imageId The ID of the next image to return.
			* @param [in] keepInBuffer Whether the image should be kept in the buffer after it is returned.
			*
			* @return Returns true if the image is in the buffer and is set as the next image to return, false otherwise.
			*
			*/
			bool SetNextImageToReturn(int imageId, bool keepInBuffer = true);

			/**
			* Returns the actual count of buffered images.
			*
			* @return Returns the actual count of buffered images.
			*
			*/
			int GetImageCount() const;

			/**
			* Determines whether the buffer is empty.
			*
			* @return Returns true if the buffer is empty, false otherwise.
			*
			*/
			bool IsBufferEmpty() const;

			/** 
			 * Clears the buffer.
			 */
			void ClearBuffer();
			
			/**
			 * Sets the usage type of a color channel in images.
			 */
			void SetColourChannelUsageType(ColourChannelUsageType type);

			/**
			* Gets the usage type of a color channel in images.
			*
			* @return Returns the usage type of a color channel in images.
			*
			*/
			ColourChannelUsageType GetColourChannelUsageType()const;

			/**
			* Sets the error listener for the image source. This function allows you to set an error listener object that will receive notifications when errors occur during image source operations.
			* If an error occurs, the error infomation will be passed to the listener's OnErrorReceived method.
			*
			* @param [in] listener A pointer to an instance of CImageSourceErrorListener or its derived class, which will handle error notifications.
			*
			*/
			void SetErrorListener(CImageSourceErrorListener* listener);
		};

		/**
		* The CPDFReadingParameter class represents the parameters for reading a PDF file. It contains the mode of PDF reading, the DPI (dots per inch) value, and the tarGetstype.
		*/
		class DS_API CPDFReadingParameter {
		public:
			/**
			* The mode of PDF reading.
			*/
			PDFReadingMode mode;

			/**
			* The DPI (dots per inch) value.
			*/
			int dpi;

			/**
			* The raster data source.
			*/
			RasterDataSource rasterDataSource;
			
			CPDFReadingParameter() {
				mode = PDFRM_RASTER;
				dpi = 300;
				rasterDataSource = RDS_RASTERIZED_PAGES;
			}
		};

		
#pragma pack(pop)
	}

	namespace intermediate_results {
		using namespace basic_structures;

		/**
		* The CRegionObjectElement class represents an element of a region object in 2D space. It is an abstract class that provides the interface for region object elements.
		*
		*/
		class DS_API CRegionObjectElement
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CRegionObjectElement() {};

		public:
			/**
			* Get the location of the region object element.
			*
			* @return Returns a CQuadrilateral object which represents the location of the region object element.
			*
			*/
			virtual CQuadrilateral GetLocation() const = 0;

			/**
			* Get a pointer to a referenced region object element.
			*
			* @return Returns a const pointer to a referenced CRegionObjectElement object.
			*
			*/
			virtual const CRegionObjectElement* GetReferencedElement() const = 0;

			/**
			* Get the type of the region object element.
			*
			* @return Returns a RegionObjectElementType enum value which represents the type of the region object element.
			*
			*/
			virtual RegionObjectElementType GetElementType() const = 0;

			/**
			 * Set the location of the region object element.
			 *
			 * @param location The location of the region object element.
			 * @return Returns 0 if success, otherwise an error code.
			 */
			virtual int SetLocation(const CQuadrilateral& location) = 0;

			/**
			 * Clone the region object element.
			 *
			 * @return Returns a pointer to a copy of the region object element.
			 */
			virtual CRegionObjectElement* Clone() const = 0;

			/**
			 * Increases the reference count of the CRegionObjectElement object.
			 *
			 * @return An object of CRegionObjectElement.
			 */
			virtual CRegionObjectElement* Retain() = 0;

			/**
			* Decreases the reference count of the CRegionObjectElement object.
			*
			*/
			virtual void Release() = 0;
		};

		/**
		* The CPredetectedRegionElement class represents a region element that has been pre-detected in an image. It is a subclass of the CRegionObjectElement.
		*
		*/
		class DS_API CPredetectedRegionElement : public CRegionObjectElement
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CPredetectedRegionElement() {};

		public:
			/**
			* Gets the name of the detection mode used to detect this region element.
			*
			* @return Returns the name of the detection mode used to detect this region element.
			*
			*/
			virtual const char* GetModeName() const = 0;
		};

		/**
		* The CIntermediateResultUnit class represents an intermediate result unit used in image processing. It is an abstract base class with multiple subclasses, each representing a different type of unit such as pre-detected regions, localized barcodes, decoded barcodes, localized text lines, binary image, gray image, etc.
		*/
		class DS_API CIntermediateResultUnit
		{
		protected:
			const char* hashId;
			const char* originalImageHashId;
			CImageTag* originalImageTag;
			double localToOriginalMatrix[9];
			double rotatedToOriginalMatrix[9];

			/**
			* Constructor
			*/
			CIntermediateResultUnit();
		
			/**
			* Destructor
			*/
			virtual ~CIntermediateResultUnit();

		public:
			/**
			* Gets the hash ID of the intermediate result unit.
			*
			* @return Returns the hash ID of the unit.
			*
			*/
			const char* GetHashId() const;

			/**
			* Gets the hash ID of the original image.
			*
			* @return Returns the hash ID of the original image.
			*
			*/
			const char* GetOriginalImageHashId() const;

			/**
			* Gets the image tag of the original image.
			*
			* @return Returns the image tag of the original image.
			*
			*/
			const CImageTag* GetOriginalImageTag() const;

			/**
			* Gets the transformation matrix via TransformMatrixType.
			*
			* @param [in] matrixType The transform matrix type.
			* @param [out] matrix A double array which represents the transform matrix. The corresponding transformation matrices are as follows: local image to original image, original image to local image, rotated image to original image, original image to rotated image.
			*
			*/
			void GetTransformMatrix(TransformMatrixType matrixType, double matrix[9]) const;

			/**
			* Sets the transformation matrix via TransformMatrixType.
			*
			* @param [in] matrixType The transform matrix type.
			* @param [in] matrix A double array which represents the transform matrix. The corresponding transformation matrices are as follows: local image to original image, original image to local image, rotated image to original image, original image to rotated image.
			*
			*/
			void SetTransformMatrix(TransformMatrixType matrixType, const double matrix[9]);

			/**
			* Gets the type of the intermediate result unit.
			*
			* @return Returns the type of the intermediate result unit.
			*
			*/
			virtual IntermediateResultUnitType GetType() const = 0;

			/**
			* Creates a copy of the intermediate result unit.
			*
			* @return Returns a copy of the intermediate result unit.
			*
			*/
			virtual CIntermediateResultUnit* Clone() const = 0;

			/**
			* Sets the hash ID of the intermediate result unit.
			*
			* @param [in] _hashId The hash ID to set.
			*
			*/
			void SetHashId(const char* _hashId);

			/**
			* Sets the hash ID of the original image.
			*
			* @param [in] _originalImageHashId The hash ID to set.
			*
			*/
			void SetOriginalImageHashId(const char* _originalImageHashId);

			/**
			* Sets the image tag of the original image.
			*
			* @param [in] _tag The image tag to set.
			*
			*/
			void SetOriginalImageTag(const CImageTag* _tag);

			/**
			 * Increases the reference count of the CIntermediateResultUnit object.
			 *
			 * @return An object of CIntermediateResultUnit.
			 */
			virtual CIntermediateResultUnit* Retain() = 0;

			/**
			* Decreases the reference count of the CIntermediateResultUnit object.
			*
			*/
			virtual void Release() = 0;

			/**
			 * Replaces the specified CIntermediateResultUnit object with the current CIntermediateResultUnit object.
			 *
			 * @param unit The CIntermediateResultUnit object to be replaced.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int Replace(CIntermediateResultUnit* unit) = 0;
		};

		/**
		* The CIntermediateResult class represents a container containing a collection of CIntermediateResultUnit objects.
		*
		*/
		class DS_API CIntermediateResult
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CIntermediateResult() {};

		public:
			/**
			* Gets the number of CIntermediateResultUnit objects in the collection.
			*
			* @return Returns the number of CIntermediateResultUnit objects in the collection.
			*
			*/
			virtual int GetCount() const = 0;

			/**
			* Gets a pointer to a specific CIntermediateResultUnit object in the collection.
			*
			* @param [in] index The index of the CIntermediateResultUnit object to retrieve.
			*
			* @return Returns a const pointer to the specified CIntermediateResultUnit object in the collection. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CIntermediateResultUnit* GetIntermediateResultUnit(int index) const = 0;
		};

		/**
		* The `CColourImageUnit` class represents a unit that contains color image. It is derived from the `CIntermediateResultUnit` class.
		*
		*/
		class DS_API CColourImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CColourImageUnit() {};

		public:
			/**
			* Gets the image data of the color image unit.
			*
			* @return Returns a pointer to the CImageData object that contains the image data of the color image unit. You are not required to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the image data of the color image unit.
			 *
			 * @param imgData The image data to set.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CScaledDownColourImageUnit class represents an intermediate result unit that contains scaled down color image. It is derived from the CIntermediateResultUnit class.
		*/
		class DS_API CScaledDownColourImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CScaledDownColourImageUnit() {};

		public:
			/**
			* Gets the image data of the unit.
			*
			* @return Returns a pointer to the CImageData object that contains the image data of the unit. You are not required to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the image data of the scaled down color image unit.
			 *
			 * @param imgData The image data to set.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CGrayscaleImageUnit class represents a grayscale image unit. It is a subclass of CIntermediateResultUnit.
		*/
		class DS_API CGrayscaleImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CGrayscaleImageUnit() {};

		public:
			/**
			* Gets the image data of the grayscale image.
			*
			* @return Returns a pointer to the CImageData object that contains the grayscale image. You are not required to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the image data of the grayscale image unit.
			 *
			 * @param imgData The image data to set.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CTransformedGrayscaleImageUnit class is a subclass of CIntermediateResultUnit that represents a transformed grayscale image. It may be the original grayscale image or the inverted image of the original grayscale image.
		*
		*/
		class DS_API CTransformedGrayscaleImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTransformedGrayscaleImageUnit() {};

		public:
			/**
			* Gets the image data of the transformed grayscale image. It may be the original grayscale image or the inverted image of the original grayscale image.
			*
			* @return Returns a const pointer to a CImageData object that represents the image data of the transformed grayscale image. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the image data of the transformed grayscale image unit.
			 *
			 * @param imgData The image data to set.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CPredetectedRegionsUnit class represents a unit that contains a collection of pre-detected regions. It inherits from the CIntermediateResultUnit class and stores the result of image color pre-detection.
		*/
		class DS_API CPredetectedRegionsUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CPredetectedRegionsUnit() {};

		public:
			/**
			* Gets the number of pre-detected regions in the collection.
			*
			* @return Returns the number of pre-detected regions in the collection.
			*
			*/
			virtual int GetCount() const = 0;

			/**
			* Gets a pointer to a specific pre-detected region in the collection.
			*
			* @param [in] index The index of the pre-detected region to retrieve.
			*
			* @return Returns a const pointer to the specified pre-detected region in the collection. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CPredetectedRegionElement* GetPredetectedRegion(int index) const = 0;

			/**
			* Gets a pointer to a specific pre-detected region in the collection.
			*
			* @param [in] index The index of the pre-detected region to retrieve.
			*
			* @return Returns a const pointer to the specified pre-detected region in the collection. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CPredetectedRegionElement* operator[](int index) const = 0;

			/**
			 * Removes all pre-detected regions in the unit.
			 *
			 */
			virtual void RemoveAllPredetectedRegions() = 0;

			/**
			 * Removes a pre-detected region in the unit at the specified index.
			 *
			 * @param index The index of the pre-detected region to remove.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int RemovePredetectedRegion(int index) = 0;

			/**
			 * Adds a pre-detected region in the unit.
			 *
			 * @param element The pre-detected region to add.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int AddPredetectedRegion(const CPredetectedRegionElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

			/**
			 * Sets a pre-detected region in the unit at the specified index.
			 *
			 * @param index The index of the pre-detected region to set.
			 * @param element The pre-detected region to set.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetPredetectedRegion(int index, const CPredetectedRegionElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
		};

		/**
		* The CEnhancedGrayscaleImageUnit class represents an intermediate result unit that contains an enhanced grayscale image data. Gray enhancement methods include gray equalization, gray smoothing, gray sharpening and smoothing.
		*/
		class DS_API CEnhancedGrayscaleImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CEnhancedGrayscaleImageUnit() {};

		public:
			/**
			* Gets the enhanced grayscale image data.
			*
			* @return Returns a const pointer to the CImageData object that contains the enhanced grayscale image data. You don't need to release the memory pointed to by the returned pointer.
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the enhanced grayscale image data.
			 *
			 * @param imgData The enhanced grayscale image data.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CBinaryImageUnit class represents a binary image unit that inherits from CIntermediateResultUnit. It inherits from the CIntermediateResultUnit class and stores binary image data.
		*
		*/
		class DS_API CBinaryImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CBinaryImageUnit() {};

		public:
			/**
			* Gets a pointer to the binary image data.
			*
			* @return Returns a pointer to the CImageData object containing the binary image data. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the binary image data.
			 *
			 * @param imgData The binary image data.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The CTextureDetectionResultUnit class represents an intermediate result unit for texture detection. It is derived from the CIntermediateResultUnit class and contains the x-direction spacing and y-direction spacing of the texture stripes.
		*
		*/
		class DS_API CTextureDetectionResultUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTextureDetectionResultUnit() {};

		public:
			/**
			* Gets x-direction spacing of the texture stripes.
			*
			* @return Returns the x-direction spacing of the texture stripes.
			*
			*/
			virtual int GetXSpacing() = 0;

			/**
			* Gets y-direction spacing of the texture stripes.
			*
			* @return Returns the y-direction spacing of the texture stripes.
			*
			*/
			virtual int GetYSpacing() = 0;

			/**
			 * Sets the x-direction spacing of the texture stripes.
			 *
			 * @param xSpacing The x-direction spacing of the texture stripes.
			 */
			virtual void SetXSpacing(int xSpacing) = 0;

			/**
			 * Sets the y-direction spacing of the texture stripes.
			 *
			 * @param ySpacing The y-direction spacing of the texture stripes.
			 */
			virtual void SetYSpacing(int ySpacing) = 0;
		};

		/**
		* The CTextureRemovedGrayscaleImageUnit class represents an intermediate result unit that contains grayscale image data with textures removed.
		*
		*/
		class DS_API CTextureRemovedGrayscaleImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTextureRemovedGrayscaleImageUnit() {};

		public:
			/**
			* Gets the grayscale image data with textures removed.
			*
			* @return Returns a constant pointer to the grayscale image data with textures removed. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the grayscale image data with textures removed.
			 *
			 * @param imgData The grayscale image data with textures removed.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		/**
		* The `CTextureRemovedBinaryImageUnit` class represents an intermediate result unit that stores binary image data with texture removed.
		*
		*/
		class DS_API CTextureRemovedBinaryImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTextureRemovedBinaryImageUnit() {};

		public:
			/**
			* Gets the image data of the binary image with texture removed.
			*
			* @return Returns a const pointer to CImageData object that represents the binary image with texture removed. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the binary image data with texture removed.
			 *
			 * @param imgData The binary image data with texture removed.
			 * @return Returns 0 if succeeds, nonzero otherwise.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};

		class DS_API CTextZone
		{
		private:
			CQuadrilateral location;
			int charContoursCount;
			int* charContoursIndices;

		public:
			/**
			 * Destructor
			 *
			 */
			~CTextZone();

			/**
			 * Constructor
			 *
			 */
			CTextZone();

			/**
			 * Constructor
			 *
			 * @param loc The location of the text zone
			 */
			CTextZone(const CQuadrilateral& loc);

			/**
			 * Constructor
			 *
			 * @param loc The location of the text zone
			 * @param charContoursIndices The indices of the character contours
			 * @param charContoursCount The count of the character contours
			 */
			CTextZone(const CQuadrilateral& loc, const int charContoursIndices[], int charContoursCount);

			/**
			 * Gets the location of the text zone
			 *
			 * @return Returns the location of the text zone
			 */
			CQuadrilateral GetLocation() const;

			/**
			 * Sets the location of the text zone
			 *
			 * @param loc The location of the text zone
			 */
			void SetLocation(const CQuadrilateral& loc);

			/**
			 * Gets the indices of the character contours
			 *
			 * @param ppCharContoursIndices The indices of the character contours
			 * @param pCharContoursCount The count of the character contours
			 */
			void GetCharContoursIndices(int** ppCharContoursIndices, int* pCharContoursCount) const;

			/**
			 * Sets the indices of the character contours
			 *
			 * @param charContoursIndices The indices of the character contours
			 * @param charContoursCount The count of the character contours
			 */
			void SetCharContoursIndices(const int charContoursIndices[], int charContoursCount);
		};

		/**
		* The `CTextZonesUnit` class represents a unit that contains text zones. It is derived from `CIntermediateResultUnit` class and provides methods to retrieve the count and details of text zones.
		*
		*/
		class DS_API CTextZonesUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTextZonesUnit() {};

		public:
			/**
			* Gets the number of text zones in the unit.
			*
			* @return Returns the number of text zones in the unit.
			*
			*/
			virtual int GetCount() const = 0;

			/**
			* Gets the quadrilateral shape of the text zone at the specified index.
			*
			* @param [in] index The index of the text zone.
			* @param [in, out] textZone A pointer to a CTextZone object to receive  the text zone.
			*
			* @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			*
			*/
			virtual int GetTextZone(int index, CTextZone* textZone) const = 0;

			/**
			* Removes all text zones from the unit.
			*
			*/
			virtual void RemoveAllTextZones() = 0;

			/**
			 * Removes the text zone at the specified index.
			 *
			 * @param index The index of the text zone to remove.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int RemoveTextZone(int index) = 0;

			/**
			 * Adds a text zone to the unit.
			 *
			 * @param textZone The text zone to add.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int AddTextZone(const CTextZone& textZone, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

			/**
			 * Sets the text zone at the specified index.
			 *
			 * @param index The index of the text zone to set.
			 * @param textZone The text zone to set.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int SetTextZone(int index, const CTextZone& textZone, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
		};

		/**
		* The CTextRemovedBinaryImageUnit class represents an intermediate result unit that contains a binary image with the text removed.
		*/
		class DS_API CTextRemovedBinaryImageUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CTextRemovedBinaryImageUnit() {};

		public:
			/**
			* Gets the binary image data with the text removed.
			*
			* @return Returns a pointer to the CImageData object that contains the binary image data with the text removed. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Sets the binary image data with the text removed.
			 *
			 * @param imgData The binary image data with the text removed.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int SetImageData(const CImageData* imgData) = 0;
		};


		/**
		* The CContoursUnit class represents a unit that contains contours as intermediate results. It is derived from the CIntermediateResultUnit class.
		*/
		class DS_API CContoursUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CContoursUnit() {};

		public:
			/**
			 * Gets the contours and hierarchies in the unit.
			 *
			 * @param count The number of contours in the unit.
			 * @param contours The contours in the unit.
			 * @param hierarchies The hierarchies in the unit.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int GetContours(int* count, const CContour** contours, const CVector4** hierarchies) const = 0;

			/**
			 * Sets the contours and hierarchies in the unit.
			 *
			 * @param count The number of contours in the unit.
			 * @param contours The contours in the unit.
			 * @param hierarchies The hierarchies in the unit.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int SetContours(int count, const CContour* contours, const CVector4* hierarchies, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
		};

		/**
		* The `CLineSegmentsUnit` class represents a collection of line segments in 2D space. It is a derived class of `CIntermediateResultUnit`.
		*
		*/
		class DS_API CLineSegmentsUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CLineSegmentsUnit() {};

		public:
			/**
			* Gets the number of line segments in the collection.
			*
			* @return Returns the number of line segments in the collection.
			*
			*/
			virtual int GetCount() const = 0;

			/**
			 * Gets the line segment at the specified index.
			 *
			 * @param index The index of the line segment to get.
			 * @return A pointer to the CLineSegment object that represents the line segment at the specified index.
			 */
			virtual const CLineSegment* GetLineSegment(int index) const = 0;

			/**
			 * Gets the line segment at the specified index.
			 *
			 * @param index The index of the line segment to get.
			 * @return A pointer to the CLineSegment object that represents the line segment at the specified index.
			 */
			virtual const CLineSegment* operator[](int index) const = 0;

			/**
			 * Removes all line segments from the unit.
			 *
			 */
			virtual void RemoveAllLineSegments() = 0;

			/**
			 * Removes the line segment at the specified index.
			 *
			 * @param index The index of the line segment to remove.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int RemoveLineSegment(int index) = 0;

			/**
			 * Adds a line segment to the unit.
			 *
			 * @param line The line segment to add.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int AddLineSegment(const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

			/**
			 * Sets the line segment at the specified index.
			 *
			 * @param index The index of the line segment to set.
			 * @param line The line segment to set.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int SetLineSegment(int index, const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
		};

		/**
		* The `CShortLinesUnit` class represents a collection of short lines in 2D space. It is a derived class of `CIntermediateResultUnit`.
		*
		*/
		class DS_API CShortLinesUnit : public CIntermediateResultUnit
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CShortLinesUnit() {};

		public:
			/**
			* Gets the number of short lines in the collection.
			*
			* @return Returns the number of short lines in the collection.
			*
			*/
			virtual int GetCount() const = 0;

			/**
			 * Gets the short line at the specified index.
			 *
			 * @param index The index of the short line to get.
			 * @return A pointer to the CLineSegment object that represents the short line at the specified index.
			 */
			virtual const CLineSegment* GetShortLine(int index) const = 0;

			/**
			 * Gets the short line at the specified index.
			 *
			 * @param index The index of the short line to get.
			 * @return A pointer to the CLineSegment object that represents the short line at the specified index.
			 */
			virtual const CLineSegment* operator[](int index) const = 0;

			/**
			 * Removes all short lines from the unit.
			 *
			 */
			virtual void RemoveAllShortLines() = 0;

			/**
			 * Removes the short line at the specified index.
			 *
			 * @param index The index of the short line to remove.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int RemoveShortLine(int index) = 0;

			/**
			 * Adds a short line to the unit.
			 *
			 * @param line The short line to add.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int AddShortLine(const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

			/**
			 * Sets the short line at the specified index.
			 *
			 * @param index The index of the short line to set.
			 * @param line The short line to set.
			 * @param matrixToOriginalImage The matrix to original image.
			 * @return Returns 0 if the operation succeeds, or a nonzero error code if the operation fails.
			 */
			virtual int SetShortLine(int index, const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
		};

		/**
		* The `CObservationParameters` class is used to set filter conditions for the CIntermediateReusltReceiver, so that only intermediate results meeting specific conditions will be called back.
		*
		*/
		class DS_API CObservationParameters
		{
		public:
			/**
			* Destructor
			*/
			virtual ~CObservationParameters() {};

			/**
			* Sets the types of intermediate result units that have been observed.
			*
			* @param [in] types The observed types of intermediate result units.
			*
			*/
			virtual void SetObservedResultUnitTypes(unsigned long long types) = 0;

			/**
			* Gets the types of intermediate result units that have been observed.
			*
			* @return Returns the observed types of intermediate result units.
			*
			*/
			virtual unsigned long long GetObservedResultUnitTypes() const = 0;

			/**
			* Determines whether the specified result unit type was observed.
			*
			* @param [in] type The specified result unit type.
			*
			* @return Returns a boolean value indicating whether the specified result unit type was observed.
			*
			*/
			virtual bool IsResultUnitTypeObserved(IntermediateResultUnitType type) const = 0;

			/**
			* Adds observed task name to be notified when relevant results are available.
			*
			* @param [in] taskName The specified task name.
			*
			*/
			virtual void AddObservedTask(const char* taskName) = 0;

			/**
			* Removes the observed task name so that intermediate results generated by the task are not notified.
			*
			* @param [in] taskName The specified task name.
			*
			*/
			virtual void RemoveObservedTask(const char* taskName) = 0;

			/**
			* Determines whether the specified task was observed.
			*
			* @param [in] taskName The specified task name.
			*
			* @return Returns a boolean value indicating whether the specified task was observed.
			*
			*/
			virtual bool IsTaskObserved(const char* taskName) const = 0;

			/**
			* Set the type of intermediate result unit that indicates skipping default calculations and replacing with input data units.
			*
			* @param types The type of intermediate result unit that serves as the combination value of IntermediateResultUnitType.
			*/
			virtual void SetResultUnitTypesOnlyForInput(unsigned long long types) = 0;

			/**
			* Gets the type of intermediate result unit that indicates skipping default calculations and replacing with input data units.
			*
			* @return Returns the type of intermediate result unit that serves as the combination value of IntermediateResultUnitType.
			*/
			virtual unsigned long long GetResultUnitTypesOnlyForInput() const = 0;

			/**
			* Determines whether the specified type of intermediate result unit indicates skipping default calculations and replacing with input data units.
			*
			* @param type The type of intermediate result unit to check.
			* @return Returns a boolean value indicating whether the specified type of intermediate result unit indicates skipping default calculations and replacing with input data units.
			*/
			virtual bool IsResultUnitTypeOnlyForInput(IntermediateResultUnitType type) const = 0;
		};

		/**
		* The `CAbstractIntermediateResultReceiver` class is responsible for receiving intermediate results of different types.
		* It provides virtual functions for each type of result, which are called when the corresponding result is received.
		*/
		class DS_API CAbstractIntermediateResultReceiver
		{
		protected:
			CObservationParameters* parameters;

			/**
			* Constructor
			*/
			CAbstractIntermediateResultReceiver();

		public:
			/**
			* Destructor
			*/
			virtual ~CAbstractIntermediateResultReceiver();

			/**
			* Gets the observed parameters of the intermediate result receiver.
			*
			* @return Returns the object of CObservationParameters. The default parameters are to observe all intermediate result unit types and all tasks.
			*
			*/
			CObservationParameters* GetObservationParameters();

			/**
			* Called when a task result has been received.
			*
			* @param [in] pResult A pointer to the CIntermediateResult object that contains several result units.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTaskResultsReceived(CIntermediateResult *pResult, const IntermediateResultExtraInfo* info) = 0;

			/**
			* Called when a intermediate result unit has been received.
			*
			* @param [in] pUnit A pointer to the CIntermediateResultUnit object.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnUnitResultReceived(CIntermediateResultUnit *pUnit, const IntermediateResultExtraInfo* info) = 0;

			/**
			* Called by function modules when a task result has been received.
			*
			* @param [in] pResult A pointer to the CIntermediateResult object that contains several result units.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			* @remark It is for internal calls of function modules such as DynamsoftBarcodeReader, DynamsoftLabelRecognizer and DynamsoftDocumentNormalizer.
			*/
			virtual void OnTaskResultsReceivedInner(CIntermediateResult *pResult, const IntermediateResultExtraInfo* info) = 0;
		};
	}
}

#endif
