#pragma once

#define DYNAMSOFT_LICENSE_VERSION "4.0.10.3895"

#if !defined(_WIN32) && !defined(_WIN64)
#define LIC_API __attribute__((visibility("default")))
#include <stddef.h>
#else
#ifdef LIC_EXPORTS
#define LIC_API __declspec(dllexport)
#else
#define LIC_API
#endif
#include <windows.h>
#endif

#ifdef __cplusplus
namespace dynamsoft
{
	namespace license
	{
#pragma pack(push)
#pragma pack(4)
		/**
		* The CLicenseManager class provides a set of APIs to manage SDK licensing.
		*
		*/
		class LIC_API CLicenseManager
		{
		public:
			/**
			* It is used to initialize the license using a license key.
			*
			* @param [in] pLicense The license key as a string.
			* @param [in, out] errorMsgBuffer The buffer is allocated by caller and the recommended length is 256. The error message will be copied to the buffer. It can be set to NULL if error messages are not required.
			* @param [in] errorMsgBufferLen The length of the error message buffer. It is ignored if errorMsgBuffer is set to NULL.
			*
			* @return Returns 0 if the license is initialized successfully, a negative value indicating an error otherwise.
			*
			*/
			static int InitLicense(const char* pLicense, char errorMsgBuffer[] = NULL, const int errorMsgBufferLen = 0);
			/**
			 * It is used to set the friendly name of the device.
			 * 
			 * @param [in] name The friendly name of the device.
			 * 
			 * @return Returns 0 if the friendly name is set successfully, a negative value indicating an error otherwise.
			 *
			 */
			static int SetDeviceFriendlyName(const char* name);
			/**
			 * It is used to set the maximum number of allowed instances for the given device and process.
			 * 
			 * @param [in] countForThisDevice The maximum number of allowed instances for the device.
			 * 
			 * @return Returns error code (returns 0 if the function operates successfully).
			 * 
			 */
			static int SetMaxConcurrentInstanceCount(int countForThisDevice);
			/**
			 * It is used to get the unique identifier of the device.
			 * 
			 * @param [in] uuidGenerationMethod The method to generate the UUID.
			 * @param [in, out] uuidBuffer The buffer to store the UUID.
			 * @param [in] uuidBufferLen The length of the UUID buffer. It is recommended to be greater than 36.
			 * 
			 * @return Returns 0 if the UUID is generated successfully, a negative value indicating an error otherwise.
			 * 
			 */
			static int GetDeviceUUID(int uuidGenerationMethod, char uuidBuffer[], const int uuidbufferLen);
			/**
			 * It is used to set the directory path for the license cache.
			 * 
			 * @param [in] directoryPath The directory path for the license cache.
			 * 
			 * @return Returns 0 if the directory path is set successfully, a negative value indicating an error otherwise.
			 * 
			 */
			static int SetLicenseCachePath(const char* directoryPath);
		};

		class LIC_API CLicenseModule
		{
		public:
		/**
		 * It is used to get the version of the licensing library.
		 * 
		 * @return Returns the version of the licensing library as a string.
		 * 
		 */
			static const char* GetVersion();
		};
#pragma pack(pop)
	}
}
#endif