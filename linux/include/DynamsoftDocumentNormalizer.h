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

#define DDN_VERSION                  "2.4.20.2248"

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
#pragma pack(1)

/**
* The SimplifiedDocumentNormalizerSettings struct contains settings for document normalization. It is a sub-parameter of SimplifiedCaptureVisionSettings.
*/
typedef struct SimplifiedDocumentNormalizerSettings
{
	/**Set the grayscale transformation modes with an array of enumeration GrayscaleTransformationMode.*/
	GrayscaleTransformationMode grayscaleTransformationModes[8];

	/**Set the grayscale enhancement modes with an array of enumeration GrayscaleEnhancementMode.*/
	GrayscaleEnhancementMode grayscaleEnhancementModes[8];

	/**Set the output image colour mode. Default value is ICM_COLOUR.*/
	ImageColourMode colourMode;

	/**Set the page size (width by height in pixels) of the normalized image.*/
	int pageSize[2];

	/**Set the brightness of the normalized image. Value range: [-100,100], default value: 0.*/
	int brightness;

	/**Set the contrast of the normalized image. Value range: [-100,100], default value: 0.*/
	int contrast;

	/**Set the maximum available threads count in one document normalization task.*/
	int maxThreadsInOneTask;

	/**Set the threshold for image shrinking. If the shorter edge size exceeds the specified threshold value,
	* the library will calculate the resized height and width of the image and and perform shrinking.
	*/
	int scaleDownThreshold;

	/** The minimum ratio between the target quadrilateral area and the total image area. Only those exceeding this value (measured in percentages) will be outputted.*/
	int minQuadrilateralAreaRatio;

	/**Set the number of documents expected to be detected.*/
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
			};

			/**
			 * The `CNormalizedImageElement` class stores an intermediate result whose type is normalized image.
			 */
			class DDN_API CNormalizedImageElement : public CRegionObjectElement
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CNormalizedImageElement() {};

			public:
				/**
				 * Gets the ImageData of current object.
				 *
				 * @return The image data.
				 *
				 * @see [CImageData]()
				 *
				 */
				virtual const CImageData* GetImageData() const = 0;
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
			 * The `CNormalizedImagesUnit` class represents an intermediate result unit whose type is normalized images.
			 */
			class DDN_API CNormalizedImagesUnit : public CIntermediateResultUnit
			{
			protected:
				/**
				 * Destructor
				 */
				virtual ~CNormalizedImagesUnit() {};

			public:
				/**
				 * Gets the count of CNormalizedImageElement objects in current object.
				 *
				 * @return Returns the count of CNormalizedImageElement objects in current object.
				 *
				 */
				virtual int GetCount() const = 0;

				/**
				 * Gets a NormalizedImage object from current object by specifying a index.
				 *
				 * @param index The index of the NormalizedImage object.
				 *
				 * @return Returns the NormalizedImage object got by the specific index.
				 *
				 * @see CNormalizedImageElement
				 */
				virtual const CNormalizedImageElement* GetNormalizedImage(int index) const = 0;

				/**
				 * Gets a NormalizedImage object from current object by specifying a index.
				 *
				 * @param index The index of the NormalizedImage object.
				 *
				 * @return Returns the NormalizedImage object got by the specific index.
				 *
				 */
				virtual const CNormalizedImageElement* operator[](int index) const = 0;

				/**
				 * Removes all the normalized images in current object.
				 *
				 */
				virtual void RemoveAllNormalizedImages() = 0;

				/**
				 * Sets the normalized image.
				 *
				 * @param element The normalized image to be set.
				 * @param matrixToOriginalImage The matrix to the original image.
				 * @return Returns 0 if successful, otherwise returns a negative value.
				 */
				virtual int SetNormalizedImage(const CNormalizedImageElement* element, const double matrixToOriginalImage[9] = IDENTITY_MATRIX) = 0;
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

		};

		/**
		 * The `CNormalizedImageResultItem` class stores a captured result item whose type is normalized image.
		 *
		 */
		class DDN_API CNormalizedImageResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CNormalizedImageResultItem() {};

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
			 * Gets the location of current object.
			 *
			 * @return Return Value: The location of current object.
			 *
			 * @see CQuadrilateral
			 */
			virtual CQuadrilateral GetLocation() const = 0;

		};

		/**
		 * The `CDetectedQuadsResult` class stores a collection of captured result items whose type is detected quads.
		 *
		 */
		class DDN_API CDetectedQuadsResult
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CDetectedQuadsResult() {}

		public:
			/**
			 * Gets the hash ID of the original image.
			 *
			 * @return Returns a pointer to a null-terminated string that represents the hash ID of the original image.
			 *
			 */
			virtual const char* GetOriginalImageHashId()const = 0;

			/**
			 * Gets the tag of the original image.
			 *
			 * @return Returns a pointer to a CImageTag object that represents the tag of the original image.
			 *
			 * @see CImageTag
			 *
			 */
			virtual const CImageTag* GetOriginalImageTag()const = 0;

			/**
			 * Get the rotation transformation matrix of the original image relative to the rotated image.
			 *
			 * @param [out] matrix A double array which represents the rotation transform matrix.
			 *
			 */
			virtual void GetRotationTransformMatrix(double matrix[9]) const = 0;

			/**
			 * Gets the number of detected quadrilaterals.
			 *
			 * @return Returns the number of detected quadrilaterals.
			 *
			 */
			virtual int GetItemsCount()const = 0;

			/**
			 * Gets the detected quadrilateral item at a specified index.
			 *
			 * @param [in] index The index of the detected quadrilateral to retrieve.
			 *
			 * @return Returns a pointer to a CDetectedQuadResultItem object that represents the detected quadrilateral at the specified index.
			 *
			 * @see CDetectedQuadResultItem
			 *
			 */
			virtual const CDetectedQuadResultItem* GetItem(int index)const = 0;

			/**
			 * Remove a specific item from the array in the detected quads.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CDetectedQuadResultItem* item) = 0;

			/**
			 * Check if the item is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CDetectedQuadResultItem* item) const = 0;

			/**
			 * Gets the error code of the detection operation.
			 *
			 * @return Returns the error code.
			 *
			 * @see ErrorCode
			 *
			 */
			virtual int GetErrorCode()const = 0;

			/**
			 * Gets the error message of the detection operation.
			 *
			 * @return Returns a pointer to a null-terminated string that represents the error message.
			 *
			 */
			virtual const char* GetErrorString()const = 0;

			/**
			 * Gets the detected quadrilateral item at a specified index.
			 *
			 * @param [in] index The index of the detected quadrilateral to retrieve.
			 *
			 * @return Returns a pointer to a CDetectedQuadResultItem object that represents the detected quadrilateral at the specified index.
			 *
			 */
			virtual const CDetectedQuadResultItem* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CDetectedQuadsResult object.
			 *
			 * @return An object of CDetectedQuadsResult.
			 */
			virtual CDetectedQuadsResult* Retain() = 0;

			/**
			 * Decreases the reference count of the CDetectedQuadsResult object.
			 *
			 */
			virtual void Release() = 0;
		};

		/**
		 * The `CNormalizedImagesResult` class stores a collection of captured result items whose type is normalized image.
		 *
		 */
		class DDN_API CNormalizedImagesResult
		{
		protected:
			/**
			 * Destructor
			 */
			virtual ~CNormalizedImagesResult() {};

		public:
			/**
			 * Gets the hash ID of the original image that was normalized.
			 *
			 * @return Returns the hash ID of the original image that was normalized.
			 *
			 */
			virtual const char* GetOriginalImageHashId()const = 0;

			/**
			 * Gets the tag of the original image that was normalized.
			 *
			 * @return Returns a pointer to the tag of the original image that was normalized.
			 *
			 * @see CImageTag
			 *
			 */
			virtual const CImageTag* GetOriginalImageTag()const = 0;

			/**
			 * Get the rotation transformation matrix of the original image relative to the rotated image.
			 *
			 * @param [out] matrix A double array which represents the rotation transform matrix.
			 *
			 */
			virtual void GetRotationTransformMatrix(double matrix[9]) const = 0;

			/**
			 * Gets the number of normalized images in the result.
			 *
			 * @return Returns the number of normalized images in the result.
			 *
			 */
			virtual int GetItemsCount()const = 0;

			/**
			 * Gets a specific normalized image from the result.
			 *
			 * @param [in] index The index of the normalized image to get.
			 *
			 * @return Returns a pointer to the normalized image at the specified index. If the index is out of range, returns nullptr.
			 *
			 * @see CNormalizedImageResultItem
			 *
			 */
			virtual const CNormalizedImageResultItem* GetItem(int index)const = 0;

			/**
			 * Remove a specific item from the array in the normalized images.
			 *
			 * @param [in] item The specific item to remove.
			 *
			 * @return Returns value indicating whether the deletion was successful or not.
			 *
			 */
			virtual int RemoveItem(const CNormalizedImageResultItem* item) = 0;

			/**
			 * Check if the item is present in the array.
			 *
			 * @param [in] item The specific item to check.
			 *
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CNormalizedImageResultItem* item) const = 0;

			/**
			 * Gets the error code of the operation.
			 *
			 * @return Returns the error code of the operation. A non-zero value indicates an error occurred.
			 *
			 * @see ErrorCode
			 *
			 */
			virtual int GetErrorCode()const = 0;

			/**
			 * Gets the error message of the operation.
			 *
			 * @return Returns the error message of the operation.
			 *
			 */
			virtual const char* GetErrorString()const = 0;

			/**
			 * Gets a specific normalized image from the result.
			 *
			 * @param [in] index The index of the normalized image to get.
			 *
			 * @return Returns a pointer to the normalized image at the specified index. If the index is out of range, returns nullptr.
			 *
			 */
			virtual const CNormalizedImageResultItem* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CNormalizedImagesResult object.
			 *
			 * @return An object of CNormalizedImagesResult.
			 */
			virtual CNormalizedImagesResult* Retain() = 0;

			/**
			 * Decreases the reference count of the CNormalizedImagesResult object.
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
			 * Create a Normalized Image Element object
			 *
			 * @return An object of CNormalizedImageElement
			 */
			static intermediate_results::CNormalizedImageElement* CreateNormalizedImageElement();

			/**
			 * Create a Detected Quad Element object
			 *
			 * @return An object of CDetectedQuadElement
			 */
			static intermediate_results::CDetectedQuadElement* CreateDetectedQuadElement();
		};
		
	}
}
#endif
