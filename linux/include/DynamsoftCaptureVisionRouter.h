#pragma once

#if !defined(_WIN32) && !defined(_WIN64)

#ifdef __EMSCRIPTEN__
#define CVR_API __attribute__((used))
#else
#define CVR_API __attribute__((visibility("default")))
#endif

#include <stddef.h>
#else
#ifdef CVR_EXPORTS
#define CVR_API __declspec(dllexport)
#else
#define CVR_API __declspec(dllimport)
#endif
#include <windows.h>
#endif

#include "DynamsoftCore.h"
#include "DynamsoftLicense.h"
#include "DynamsoftBarcodeReader.h"
//#if !defined(ONLY_DBR)
#include "DynamsoftLabelRecognizer.h"
#include "DynamsoftDocumentNormalizer.h"
#include "DynamsoftCodeParser.h"
//#endif

#define DCV_VERSION                  "3.2.50.6558"

/**Enumeration section*/

/**
* @enum CaptureState
*
* Describes the state of data capturing.
*/
typedef enum CaptureState
{
	/**The data capturing is started.*/
	CS_STARTED,

	/**The data capturing is stopped.*/
	CS_STOPPED,

	/**The data capturing is paused.*/
	CS_PAUSED,

	/**The data capturing is resumed.*/
	CS_RESUMED
} CaptureState;

/**
* @enum ImageSourceState
*
* Describes the state of ImageSourceAdapter.
*/
typedef enum ImageSourceState
{
	/**The buffer of ImageSourceAdapter is temporarily empty.*/
	ISS_BUFFER_EMPTY,

	/**The source of ImageSourceAdapter is empty.*/
	ISS_EXHAUSTED
} ImageSourceState;

/**Structures section*/
#pragma pack(push)
#pragma pack(4)

/**
* The `SimplifiedCaptureVisionSettings` struct contains settings for capturing and recognizing images with the `CCaptureVisionRouter` class.
*/
typedef struct tagSimplifiedCaptureVisionSettings
{
	/**
	 * Specifies whether to output the original image.
	 */
	int outputOriginalImage;

	/**
	 * Specifies the region of interest (ROI) where the image capture and recognition will take place.
	 * Default is [[0,0],[100,0],[100,100],[0,100]]
	 */
	CQuadrilateral roi;

	/**
	 * Specifies whether the ROI is measured in pixels or as a percentage of the image size.
	 *
	 * @values 0 if the ROI is measured in pixels.
	 *         1 if the ROI is measured as a percentage of the image size.
	 */
	int roiMeasuredInPercentage;

	/**
	 * Specifies the maximum number of parallel tasks that can be used for image capture and recognition.
	 */
	int maxParallelTasks;

	/**
	 * Specifies the maximum time (in milliseconds) allowed for image capture and recognition.
	 */
	int timeout;

	/**
	 * Specifies the settings for barcode recognition.
	 */
	SimplifiedBarcodeReaderSettings barcodeSettings;

	/**
	 * Specifies the settings for label recognition.
	 */
	SimplifiedLabelRecognizerSettings labelSettings;
	/**
	 * @brief Minimum time interval (in milliseconds) allowed between consecutive image captures.
	 *
	 * This property represents the minimum time interval (in milliseconds) that must
	 * elapse before the next image capture operation can be initiated.
	 * Setting a larger value for this property will introduce a delay between image
	 * captures, while setting a smaller value allows for more frequent captures. It
	 * can be used to reduce the computational frequency, which can effectively lower
	 * energy consumption.
	 *
	 * @note The actual time interval between captures may be longer than the specified
	 *       minimum interval due to various factors, such as image processing time and
	 *       hardware limitations.
	 *
	 */
	int minImageCaptureInterval;

	/**
	 * Specifies the settings for document normalization.
	 */
	SimplifiedDocumentNormalizerSettings documentSettings;

	/**
	 * Reserved for future use.
	 */
	char reserved[1440];

} SimplifiedCaptureVisionSettings;

#pragma pack(pop)

/**C++ API section*/
#ifdef __cplusplus

using namespace dynamsoft::basic_structures;
using namespace dynamsoft::intermediate_results;

class CaptureVisionInner;

namespace dynamsoft
{
	namespace cvr
	{
#pragma pack(push)
#pragma pack(4)
		/**
		* The CCapturedResult class represents the result of a capture operation on an image. Internally, CaptureResult stores an array that contains multiple items, each of which may be a barcode, text line, detected quad, deskewed image, original image, parsed item, etc.
		*/
		class CVR_API CCapturedResult : public CCapturedResultBase
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CCapturedResult() {};

		public:
			/**
			 * Gets the number of items in the captured result.
			 *
			 * @return Returns the number of items in the captured result.
			 *
			 */
			virtual int GetItemsCount() const = 0;

			/**
			 * Gets a pointer to the CCapturedResultItem object at the specified index.
			 *
			 * @param [in] index The index of the item to retrieve.
			 *
			 * @return Returns a pointer to the CCapturedResultItem object at the specified index.
			 *
			 */
			virtual const CCapturedResultItem* GetItem(int index) const = 0;

			/**
			 * Remove a specific item from the array in the captured result.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CCapturedResultItem* item) = 0;

			/**
			 * Check if the item is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CCapturedResultItem* item) const = 0;

			/**
			 * Gets a pointer to the CCapturedResultItem object at the specified index.
			 *
			 * @param [in] index The index of the item to retrieve.
			 *
			 * @return Returns a pointer to the CCapturedResultItem object at the specified index.
			 *
			 */
			virtual const CCapturedResultItem* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CCapturedResult object.
			 *
			 * @return An object of CCapturedResult.
			 */
			virtual CCapturedResult* Retain() = 0;

			/**
			* Decreases the reference count of the CCapturedResult object.
			*
			*/
			virtual void Release() = 0;

			/**
			 * Gets the decoded barcode items from the `CCapturedResult`.
			 *
			 * @return Returns a pointer to the CDecodedBarcodesResult object containing the decoded barcode items.
			 * @remark Do not forget to release the memory pointed to by the returned pointer.
			 */
			virtual dbr::CDecodedBarcodesResult* GetDecodedBarcodesResult() const = 0;

			/**
			 * Gets the recognized text line items from the `CCapturedResult`.
			 *
			 * @return Returns a pointer to the CRecognizedTextLinesResult object containing the recognized text line items.
			 * @remark Do not forget to release the memory pointed to by the returned pointer.
			 */
			virtual dlr::CRecognizedTextLinesResult* GetRecognizedTextLinesResult() const = 0;

			/**
			 * Gets the document result items from the `CCapturedResult`.
			 *
			 * @return Returns a pointer to the CProcessedDocumentResult object containing the detected quadrilateral, deskewed image and enhanced image items.
			 * @remark Do not forget to release the memory pointed to by the returned pointer.
			 */
			virtual ddn::CProcessedDocumentResult* GetProcessedDocumentResult() const = 0;

			/**
			 * Gets the parsed result items from the `CCapturedResult`.
			 *
			 * @return Returns a pointer to the CParsedResult object containing the parsed result items.
			 * @remark Do not forget to release the memory pointed to by the returned pointer.
			 */
			virtual dcp::CParsedResult* GetParsedResult() const = 0;

			/**
			 * Add a specific item to the array in the captured result.
			 *
			 * @param [in] item The specific item to add.
			 *
			 * @return Returns value indicating whether the addition was successful or not.
			 *
			 */
			virtual int AddItem(const CCapturedResultItem* item) = 0;
		};

		class CVR_API CCapturedResultArray
		{
		protected:
			/**
			* Destructor
			*/
			virtual ~CCapturedResultArray() {};

		public:
			/**
			 * Gets the number of captured results.
			 *
			 * @return Returns the number of captured results. Each captured result represents a collection of CCapturedResultItem objects in an image.
			 *
			 */
			virtual int GetResultsCount() const = 0;

			/**
			 * Gets a pointer to the CCapturedResult object at the specified index.
			 *
			 * @param [in] index The index of the result to retrieve.
			 *
			 * @return Returns a pointer to the CCapturedResult object at the specified index.
			 *
			 */
			virtual const CCapturedResult* GetResult(int index) const = 0;

			/**
			 * Gets a pointer to the CCapturedResult object at the specified index.
			 *
			 * @param [in] index The index of the result to retrieve.
			 *
			 * @return Returns a pointer to the CCapturedResult object at the specified index.
			 *
			 */
			virtual const CCapturedResult* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CCapturedResultArray object.
			 *
			 * @return An object of CCapturedResultArray.
			 */
			virtual CCapturedResultArray* Retain() = 0;

			/**
			* Decreases the reference count of the CCapturedResultArray object.
			*
			*/
			virtual void Release() = 0;
		};

		/**
		* The `CIntermediateResultReceiver` class is responsible for receiving intermediate results of different types.
		* It provides virtual functions for each type of result, which are called when the corresponding result is received.
		*/
		class CVR_API CIntermediateResultReceiver : public CAbstractIntermediateResultReceiver
		{
		protected:
			/**
			* Constructor
			*/
			CIntermediateResultReceiver();

		public:
			/**
			* Destructor
			*/
			virtual ~CIntermediateResultReceiver();

			/**
			* Called when a task result has been received.
			*
			* @param [in] pResult A pointer to the CIntermediateResult object that contains several result units.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTaskResultsReceived(CIntermediateResult *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when predetected regions have been received.
			*
			* @param [in] pResult A pointer to the CPredetectedRegionsUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnPredetectedRegionsReceived(CPredetectedRegionsUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when localized barcodes have been received.
			*
			* @param [in] pResult A pointer to the CLocalizedBarcodesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnLocalizedBarcodesReceived(dbr::intermediate_results::CLocalizedBarcodesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when decoded barcodes have been received.
			*
			* @param [in] pResult A pointer to the CDecodedBarcodesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnDecodedBarcodesReceived(dbr::intermediate_results::CDecodedBarcodesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when localized text lines have been received.
			*
			* @param [in] pResult A pointer to the CLocalizedTextLinesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnLocalizedTextLinesReceived(dlr::intermediate_results::CLocalizedTextLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when recognized text lines have been received.
			*
			* @param [in] pResult A pointer to the CRecognizedTextLinesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnRecognizedTextLinesReceived(dlr::intermediate_results::CRecognizedTextLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when detected quadrilaterals have been received.
			*
			* @param [in] pResult A pointer to the CDetectedQuadsUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnDetectedQuadsReceived(ddn::intermediate_results::CDetectedQuadsUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when deskewed images have been received.
			*
			* @param [in] pResult A pointer to the CDeskewedImageUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnDeskewedImageReceived(ddn::intermediate_results::CDeskewedImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when colour image units have been received.
			*
			* @param [in] pResult A pointer to the received colour image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnColourImageUnitReceived(CColourImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when scaled colour image units have been received.
			*
			* @param [in] pResult A pointer to the received scaled colour image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnScaledColourImageUnitReceived(CScaledColourImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when grayscale image units have been received.
			*
			* @param [in] pResult A pointer to the received grayscale image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnGrayscaleImageUnitReceived(CGrayscaleImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when transformed grayscale image units have been received.
			*
			* @param [in] pResult A pointer to the received transformed grayscale image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTransformedGrayscaleImageUnitReceived(CTransformedGrayscaleImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when enhanced grayscale image units have been received.
			*
			* @param [in] pResult A pointer to the received enhanced grayscale image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnEnhancedGrayscaleImageUnitReceived(CEnhancedGrayscaleImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when binary image units have been received.
			*
			* @param [in] pResult A pointer to the received binary image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnBinaryImageUnitReceived(CBinaryImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when texture detection result units have been received.
			*
			* @param [in] pResult A pointer to the received texture detection result unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTextureDetectionResultUnitReceived(CTextureDetectionResultUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when texture-removed grayscale image units have been received.
			*
			* @param [in] pResult A pointer to the received texture-removed grayscale image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTextureRemovedGrayscaleImageUnitReceived(CTextureRemovedGrayscaleImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when texture-removed binary image units have been received.
			*
			* @param [in] pResult A pointer to the received texture-removed binary image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTextureRemovedBinaryImageUnitReceived(CTextureRemovedBinaryImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when contours units have been received.
			*
			* @param [in] pResult A pointer to the received contours unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnContoursUnitReceived(CContoursUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when short lines units have been received.
			*
			* @param [in] pResult A pointer to the received short lines unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnShortLinesUnitReceived(CShortLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when line segments units have been received.
			*
			* @param [in] pResult A pointer to the received line segments unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnLineSegmentsUnitReceived(CLineSegmentsUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when text zones units have been received.
			*
			* @param [in] pResult A pointer to the received text zones unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTextZonesUnitReceived(CTextZonesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when text-removed binary image units have been received.
			*
			* @param [in] pResult A pointer to the received text-removed binary image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTextRemovedBinaryImageUnitReceived(CTextRemovedBinaryImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when long lines units have been received.
			*
			* @param [in] pResult A pointer to the received long lines unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnLongLinesUnitReceived(ddn::intermediate_results::CLongLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when corners units have been received.
			*
			* @param [in] pResult A pointer to the received corners unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnCornersUnitReceived(ddn::intermediate_results::CCornersUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when candidate quad edges units have been received.
			*
			* @param [in] pResult A pointer to the received candidate quad edges unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnCandidateQuadEdgesUnitReceived(ddn::intermediate_results::CCandidateQuadEdgesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when candidate barcode zones units have been received.
			*
			* @param [in] pResult A pointer to the received candidate barcode zones unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnCandidateBarcodeZonesUnitReceived(dbr::intermediate_results::CCandidateBarcodeZonesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when scaled barcode image units have been received.
			*
			* @param [in] pResult A pointer to the received scaled barcode image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnScaledBarcodeImageUnitReceived(dbr::intermediate_results::CScaledBarcodeImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when deformation resisted barcode image units have been received.
			*
			* @param [in] pResult A pointer to the received deformation resisted barcode image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnDeformationResistedBarcodeImageUnitReceived(dbr::intermediate_results::CDeformationResistedBarcodeImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when complemented barcode image units have been received.
			*
			* @param [in] pResult A pointer to the received complemented barcode image unit.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnComplementedBarcodeImageUnitReceived(dbr::intermediate_results::CComplementedBarcodeImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when raw text lines units have been received.
			*
			* @param [in] pResult A pointer to the CRawTextLinesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnRawTextLinesUnitReceived(dlr::intermediate_results::CRawTextLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when logic lines units have been received.
			*
			* @param [in] pResult A pointer to the CLogicLinesUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnLogicLinesUnitReceived(ddn::intermediate_results::CLogicLinesUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when enhanced images have been received.
			*
			* @param [in] pResult A pointer to the CEnhancedImageUnit object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnEnhancedImageReceived(ddn::intermediate_results::CEnhancedImageUnit *pResult, const IntermediateResultExtraInfo* info);

			/**
			* Called when all tasks for the target ROI are completed and the results are deduplicated.
			*
			* @param [in] pResult A pointer to the CIntermediateResult object that contains the result.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnTargetROIResultsReceived(CIntermediateResult *pResult, const IntermediateResultExtraInfo* info);

			virtual const char* GetEncryptedString();

			/**
			* Called when a intermediate result unit has been received.
			*
			* @param [in] pUnit A pointer to the CIntermediateResultUnit object.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			*/
			virtual void OnUnitResultReceived(CIntermediateResultUnit *pUnit, const IntermediateResultExtraInfo* info) final;

			/**
			* Called by function modules when a task result has been received.
			*
			* @param [in] pResult A pointer to the CIntermediateResult object that contains several result units.
			* @param [in] info A pointer to the IntermediateResultExtraInfo object that contains the extra info of intermediate result.
			*
			* @remark It is for internal calls of function modules such as DynamsoftBarcodeReader, DynamsoftLabelRecognizer and DynamsoftDocumentNormalizer.
			*/
			virtual void OnTaskResultsReceivedInner(CIntermediateResult *pResult, const IntermediateResultExtraInfo* info) final;
		};

		/**
		* The CIntermediateResultManager class manages intermediate results generated during data capturing.
		*
		*/
		class CVR_API CIntermediateResultManager
		{
		public:
			/**
			* Destructor
			*/
			virtual ~CIntermediateResultManager() {};

			/**
			* Adds an intermediate result receiver to the manager.
			*
			* @param [in] receiver The intermediate result receiver to add.
			*
			*/
			virtual int AddResultReceiver(CIntermediateResultReceiver* receiver) = 0;

			/**
			* Removes an intermediate result receiver from the manager.
			*
			* @param [in] receiver The intermediate result receiver to remove.
			*
			*/
			virtual int RemoveResultReceiver(CIntermediateResultReceiver* receiver) = 0;

			/**
			* Gets the original image data using an image hash id.
			*
			* @param [in] imageHashId The hash id of the image to retrieve.
			*
			* @return Returns a pointer to the CImageData object containing the original image data. You don't need to release the memory pointed to by the returned pointer.
			*
			*/
			virtual CImageData* GetOriginalImage(const char* imageHashId) = 0;

			virtual void AddOriginalImageToCache(const char* imageHashId) = 0;

			virtual void RemoveOriginalImageFromCache(const char* imageHashId) = 0;
		};

		/**
		* The `CCapturedResultReceiver` class is responsible for receiving captured results. It contains several callback functions for different types of results, including original image, decoded barcodes, recognized text lines, detected quads, deskewed images, and parsed results.
		*/
		class CVR_API CCapturedResultReceiver
		{
		protected:
			unsigned int observedResultItemTypes;
			const char* name;

			/**
			* Constructor.
			*/
			CCapturedResultReceiver();

		public:
			/**
			* Destructor.
			*/
			virtual ~CCapturedResultReceiver();

			/**
			* Gets the types of observed result items.
			*
			* @return Returns the types of observed result items.
			*
			*/
			unsigned int GetObservedResultItemTypes();

			/**
			* Gets the name of the captured result receiver.
			*
			* @return Returns the name of the captured result receiver.
			*
			*/
			const char* GetName() const;

			/**
			* Sets the name of the captured result receiver.
			*
			* @param [in] name The name of the captured result receiver.
			*
			*/
			void SetName(const char* name);

			/**
			* Callback function for all captured results. It will be called once for each captured result.
			*
			* @param [in] pResult The captured result.
			*
			*/
			virtual void OnCapturedResultReceived(CCapturedResult* pResult);

			/**
			* Callback function for original image result. It will be called once for each original image result.
			*
			* @param [in] pResult The original image result.
			*
			*/
			virtual void OnOriginalImageResultReceived(COriginalImageResultItem* pResult);

			/**
			* Callback function for decoded barcodes results. It will be called once for each decoded barcodes result.
			*
			* @param [in] pResult The decoded barcodes result.
			*
			*/
			virtual void OnDecodedBarcodesReceived(dbr::CDecodedBarcodesResult* pResult);

			/**
			* Callback function for recognized text lines results. It will be called once for each recognized text lines result.
			*
			* @param [in] pResult The recognized text lines result.
			*
			*/
			virtual void OnRecognizedTextLinesReceived(dlr::CRecognizedTextLinesResult* pResult);

			/**
			* Callback function for processed document results. It will be called once for each processed document result.
			*
			* @param [in] pResult The processed document result.
			*
			*/
			virtual void OnProcessedDocumentResultReceived(ddn::CProcessedDocumentResult* pResult);

			/**
			* Callback function for parsed results. It will be called once for each parsed result.
			*
			* @param [in] pResult The parsed result.
			*
			*/
			virtual void OnParsedResultsReceived(dcp::CParsedResult* pResult);

		};

		class CVR_API CCaptureVisionRouter;
		/**
		* The `CCapturedResultFilter` class is responsible for filtering captured results. It contains several callback functions for different types of results, including original image, decoded barcodes, recognized text lines, detected quads, deskewed images, and parsed results.
		*/
		class CVR_API CCapturedResultFilter
		{
		protected:
			unsigned int filteredResultItemTypes;
			const char* name;

			/**
			* Constructor.
			*/
			CCapturedResultFilter();

		public:
			/**
			* Destructor.
			*/
			virtual ~CCapturedResultFilter();

			/**
			* Gets the types of filtered result items.
			*
			* @return Returns the types of filtered result items.
			*
			*/
			unsigned int GetFilteredResultItemTypes();

			/**
			* Gets the name of the captured result filter.
			*
			* @return Returns the name of the captured result filter.
			*
			*/
			const char* GetName() const;

			/**
			* Sets the name of the captured result filter.
			*
			* @param [in] name The name of the captured result filter.
			*
			*/
			void SetName(const char* name);

			/**
			* Callback function for original image result. It will be called once for each original image result.
			*
			* @param [in] pResult The original image result.
			*
			*/
			virtual void OnOriginalImageResultReceived(COriginalImageResultItem* pResult);

			/**
			* Callback function for decoded barcodes results. It will be called once for each decoded barcodes result.
			*
			* @param [in] pResult The decoded barcodes result.
			*
			*/
			virtual void OnDecodedBarcodesReceived(dbr::CDecodedBarcodesResult* pResult);

			/**
			* Callback function for recognized text lines results. It will be called once for each recognized text lines result.
			*
			* @param [in] pResult The recognized text lines result.
			*
			*/
			virtual void OnRecognizedTextLinesReceived(dlr::CRecognizedTextLinesResult* pResult);

			/**
			* Callback function for processed document results. It will be called once for each processed document result.
			*
			* @param [in] pResult The processed document result.
			*
			*/
			virtual void OnProcessedDocumentResultReceived(ddn::CProcessedDocumentResult* pResult);

			/**
			* Callback function for parsed results. It will be called once for each parsed result.
			*
			* @param [in] pResult The parsed result.
			*
			*/
			virtual void OnParsedResultsReceived(dcp::CParsedResult* pResult);

			virtual void ClearStatus();

			/**
			* Initializes the filter. It will be called by Capture Vision Router before using the filter.
			*
			*/
			virtual void Init(CCaptureVisionRouter* router = nullptr);

			virtual const char* GetEncryptedString();

		};

		/**
		* The CCaptureStateListener class is an abstract class that defines a listener for capture state changes.
		*
		*/
		class CVR_API CCaptureStateListener
		{
		public:
			/**
			* Destructor.
			*/
			virtual ~CCaptureStateListener() {};

			/**
			* Called when the capture state changes.
			*
			* @param [in] state The new capture state.
			*
			* @return This method does not return a value. It is a pure virtual method and must be implemented by a derived class.
			*
			*/
			virtual void OnCaptureStateChanged(CaptureState state) = 0;
		};

		/**
		* The `CImageSourceStateListener` class is an abstract class that defines a listener for image source state changes.
		*
		*/
		class CVR_API CImageSourceStateListener
		{
		public:
			/**
			* Destructor.
			*/
			virtual ~CImageSourceStateListener() {};

			/**
			* This method is called when the state of the image source changes.
			*
			* @param [in] state The new state of the image source.
			*
			*/
			virtual void OnImageSourceStateReceived(ImageSourceState state) = 0;
		};

		/**
		* The `CPresetTemplate` class defines all preset template names of Capture Vision SDK.
		*
		*/
		class CPresetTemplate 
		{
		public:
			CVR_API static const char*  PT_DEFAULT;							//"Default", compatible with "default": DBR + DLR + DDN-Normalize
			CVR_API static const char*	PT_READ_BARCODES;					//"ReadBarcodes_Default", compatible with "read-barcodes"
			CVR_API static const char*	PT_RECOGNIZE_TEXT_LINES;			//"RecognizeTextLines_Default", compatible with "recognize-textlines"
			CVR_API static const char*	PT_DETECT_DOCUMENT_BOUNDARIES;		//"DetectDocumentBoundaries_Default", compatible with "detect-document-boundaries"
			CVR_API static const char*	PT_DETECT_AND_NORMALIZE_DOCUMENT;   //"DetectAndNormalizeDocument_Default", compatible with "detect-and-normalize-document"
			CVR_API static const char*	PT_NORMALIZE_DOCUMENT;				//"NormalizeDocument_Default", compatible with "normalize-document"

			/**
			 * @brief Represents a barcode reading mode where speed is prioritized.
			 *
			 * In this mode, the barcode reader will optimize for faster barcode detection
			 * and decoding, sacrificing some level of accuracy and read rate. It is suitable
			 * for situations where a quick response time is more important than perfect
			 * barcode recognition.
			 */
			CVR_API static const char* PT_READ_BARCODES_SPEED_FIRST; //"ReadBarcodes_SpeedFirst";

			/**
			 * @brief Represents a barcode reading mode where barcode read rate is prioritized.
			 *
			 * In this mode, the barcode reader will optimize for higher barcode read rates,
			 * even if it may sometimes result in reduced accuracy and speed. It is suitable for
			 * scenarios where maximizing the number of successfully read barcodes is critical.
			 */
			CVR_API static const char* PT_READ_BARCODES_READ_RATE_FIRST; //"ReadBarcodes_ReadRateFirst";

			/**
			* @brief Represents a barcode reading mode for single barcode code detection.
			*
			* In this mode, the barcode reader will focus on detecting and decoding a single
			* barcode code, ignoring any additional codes in the same image. It is efficient
			* when the target image has only one barcode.
			*/
			CVR_API static const char* PT_READ_SINGLE_BARCODE; //"ReadSingleBarcode";

			/**
			* @brief Represents a text recognition mode focused on recognizing numbers.
			*/
			CVR_API static const char* PT_RECOGNIZE_NUMBERS; //"RecognizeNumbers";

			/**
			 * @brief Represents a text recognition mode focused on recognizing alphabetic characters (letters).
			 *
			 */
			CVR_API static const char* PT_RECOGNIZE_LETTERS; //"RecognizeLetters";

			/**
			 * @brief Represents a text recognition mode that combines numbers and alphabetic characters (letters) recognition.
			 */
			CVR_API static const char* PT_RECOGNIZE_NUMBERS_AND_LETTERS; //"RecognizeNumbersAndLetters";

			/**
			 * @brief Represents a text recognition mode that combines numbers and uppercase letters recognition.
			 */
			CVR_API static const char* PT_RECOGNIZE_NUMBERS_AND_UPPERCASE_LETTERS; //"RecognizeNumbersAndUppercaseLetters";

			/**
			 * @brief Represents a text recognition mode focused on recognizing uppercase letters.
			 */
			CVR_API static const char* PT_RECOGNIZE_UPPERCASE_LETTERS; //"RecognizeUppercaseLetters";
		};

		/**
		* The CBufferedItemsManager class is used to manager the buffered items.
		*
		*/
		class CVR_API CBufferedItemsManager
		{
		public:
			/**
			* Destructor.
			*/
			virtual ~CBufferedItemsManager() {};

			/**
			* Sets the maximum number of buffered items.
			*
			* @param count The maximum number of buffered items.
			*/
			virtual void SetMaxBufferedItems(int count) = 0;

			/**
			* Gets the maximum number of buffered items.
			*
			* @return Returns the maximum number of buffered items.
			*/
			virtual int GetMaxBufferedItems() const = 0;

			/**
			* Gets the buffered character items.
			*
			* @return Returns the buffered character items.
			*/
			virtual dlr::CBufferedCharacterItemSet* GetBufferedCharacterItemSet() const = 0;
		};

		/**
		* The CCaptureVisionRouter class is what a user uses to interact with image-processing and semantic-processing products in their applications.
		* It accepts an image source and returns processing results which may contain Final results or Intermediate Results.
		*/
		class CVR_API CCaptureVisionRouter
		{
		protected:
			CaptureVisionInner * m_DCVInner;

		public:
			/**
			* Default constructor of `CCaptureVisionRouter` object.
			*/
			CCaptureVisionRouter();

			/**
			* Destructor of `CCaptureVisionRouter` object.
			*/
			~CCaptureVisionRouter();

			/**
			* Loads and initializes a template from a string.
			*
			* @param [in] content The string containing the template.
			* @param [in,out] errorMsgBuffer A buffer for error messages.
			* @param [in] errorMsgBufferLen The length of the error message buffer.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int InitSettings(const char* content, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);

			/**
			* Loads and initializes a template from a file.
			*
			* @param [in] filePath The path to the file containing the template.
			* @param [in,out] errorMsgBuffer A buffer for error messages.
			* @param [in] errorMsgBufferLen The length of the error message buffer.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int InitSettingsFromFile(const char* filePath, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);

			/**
			* Exports a specific template to a string.
			*
			* @param [in] templateName The name of the template to export.
			* @param [in] includeDefaultValues Specifies whether to include default values in the exported template.
			* @param [out] pErrorCode An error code.
			*
			* @return Returns a string containing the exported template. The string is allocated by the SDK and must be freed by calling `CoreModule::FreeBytes`.
			*
			*/
			char* OutputSettings(const char* templateName, bool includeDefaultValues = false, int* pErrorCode = NULL);

			/**
			* Exports a specific template to a file.
			*
			* @param [in] templateName The name of the template to export.
			* @param [in] filePath The path to the output file.
			* @param [in] includeDefaultValues Specifies whether to include default values in the exported template.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int OutputSettingsToFile(const char* templateName, const char* filePath, bool includeDefaultValues = false);

			/**
			* Retrieves a simplified version of the capture settings for a specific template.
			*
			* @param [in] templateName The name of the template.
			* @param [out] settings A pointer to a `SimplifiedCaptureVisionSettings` object.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int GetSimplifiedSettings(const char* templateName, SimplifiedCaptureVisionSettings* settings);
			
			/**
			* Updates a template with simplified capture settings.
			*
			* @param [in] templateName The name of the template to update.
			* @param [in] settings A pointer to a `SimplifiedCaptureVisionSettings` object.
			* @param [in,out] errorMsgBuffer A buffer for error messages.
			* @param [in] errorMsgBufferLen The length of the error message buffer.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int UpdateSettings(const char* templateName, const SimplifiedCaptureVisionSettings* settings, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);
			
			/**
			* Resets all templates to factory settings.
			*
			*/
			int ResetSettings();

			/**
			* Processes an image file to derive important information. It can optionally use a specified template for the capture.
			*
			* @param [in] filePath Specifies the path of the file to be processed.
			* @param [in] templateName Specifies the template to use for capturing. Default value is an empty string which means the factory default template.
			*
			* @return Returns a pointer to a `CCapturedResult` object containing the captured result.
			*
			*/
			CCapturedResult* Capture(const char* filePath, const char* templateName = "");

			/**
			* Processes an image file in memory to derive important information. It can optionally use a specified template for the capture.
			*
			* @param [in] fileBytes Specifies the memory location containing the image to be processed.
			* @param [in] fileSize Specifies the size of the image in bytes.
			* @param [in] templateName Specifies the template to use for capturing. Default value is an empty string which means the factory default template.
			*
			* @return Returns a pointer to a `CCapturedResult` object containing the captured result.
			*
			*/
			CCapturedResult* Capture(const unsigned char* fileBytes, int fileSize, const char* templateName = "");

			/**
			* Processes an image data to derive important information. It can optionally use a specified template for the capture.
			*
			* @param [in] pImageData Specifies the image data to be processed.
			* @param [in] templateName Specifies the template to use for capturing. Default value is an empty string which means the factory default template.
			*
			* @return Returns a pointer to a `CCapturedResult` object containing the captured result.
			*
			*/
			CCapturedResult* Capture(const CImageData* pImageData, const char* templateName = "");

			/**
			* Processes a multi-page image file to derive important information. It can optionally use a specified template for the capture.
			*
			* @param [in] filePath Specifies the path of the file to be processed.
			* @param [in] templateName Specifies the template to use for capturing. Default value is an empty string which means the factory default template.
			*
			* @return Returns a pointer to a `CCapturedResultArray` object containing the captured result.
			*
			*/
			CCapturedResultArray* CaptureMultiPages(const char* filePath, const char* templateName = "");

			/**
			* Processes a multi-page image file in memory to derive important information. It can optionally use a specified template for the capture.
			*
			* @param [in] fileBytes Specifies the memory location containing the image to be processed.
			* @param [in] fileSize Specifies the size of the image in bytes.
			* @param [in] templateName Specifies the template to use for capturing. Default value is an empty string which means the factory default template.
			*
			* @return Returns a pointer to a `CCapturedResultArray` object containing the captured result.
			*
			*/
			CCapturedResultArray* CaptureMultiPages(const unsigned char* fileBytes, int fileSize, const char* templateName = "");

			/**
			* Sets an image source to provide images for consecutive processing.
			*
			* @param [in] pAdaptor Specifies an object which has implemented the Image Source Adapter Interface.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int SetInput(CImageSourceAdapter* pAdapter);

			/**
			* Gets the attached image source adapter object of the capture vision router.
			*
			* @return Returns the attached image source adapter object of the capture vision router.
			*
			*/
			CImageSourceAdapter* GetInput();

			/**
			* Adds an object that listens to state changes of the image source.
			*
			* @param [in] listener Specifies a listening object of the type CImageSourceStateListener to be added.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int AddImageSourceStateListener(CImageSourceStateListener* listener);

			/**
			* Removes an object which listens to state changes of the image source.
			*
			* @param [in] listener Specifies a listening object of the type CImageSourceStateListener to be removed.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int RemoveImageSourceStateListener(CImageSourceStateListener* listener);

			/**
			* Adds an object as the receiver of captured results.
			*
			* @param [in] receiver Specifies a receiver object of the type CCapturedResultReceiver to be added.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int AddResultReceiver(CCapturedResultReceiver* receiver);

			/**
			* Removes an object which was added as a receiver of captured results.
			*
			* @param [in] receiver Specifies a receiver object of the type CCapturedResultReceiver to be removed.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int RemoveResultReceiver(CCapturedResultReceiver* receiver);

			/**
			* Adds an object as the filter of captured results.
			*
			* @param [in] filter Specifies a filter object of the type CCapturedResultFilter to be added.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int AddResultFilter(CCapturedResultFilter* filter);

			/**
			* Removes an object which was added as a filter of captured results.
			*
			* @param [in] filter Specifies a filter object of the type CCapturedResultFilter to be removed.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int RemoveResultFilter(CCapturedResultFilter* filter);

			/**
			* Adds an object that listens to the state changes of the capture process.
			*
			* @param [in] listener Specifies a listening object of the type CCaptureStateListener to be added.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int AddCaptureStateListener(CCaptureStateListener* listener);

			/**
			* Removes an object which listens to the state changes of the capture process.
			*
			* @param [in] listener Specifies a listening object of the type CCaptureStateListener to be removed.
			*
			* @return Returns an error code. Zero indicates success.
			*
			*/
			int RemoveCaptureStateListener(CCaptureStateListener* listener);

			/**
			* Starts to capture meaningful information such as barcodes, labels, document borders, etc. from images consecutively.
			*
			* @param [in] templateName Specifies a template to use for capturing. If not specified, an empty string is used which means the factory default template.
			*
			* @param [in] waitForThreadExit Indicates whether to wait for the capture process to complete before returning. The default value is false.
			*
			* @param [in,out] errorMsgBuffer Stores any error messages generated during the capturing process. If no buffer is provided, the error messages will not be output.
			*
			* @param [in] errorMsgBufferLen Specifies the length of the provided error message buffer. If no buffer is provided, this parameter is ignored.
			*
			* @return The function returns an integer value representing the success or failure of the capturing process. A value of 0 indicates success, while a non-zero value indicates failure. If an error message buffer is provided, any error messages generated during the capturing process will be stored in the buffer.
			*
			*/
			int StartCapturing(const char* templateName = "", bool waitForThreadExit = false, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);

			/**
			* Stops the capture process.
			*
			* @param [in] waitForRemainingTasks Indicates whether to wait for the remaining tasks to complete before returning. The default value is true.
			*
			* @param [in] waitForThreadExit Indicates whether to wait for the capture process to complete before returning. The default value is true.
			*
			*/
			void StopCapturing(bool waitForRemainingTasks = true, bool waitForThreadExit = true);

			/**
			* Frees the memory allocated for a string.
			*
			* @param [in] content The string whose memory needs to be freed.
			*
			*/
			static void FreeString(char* content);

			/**
			* Returns an `CIntermediateResultManager` object.
			*
			* @return Returns a pointer to the `CIntermediateResultManager` object.
			*
			*/
			CIntermediateResultManager* GetIntermediateResultManager();

			/**
			* Pauses the capture process. The current thread will be blocked until the capture process is resumed.
			*
			*/
			void PauseCapturing();

			/**
			* Resumes the capture process. The current thread will be unblocked after the capture process is resumed.
			*
			*/
			void ResumeCapturing();

			/**
			* Gets the buffered items manager.
			*
			* @return Returns a pointer to the `CBufferedItemsManager` object.
			*/
			CBufferedItemsManager* GetBufferedItemsManager();

			int GetParameterTemplateCount();
			// @param [in] index The index of the parameter template array.
			// @param [in,out] nameBuffer The buffer is allocated by caller and the recommended nameBufferLen is 256. The template name will be copied to the buffer.
			// @param [in] nameBufferLen The length of allocated buffer.
			int GetParameterTemplateName(const int index, char nameBuffer[], int nameBufferLen);

			/**
			* Appends a deep learning model to the memory buffer.
			*
			* @param [in] modelName The name of the model.
			* @param [in] modelBytes The bytes of the model.
			* @param [in] modelBytesLength The length of the model bytes.
			* @param [in] maxModelInstances The max instances created for the model.
			*
			* @return Returns 0 if succeeds, nonzero otherwise.
			*/
			static int AppendDLModelBuffer(const char* modelName, const unsigned char* modelBytes, int modelBytesLength, int maxModelInstances);

			/**
			* Clears all deep learning models from buffer to free up memory.
			*
			*/
			static void ClearDLModelBuffers();

			/**
			* Deprecated. Will be removed in future versions. Use AppendDLModelBuffer instead.
			*
			*/
			static int AppendModelBuffer(const char* modelName, const unsigned char* modelBytes, int modelBytesLength, int maxModelInstances);

			/**
			* Switch the capturing template during the image processing workflow.
			*
			* @param [in] templateName The name of the new capturing template to apply.
			* @param [in,out] errorMsgBuffer A buffer for error messages.
			* @param [in] errorMsgBufferLen The length of the error message buffer.
			*
			* @return Returns an error code. Zero indicates success.
			*/
			int SwitchCapturingTemplate(const char* templateName, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);

			/*
			* Sets the global number of threads used internally for model execution.
			*
			* @param [in] intraOpNumThreads Number of threads used internally for model execution. Valid range: [0, 256]
			*                               If the value is outside the range [0, 256], it will be treated as 0 (default).
			*/
			static void SetGlobalIntraOpNumThreads(int intraOpNumThreads = 0);

		private:
			CCaptureVisionRouter(const CCaptureVisionRouter& r);
			CCaptureVisionRouter& operator=(const CCaptureVisionRouter& r);
		};

		/**
		 * The CCaptureVisionRouterModule class defines general functions in the capture vision router module.
		 */
		class CVR_API CCaptureVisionRouterModule
		{
		public:
			/**
			 * Returns the version of the capture vision router module.
			 *
			 * @return Returns a const char pointer representing the version of the capture vision router module.
			 */
			static const char* GetVersion();
		};
#pragma pack(pop)
	}
}
#endif