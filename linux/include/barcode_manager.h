#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCommon.h"
#include "DynamsoftBarcodeReader.h"


#include <vector>
#include <iostream>
#include <map>

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

using namespace std;
using namespace dynamsoft::dbr;

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

    FlValue* WrapResults() 
    {
        FlValue* out = fl_value_new_list();
        if (reader == NULL) return out;

        TextResultArray *results = NULL;
        reader->GetAllTextResults(&results);
            
        if (results == NULL || results->resultsCount == 0)
        {
            // printf("No barcode found.\n");
        }
        else
        {
            for (int index = 0; index < results->resultsCount; index++)
            {
                FlValue* map = fl_value_new_map ();
                fl_value_set_string_take (map, "format", fl_value_new_string(results->results[index]->barcodeFormatString));
                // fl_value_set_string_take (map, "text", fl_value_new_string(results->results[index]->barcodeText));
                fl_value_set_string_take (map, "x1", fl_value_new_int(results->results[index]->localizationResult->x1));
                fl_value_set_string_take (map, "y1", fl_value_new_int(results->results[index]->localizationResult->y1));
                fl_value_set_string_take (map, "x2", fl_value_new_int(results->results[index]->localizationResult->x2));
                fl_value_set_string_take (map, "y2", fl_value_new_int(results->results[index]->localizationResult->y2));
                fl_value_set_string_take (map, "x3", fl_value_new_int(results->results[index]->localizationResult->x3));
                fl_value_set_string_take (map, "y3", fl_value_new_int(results->results[index]->localizationResult->y3));
                fl_value_set_string_take (map, "x4", fl_value_new_int(results->results[index]->localizationResult->x4));
                fl_value_set_string_take (map, "y4", fl_value_new_int(results->results[index]->localizationResult->y4));
                fl_value_set_string_take (map, "angle", fl_value_new_int(results->results[index]->localizationResult->angle));
                fl_value_set_string_take (map, "barcodeBytes", fl_value_new_uint8_list(results->results[index]->barcodeBytes, results->results[index]->barcodeBytesLength));
                fl_value_append_take (out, map);
            }
        }

        CBarcodeReader::FreeTextResults(&results);
        return out;
    }

    int Init() 
    {
        reader = new CBarcodeReader();
        return 0;
    }

    int SetLicense(const char * license) 
    {
        int ret = CBarcodeReader::InitLicense(license);
        return ret;
    }

    FlValue* DecodeFile(const char * filename) 
    {
        FlValue* out = fl_value_new_list();
        if (reader == NULL) return out;

        int ret = reader->DecodeFile(filename, "");

        if (ret == DBRERR_FILE_NOT_FOUND)
        {
            printf("Error code %d. %s\n", ret, CBarcodeReader::GetErrorString(ret));
            return out;
        }

        return WrapResults();
    }

    FlValue* DecodeFileBytes(const unsigned char * bytes, int size) 
    {
        FlValue* out = fl_value_new_list();
        if (reader == NULL) return out;

        reader->DecodeFileInMemory(bytes, size, "");
        return WrapResults();
    }

    FlValue* DecodeImageBuffer(const unsigned char * buffer, int width, int height, int stride, int format) 
    {
        FlValue* out = fl_value_new_list();
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

    FlValue* GetParameters()
    {
        if (reader == NULL) return fl_value_new_string("");

        char* content = NULL;
        reader->OutputSettingsToStringPtr(&content, "currentRuntimeSettings");
        FlValue* params = fl_value_new_string((const char*)content);
        reader->FreeSettingsString(&content);
        return params;
    }

    FlValue* SetParameters(const char *params)
    {
        if (reader == NULL) return fl_value_new_int(-1);
        
        char errorMessage[256];
        int ret = reader->InitRuntimeSettingsWithString(params, CM_IGNORE, errorMessage, 256);
        return fl_value_new_int(ret);
    }

    private:
        CBarcodeReader *reader; 
};

#endif 