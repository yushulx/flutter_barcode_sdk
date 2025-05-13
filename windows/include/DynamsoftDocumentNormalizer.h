#pragma once

#if !defined(_WIN32) && !defined(_WIN64)

#ifdef __EMSCRIPTEN__
#define DDN_API __attribute__((used))
#else
#define DDN_API __attribute__((visibility("default")))
#include <stddef.h>
#endif

#else
#ifdef DDN_EXPORTS
#define DDN_API __declspec(dllexport)
#else
#define DDN_API 
#endif
#include <windows.h>
#endif
#include "DynamsoftCore.h"

#define DDN_VERSION                  "3.0.10.3895"

/**Enums section*/

/**
* @enum ImageColourMode
*
* Describes the output colour mode of the normalized image.
*/
typedef enum ImageColourMode
{
	/** Output image in colour mode. */
	ICM_COLOUR = 0,

	/** Output image in grayscale mode. */
	ICM_GRAYSCALE = 1,

	/** Output image in binary mode. */
	ICM_BINARY = 2
}ImageColourMode;

/**Structures section*/

#pragma pack(push)
#pragma pack(4)

/**
* The SimplifiedDocumentNormalizerSettings struct contains settings for document normalization. It is a sub-parameter of SimplifiedCaptureVisionSettings.
*/
typedef struct SimplifiedDocumentNormalizerSettings
{
	/**Sets the grayscale transformation modes with an array of enumeration GrayscaleTransformationMode.*/
	GrayscaleTransformationMode grayscaleTransformationModes[8];

	/**Sets the grayscale enhancement modes with an array of enumeration GrayscaleEnhancementMode.*/
	GrayscaleEnhancementMode grayscaleEnhancementModes[8];

	/**Sets the output image colour mode. Default value is ICM_COLOUR.*/
	ImageColourMode colourMode;

	/**Sets the page size (width by height in pixels) of the normalized image.*/
	int pageSize[2];

	/**Sets the brightness of the normalized image. Value range: [-100,100], default value: 0.*/
	int brightness;

	/**Sets the contrast of the normalized image. Value range: [-100,100], default value: 0.*/
	int contrast;

	/**Sets the maximum available threads count in one document normalization task.*/
	int maxThreadsInOneTask;

	/**Sets the threshold for image shrinking. If the shorter edge size exceeds the specified threshold value,
	* the library will calculate the resized height and width of the image and and perform shrinking.
	*/
	int scaleDownThreshold;

	/** The minimum ratio between the target quadrilateral area and the total image area. Only those exceeding this value (measured in percentages) will be output.*/
	int minQuadrilateralAreaRatio;

	/**Sets the number of documents expected to be detected.*/
	int expectedDocumentsCount;

	/**Reserved for future use.*/
	char reserved[504];
}SimplifiedDocumentNormalizerSettings;

#pragma pack(pop)

#ifdef __cplusplus

using namespace dynamsoft::basic_structures;
using namespace dynamsoft::intermediate_results;

namespace dynamsoft
{
	namespace ddn
	{
		namespace intermediate_results
		{
#pragma pack(push)
#pragma pack(4)
			/**
			 * The `CDetectedQuadElement` class stores an intermediate result whose type is detected quad.
			 */
			class DDN_API CDetectedQuadElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDetectedQuadElement() {};

			public:
				/**
				 * Gets the confidence as document boundary of current object.
				 *
				 * @return The confidence as document boundary of current object.
				 *
				 */
				virtual int GetConfidenceAsDocumentBoundary() const = 0;

				/**
				 * Sets the location of the detected quad element.
				 *
				 * @param location The location of the detected quad element.
				 * @return Returns 0 if success, otherwise an error code.
				 */
				virtual int SetLocation(const CQuadrilateral& location) = 0;
			};

			/**
			 * The `CDeskewedImageElement` class stores an intermediate result whose type is deskewed image.
			 */
			class DDN_API CDeskewedImageElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDeskewedImageElement() {};

			public:
				/**
				 * Sets the image data of the deskewed image element.
				 *
				 * @param imgData The image data to set.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetImageData(const CImageData* imgData) = 0;

				/**
				 * Gets the quadrilateral used for deskewing the image.
				 *
				 * @return A CQuadrilateral object representing the four corners of the quadrilateral used to deskew the image.
				 *
				 */
				virtual CQuadrilateral GetSourceDeskewQuad() const = 0;

			};

			class DDN_API CEnhancedImageElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CEnhancedImageElement() {};

			public:
				/**
				 * Sets the image data of the enhanced image element.
				 *
				 * @param imgData The image data to set.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetImageData(const CImageData* imgData) = 0;

			};

			/**
			 * The `CLongLinesUnit` class represents an intermediate result unit whose type is long lines.
			 */
			class DDN_API CLongLinesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CLongLinesUnit() {};

			public:
				/**
				 * Gets the count of LongLine objects in current object.
				 *
				 * @return The count of LongLine objects in current object.
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a LongLine object from current object by specifying an index.
				 *
				 * @param [in] index The index of the LongLine object.
				 *
				 * @return Returns the LongLine object got by the specific index.
				 *
				 * @see [CLineSegment]()
				 */
				virtual const CLineSegment* GetLongLine(int index) const = 0;

				/**
				 * Gets a LongLine object from current object by specifying an index.
				 *
				 * @param [in] index The index of the LongLine object.
				 *
				 * @return Returns the LongLine object got by the specific index.
				 *
				 */
				virtual const CLineSegment* operator[](int index) const = 0;

				/**
				 * Removes all the long lines in current object.
				 *
				 */
				virtual void RemoveAllLongLines() = 0;

				/**
				 * Removes a longline from current object by specifying an index.
				 *
				 * @param index The index of the longline to be removed.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveLongLine(int index) = 0;

				/**
				 * Adds a longline to current object.
				 *
				 * @param line The longline to be added.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int AddLongLine(const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * @brief Set the longline at the specified index.
				 *
				 * @param index The index of the longline to be set.
				 * @param line The longline to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetLongLine(int index, const CLineSegment& line, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CLogicLinesUnit` class represents an intermediate result unit containing logic lines.
			 */
			class DDN_API CLogicLinesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CLogicLinesUnit() {};

			public:
				/**
				 * Gets the number of logic lines in the unit.
				 *
				 * @return Returns the number of logic lines in the unit.
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a logic line at the specified index.
				 *
				 * @param [in] index The index of the logic line.
				 *
				 * @return Returns a pointer to the CLineSegment object at the specified index.
				 *
				 */
				virtual const CLineSegment* GetLogicLine(int index) const = 0;

				/**
				 * Gets a logic line at the specified index.
				 *
				 * @param [in] index The index of the logic line.
				 *
				 * @return Returns a pointer to the CLineSegment object at the specified index.
				 *
				 */
				virtual const CLineSegment* operator[](int index) const = 0;

				/**
				 * Removes all logic lines.
				 *
				 */
				virtual void RemoveAllLogicLines() = 0;

				/**
				 * Removes the logic line at the specified index.
				 *
				 * @param [in] index The index of the logic line to remove.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int RemoveLogicLine(int index) = 0;

				/**
				 * Adds a logic line.
				 *
				 * @param [in] logicline The logic line to add.
				 * @param [in] matrixToOriginalImage The matrix to original image.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int AddLogicLine(const CLineSegment& logicline, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * Sets the logic line at the specified index.
				 *
				 * @param [in] index The index of the logic line to set.
				 * @param [in] logicline The logic line to set.
				 * @param [in] matrixToOriginalImage The matrix to original image.
				 *
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 *
				 */
				virtual int SetLogicLine(int index, const CLineSegment& logicline, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CCornersUnit` class represents an intermediate result unit whose type is corners.
			 */
			class DDN_API CCornersUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CCornersUnit() {};

			public:
				/**
				 * Gets the count of Corner objects in current object.
				 *
				 * @return The count of Corner objects in current object.
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a Corner object from current object by specifying a index.
				 *
				 * @param [in] index The index of the Corner object.
				 * @param [in, out] corner The Corner object got by the specific index.
				 *
				 * @return Returns the error code.
				 *
				 * @see CCorner
				 * @see ErrorCode
				 */
				virtual int GetCorner(int index, CCorner* corner) const = 0;

				/**
				 * Removes all the corners in current object.
				 *
				 */
				virtual void RemoveAllCorners() = 0;

				/**
				 * Removes a corner from current object by specifying an index.
				 *
				 * @param index The index of the corner to be removed.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveCorner(int index) = 0;

				/**
				 * Adds a corner to current object.
				 *
				 * @param corner The corner to be added.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int AddCorner(const CCorner& corner, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * Sets the corner at the specified index.
				 *
				 * @param index The index of the corner to be set.
				 * @param corner The corner to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetCorner(int index, const CCorner& corner, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CCandidateQuadEdgesUnit` class represents an intermediate result unit whose type is candidate quad edges.
			 */
			class DDN_API CCandidateQuadEdgesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CCandidateQuadEdgesUnit() {};

			public:
				/**
				 * Gets the count of CandidateQuadEdge objects in current object.
				 *
				 * @return The count of CandidateQuadEdge objects in current object.
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a CandidateQuadEdge object from current object by specifying a index.
				 *
				 * @param [in] index The index of the CandidateQuadEdge object.
				 * @param [in, out] edge The CandidateQuadEdge object got by the specific index.
				 * @return Returns the error code.
				 */
				virtual int GetCandidateQuadEdge(int index, CEdge* edge) const = 0;

				/**
				 * Removes all the candidate quad edges in current object.
				 *
				 */
				virtual void RemoveAllCandidateQuadEdges() = 0;

				/**
				 * Removes a candidate quad edge from current object by specifying an index.
				 *
				 * @param index The index of the candidate quad edge to be removed.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveCandidateQuadEdge(int index) = 0;

				/**
				 * Adds a candidate quad edge to current object.
				 *
				 * @param edge The candidate quad edge to be added.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int AddCandidateQuadEdge(const CEdge& edge, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * Sets the candidate quad edge at the specified index.
				 *
				 * @param index The index of the candidate quad edge to be set.
				 * @param edge The candidate quad edge to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetCandidateQuadEdge(int index, const CEdge& edge, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CDetectedQuadsUnit` class represents an intermediate result unit whose type is detected quads.
			 */
			class DDN_API CDetectedQuadsUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDetectedQuadsUnit() {};

			public:
				/**
				 * Gets the count of DetectedQuad objects in current object.
				 *
				 * @return Returns the count of DetectedQuad objects in current object.
				 *
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a DetectedQuad object from current object by specifying a index.
				 *
				 * @param index The index of the DetectedQuad object.
				 *
				 * @return Returns the DetectedQuad object got by the specific index.
				 *
				 * @see CDetectedQuadElement
				 */
				virtual const CDetectedQuadElement* GetDetectedQuad(int index) const = 0;

				/**
				 * Gets a DetectedQuad object from current object by specifying a index.
				 *
				 * @param index The index of the DetectedQuad object.
				 *
				 * @return Returns the DetectedQuad object got by the specific index.
				 *
				 */
				virtual const CDetectedQuadElement* operator[](int index) const = 0;

				/**
				 * Removes all the detected quads in current object.
				 *
				 */
				virtual void RemoveAllDetectedQuads() = 0;

				/**
				 * Removes a detected quad from current object by specifying an index.
				 *
				 * @param index The index of the detected quad to be removed.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int RemoveDetectedQuad(int index) = 0;

				/**
				 * Adds a detected quad to current object.
				 *
				 * @param element The detected quad to be added.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int AddDetectedQuad(const CDetectedQuadElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;

				/**
				 * Sets the detected quad at the specified index.
				 *
				 * @param index The index of the detected quad to be set.
				 * @param element The detected quad to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetDetectedQuad(int index, const CDetectedQuadElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			/**
			 * The `CDeskewedImageUnit` class represents an intermediate result unit whose type is deskewed images.
			 */
			class DDN_API CDeskewedImageUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CDeskewedImageUnit() {};

			public:
				/**
				 * Gets a CDeskewedImageElement object from current unit.
				 *
				 * @return Returns the CDeskewedImageElement object.
				 *
				 * @see CDeskewedImageElement
				 */
				virtual const CDeskewedImageElement* GetDeskewedImage() const = 0;

				/**
				 * Sets the deskewed image.
				 *
				 * @param element The deskewed image to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetDeskewedImage(const CDeskewedImageElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
			};

			class DDN_API CEnhancedImageUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CEnhancedImageUnit() {};

			public:
				/**
				 * Gets a CEnhancedImage object from current object.
				 *
				 * @return Returns the CEnhancedImage object.
				 *
				 * @see CEnhancedImageElement
				 */
				virtual const CEnhancedImageElement* GetEnhancedImage() const = 0;

				/**
				 * Sets the enhanced image.
				 *
				 * @param element The enhanced image to be set.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetEnhancedImage(const CEnhancedImageElement* element) = 0;
			};
		}

		/**
		 * The `CDetectedQuadResultItem` class represents an item that stores the detection result of a quadrilateral object. It inherits from the `CCapturedResultItem` class.
		 *
		 */
		class DDN_API CDetectedQuadResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CDetectedQuadResultItem() {};

		public:
			/**
			 * Gets the location of current object.
			 *
			 * @return Return Value: The location of current object.
			 *
			 * @see CQuadrilateral
			 */
			virtual CQuadrilateral GetLocation() const = 0;

			/**
			 * Gets the confidence of current object as a document boundary.
			 *
			 * @return Return Value: The confidence as document boundary of the detected quad result.
			 *
			 */
			virtual int GetConfidenceAsDocumentBoundary() const = 0;

			/**
			 * Gets the status of current object as a verified document boundary.
			 *
			 * @return Return the CrossVerificationStatus of the detected quad result.
			 *
			 */
			virtual CrossVerificationStatus GetCrossVerificationStatus() const = 0;

			/**
			 * Sets the status of current object.
			 *
			 * @param status The CrossVerificationStatus to be set.
			 *
			 */
			virtual void SetCrossVerificationStatus(CrossVerificationStatus status) = 0;
		};

		/**
		 * The `CDeskewedImageResultItem` class stores a captured result item whose type is deskewed image.
		 *
		 */
		class DDN_API CDeskewedImageResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CDeskewedImageResultItem() {};

		public:
			/**
			 * Gets the ImageData of current object.
			 *
			 * @return The image data.
			 *
			 * @see CImageData
			 *
			 */
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Gets the quadrilateral used for deskewing the image.
			 *
			 * @return A CQuadrilateral object representing the four corners of the quadrilateral used to deskew the image.
			 *
			 */
			virtual CQuadrilateral GetSourceDeskewQuad() const = 0;

			/**
			 * Gets the status of current object as a verified deskewed image.
			 *
			 * @return Return the CrossVerificationStatus of the deskewed image result.
			 *
			 */
			virtual CrossVerificationStatus GetCrossVerificationStatus() const = 0;

			/**
			 * Sets the status of current object.
			 *
			 * @param status The CrossVerificationStatus to be set.
			 *
			 */
			virtual void SetCrossVerificationStatus(CrossVerificationStatus status) = 0;

			/**
			 * Gets the transformation matrix from the original image coordinate system to the local coordinate system.
			 *
			 * @param [out] matrix A double array of size 9, representing the 3x3 transformation matrix that converts
			 *                     coordinates from the original image to the local image.
			 */
			virtual void GetOriginalToLocalMatrix(double matrix[9]) const = 0;
		};

		/**
		 * The `CEnhancedImageResultItem` class stores a captured result item whose type is enhanced image.
		 *
		 */
		class DDN_API CEnhancedImageResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CEnhancedImageResultItem() {};

		public:
			/**
			 * Gets the ImageData of current object.
			 *
			 * @return The image data.
			 *
			 * @see CImageData
			 *
			 */
			virtual const CImageData* GetImageData() const = 0;

			/**
			 * Gets the transformation matrix from the original image coordinate system to the local coordinate system.
			 *
			 * @param [out] matrix A double array of size 9, representing the 3x3 transformation matrix that converts
			 *                     coordinates from the original image to the local image.
			 */
			virtual void GetOriginalToLocalMatrix(double matrix[9]) const = 0;
		};

		/**
		 * The `CDocumentResult` class stores a collection of captured result items.
		 *
		 */
		class DDN_API CProcessedDocumentResult : public CCapturedResultBase
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CProcessedDocumentResult() {};

		public:

			/**
			 * Gets the count of detected quad result items.
			 *
			 * @return The number of detected quad result items.
			 *
			 */
			virtual int GetDetectedQuadResultItemsCount()const = 0;

			/**
			 * Gets the count of deskewed image result items.
			 *
			 * @return The number of deskewed image result items.
			 *
			 */
			virtual int GetDeskewedImageResultItemsCount()const = 0;

			/**
			 * Gets the count of enhanced image result items.
			 *
			 * @return The number of enhanced image result items.
			 *
			 */
			virtual int GetEnhancedImageResultItemsCount()const = 0;

			/**
			 * Retrieves the detected quad result item at the specified index.
			 *
			 * @param [in] index The index of the detected quad result item.
			 *
			 * @return A CDetectedQuadResultItem object representing the detected quad result at the specified index.
			 *
			 */
			virtual const CDetectedQuadResultItem* GetDetectedQuadResultItem(int index)const = 0;

			/**
			 * Retrieves the deskewed image result item at the specified index.
			 *
			 * @param [in] index The index of the deskewed image result item.
			 *
			 * @return A CDeskewedImageResultItem object representing the deskewed image result at the specified index.
			 *
			 */
			virtual const CDeskewedImageResultItem* GetDeskewedImageResultItem(int index)const = 0;

			/**
			 * Retrieves the enhanced image result item at the specified index.
			 *
			 * @param [in] index The index of the enhanced image result item.
			 *
			 * @return A CEnhancedImageResultItem object representing the deskewed image result at the specified index.
			 *
			 */
			virtual const CEnhancedImageResultItem* GetEnhancedImageResultItem(int index)const = 0;

			/**
			 * Removes the DetectedQuadResultItem from the array in the recognition result.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CDetectedQuadResultItem* item) = 0;

			/**
			 * Checks if the DetectedQuadResultItem is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CDetectedQuadResultItem* item) const = 0;

			/**
			 * Removes the DeskewedImageResultItem from the array in the recognition result.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CDeskewedImageResultItem* item) = 0;

			/**
			 * Checks if the DeskewedImageResultItem is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CDeskewedImageResultItem* item) const = 0;

			/**
			 * Removes the EnhancedImageResultItem from the array in the recognition result.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CEnhancedImageResultItem* item) = 0;

			/**
			 * Checks if the EnhancedImageResultItem is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CEnhancedImageResultItem* item) const = 0;

			/**
			 * Increases the reference count of the CProcessedDocumentResult object.
			 *
			 * @return An object of CProcessedDocumentResult.
			 */
			virtual CProcessedDocumentResult* Retain() = 0;

			/**
			 * Decreases the reference count of the CProcessedDocumentResult object.
			 *
			 */
			virtual void Release() = 0;

		};


		/**
		 * The CDocumentNormalizerModule class defines general functions in the document normalizer module.
		 */
		class DDN_API CDocumentNormalizerModule
		{
		public:
			/**
			 * Returns the version of the document normalizer module.
			 *
			 * @return Returns a const char pointer representing the version of the document normalizer module.
			 */
			static const char* GetVersion();

			/**
			 * Creates a CDeskewedImageElement object
			 *
			 * @return An object of CDeskewedImageElement
			 */
			static intermediate_results::CDeskewedImageElement* CreateDeskewedImageElement();

			/**
			 * Creates a CDetectedQuadElement object
			 *
			 * @return An object of CDetectedQuadElement
			 */
			static intermediate_results::CDetectedQuadElement* CreateDetectedQuadElement();

			/**
			 * Creates a CEnhancedImageElement object
			 *
			 * @return An object of CEnhancedImageElement
			 */
			static intermediate_results::CEnhancedImageElement* CreateEnhancedImageElement();
		};
#pragma pack(pop)
	}
}
#endif
