#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCaptureVisionRouter.h"
#include "DynamsoftUtility.h"
#include "template.h"

#include <vector>
#include <iostream>
#include <map>
#include <mutex>

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

using namespace std;
using namespace dynamsoft::license;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dbr;
using namespace dynamsoft::utility;
using namespace dynamsoft::basic_structures;

FlValue *CreateBarcodeResultMap(const CBarcodeResultItem *barcodeResultItem)
{
    FlValue *map = fl_value_new_map();
    fl_value_set_string_take(map, "format", fl_value_new_string(barcodeResultItem->GetFormatString()));
    fl_value_set_string_take(map, "text", fl_value_new_string(barcodeResultItem->GetText()));
    fl_value_set_string_take(map, "angle", fl_value_new_int(barcodeResultItem->GetAngle()));

    CPoint points[4];
    memcpy(points, barcodeResultItem->GetLocation().points, sizeof(CPoint) * 4);

    for (int i = 0; i < 4; ++i)
    {
        std::string x_key = "x" + std::to_string(i + 1);
        std::string y_key = "y" + std::to_string(i + 1);
        fl_value_set_string_take(map, x_key.c_str(), fl_value_new_int(points[i][0]));
        fl_value_set_string_take(map, y_key.c_str(), fl_value_new_int(points[i][1]));
    }

    fl_value_set_string_take(map, "barcodeBytes", fl_value_new_uint8_list(barcodeResultItem->GetBytes(), barcodeResultItem->GetBytesLength()));
    return map;
}

class MyCapturedResultReceiver : public CCapturedResultReceiver
{
public:
    std::vector<CDecodedBarcodesResult *> results;
    std::mutex results_mutex;

    void OnDecodedBarcodesReceived(CDecodedBarcodesResult *pResult) override
    {
        pResult->Retain();
        std::lock_guard<std::mutex> lock(results_mutex);
        results.push_back(pResult);
    }
};

class MyImageSourceStateListener : public CImageSourceStateListener
{
private:
    CCaptureVisionRouter *m_router;
    MyCapturedResultReceiver *m_receiver;
    FlMethodCall *m_method_call;

public:
    MyImageSourceStateListener(CCaptureVisionRouter *router, MyCapturedResultReceiver *receiver)
        : m_router(router), m_receiver(receiver), m_method_call(nullptr) {}

    ~MyImageSourceStateListener()
    {
        if (m_method_call)
        {
            g_object_unref(m_method_call);
        }
    }

    void OnImageSourceStateReceived(ImageSourceState state) override
    {
        if (state == ISS_EXHAUSTED)
        {
            m_router->StopCapturing();

            FlValue *out = fl_value_new_list();

            for (auto *result : m_receiver->results)
            {
                if (!result || result->GetItemsCount() == 0)
                {
                    continue;
                }

                int barcodeResultItemCount = result->GetItemsCount();
                for (int j = 0; j < barcodeResultItemCount; ++j)
                {
                    const CBarcodeResultItem *barcodeResultItem = result->GetItem(j);
                    fl_value_append_take(out, CreateBarcodeResultMap(barcodeResultItem));
                }

                result->Release();
            }

            m_receiver->results.clear();

            g_autoptr(FlMethodResponse) response = FL_METHOD_RESPONSE(fl_method_success_response_new(out));
            if (m_method_call)
            {
                fl_method_call_respond(m_method_call, response, nullptr);
                g_object_unref(m_method_call); // Release the method call
                m_method_call = nullptr;
            }
        }
    }

    void SetMethodCall(FlMethodCall *method_call)
    {
        if (m_method_call)
        {
            g_object_unref(m_method_call);
        }
        m_method_call = method_call;
        if (m_method_call)
        {
            g_object_ref(m_method_call); // Retain the method call
        }
    }
};

class BarcodeManager
{
public:
    BarcodeManager() : handler(nullptr), fileFetcher(nullptr), listener(nullptr), capturedReceiver(nullptr) {}

    ~BarcodeManager()
    {
        delete handler;
        delete listener;
        delete fileFetcher;
        delete capturedReceiver;
        handler = nullptr;
        listener = nullptr;
        fileFetcher = nullptr;
        capturedReceiver = nullptr;
    }

    int Init()
    {
        handler = new CCaptureVisionRouter;
        char errorMsgBuffer[256];
        int ret = handler->InitSettings(jsonString.c_str(), errorMsgBuffer, 256);
        if (ret)
        {
            printf("InitSettings: %s\n", errorMsgBuffer);
            return ret;
        }

        fileFetcher = new CFileFetcher();
        handler->SetInput(fileFetcher);

        capturedReceiver = new MyCapturedResultReceiver;
        handler->AddResultReceiver(capturedReceiver);

        listener = new MyImageSourceStateListener(handler, capturedReceiver);
        handler->AddImageSourceStateListener(listener);

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

    FlValue *DecodeFile(const char *filename)
    {
        if (!handler)
            return fl_value_new_list();

        CCapturedResult *result = handler->Capture(filename, "");
        return WrapResults(result);
    }

    FlValue *DecodeFileBytes(const unsigned char *bytes, int size)
    {
        if (!handler)
            return fl_value_new_list();

        CCapturedResult *result = handler->Capture(bytes, size);
        return WrapResults(result);
    }

    void DecodeImageBuffer(FlMethodCall *method_call, const unsigned char *buffer, int width, int height, int stride, int format)
    {
        if (!handler)
            return;

        ImagePixelFormat pixelFormat = GetImagePixelFormat(format);
        CImageData *imageData = new CImageData(stride * height, buffer, width, height, stride, pixelFormat);
        fileFetcher->SetFile(imageData);
        delete imageData;

        char errorMsg[512] = {0};
        listener->SetMethodCall(method_call);
        int errorCode = handler->StartCapturing("", false, errorMsg, 512);
        if (errorCode != 0)
        {
            printf("StartCapturing: %s\n", errorMsg);
        }
    }

    int SetFormats(unsigned long long formats)
    {
        if (!handler)
            return -1;

        SimplifiedCaptureVisionSettings pSettings;
        handler->GetSimplifiedSettings("", &pSettings);
        pSettings.barcodeSettings.barcodeFormatIds = formats;

        char errorMessage[256];
        int ret = handler->UpdateSettings("", &pSettings, errorMessage, 256);
        if (ret)
        {
            printf("UpdateSettings: %s\n", errorMessage);
        }
        return ret;
    }

    FlValue *GetParameters()
    {
        if (!handler)
            return fl_value_new_string("");

        char *content = handler->OutputSettings("");
        FlValue *params = fl_value_new_string(content);
        CCaptureVisionRouter::FreeString(content);
        return params;
    }

    FlValue *SetParameters(const char *params)
    {
        if (!handler)
            return fl_value_new_int(-1);

        char errorMessage[256];
        int ret = handler->InitSettings(params, errorMessage, 256);
        if (ret)
        {
            printf("InitSettings: %s\n", errorMessage);
        }
        return fl_value_new_int(ret);
    }

private:
    CCaptureVisionRouter *handler;
    CFileFetcher *fileFetcher;
    MyImageSourceStateListener *listener;
    MyCapturedResultReceiver *capturedReceiver;

    FlValue *WrapResults(CCapturedResult *result)
    {
        FlValue *out = fl_value_new_list();
        if (!handler || !result)
            return out;

        CDecodedBarcodesResult *barcodeResult = result->GetDecodedBarcodesResult();
        if (!barcodeResult || barcodeResult->GetItemsCount() == 0)
        {
            result->Release();
            return out;
        }

        int barcodeResultItemCount = barcodeResult->GetItemsCount();
        for (int j = 0; j < barcodeResultItemCount; ++j)
        {
            const CBarcodeResultItem *barcodeResultItem = barcodeResult->GetItem(j);
            fl_value_append_take(out, CreateBarcodeResultMap(barcodeResultItem));
        }

        barcodeResult->Release();
        result->Release();
        return out;
    }

    ImagePixelFormat GetImagePixelFormat(int format)
    {
        switch (format)
        {
        case 0:
            return IPF_BINARY;
        case 1:
            return IPF_BINARYINVERTED;
        case 2:
            return IPF_GRAYSCALED;
        case 3:
            return IPF_NV21;
        case 4:
            return IPF_RGB_565;
        case 5:
            return IPF_RGB_555;
        case 6:
            return IPF_RGB_888;
        case 7:
            return IPF_ARGB_8888;
        case 8:
            return IPF_RGB_161616;
        case 9:
            return IPF_ARGB_16161616;
        case 10:
            return IPF_ABGR_8888;
        case 11:
            return IPF_ABGR_16161616;
        case 12:
            return IPF_BGR_888;
        default:
            return IPF_BGR_888;
        }
    }
};

#endif