#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCommon.h"
#include "DynamsoftBarcodeReader.h"


#include <vector>
#include <iostream>
#include <map>

#include <flutter/standard_method_codec.h>

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>

using namespace std;
using namespace dynamsoft::dbr;
using flutter::EncodableMap;
using flutter::EncodableValue;
using flutter::EncodableList;

class Task
  {
  public:
      std::function<void()> func;
      unsigned char* buffer;
  };

  class WorkerThread
  {
  public:
      std::mutex m;
      std::condition_variable cv;
      std::queue<Task> tasks = {};
      volatile bool running;
    std::thread t;
  };

class BarcodeManager {
    public:
    

    ~BarcodeManager() 
    {
        clear();
        if (reader != NULL)
        {
            delete reader;
            reader = NULL;
        }
    };

    void clearTasks()
    {
        if (worker->tasks.size() > 0)
        {
            for (int i = 0; i < worker->tasks.size(); i++)
            {
                free(worker->tasks.front().buffer);
                worker->tasks.pop();
            }
        }
    }

    void clear()
    {
        if (worker)
        {
            std::unique_lock<std::mutex> lk(worker->m);
            worker->running = false;
            
            clearTasks();

            worker->cv.notify_one();
            lk.unlock();

            worker->t.join();
            delete worker;
            worker = NULL;
            printf("Quit native thread.\n");
        }
    }

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
            // printf("No barcode found.\n");
        }
        else 
        {
            for (int index = 0; index < results->resultsCount; index++)
            {
                EncodableMap map;
                map[EncodableValue("format")] = results->results[index]->barcodeFormatString;
                // map[EncodableValue("text")] = results->results[index]->barcodeText;
                map[EncodableValue("x1")] = results->results[index]->localizationResult->x1;
                map[EncodableValue("y1")] = results->results[index]->localizationResult->y1;
                map[EncodableValue("x2")] = results->results[index]->localizationResult->x2;
                map[EncodableValue("y2")] = results->results[index]->localizationResult->y2;
                map[EncodableValue("x3")] = results->results[index]->localizationResult->x3;
                map[EncodableValue("y3")] = results->results[index]->localizationResult->y3;
                map[EncodableValue("x4")] = results->results[index]->localizationResult->x4;
                map[EncodableValue("y4")] = results->results[index]->localizationResult->y4;
                map[EncodableValue("angle")] = results->results[index]->localizationResult->angle;
                std::vector<uint8_t> rawBytes(results->results[index]->barcodeBytes, results->results[index]->barcodeBytes + results->results[index]->barcodeBytesLength);
                map[EncodableValue("barcodeBytes")] = rawBytes;
                out.push_back(map);
            }
        }

        CBarcodeReader::FreeTextResults(&results);
        return out;
    }

    static void run(BarcodeManager *self)
    {
        while (self->worker->running)
        {
            std::function<void()> task;
            std::unique_lock<std::mutex> lk(self->worker->m);
            self->worker->cv.wait(lk, [&]
                                { return !self->worker->tasks.empty() || !self->worker->running; });
            if (!self->worker->running)
            {
                break;
            }
            task = std::move(self->worker->tasks.front().func);
            self->worker->tasks.pop();
            lk.unlock();

            task();
        }
    }

    void queueTask(unsigned char* barcodeBuffer, int width, int height, int stride, int format, int len)
    {    
        unsigned char *data = (unsigned char *)malloc(len);
        memcpy(data, barcodeBuffer, len);

        std::unique_lock<std::mutex> lk(worker->m);
        clearTasks();
        std::function<void()> task_function = std::bind(scan, this, data, width, height, stride, format);
        Task task;
        task.func = task_function;
        task.buffer = data;
        worker->tasks.push(task);
        worker->cv.notify_one();
        lk.unlock();
    }

    static void scan(BarcodeManager *self, unsigned char * buffer, int width, int height, int stride, int format)
    {
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

        self->reader->DecodeBuffer(buffer, width, height, stride, pixelFormat, "");

        free(buffer);
        EncodableList results = self->WrapResults();
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result = std::move(self->pendingResults.front());
        self->pendingResults.erase(self->pendingResults.begin());
        result->Success(results);
    }

    int Init() 
    {
        reader = new CBarcodeReader();
        worker = new WorkerThread();
        worker->running = true;
        worker->t = thread(&run, this);
        return 0;
    }

    int SetLicense(const char * license) 
    {
        int ret = CBarcodeReader::InitLicense(license);
        return ret;
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

    void DecodeImageBuffer(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>& pendingResult, const unsigned char * buffer, int width, int height, int stride, int format) 
    {
        pendingResults.push_back(std::move(pendingResult));
        queueTask((unsigned char*)buffer, width, height, stride, format, stride * height);
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
        WorkerThread *worker;
        vector<std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>> pendingResults = {};
};

#endif 