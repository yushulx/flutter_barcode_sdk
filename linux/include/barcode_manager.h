#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

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

    FlValue* WrapResults() 
    {
        FlValue* out = fl_value_new_list();

        TextResultArray *results = NULL;
        reader->GetAllTextResults(&results);
            
        if (results->resultsCount == 0)
        {
            printf("No barcode found.\n");
            CBarcodeReader::FreeTextResults(&results);
        }
        
        for (int index = 0; index < results->resultsCount; index++)
        {
            FlValue* map = fl_value_new_map ();
            fl_value_set_string_take (map, "format", fl_value_new_string(results->results[index]->barcodeFormatString));
            fl_value_set_string_take (map, "text", fl_value_new_string(results->results[index]->barcodeText));
            fl_value_set_string_take (map, "x1", fl_value_new_int(results->results[index]->localizationResult->x1));
            fl_value_set_string_take (map, "y1", fl_value_new_int(results->results[index]->localizationResult->y1));
            fl_value_set_string_take (map, "x2", fl_value_new_int(results->results[index]->localizationResult->x2));
            fl_value_set_string_take (map, "y2", fl_value_new_int(results->results[index]->localizationResult->y2));
            fl_value_set_string_take (map, "x3", fl_value_new_int(results->results[index]->localizationResult->x3));
            fl_value_set_string_take (map, "y3", fl_value_new_int(results->results[index]->localizationResult->y3));
            fl_value_set_string_take (map, "x4", fl_value_new_int(results->results[index]->localizationResult->x4));
            fl_value_set_string_take (map, "y4", fl_value_new_int(results->results[index]->localizationResult->y4));
            fl_value_set_string_take (map, "angle", fl_value_new_int(results->results[index]->localizationResult->angle));
            fl_value_append_take (out, map);
        }

        CBarcodeReader::FreeTextResults(&results);
        return out;
    }

    void SetLicense(const char * license) 
    {
        reader->InitLicense(license);
    }

    FlValue* DecodeFile(const char * filename) 
    {
        FlValue* out = fl_value_new_list();
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
        reader->DecodeFileInMemory(bytes, size, "");
        return WrapResults();
    }

    FlValue* DecodeImageBuffer(const unsigned char * buffer, int width, int height, int stride, int format) 
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