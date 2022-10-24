#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCommon.h"
#include "DynamsoftBarcodeReader.h"


#include <vector>
#include <iostream>
#include <map>

#include <flutter/standard_method_codec.h>

using namespace std;
using namespace dynamsoft::dbr;
using flutter::EncodableMap;
using flutter::EncodableValue;
using flutter::EncodableList;

class BarcodeManager {
    public:

    ~BarcodeManager() 
    {
        if (reader != NULL)
        {
            delete reader;
            reader = NULL;
        }
    };

    const char* GetVersion() 
    {
        return DBR_GetVersion();
    }

    EncodableList WrapResults() 
    {
        EncodableList out;
        if (reader == NULL) return out;
        TextResultArray *results = NULL;
        reader->GetAllTextResults(&results);
            
        if (results == NULL || results->resultsCount == 0)
        {
            printf("No barcode found.\n");
        }
        else 
        {
            for (int index = 0; index < results->resultsCount; index++)
            {
                EncodableMap map;
                map[EncodableValue("format")] = results->results[index]->barcodeFormatString;
                map[EncodableValue("text")] = results->results[index]->barcodeText;
                map[EncodableValue("x1")] = results->results[index]->localizationResult->x1;
                map[EncodableValue("y1")] = results->results[index]->localizationResult->y1;
                map[EncodableValue("x2")] = results->results[index]->localizationResult->x2;
                map[EncodableValue("y2")] = results->results[index]->localizationResult->y2;
                map[EncodableValue("x3")] = results->results[index]->localizationResult->x3;
                map[EncodableValue("y3")] = results->results[index]->localizationResult->y3;
                map[EncodableValue("x4")] = results->results[index]->localizationResult->x4;
                map[EncodableValue("y4")] = results->results[index]->localizationResult->y4;
                map[EncodableValue("angle")] = results->results[index]->localizationResult->angle;
                std::vector<uint8_t> raw_image(results->results[index]->barcodeBytes, results->results[index]->barcodeBytes + results->results[index]->barcodeBytesLength);
                map[EncodableValue("barcodeBytes")] = raw_image;
                out.push_back(map);
            }
        }

        CBarcodeReader::FreeTextResults(&results);
        return out;
    }

    void Init() 
    {
        reader = new CBarcodeReader();
    }

    void SetLicense(const char * license) 
    {
        CBarcodeReader::InitLicense(license);
    }

    EncodableList DecodeFile(const char * filename) 
    {
        EncodableList out;   
        if (reader == NULL) return out;
        int ret = reader->DecodeFile(filename, "");

        if (ret == DBRERR_FILE_NOT_FOUND)
        {
            printf("Error code %d. %s\n", ret, CBarcodeReader::GetErrorString(ret));
            return out;
        }

        return WrapResults();
    }

    EncodableList DecodeFileBytes(const unsigned char * bytes, int size) 
    {
        EncodableList out;
        if (reader == NULL) return out;
        reader->DecodeFileInMemory(bytes, size, "");
        return WrapResults();
    }

    EncodableList DecodeImageBuffer(const unsigned char * buffer, int width, int height, int stride, int format) 
    {
        EncodableList out;
        if (reader == NULL) return out;

        ImagePixelFormat pixelFormat = IPF_BGR_888;
        switch(format) {
            case 0:
                pixelFormat = IPF_BINARY;
                break;
            case 1:
                pixelFormat = IPF_BINARYINVERTED;
                break;
            case 2:
                pixelFormat = IPF_GRAYSCALED;
                break;
            case 3:
                pixelFormat = IPF_NV21;
                break;
            case 4:
                pixelFormat = IPF_RGB_565;
                break;
            case 5:
                pixelFormat = IPF_RGB_555;
                break;
            case 6:
                pixelFormat = IPF_RGB_888;
                break;
            case 7:
                pixelFormat = IPF_ARGB_8888;
                break;
            case 8:
                pixelFormat = IPF_RGB_161616;
                break;
            case 9: 
                pixelFormat = IPF_ARGB_16161616;
                break;
            case 10:
                pixelFormat = IPF_ABGR_8888;
                break;
            case 11:
                pixelFormat = IPF_ABGR_16161616;
                break;
            case 12:
                pixelFormat = IPF_BGR_888;
                break;
        }

        reader->DecodeBuffer(buffer, width, height, stride, pixelFormat, "");

        return WrapResults();
    }

    int SetFormats(int formats) 
    {
        if (reader == NULL) return -1;

        int ret = 0;
        char sError[512];
        PublicRuntimeSettings* runtimeSettings = new PublicRuntimeSettings();
        reader->GetRuntimeSettings(runtimeSettings);
        runtimeSettings->barcodeFormatIds = formats; 
        reader->UpdateRuntimeSettings(runtimeSettings, sError, 512);
        delete runtimeSettings;

        return ret;
    }

    EncodableValue GetParameters()
    {
        EncodableValue out;
        if (reader == NULL) return out;

        char* content = NULL;
        reader->OutputSettingsToStringPtr(&content, "currentRuntimeSettings");
        EncodableValue params = EncodableValue((const char*)content);
        reader->FreeSettingsString(&content);
        return EncodableValue(params);
    }

    int SetParameters(const char *params)
    {
        if (reader == NULL) return -1;
        char errorMessage[256];
        int ret = reader->InitRuntimeSettingsWithString(params, CM_IGNORE, errorMessage, 256);
        return ret;
    }

    private:
        CBarcodeReader *reader; 
};

#endif 