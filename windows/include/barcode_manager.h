#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

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
     BarcodeManager() 
     {
         reader = new CBarcodeReader();
         reader->InitLicense(""); // Get 30-day FREEE trial license from https://www.dynamsoft.com/customer/license/trialLicense?product=dbr
     };

    ~BarcodeManager() 
    {
        delete reader;
    };

    const char* GetVersion() 
    {
        return reader->GetVersion();
    }

    EncodableList WrapResults() 
    {
        EncodableList out;
        TextResultArray *results = NULL;
        reader->GetAllTextResults(&results);
            
        if (results->resultsCount == 0)
        {
            printf("No barcode found.\n");
            CBarcodeReader::FreeTextResults(&results);
        }
        
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
            out.push_back(map);
        }

        CBarcodeReader::FreeTextResults(&results);
        return out;
    }

    void SetLicense(const char * license) 
    {
        reader->InitLicense(license);
    }

    EncodableList DecodeFile(const char * filename) 
    {
        EncodableList out;   
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
        reader->DecodeFileInMemory(bytes, size, "");
        return WrapResults();
    }

    EncodableList DecodeImageBuffer(const unsigned char * buffer, int width, int height, int stride, int format) 
    {
        ImagePixelFormat pixelFormat = IPF_BGR_888;
        switch(format) {
            case 0:
                pixelFormat = IPF_GRAYSCALED;
                break;
            case 1:
                pixelFormat = IPF_ARGB_8888;
                break;
        }

        reader->DecodeBuffer(buffer, width, height, stride, pixelFormat, "");

        return WrapResults();
    }

    private:
        CBarcodeReader *reader; 
};

#endif 