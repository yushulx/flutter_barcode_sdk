#pragma once

#if !defined(_WIN32) && !defined(_WIN64)
#define DCP_API __attribute__((visibility("default")))
#else //windows
#if defined(DCP_EXPORTS)
#define DCP_API __declspec(dllexport)
#else
#define DCP_API __declspec(dllimport)
#endif
#endif
#include "DynamsoftCore.h"

#define DCP_VERSION "3.2.50.6520"
/**
 * @enum MappingStatus 
 *
 * Describes the mapping status of a parsed field.
 */
typedef enum MappingStatus
{
	/** The field has no mapping specified. */
	MS_NONE,

	/** Find a mapping for the field value. */
	MS_SUCCEEDED,

	/** Failed to find a mapping for the field value. */
	MS_FAILED
} MappingStatus;

/** 
 * @enum ValidationStatus
 * 
 * Describes the validation status of a parsed field.
 */
typedef enum ValidationStatus
{
	/** The field has no validation specified. */
	VS_NONE,

	/** The validation for the field has been succeeded. */
	VS_SUCCEEDED,

	 /** The validation for the field has been failed. */
	VS_FAILED
} ValidationStatus;




#ifdef __cplusplus
using namespace dynamsoft::basic_structures;

namespace dynamsoft
{
	namespace dcp
	{
#pragma pack(push)
#pragma pack(4)
		class CCodeType
		{
		public:
			DCP_API static const char*  CT_MRTD_TD1_ID;					//"MRTD_TD1_ID"
			DCP_API static const char*  CT_MRTD_TD2_ID;					//"MRTD_TD2_ID"
			DCP_API static const char*  CT_MRTD_TD2_VISA;				//"MRTD_TD2_VISA"
			DCP_API static const char*  CT_MRTD_TD3_PASSPORT;			//"MRTD_TD3_PASSPORT"
			DCP_API static const char*  CT_MRTD_TD3_VISA;				//"MRTD_TD3_VISA"
			DCP_API static const char*  CT_MRTD_TD2_FRENCH_ID;			//"MRTD_TD2_FRENCH_ID"
			DCP_API static const char*  CT_AAMVA_DL_ID;					//"AAMVA_DL_ID"
			DCP_API static const char*  CT_AAMVA_DL_ID_WITH_MAG_STRIPE;	//"AAMVA_DL_ID_WITH_MAG_STRIPE"
			DCP_API static const char*  CT_SOUTH_AFRICA_DL;				//"SOUTH_AFRICA_DL"
			DCP_API static const char*  CT_AADHAAR;						//"AADHAAR"
			DCP_API static const char*  CT_VIN;							//"VIN"
			DCP_API static const char*  CT_GS1_AI;						//"GS1_AI"
		};
		
		/**
		 * The `CParsedResultItem` class represents a item parsed by code parser sdk. It is derived from `CCapturedResultItem`.
		 *
		 */
		class DCP_API CParsedResultItem : public CCapturedResultItem
		{
		protected:
			/**
			 * Destructor.
			 * 
			 */
			virtual ~CParsedResultItem() {};

		public:
			/**
			 * Gets the parsed result as a JSON formatted string.
			 * 
			 * @return Returns a JSON formatted string representing the parsed result.
			 * 
			 */
			virtual const char* GetJsonString() const = 0;

			/**
			 * Gets the code type of the parsed result.
			 * 
			 * @return Returns a string value representing the code type.
			 * 
			 */
			virtual const char* GetCodeType() const = 0;

			/**
			 * Gets the value of a specified field from the parsed result.
			 * 
			 * @param [in] fieldName The name of the field.
			 * 
			 * @return Returns a string representing the specified field value.
			 * 
			 */
			virtual const char* GetFieldValue(const char* fieldName) const = 0;

			/**
			 * Gets the mapping status of a specified field from the parsed result.
			 * 
			 * @param [in] fieldName The name of the field.
			 * 
			 * @return Returns a MappingStatus enumeration value representing the mapping status of a specified field.
			 * 
			 */
			virtual MappingStatus GetFieldMappingStatus(const char* fieldName) const = 0;

			/**
			 * Gets the validation status of a specified field from the parsed result.
			 * 
			 * @param [in] fieldName The name of the field.
			 * 
			 * @return Returns a ValidationStatus enumeration value representing the validation status of a specified field.
			 * 
			 */
			virtual ValidationStatus GetFieldValidationStatus(const char* fieldName) const = 0;

			/**
			* Gets the raw string of a specified field from the parsed result.
			*
			* @param [in] fieldName The name of the field.
			*
			* @return Returns a string representing the specified field raw string.
			*
			*/
			virtual const char* GetFieldRawValue(const char* fieldName) const = 0;

			/**
			* Gets the total number of parsed fields.
			*
			* @return Returns an integer representing the count of parsed fields.
			*
			*/
			virtual int GetFieldCount() const = 0;

			/**
			* Gets the name of a specific parsed field by its index.
			*
			* @param [in] index The index of the parsed field.
			*
			* @return Returns a string representing the specified field name. If the field is nested, the name includes all parent fields, separated by a dot (.).
			* The format follows this pattern: <root_field>[.<child_field1>[.<child_field2>...]]
			*
			*/
			virtual const char* GetFieldName(int index) const = 0;
		};

		/**
		 * The `CParsedResult` class represents the result of code parsing process. It provides access to information about the parsed items, the source image, and any errors that occurred during the process.
		 *
		 */
		class DCP_API CParsedResult : public CCapturedResultBase
		{
		protected:
			/**
			 * Destructor.
			 * 
			 */
			virtual ~CParsedResult() {};

		public:
			/**
			 * Gets the number of parsed result items in the parsed result.
			 * 
			 * @return Returns the number of parsed result items in the parsed result.
			 * 
			 */
			virtual int GetItemsCount()const = 0;
			 
			/**
			 * Gets the parsed result item at the specified index.
			 * 
			 * @param [in] index The zero-based index of the parsed result item to retrieve.
			 * 
			 * @return Returns a pointer to the CParsedResultItem object at the specified index.
			 * 
			 */
			virtual const CParsedResultItem* GetItem(int index)const = 0;

			/**
			 * Remove a specific item from the array in the parsed results.
			 * 
			 * @param [in] item The specific item to remove.
			 * 
			 * @return Return value indicating whether the deletion was successful or not.
			 * 
			 */
			virtual int RemoveItem(const CParsedResultItem* item) = 0;

			/**
			 * Check if the item is present in the array.
			 * 
			 * @param [in] item The specific item to check.
			 * 
			 * @return Returns a bool value indicating whether the item is present in the array or not.
			 *
			 */
			virtual bool HasItem(const CParsedResultItem* item) const = 0;

			/**
			 * Gets the parsed result item at the specified index.
			 *
			 * @param [in] index The zero-based index of the parsed result item to retrieve.
			 *
			 * @return Returns a pointer to the CParsedResultItem object at the specified index.
			 *
			 */
			virtual const CParsedResultItem* operator[](int index) const = 0;

			/**
			 * Increases the reference count of the CParsedResult object.
			 *
			 * @return An object of CParsedResult.
			 */
			virtual CParsedResult* Retain() = 0;

			/**
			* Decreases the reference count of the CParsedResult object.
			*
			*/
			virtual void Release() = 0;

			/**
			 * Adds a specific item to the array in the parsed result.
			 *
			 * @param [in] item The specific item to add.
			 *
			 * @return Returns value indicating whether the addition was successful or not.
			 *
			 */
			virtual int AddItem(const CParsedResultItem* item) = 0;
		};

		class CodeParserInner;

		/**
		 * The `CCodeParser` class provides methods for parsing code data for human-readable results.
		 *
		 */
		class DCP_API CCodeParser
		{
		protected:
			CodeParserInner * m_DCPInner;
		public:
			/**
			 * Default constructor of a CCodeParser object.
			 */
			CCodeParser();

			/**
			 * Destructor of a CCodeParser object.
			 */
			~CCodeParser();

			/**
			 * Parses code data for human-readable results.
			 *
			 * @param [in] pData The array of bytes which contain the code string.
			 * @param [in] length The length of the code string bytes.
			 * @param [in] taskSettingNameOptional The name of CodeParserTaskSetting which defines the settings used for code parsing.
			 * @param [in,out] errorCodeOptional The error code.
			 *
			 * @return Returns CParsedResultItem which stores the human-readable results.
			 *
			 */
			CParsedResultItem* Parse(const unsigned char* pData, int length, const char* taskSettingName = "", int* errorCode = NULL);
			
			/**
			 * Initialize settings from a given string.
			 * 
			 * @param [in] content A JSON string that represents the content of the settings. [in,out] errorMsgBufferOptional The buffer is allocated by caller and the recommended length is 512. The error message will be copied to the buffer.
			 * @param [in,out] errorMsgBufferOptional The buffer is allocated by caller and the recommended length is 512. The error message will be copied to the buffer.
			 * @param [in] errorMsgBufferLenOptional The length of the allocated buffer.
			 * 
			 * @return Returns error code.
			 * 
			 */
			int InitSettings(const char* content, char errorMsgBuffer[] = NULL, int errorMsgBufferLen = 0);
			
			/**
			 * Initialize settings from a given file.
			 * 
			 * @param [in] filePath The path of the settings file.
			 * @param [in,out] errorMsgBuffer (Optional) The buffer is allocated by caller and the recommended length is 512. The error message will be copied to the buffer.
			 * @param [in] errorMsgBufferLen (Optional) The length of the allocated buffer.
			 * 
			 * @return Returns error code. 
			 * 
			 */
			int InitSettingsFromFile(const char* filePath, char errorMsgBuffer[] = NULL, int errorMsgBufferLen = 0);
			
			/**
			 * Reset all parameters to default values.
			 * 	
			 * @return Returns error code.
			 * 
			 */
			int ResetSettings();
		private:
			CCodeParser(const CCodeParser& r);
			CCodeParser& operator=(const CCodeParser& r);
		};

		/**
		 * The CCodeParserModule class defines general functions in the code parser  module.
		 */
		class DCP_API CCodeParserModule
		{
		public:
			/**
			 * Get version information of SDK.
			 * 	
			 * @return Returns the version information string.
			 * 
			 */
			static const char* GetVersion();
		};

#pragma pack(pop)
	}
}
#endif