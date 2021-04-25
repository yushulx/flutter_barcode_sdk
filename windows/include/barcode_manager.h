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
         reader->InitLicense("");
     };

    ~BarcodeManager() 
    {
        delete reader;
    };

    const char* GetVersion() 
    {
        return reader->GetVersion();
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
            out.push_back(map);
        }

        CBarcodeReader::FreeTextResults(&results);

        return out;
    }

    private:
        CBarcodeReader *reader; 
};

#endif 