#pragma once

#if !defined(_WIN32) && !defined(_WIN64)
#define DIP_API __attribute__((visibility("default")))
#include <stddef.h>
#else
#ifdef DIP_EXPORTS
#define DIP_API __declspec(dllexport)
#else
#define DIP_API __declspec(dllimport)
#endif
#include <windows.h>
#endif
#include "DynamsoftCore.h"

#define DIP_VERSION                  "3.2.50.6520"

namespace dynamsoft
{
	namespace dip
	{
#pragma pack(push)
#pragma pack(4)
		/**
		 * The CImageProcessingModule class defines general functions in the image processing module.
		 */
		class DIP_API CImageProcessingModule
		{
		public:
			/**
			 * Returns the version of the image processing module.
			 *
			 * @return Returns a const char pointer representing the version of the image processing module.
			 */
			static const char* GetVersion();

			/**
			 * Create a Predetected Region Element object.
			 *
			 * @return An object of CPredetectedRegionElement
			 */
			static intermediate_results::CPredetectedRegionElement* CreatePredetectedRegionElement();
		};
#pragma pack(pop)
	}
}