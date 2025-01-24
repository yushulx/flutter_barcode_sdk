#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCaptureVisionRouter.h"
#include "DynamsoftUtility.h"
#include "template.h"

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
using namespace dynamsoft::license;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dbr;
using namespace dynamsoft::utility;
using namespace dynamsoft::basic_structures;

using flutter::EncodableList;
using flutter::EncodableMap;
using flutter::EncodableValue;

class Task
{
public:
    std::function<void()> func;
    unsigned char *buffer;
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

class BarcodeManager
{
public:
    ~BarcodeManager()
    {
        clear();
        if (handler != NULL)
        {
            delete handler;
            handler = NULL;
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

    EncodableList WrapResults(CCapturedResult *result)
    {
        EncodableList out;
        if (handler == NULL)
            return out;

        CDecodedBarcodesResult *barcodeResult = result->GetDecodedBarcodesResult();
        if (!barcodeResult || barcodeResult->GetItemsCount() == 0)
        {
            return out;
        }

        int barcodeResultItemCount = barcodeResult->GetItemsCount();

        for (int j = 0; j < barcodeResultItemCount; j++)
        {
            const CBarcodeResultItem *barcodeResultItem = barcodeResult->GetItem(j);
            const char *format = barcodeResultItem->GetFormatString();
            const char *text = barcodeResultItem->GetText();
            int angle = barcodeResultItem->GetAngle();
            CPoint *points = barcodeResultItem->GetLocation().points;
            unsigned char *raw = barcodeResultItem->GetBytes();

            EncodableMap map;
            map[EncodableValue("format")] = format;
            map[EncodableValue("text")] = text;
            map[EncodableValue("x1")] = points[0][0];
            map[EncodableValue("y1")] = points[0][1];
            map[EncodableValue("x2")] = points[1][0];
            map[EncodableValue("y2")] = points[1][1];
            map[EncodableValue("x3")] = points[2][0];
            map[EncodableValue("y3")] = points[2][1];
            map[EncodableValue("x4")] = points[3][0];
            map[EncodableValue("y4")] = points[3][1];
            map[EncodableValue("angle")] = angle;
            std::vector<uint8_t> rawBytes(raw, raw + barcodeResultItem->GetBytesLength());
            map[EncodableValue("barcodeBytes")] = rawBytes;
            out.push_back(map);
        }

        barcodeResult->Release();
        result->Release();
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

    void queueTask(unsigned char *barcodeBuffer, int width, int height, int stride, int format, int len)
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

    static void scan(BarcodeManager *self, unsigned char *buffer, int width, int height, int stride, int format)
    {
        ImagePixelFormat pixelFormat = IPF_BGR_888;
        switch (format)
        {
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

        CImageData *imageData = new CImageData(stride * height, buffer, width, height, stride, pixelFormat);
        CCapturedResult *captureResult = self->handler->Capture(imageData);
        delete imageData, imageData = NULL;
        free(buffer);

        EncodableList results = self->WrapResults(captureResult);
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result = std::move(self->pendingResults.front());
        self->pendingResults.erase(self->pendingResults.begin());
        result->Success(results);
    }

    int Init()
    {
        handler = new CCaptureVisionRouter;
        char errorMsgBuffer[256];
        int ret = handler->InitSettings(jsonString.c_str(), errorMsgBuffer, 256);
        if (ret)
        {
            printf("InitSettings: %s\n", errorMsgBuffer);
        }
        worker = new WorkerThread();
        worker->running = true;
        worker->t = thread(&run, this);
        return 0;
    }

    int SetLicense(const char *license)
    {
        char errorMsgBuffer[512];
        int ret = CLicenseManager::InitLicense(license, errorMsgBuffer, 512);
        if (ret)
        {
            printf("InitLicense: %s\n", errorMsgBuffer);
        }
        return ret;
    }

    EncodableList DecodeFile(const char *filename)
    {
        EncodableList out;
        if (handler == NULL)
            return out;

        CCapturedResult *result = handler->Capture(filename, "");
        return WrapResults(result);
    }

    EncodableList DecodeFileBytes(const unsigned char *bytes, int size)
    {
        EncodableList out;
        if (handler == NULL)
            return out;
        CCapturedResult *result = handler->Capture(bytes, size);
        return WrapResults(result);
    }

    void DecodeImageBuffer(std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> &pendingResult, const unsigned char *buffer, int width, int height, int stride, int format)
    {
        pendingResults.push_back(std::move(pendingResult));
        queueTask((unsigned char *)buffer, width, height, stride, format, stride * height);
    }

    int SetFormats(unsigned long long formats)
    {
        if (handler == NULL)
            return -1;

        SimplifiedCaptureVisionSettings pSettings = {};
        handler->GetSimplifiedSettings("", &pSettings);
        pSettings.barcodeSettings.barcodeFormatIds = formats;

        char errorMessage[256];
        int ret = handler->UpdateSettings("", &pSettings, errorMessage, 256);
        if (ret)
        {
            printf("InitSettings: %s\n", errorMessage);
        }
        return ret;
    }

    EncodableValue GetParameters()
    {
        EncodableValue out;
        if (handler == NULL)
            return out;

        char *content = handler->OutputSettings("");
        EncodableValue params = EncodableValue((const char *)content);
        CCaptureVisionRouter::FreeString(content);
        return EncodableValue(params);
    }

    int SetParameters(const char *params)
    {
        if (handler == NULL)
            return -1;
        char errorMessage[256];
        int ret = handler->InitSettings(params, errorMessage, 256);
        if (ret)
        {
            printf("InitSettings: %s\n", errorMessage);
        }
        return ret;
    }

private:
    CCaptureVisionRouter *handler;
    WorkerThread *worker;
    vector<std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>> pendingResults = {};
};

#endif