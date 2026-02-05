#pragma once

#if !defined(_WIN32) && !defined(_WIN64)
#define DBR_API __attribute__((visibility("default")))
#if !defined(__APPLE__)
typedef signed char BOOL;
#endif
typedef void* HANDLE;
#include <stddef.h>
#else //windows
#if defined(DBR_EXPORTS)
#define DBR_API __declspec(dllexport)
#else
#define DBR_API __declspec(dllimport)
#endif
#include <windows.h>
#endif

#include "DynamsoftCore.h"

#define DBR_VERSION "11.2.50.6558"

/**Enumeration section*/

/**
* @enum BarcodeFormat
*
* Describes the supported barcode formats.
*/
enum BarcodeFormat : unsigned long long
{
	/**No barcode format in BarcodeFormat*/
	BF_NULL = 0x00,

	/**All supported formats in BarcodeFormat*/
	BF_ALL = 0xFFFFFFFEFFFFFFFF,

	/**Use the default barcode format settings*/
	BF_DEFAULT = 0xFE3BFFFF,

	/**Combined value of BF_CODABAR, BF_CODE_128, BF_CODE_39, BF_CODE_39_Extended, BF_CODE_93, BF_EAN_13, BF_EAN_8, INDUSTRIAL_25, BF_ITF, BF_UPC_A, BF_UPC_E, BF_MSI_CODE, BF_CODE_11;  */
	BF_ONED = 0x003007FF,

	/**Combined value of BF_GS1_DATABAR_OMNIDIRECTIONAL, BF_GS1_DATABAR_TRUNCATED, BF_GS1_DATABAR_STACKED, BF_GS1_DATABAR_STACKED_OMNIDIRECTIONAL, BF_GS1_DATABAR_EXPANDED, BF_GS1_DATABAR_EXPANDED_STACKED, BF_GS1_DATABAR_LIMITED*/
	BF_GS1_DATABAR = 0x0003F800,

	/**Code 39 */
	BF_CODE_39 = 0x1,

	/**Code 128 */
	BF_CODE_128 = 0x2,

	/**Code 93 */
	BF_CODE_93 = 0x4,

	/**Codabar */
	BF_CODABAR = 0x8,

	/**Interleaved 2 of 5 */
	BF_ITF = 0x10,

	/**EAN-13 */
	BF_EAN_13 = 0x20,

	/**EAN-8 */
	BF_EAN_8 = 0x40,

	/**UPC-A */
	BF_UPC_A = 0x80,

	/**UPC-E */
	BF_UPC_E = 0x100,

	/**Industrial 2 of 5 */
	BF_INDUSTRIAL_25 = 0x200,

	/**CODE39 Extended */
	BF_CODE_39_EXTENDED = 0x400,

	/**GS1 Databar Omnidirectional*/
	BF_GS1_DATABAR_OMNIDIRECTIONAL = 0x800,

	/**GS1 Databar Truncated*/
	BF_GS1_DATABAR_TRUNCATED = 0x1000,

	/**GS1 Databar Stacked*/
	BF_GS1_DATABAR_STACKED = 0x2000,

	/**GS1 Databar Stacked Omnidirectional*/
	BF_GS1_DATABAR_STACKED_OMNIDIRECTIONAL = 0x4000,

	/**GS1 Databar Expanded*/
	BF_GS1_DATABAR_EXPANDED = 0x8000,

	/**GS1 Databar Expaned Stacked*/
	BF_GS1_DATABAR_EXPANDED_STACKED = 0x10000,

	/**GS1 Databar Limited*/
	BF_GS1_DATABAR_LIMITED = 0x20000,

	/**Patch code*/
	BF_PATCHCODE = 0x00040000,

	/**Code 32*/
	BF_CODE_32 = 0x1000000,

	/**PDF417 */
	BF_PDF417 = 0x02000000,

	/**QRCode */
	BF_QR_CODE = 0x04000000,

	/**DataMatrix */
	BF_DATAMATRIX = 0x08000000,

	/**AZTEC */
	BF_AZTEC = 0x10000000,

	/**MAXICODE */
	BF_MAXICODE = 0x20000000,

	/**Micro QR Code*/
	BF_MICRO_QR = 0x40000000,

	/**Micro PDF417*/
	BF_MICRO_PDF417 = 0x00080000,

	/**GS1 Composite Code*/
	BF_GS1_COMPOSITE = 0x80000000,

	/**MSI Code*/
	BF_MSI_CODE = 0x100000,

	/*Code 11*/
	BF_CODE_11 = 0x200000,

	/*Decode barcode with 2 digital addons*/
	BF_TWO_DIGIT_ADD_ON = 0x400000,

	/*Decode barcode with 5 digital addons*/
	BF_FIVE_DIGIT_ADD_ON = 0x800000,

	/*Matrix 25*/
	BF_MATRIX_25 = 0x1000000000,
	
	/**
	 * Telepen barcode format.
	 * Designed primarily for library and membership systems, it can encode
	 * the full ASCII character set, offering a high-density barcode solution.
	 */
	BF_TELEPEN = 0x2000000000,

	/**
	 * Telepen Numeric barcode format.
	 * A variation of the Telepen format optimized for encoding numeric data only.
	 */
	BF_TELEPEN_NUMERIC = 0x4000000000,

	/**Combined value of BF_USPSINTELLIGENTMAIL, BF_POSTNET, BF_PLANET, BF_AUSTRALIANPOST, BF_RM4SCC, BF_KIX.*/
	BF_POSTALCODE = 0x3F0000000000000,

	/**Nonstandard barcode */
	BF_NONSTANDARD_BARCODE = 0x100000000,

	/**USPS Intelligent Mail.*/
	BF_USPSINTELLIGENTMAIL = 0x10000000000000,

	/**Postnet.*/
	BF_POSTNET = 0x20000000000000,

	/**Planet.*/
	BF_PLANET = 0x40000000000000,

	/**Australian Post.*/
	BF_AUSTRALIANPOST = 0x80000000000000,

	/**Royal Mail 4-State Customer Barcode.*/
	BF_RM4SCC = 0x100000000000000,

	/**KIX.*/
	BF_KIX = 0x200000000000000,

	/**DotCode.*/
	BF_DOTCODE = 0x200000000,

	/**_PHARMACODE_ONE_TRACK.*/
	BF_PHARMACODE_ONE_TRACK = 0x400000000,

	/**PHARMACODE_TWO_TRACK.*/
	BF_PHARMACODE_TWO_TRACK = 0x800000000,

	/**PHARMACODE.*/
	BF_PHARMACODE = 0xC00000000
};

/**
* @enum LocalizationMode
*
* Describes the localization mode.
*/
typedef enum LocalizationMode
{
	/**Not supported yet. */
	LM_AUTO = 0x01,

	/**Localizes barcodes by searching for connected blocks. This algorithm usually gives best result and it is recommended to set ConnectedBlocks to the highest priority. */
	LM_CONNECTED_BLOCKS = 0x02,

	/**Localizes barcodes by groups of contiguous black-white regions. This is optimized for QRCode and DataMatrix. */
	LM_STATISTICS = 0x04,

	/**Localizes barcodes by searching for groups of lines. This is optimized for 1D and PDF417 barcodes. */
	LM_LINES = 0x08,

	/**Localizes barcodes quickly. This mode is recommended in interactive scenario. Check @ref LM for available argument settings.*/
	LM_SCAN_DIRECTLY = 0x10,

	/**Localizes barcodes by groups of marks.This is optimized for DPM codes. */
	LM_STATISTICS_MARKS = 0x20,

	/**Localizes barcodes by groups of connected blocks and lines.This is optimized for postal codes. */
	LM_STATISTICS_POSTAL_CODE = 0x40,

	/**Localizes barcodes from the centre of the image. Check @ref LM for available argument settings. */
	LM_CENTRE = 0x80,

	/**Localizes 1D barcodes fast. Check @ref LM for available argument settings. */
	LM_ONED_FAST_SCAN = 0x100,

	/** Localizes barcodes by utilizing a neural network model. */
	LM_NEURAL_NETWORK = 0X200,

	/**Reserved setting for localization mode.*/
#if defined(_WIN32) || defined(_WIN64)
	LM_REV = 0x80000000,
	/**Placeholder value with no functional meaning.*/
	LM_END = 0xFFFFFFFF,
#else
	LM_REV = -2147483648,
	/**Placeholder value with no functional meaning.*/
	LM_END = -1,
#endif

	/**Skips localization. */
	LM_SKIP = 0x00

}LocalizationMode;

/**
* @enum DeblurMode
*
* Describes the deblur mode.
*/
typedef enum DeblurMode
{
	/**Performs deblur process using the direct binarization algorithm.*/
	DM_DIRECT_BINARIZATION = 0x01,

	/**Performs deblur process using the threshold binarization algorithm.*/
	DM_THRESHOLD_BINARIZATION = 0x02,

	/**Performs deblur process using the gray equalization algorithm.*/
	DM_GRAY_EQUALIZATION = 0x04,

	/**Performs deblur process using the smoothing algorithm.*/
	DM_SMOOTHING = 0x08,

	/**Performs deblur process using the morphing algorithm.*/
	DM_MORPHING = 0x10,

	/**Performs deblur process using the deep analysis algorithm.*/
	DM_DEEP_ANALYSIS = 0x20,

	/**Performs deblur process using the sharpening algorithm.*/
	DM_SHARPENING = 0x40,

	/**Performs deblur process based on the binary image from the localization process.*/
	DM_BASED_ON_LOC_BIN = 0x80,

	/**Performs deblur process using the sharpening and smoothing algorithm.*/
	DM_SHARPENING_SMOOTHING = 0x100,

	// add in v11.0.10
	/**Performs deblur process by utilizing a neural network model. */
	DM_NEURAL_NETWORK = 0x200,

	/**Reserved setting for deblur mode.*/
#if defined(_WIN32) || defined(_WIN64)
	DM_REV = 0x80000000,
	/**Placeholder value with no functional meaning.*/
	DM_END = 0xFFFFFFFF,
#else
	DM_REV = -2147483648,
	DM_END = -1,
#endif
	/**Skips the deblur process.*/
	DM_SKIP = 0x00
}DeblurMode;

/**
* @enum QRCodeErrorCorrectionLevel
*
* Describes the QR Code error correction level.
*/
typedef enum QRCodeErrorCorrectionLevel
{
	/**Error Correction Level H (high) */
	QRECL_ERROR_CORRECTION_H,

	/**Error Correction Level L (low) */
	QRECL_ERROR_CORRECTION_L,

	/**Error Correction Level M (medium-low) */
	QRECL_ERROR_CORRECTION_M,

	/**Error Correction Level Q (medium-high) */
	QRECL_ERROR_CORRECTION_Q

}QRCodeErrorCorrectionLevel;

/**
* @enum ExtendedBarcodeResultType
*
* Describes the type of the extended barcode result.
*/
typedef enum ExtendedBarcodeResultType
{
	/**Specifies the standard text. This means the barcode value. */
	EBRT_STANDARD_RESULT,

	/**Specifies all the candidate text. This means all the standard text results decoded from the barcode. */
	EBRT_CANDIDATE_RESULT,

	/**Specifies the partial text. This means part of the text result decoded from the barcode. */
	EBRT_PARTIAL_RESULT
}ExtendedBarcodeResultType;


/**Structures section*/

#pragma pack(push)
#pragma pack(4)

/**
* The SimplifiedBarcodeReaderSettings struct contains settings for barcode decoding. It is a sub-parameter of SimplifiedCaptureVisionSettings.
*/
typedef struct tagSimplifiedBarcodeReaderSettings
{
	/**Input a combined value of enumeration BarcodeFormat to specify the targeting barcode formats.*/
	unsigned long long barcodeFormatIds;

	/**
	* Set the expected barcode count. The default value is 0.
	* Set expectedBarcodesCount to 0 if the barcode count is unknown. The library will try to find at least 1 barcode.
	* Set expectedBarcodesCount to 1 to reach the highest speed for processing single barcode.
	* Set expectedBarcodesCount to "n" if there will be "n" barcodes to process from an image.
	* Set expectedBarcodesCount to the highest expected value if there exists multiple barcode but the exact count is not confirmed.
	*/
	int expectedBarcodesCount;

	/**Set the grayscale transformation modes with an array of enumeration GrayscaleTransformationMode.*/
	GrayscaleTransformationMode grayscaleTransformationModes[8];

	/**Set the grayscale enhancement modes with an array of enumeration GrayscaleEnhancementMode.*/
	GrayscaleEnhancementMode grayscaleEnhancementModes[8];

	/**Set the location modes with an array of enumeration LocalizationMode.*/
	LocalizationMode localizationModes[8];

	/**Set the deblur modes with an array of enumeration DeblurMode.*/
	DeblurMode deblurModes[10];

	/**Set the minimum result confidence to filter out the low confidence results. The default value is 30.*/
	int minResultConfidence;

	/**Set the minimum barcode text length to filter out the unqualified results.*/
	int minBarcodeTextLength;

	/**Set the RegEx pattern of the barcode text to filter out the unqualified results.*/
	char barcodeTextRegExPattern[256];

	/**Set the maximum available threads count in one barcode decoding task.*/
	int maxThreadsInOneTask;

	/**Set the threshold for image shrinking. If the shorter edge size exceeds the specified threshold value, 
	* the library will calculate the resized height and width of the image and and perform shrinking.
	*/
	int scaleDownThreshold;

	/**Reserved for future use.*/
	char reserved[508];
}SimplifiedBarcodeReaderSettings;

#pragma pack(pop)


#ifdef __cplusplus

using namespace dynamsoft::basic_structures;
using namespace dynamsoft::intermediate_results;

namespace dynamsoft
{
	namespace dbr
	{
#pragma pack(push)
#pragma pack(4)
		/**
		 * The `CBarcodeDetails` class represents the details of a barcode. It is an abstract base class.
		 *
		 */
		class DBR_API CBarcodeDetails 
		{
		public:
			/**
			 * Destructor
			 */
			virtual ~CBarcodeDetails() {};
		};

		/**
		 * The `COneDCodeDetails` class represents detailed information about a one-dimensional barcode. It inherits from the `CBarcodeDetails` class.
		 */
		class DBR_API COneDCodeDetails :public CBarcodeDetails 
		{
		public:
			/**
			 * Constructor for the COneDCodeDetails class.
			 */
			COneDCodeDetails();

			COneDCodeDetails(const COneDCodeDetails& other);

			COneDCodeDetails& operator=(const COneDCodeDetails& other);

			/**
			 * Destructor for the COneDCodeDetails class.
			 */
			~COneDCodeDetails();

			/*The start chars in a byte array */
			unsigned char* startCharsBytes;

			/*The length of the start chars byte array*/
			int startCharsBytesLength;

			/*The stop chars in a byte array*/
			unsigned char* stopCharsBytes;

			/*The length of the stop chars byte array*/
			int stopCharsBytesLength;

			/*The check digit chars in a byte array*/
			unsigned char* checkDigitBytes;

			/*The length of the check digit chars byte array*/
			int checkDigitBytesLength;

			/*The position of the start pattern relative to the barcode location.
			Index 0 : X coordinate of the start position in percentage value;
			Index 1 : X coordinate of the end position in percentage value.*/
			float startPatternRange[2];

			/*The position of the middle pattern relative to the barcode location.
			Index 0 : X coordinate of the start position in percentage value;
			Index 1 : X coordinate of the end position in percentage value.*/
			float middlePatternRange[2];

			/*The position of the end pattern relative to the barcode location.
			Index 0 : X coordinate of the start position in percentage value;
			Index 1 : X coordinate of the end position in percentage value.*/
			float endPatternRange[2];

			/*Reserved memory for the struct.The length of this array indicates 
			the size of the memory reserved for this struct.*/
			char reserved[8];
			
		private:
			void CopyMembers(const COneDCodeDetails& other);
		};

		/**
		 * The `CQRCodeDetails` class represents the details of a QR Code barcode. It is derived from the `CBarcodeDetails` class and contains various attributes related to the QR Code barcode.
		 *
		 */
		class DBR_API CQRCodeDetails : public CBarcodeDetails 
		{
		public:
			CQRCodeDetails(int _rows = -1, int _columns = -1, QRCodeErrorCorrectionLevel _level = QRECL_ERROR_CORRECTION_H,
				int _version = -1, int _model = -1, int _mode = -1, int _page = -1, int _totalPage = -1, int _parityData = -1);

			CQRCodeDetails& operator=(const CQRCodeDetails& other);

			/**
			 * Destructor for the CQRCodeDetails class.
			 */
			~CQRCodeDetails();

			/*The row count of the barcode*/
			int rows;

			/*The column count of the barcode*/
			int columns;

			/*The error correction level of the barcode*/
			QRCodeErrorCorrectionLevel errorCorrectionLevel;

			/*The version of the QR Code*/
			int version;

			/*Number of the models*/
			int model;

			/*Identify the first data encoding mode*/
			int mode;

			/*Identify the position of the particular symbol*/
			int page;

			/*Identify the total number of symbols to be concatenated in the Structured Append format*/
			int totalPage;

			/*The Parity Data shall be an 8 bit byte following the Symbol Sequence Indicator.
			The parity data is a value obtained by XORing byte by byte the ASCII/JIS values of 
			all the original input data before division into symbol blocks.*/
			unsigned char parityData;

			int dataMaskPattern;

			unsigned char* codewords;

			int codewordsCount;
		};

		/**
		 * The `CPDF417Details` class represents a barcode in PDF417 format. It inherits from the `CBarcodeDetails` class and contains information about the row count, column count, and error correction level of the barcode.
		 *
		 */
		class DBR_API CPDF417Details :public CBarcodeDetails 
		{
		public:
			CPDF417Details(int _rows = -1, int _columns = -1, int _level = -1,
				int _hasLeftRowIndicator = -1, int _hasRightRowIndicator = -1);

			CPDF417Details& operator=(const CPDF417Details& other);

			/**
			 * Destructor for the CPDF417Details class.
			 */
			~CPDF417Details();

			/*The row count of the barcode*/
			int rows;

			/*The column count of the barcode*/
			int columns;

			/*The error correction level of the barcode*/
			int errorCorrectionLevel;

			/*Whether the left row indicator of the PDF417 code exists*/
			int hasLeftRowIndicator;

			/*Whether the right row indicator of the PDF417 code exists*/
			int hasRightRowIndicator;

			unsigned int * codewords;

			int codewordsCount;
		};
		
		/**
		 * The `CDataMatrixDetails` class represents the details of a DataMatrix barcode. It is derived from the `CBarcodeDetails` class and contains various attributes related to the DataMatrix barcode.
		 *
		 */
		class DBR_API CDataMatrixDetails : public CBarcodeDetails 
		{
		public:
			CDataMatrixDetails(int _rows = -1, int _columns = -1, int _dataRegionRows = -1,
				int _dataRegionColumns = -1, int _dataRegionNumber = -1);

			/*The row count of the barcode*/
			int rows;

			/*The column count of the barcode*/
			int columns;

			/*The data region row count of the barcode*/
			int dataRegionRows;

			/*The data region column count of the barcode*/
			int dataRegionColumns;

			/*The data region count*/
			int dataRegionNumber;
		};
		
		/**
		 * The `CAztecDetails` class represents a barcode in Aztec format. It inherits from the `CBarcodeDetails` class and contains information about the row count, column count, and layer number of the barcode.
		 *
		 */
		class DBR_API CAztecDetails :public CBarcodeDetails 
		{
		public:
			CAztecDetails(int _rows = -1, int _columns = -1, int _layerNumber = -1);

			/*The row count of the barcode*/
			int rows;

			/*The column count of the barcode*/
			int columns;

			/*A negative number (-1, -2, -3, -4) specifies a compact Aztec code.
			 *A positive number (1, 2, .. 32) specifies a normal(full-rang) Aztec code */
			int layerNumber;
		};

		namespace intermediate_results
		{
			/**
			 * The `CLocalizedBarcodeElement` class represents a localized barcode element detected in an image. It is inherited from `CRegionObjectElement` class.
			 */
			class DBR_API CLocalizedBarcodeElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CLocalizedBarcodeElement() {};

			public:
				/**
				 * Gets the format of the barcode.
				 *
				 * @return Returns the format of the barcode.
				 *
				 */
				virtual unsigned long long GetPossibleFormats() const = 0;

				/**
				 * Gets the string representation of the barcode format.
				 *
				 * @return Returns the string representation of the barcode format.
				 *
				 */
				virtual const char* GetPossibleFormatsString() const = 0;

				/**
				 * Gets the orientation angle of the barcode.
				 *
				 * @return Returns the orientation angle of the barcode.
				 *
				 */
				virtual int GetAngle() const = 0;

				/**
				 * Gets the module size of the barcode.
				 *
				 * @return Returns the module size of the barcode.
				 *
				 */
				virtual int GetModuleSize() const = 0;

				/**
				 * Gets the confidence score of the barcode recognition result.
				 *
				 * @return Returns the confidence score of the barcode recognition result.
				 *
				 */
				virtual int GetConfidence() const = 0;

				/**
				 * Sets the possible formats of the barcode.
				 *
				 * @param [in] possibleFormats The possible formats of the barcode.
				 *
				 */
				virtual void SetPossibleFormats(unsigned long long possibleFormats) = 0;

				/**
				 * Sets the location of the localized barcode element.
				 *
				 * @param location The location of the localized barcode element.
				 * @return Returns 0 if success, otherwise an error code.
				 */
				virtual int SetLocation(const CQuadrilateral& location) = 0;
			};

			class DBR_API CExtendedBarcodeResult;
			/**
			 * The `CDecodedBarcodeElement` class represents a decoded barcode element. 
			 * It is inherited from `CRegionObjectElement` class and provides additional functionality for retrieving information about the decoded barcode.
			 */
			class DBR_API CDecodedBarcodeElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDecodedBarcodeElement() {};

			public:
				/**
				 * Gets the format of the barcode.
				 *
				 * @return Returns the format of the barcode.
				 *
				 */
				virtual BarcodeFormat GetFormat() const = 0;

				/**
				 * Gets the string representation of the barcode format.
				 *
				 * @return Returns the string representation of the barcode format.
				 *
				 */
				virtual const char* GetFormatString() const = 0;

				/**
				 * Gets the text of the decoded barcode.
				 *
				 * @return Returns a pointer to the text of the decoded barcode.
				 *
				 */
				virtual const char* GetText() const = 0;

				/**
				 * Gets the raw bytes of the decoded barcode.
				 *
				 * @return Returns a pointer to the raw bytes of the decoded barcode.
				 *
				 */
				virtual unsigned char* GetBytes() const = 0;

				/**
				 * Gets the length of the raw bytes of the decoded barcode.
				 *
				 * @return Returns the length of the raw bytes of the decoded barcode.
				 *
				 */
				virtual int GetBytesLength() const = 0;

				/**
				 * Gets the details of the decoded barcode.
				 *
				 * @return Returns a pointer to the details of the decoded barcode.
				 *
				 */
				virtual const CBarcodeDetails* GetDetails()const = 0;

				/**
				 * Determines whether the decoded barcode is a DPM(Direct Part Marking) code.
				 *
				 * @return Returns true if the decoded barcode is a DPM code, false otherwise.
				 *
				 */
				virtual bool IsDPM() const = 0;

				/**
				 * Determines whether the decoded barcode is mirrored.
				 *
				 * @return Returns true if the decoded barcode is mirrored, false otherwise.
				 *
				 */
				virtual bool IsMirrored() const = 0;

				/**
				 * Gets the orientation angle of the barcode.
				 *
				 * @return Returns the orientation angle of the barcode.
				 *
				 */
				virtual int GetAngle() const = 0;

				/**
				 * Gets the module size of the barcode.
				 *
				 * @return Returns the module size of the barcode.
				 *
				 */
				virtual int GetModuleSize() const = 0;

				/**
				 * Gets the confidence score of the barcode recognition result.
				 *
				 * @return Returns the confidence score of the barcode recognition result.
				 *
				 */
				virtual int GetConfidence() const = 0;


				/**
				 * Gets the number of extended barcode results for the decoded barcode.
				 *
				 * @return Returns the number of extended barcode results for the decoded barcode.
				 *
				 */
				virtual int GetExtendedBarcodeResultsCount() const = 0;

				/**
				 * Gets the extended barcode result at the specified index for the decoded barcode.
				 *
				 * @param [in] index The index of the extended barcode result to retrive.
				 *
				 * @return Returns a pointer to the extended barcode result at the specified index for the decoded barcode.
				 *
				 */
				virtual const CExtendedBarcodeResult* GetExtendedBarcodeResult(int index) const = 0;

				/**
				 * Sets the format of the barcode.
				 *
				 * @param [in] format The format of the barcode.
				 *
				*/
				virtual void SetFormat(BarcodeFormat format) = 0;

				/**
				 * Sets the text of the barcode.
				 *
				 * @param text The text of the barcode.
				 */
				virtual void SetText(const char* text) = 0;

				/**
				 * Sets the raw bytes of the decoded barcode.
				 *
				 * @param [in] bytes The raw bytes of the decoded barcode.
				 * @param [in] bytesLength The length of the raw bytes of the decoded barcode.
				 *
				 */
				virtual void SetBytes(unsigned char* bytes, int bytesLength) = 0;

				/**
				 * Sets the confidence of the decoded barcode.
				 *
				 * @param confidence The confidence of the decoded barcode.
				 *
				 */
				virtual void SetConfidence(int confidence) = 0;

				/**
				 * Sets the location of the decoded barcode element.
				 *
				 * @param location The location of the decoded barcode element.
				 * @return Returns 0 if success, otherwise an error code.
				 */
				virtual int SetLocation(const CQuadrilateral& location) = 0;
			};

			/**
			 * The `CExtendedBarcodeResult` class represents an extended barcode result in a decoded barcode element. It contains information such as the type of extended barcode, deformation, clarity, and a sampling image of the barcode.
			 *
			 */
			class DBR_API CExtendedBarcodeResult : public CDecodedBarcodeElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CExtendedBarcodeResult() {};

			public:
				/**
				 * Gets the type of extended barcode result.
				 *
				 * @return Returns the type of the extended barcode result.
				 *
				 */
				virtual ExtendedBarcodeResultType GetExtendedBarcodeResultType() const = 0;

				/**
				 * Gets the deformation of the barcode.
				 *
				 * @return Returns the deformation of the barcode.
				 *
				 */
				virtual int GetDeformation() const = 0;

				/**
				 * Gets the clarity of the barcode.
				 *
				 * @return Returns the clarity of the barcode.
				 *
				 */
				virtual int GetClarity() const = 0;

				/**
				 * Gets the sampling image of the barcode.
				 *
				 * @return Returns a pointer to the sampling image of the barcode.
				 *
				 */
				virtual const CImageData* GetSamplingImage() const = 0;
			};

			/**
			 * Represents a candidate zone for barcode detection.
			 *
			 */
			class DBR_API CCandidateBarcodeZone
			{
			private:
				CQuadrilateral location;
				unsigned long long barcodeFormats;

			public:
				/**
				 * Default Constructor
				 */
				CCandidateBarcodeZone();

				/**
				 * Constructor
				 *
				 * @param [in] location The location of the candidate barcode.
				 * @param [in] possibleFormats The posssible formats of the candidate barcode.
				 *
				 */
				CCandidateBarcodeZone(const CQuadrilateral& location, unsigned long long possibleFormats);

				/**
				 * Gets the location of the candidate barcode.
				 *
				 * @return Returns the location of the candidate barcode.
				 *
				 */
				CQuadrilateral GetLocation() const;

				/**
				 * Sets the location of the candidate barcode.
				 *
				 * @param [in] loc The location of the candidate barcode.
				 *
				 */
				void SetLocation(const CQuadrilateral& loc);

				/**
				 * Gets the posssible formats of the candidate barcode.
				 *
				 * @return Returns the posssible formats of the candidate barcode.
				 *
				 */
				unsigned long long GetPossibleFormats() const;

				/**
				 * Sets the posssible formats of the candidate barcode.
				 *
				 * @param [in] formats The posssible formats of the candidate barcode.
				 *
				 */
				void SetPossibleFormats(unsigned long long formats);
			};

			/**
			 * The `CCandidateBarcodeZonesUnit` class represents a unit that contains candidate barcode zones unit. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CCandidateBarcodeZonesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CCandidateBarcodeZonesUnit() {};

			public:
				/**
				 * Gets the number of candidate barcode zones in the unit.
				 *
				 * @return Returns the number of candidate barcode zones in the unit.
				 *
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a pointer to a specific candidate barcode zone.
				 *
				 * @param [in] index The index of the candidate barcode zone.
				 * @param [out] barcodeZone The pointer to the candidate barcode zone.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int GetCandidateBarcodeZone(int index, CCandidateBarcodeZone* barcodeZone) const = 0;

				/**
				 * Removes all the candidate barcode zones.
				 */
				virtual void RemoveAllCandidateBarcodeZones() = 0;

				/**
				 * Removes a candidate barcode zone at the specified index.
				 *
				 * @param index The index of the candidate barcode zone.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveCandidateBarcodeZone(int index) = 0;

				/**
				 * Adds a candidate barcode zone.
				 *
				 * @param [in] barcodeZone The candidate barcode zone.
				 * @param [in] matrixToOriginalImage The matrix to original image.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int AddCandidateBarcodeZone(const CCandidateBarcodeZone& barcodeZone, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * Sets a candidate barcode zone at the specified index.
				 *
				 * @param [in] index The index of the candidate barcode zone.
				 * @param [in] barcodeZone The candidate barcode zone.
				 * @param [in] matrixToOriginalImage The matrix to original image.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int SetCandidateBarcodeZone(int index, const CCandidateBarcodeZone& barcodeZone, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CLocalizedBarcodesUnit` class represents a unit that contains localized barcodes unit. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CLocalizedBarcodesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CLocalizedBarcodesUnit() {};

			public:
				/**
				 * Gets the number of localized barcodes in the unit.
				 *
				 * @return Returns the number of localized barcodes in the unit.
				 *
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a pointer to a specific localized barcode element.
				 *
				 * @param [in] index The index of the localized barcode element to retrieve.
				 *
				 * @return Returns a const pointer to the localized barcode element at the specified index.
				 *
				 */
				virtual const CLocalizedBarcodeElement* GetLocalizedBarcode(int index) const = 0;

				/**
				 * Gets a pointer to a specific localized barcode element.
				 *
				 * @param [in] index The index of the localized barcode element to retrieve.
				 *
				 * @return Returns a const pointer to the localized barcode element at the specified index.
				 *
				 */
				virtual const CLocalizedBarcodeElement* operator[](int index) const = 0;

				/**
				 * Removes all the localized barcodes.
				 */
				virtual void RemoveAllLocalizedBarcodes() = 0;

				/**
				 * Removes a localized barcode at the specified index.
				 *
				 * @param index The index of the localized barcode.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveLocalizedBarcode(int index) = 0;

				/**
				 * Adds a localized barcode.
				 *
				 * @param element The localized barcode element.
				 * @param matrixToOriginalImage The matrix to original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int AddLocalizedBarcode(const CLocalizedBarcodeElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * @brief Sets a localized barcode at the specified index.
				 *
				 * @param index The index of the localized barcode.
				 * @param element The localized barcode element.
				 * @param matrixToOriginalImage The matrix to original image.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetLocalizedBarcode(int index, const CLocalizedBarcodeElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CScaledBarcodeImageUnit` class represents a unit that contains scaled barcode image. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CScaledBarcodeImageUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CScaledBarcodeImageUnit() {};

			public:
				/**
				 * Gets the scaled barcode image data.
				 *
				 * @return Returns a pointer to the scaled image of the barcode.
				 *
				 */
				virtual const CImageData* GetImageData() const = 0;

				/**
				 * @brief Sets the scaled image data.
				 *
				 * @param imgData The pointer to the scaled image data.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetImageData(const CImageData* imgData) = 0;
			};

			typedef void(*FreeImageFunc)(const CImageData*);
			/**
			 * Represents a deformation resisted barcode.
			 *
			 */
			class DBR_API CDeformationResistedBarcode
			{
			private:
				const CImageData* image;
				CQuadrilateral location;
				BarcodeFormat format;
				FreeImageFunc freeImageFunc;

			public:
				/**
				 * Destructor
				 */
				~CDeformationResistedBarcode();

				/**
				 * Default Constructor
				 */
				CDeformationResistedBarcode();

				/**
				 * Move constructor for CDeformationResistedBarcode.
				 *
				 * @param barcode The rvalue reference to another CDeformationResistedBarcode object.
				 */
				CDeformationResistedBarcode(CDeformationResistedBarcode&& barcode);

				/**
				 * Move assignment operator for CDeformationResistedBarcode.
				 *
				 * @param barcode The rvalue reference to another CDeformationResistedBarcode object.
				 * @return A reference to the moved CDeformationResistedBarcode object.
				 */
				CDeformationResistedBarcode& operator=(CDeformationResistedBarcode&& barcode);

				/**
				 * @brief Construct a new CDeformationResistedBarcode object
				 *
				 * @param img The pointer to the deformation resisted barcode image.
				 * @param freeImageFunc The function pointer to free image.
				 * @param location The location of the deformation resisted barcode.
				 * @param format The format of the deformation resisted barcode.
				 */
				CDeformationResistedBarcode(const CImageData* img, FreeImageFunc freeImageFunc, const CQuadrilateral& location, BarcodeFormat format);

				/**
				 * Gets the deformation resisted barcode image.
				 *
				 * @return The pointer to the deformation resisted barcode image.
				 */
				const CImageData* GetImageData() const;

				/**
				 * Sets the deformation resisted barcode image.
				 *
				 * @param img The pointer to the deformation resisted barcode image.
				 * @param freeImageFunc The function pointer to free image.
				 */
				void SetImageData(const CImageData* img, FreeImageFunc freeImageFunc);

				/**
				 * Gets the location of the deformation resisted barcode in the image.
				 *
				 * @return The location of the deformation resisted barcode.
				 */
				CQuadrilateral GetLocation() const;

				/**
				 * Sets the location of the deformation resisted barcode in the image.
				 *
				 * @param loc The location of the deformation resisted barcode.
				 */
				void SetLocation(const CQuadrilateral& loc);

				/**
				 * Gets the format of the deformation resisted barcode.
				 *
				 * @return The format of the deformation resisted barcode.
				 */
				BarcodeFormat GetFormat() const;

				/**
				 * Sets the format of the deformation resisted barcode.
				 *
				 * @param format The format of the deformation resisted barcode.
				 */
				void SetFormat(BarcodeFormat format);

			private:
				CDeformationResistedBarcode(const CDeformationResistedBarcode&);
				CDeformationResistedBarcode& operator=(const CDeformationResistedBarcode&);
			};

			/**
			 * The `CDeformationResistedBarcodeImageUnit` class represents a unit that contains deformation resisted barcode image data. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CDeformationResistedBarcodeImageUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDeformationResistedBarcodeImageUnit() {};

			public:
				/**
				 * @brief Gets the deformation resisted barcode object
				 *
				 * @return An instance of CDeformationResistedBarcode
				 */
				virtual CDeformationResistedBarcode GetDeformationResistedBarcode() const = 0;

				/**
				 * @brief Sets the deformation resisted barcode object
				 *
				 * @param barcode The deformation resisted barcode object
				 * @param matrixToOriginalImage The matrix to original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int SetDeformationResistedBarcode(const CDeformationResistedBarcode& barcode, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CComplementedBarcodeImageUnit` class represents a unit that contains complemented barcode image data. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CComplementedBarcodeImageUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CComplementedBarcodeImageUnit() {};

			public:
				/**
				 * Gets the complemented barcode image data.
				 *
				 * @return Returns a pointer to the complemented image of the barcode.
				 *
				 */
				virtual const CImageData* GetImageData() const = 0;

				/**
				 * Gets the location of the complemented barcode in a quadrilateral.
				 *
				 * @return Returns the location of the complemented barcode in a quadrilateral.
				 *
				 */
				virtual CQuadrilateral GetLocation() const = 0;

				/**
				 * Sets the location of the complemented barcode
				 *
				 * @param location The location of the complemented barcode
				 * @param matrixToOriginalImage The matrix to original image
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetLocation(const CQuadrilateral& location, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CDecodedBarcodesUnit` class represents a unit that contains decoded barcode elements. It inherits from the `CIntermediateResultUnit` class.
			 *
			 */
			class DBR_API CDecodedBarcodesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDecodedBarcodesUnit() {};

			public:
				/**
				 * Gets the number of decoded barcodes in the unit.
				 *
				 * @return Returns the number of decoded barcodes in the unit.
				 *
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a pointer to the CDecodedBarcodeElement object at the specified index.
				 *
				 * @param [in] index The index of the desired CDecodedBarcodeElement object.
				 *
				 * @return Returns a pointer to the CDecodedBarcodeElement object at the specified index.
				 *
				 */
				virtual const CDecodedBarcodeElement* GetDecodedBarcode(int index) const = 0;

				/**
				 * Gets a pointer to the CDecodedBarcodeElement object at the specified index.
				 *
				 * @param [in] index The index of the desired CDecodedBarcodeElement object.
				 *
				 * @return Returns a pointer to the CDecodedBarcodeElement object at the specified index.
				 *
				 */
				virtual const CDecodedBarcodeElement* operator[](int index) const = 0;

				/**
				 * Removes all decoded barcodes in the unit.
				 *
				 */
				virtual void RemoveAllDecodedBarcodes() = 0;

				/**
				 * Sets the decoded barcode.
				 *
				 * @param element The decoded barcode
				 * @param matrixToOriginalImage The matrix to original image
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetDecodedBarcode(const CDecodedBarcodeElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				virtual int RemoveDecodedBarcode(int index) = 0;
			};
		}

		/**
		 * The `CBarcodeResultItem` class represents a barcode result item decoded by barcode reader engine. It is derived from `CCapturedResultItem`.
		 */
		class DBR_API CBarcodeResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CBarcodeResultItem() {};

		public:
			/**
			 * It is used to get the format of the decoded barcode result.
			 *
			 * @return Returns the format of the decoded barcode result.
			 *
			 */
			virtual BarcodeFormat GetFormat() const = 0;

			/**
			 * It is used to get the format string of the decoded barcode result.
			 *
			 * @return Returns the format string of the decoded barcode result.
			 *
			 */
			virtual const char* GetFormatString() const = 0;

			/**
			 * It is used to get the text result of the decoded barcode.
			 *
			 * @return Returns the text result of the decoded barcode.
			 *
			 */
			virtual const char* GetText() const = 0;

			/**
			 * It is used to get the text bytes of the decoded barcode result.
			 *
			 * @return Returns the text bytes of the decoded barcode result.
			 *
			 */
			virtual unsigned char* GetBytes() const = 0;

			/**
			 * It is used to get the text bytes length of the decoded barcode result.
			 *
			 * @return Returns the text bytes length of the decoded barcode result.
			 *
			 */
			virtual int GetBytesLength() const = 0;

			/**
			 * It is used to get the location of the decoded barcode in a quadrilateral.
			 *
			 * @return Returns the location of the decoded barcode in a quadrilateral.
			 *
			 */
			virtual CQuadrilateral GetLocation() const = 0;

			/**
			 * It is used to get the confidence of the decoded barcode result.
			 *
			 * @return Returns the confidence of the decoded barcode result.
			 *
			 */
			virtual int GetConfidence() const = 0;

			/**
			 * It is used to get the angle of the decoded barcode result.
			 *
			 * @return Returns the angle of the decoded barcode result.
			 *
			 */
			virtual int GetAngle() const = 0;

			/**
			 * It is used to get the module size of the decoded barcode result.
			 *
			 * @return Returns the module size of the decoded barcode result.
			 *
			 */
			virtual int GetModuleSize() const = 0;

			/**
			 * It is used to get the details of the decoded barcode result.
			 *
			 * @return Returns the details of the decoded barcode result.
			 *
			 */
			virtual const CBarcodeDetails* GetDetails() const = 0;

			/**
			 * It is used to get whether the decoded barcode is a DPM code.
			 *
			 * @return Returns whether the decoded barcode is a DPM code.
			 *
			 */
			virtual bool IsDPM() const = 0;

			/**
			 * It is used to get whether the decoded barcode is mirrored.
			 *
			 * @return Returns whether the decoded barcode is mirrored.
			 *
			 */
			virtual bool IsMirrored() const = 0;

			/**
			 * Sets the location of the barcode item.
			 *
			 * @param [in] location The location of the barcode item.
			 *
			 * @return Returns 0 if success, otherwise an error code.
			 *
			 */
			virtual int SetLocation(const CQuadrilateral& location) = 0;

		};

		/**
		 * The `CDecodedBarcodesResult` class represents the result of a barcode reading process. It provides access to information about the decoded barcodes, the source image, and any errors that occurred during the barcode reading process.
		 *
		 */
		class DBR_API CDecodedBarcodesResult : public CCapturedResultBase
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CDecodedBarcodesResult() {};

		public:
			/**
			 * Gets the number of decoded barcode items in the barcode reading result.
			 *
			 * @return Returns the number of decoded barcode items in the barcode reading result.
			 *
			 */
			virtual int GetItemsCount()const = 0;

			/**
			 * Gets the decoded barcode result item at the specified index.
			 *
			 * @param [in] index The zero-based index of the barcode result item to retrieve.
			 *
			 * @return Returns a pointer to the CBarcodeResultItem object at the specified index.
			 *
			 */
			virtual const CBarcodeResultItem* GetItem(int index) const = 0;

			/**
			 * Removes a specific item from the array in the barcodes.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CBarcodeResultItem* item) = 0;

			/**
			 * Checks if the item is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CBarcodeResultItem* item) const = 0;

			/**
			 * Gets the decoded barcode result item at the specified index.
			 *
			 * @param [in] index The zero-based index of the barcode result item to retrieve.
			 *
			 * @return Returns a pointer to the CBarcodeResultItem object at the specified index.
			 *
			 */
			virtual const CBarcodeResultItem* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CDecodedBarcodesResult object.
			 *
			 * @return An object of CDecodedBarcodesResult.
			 */
			virtual CDecodedBarcodesResult* Retain() = 0;

			/**
			* Decreases the reference count of the CDecodedBarcodesResult object.
			*
			*/
			virtual void Release() = 0;

			/**
			 * Adds a specific item to the array in the decoded barcodes result.
			 *
			 * @param [in] item The specific item to add.
			 *
			 * @return Returns value indicating whether the addition was successful or not.
			 *
			 */
			virtual int AddItem(const CBarcodeResultItem* item) = 0;
		};

		/**
		 * The CBarcodeReaderModule class defines general functions in the barcode reader module.
		 */
		class DBR_API CBarcodeReaderModule
		{
		public:
			/**
			 * Returns the version of the barcode reader module.
			 *
			 * @return Returns a const char pointer representing the version of the barcode reader module.
			 */
			static const char* GetVersion();

			/**
			 * Creates a Decoded Barcode Element object.
			 *
			 * @return An object of CDecodedBarcodeElement
			 */
			static intermediate_results::CDecodedBarcodeElement* CreateDecodedBarcodeElement();

			/**
			 * Creates a Localized Barcode Element object.
			 *
			 * @return An object of CLocalizedBarcodeElement
			 */
			static intermediate_results::CLocalizedBarcodeElement* CreateLocalizedBarcodeElement();
		};
#pragma pack(pop)
	}
}
#endif
