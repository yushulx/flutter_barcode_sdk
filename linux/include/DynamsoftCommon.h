#ifndef __DYNAMSOFT_COMMON_H__
#define __DYNAMSOFT_COMMON_H__

#ifndef DBR_API
#if !defined(_WIN32) && !defined(_WIN64)
#define DBR_API __attribute__((visibility("default")))
#else
#ifdef DBR_EXPORTS
#define DBR_API __declspec(dllexport)
#else
#define DBR_API 
#endif
#endif
#endif

/**No license.*/
#define DMERR_NO_LICENSE -20000

/**The Handshake Code is invalid.*/
#define DMERR_HANDSHAKE_CODE_INVALID -20001

/**Failed to read or write license buffer. */
#define DMERR_LICENSE_BUFFER_FAILED	-20002

/**Failed to synchronize license info with license tracking server. */
#define DMERR_LICENSE_SYNC_FAILED	-20003

/**Device dose not match with buffer. */
#define DMERR_DEVICE_NOT_MATCH	-20004

/**Failed to bind device. */
#define DMERR_BIND_DEVICE_FAILED	-20005

/**Interface InitLicenseFromLTS can not be used together with other license initiation interfaces. */
#define DMERR_LICENSE_INTERFACE_CONFLICT -20006

/**License Client dll is missing.*/
#define DMERR_LICENSE_CLIENT_DLL_MISSING -20007

/**Instance count is over limit.*/
#define DMERR_INSTANCE_COUNT_OVER_LIMIT -20008

/**Interface InitLicenseFromLTS has to be called before creating any SDK objects.*/
#define DMERR_LICENSE_INIT_SEQUENCE_FAILED -20009

/**Trial License*/
#define DMERR_TRIAL_LICENSE -20010

/**Failed to reach License Tracking Server.*/
#define DMERR_FAILED_TO_REACH_LTS -20200


/**
* @enum DM_DeploymentType
*
* Describes the deployment type.
*/
typedef enum DM_DeploymentType
{
	/**Server deployment type*/
	DM_DT_SERVER = 1,

	/**Desktop*/
	DM_DT_DESKTOP = 2,

	/**Embedded device deployment type*/
	DM_DT_EMBEDDED_DEVICE = 6,

	/**OEM deployment type*/
	DM_DT_OEM = 7
}DM_DeploymentType;

/**
* @enum DM_LicenseModule
*
* Describes the license module.
*/
typedef enum DM_LicenseModule
{
	/**One-D barcodes license module*/
	DM_LM_ONED = 1,

	/**QR Code barcodes license module*/
	DM_LM_QR_CODE = 2,

	/**PDF417 barcodes license module*/
	DM_LM_PDF417 = 3,

	/**Datamatrix barcodes license module*/
	DM_LM_DATAMATRIX = 4,

	/**Aztec barcodes license module*/
	DM_LM_AZTEC = 5,

	/**MAXICODE barcodes license module*/
	DM_LM_MAXICODE = 6,

	/**Patch code barcodes license module*/
	DM_LM_PATCHCODE = 7,

	/**GS1 Databar barcodes license module*/
	DM_LM_GS1_DATABAR = 8,

	/**GS1 Composite barcodes license module*/
	DM_LM_GS1_COMPOSITE = 9,

	/**Postal code barcodes license module*/
	DM_LM_POSTALCODE = 10,

	/**DotCode barcodes license module*/
	DM_LM_DOTCODE = 11,

	/**Intermediate result license module*/
	DM_LM_INTERMEDIATE_RESULT = 12,

	/**Datamatrix DPM(Direct Part Marking) license module*/
	DM_LM_DPM = 13,

	/**Nonstandard barcodes license module*/
	DM_LM_NONSTANDARD_BARCODE = 16
}DM_LicenseModule;

/**
* @enum DM_UUIDGenerationMethod
*
* Describes the UUID generation method.
*/
typedef enum DM_UUIDGenerationMethod
{
	/**Generates UUID with randon values.*/
	DM_UUIDGM_RANDOM = 1,

	/**Generates UUID based on hardware info.*/
	DM_UUIDGM_HARDWARE = 2
}DM_UUIDGenerationMethod;

/**
* @enum DM_ChargeWay
*
* Describes the charge way.
*/
typedef enum DM_ChargeWay
{
	/**The charge way automatically determined by the license server.*/
	DM_CW_AUTO = 0,

	/**Charges by the count of devices.*/
	DM_CW_DEVICE_COUNT = 1 ,

	/**Charges by the count of barcode scans.*/
	DM_CW_SCAN_COUNT = 2,

	/**Charges by the count of concurrent devices.*/
	DM_CW_CONCURRENT_DEVICE_COUNT = 3,

	/**Charges by the count of app domains.*/
	DM_CW_APP_DOMIAN_COUNT = 6,

	/**Charges by the count of active devices.*/
	DM_CW_ACTIVE_DEVICE_COUNT = 8,

	/**Charges by the count of instances.*/
	DM_CW_INSTANCE_COUNT = 9,

	/**Charges by the count of concurrent instances.*/
	DM_CW_CONCURRENT_INSTANCE_COUNT = 10
}DM_ChargeWay;


#pragma pack(push)
#pragma pack(1)
/**
* @defgroup DBRLTSConnectionParameters DBRLTSConnectionParameters
* @{
*/
/**
* Stores DBR License Tracking Server connection parameters.
*
*/
typedef struct tagDM_LTSConnectionParameters
{
	/**The URL of the license tracking server.*/
	const char* mainServerURL;

	/**The URL of the standby license tracking server.*/
	const char* standbyServerURL;

	/**The handshake code.*/
	const char* handshakeCode;

	/**The session password of the handshake code set in license tracking server.*/
	const char* sessionPassword;

	/**Sets the deployment type.*/
	DM_DeploymentType deploymentType;

	/**Sets the charge way.*/
	DM_ChargeWay chargeWay;

	/**Sets the method to generate UUID.*/
	DM_UUIDGenerationMethod UUIDGenerationMethod;

	/**Sets the max days to buffer the license info.*/
	int maxBufferDays;

	/**Sets the count of license modules to use.*/
	int limitedLicenseModulesCount;

	/**Sets the license modules to use.*/
	DM_LicenseModule* limitedLicenseModules;

	/**Sets the max concurrent instance count.*/
	int maxConcurrentInstanceCount;

	/**Reserved memory for struct. The length of this array indicates the size of the memory reserved for this struct.*/
	char reserved[60];
}DM_LTSConnectionParameters;

/**
* @}defgroup DBRLTSConnectionParameters
*/
#pragma pack(pop)

DBR_API void* DM_CreateImgReference();

DBR_API void DM_DestroyImgReference(void** ref);

#endif
