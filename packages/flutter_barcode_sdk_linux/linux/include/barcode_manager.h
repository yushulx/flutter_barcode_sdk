#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCaptureVisionRouter.h"
#include "DynamsoftUtility.h"

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

inline void printf_to_cerr(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    std::cerr << buffer;
}

#define printf printf_to_cerr

/// Responds with a success response carrying [value] and releases the
/// caller's reference to it.
void RespondFlValue(FlMethodCall *method_call, FlValue *value)
{
    g_autoptr(FlMethodResponse) response =
        FL_METHOD_RESPONSE(fl_method_success_response_new(value));
    fl_method_call_respond(method_call, response, nullptr);
    fl_value_unref(value);
}

FlValue *WrapError(int errorCode, const char *errorMsg)
{
    FlValue *out = fl_value_new_list();
    FlValue *data_map = fl_value_new_map();
    fl_value_set_string_take(data_map, "format", fl_value_new_string(""));
    fl_value_set_string_take(data_map, "text", fl_value_new_string(""));
    fl_value_set_string_take(data_map, "angle", fl_value_new_int(0));

    for (int i = 0; i < 4; ++i)
    {
        std::string x_key = "x" + std::to_string(i + 1);
        std::string y_key = "y" + std::to_string(i + 1);
        fl_value_set_string_take(data_map, x_key.c_str(), fl_value_new_int(0));
        fl_value_set_string_take(data_map, y_key.c_str(), fl_value_new_int(0));
    }

    fl_value_set_string_take(data_map, "barcodeBytes", fl_value_new_uint8_list(NULL, 0));
    fl_value_set_string_take(data_map, "errorCode", fl_value_new_int(errorCode));
    fl_value_set_string_take(data_map, "errorMsg", fl_value_new_string(errorMsg));

    fl_value_append_take(out, data_map);
    return out;
}

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
    fl_value_set_string_take(map, "errorCode", fl_value_new_int(0));
    fl_value_set_string_take(map, "errorMsg", fl_value_new_string(""));
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
        if (state != ISS_EXHAUSTED)
        {
            return;
        }

        m_router->StopCapturing();

        FlValue *out = fl_value_new_list();
        bool hasError = false;

        for (auto *result : m_receiver->results)
        {
            if (!hasError && result->GetErrorCode())
            {
                fl_value_unref(out);
                out = WrapError(result->GetErrorCode(), result->GetErrorString());
                hasError = true;
            }
            else if (!hasError)
            {
                int barcodeResultItemCount = result->GetItemsCount();
                for (int j = 0; j < barcodeResultItemCount; ++j)
                {
                    const CBarcodeResultItem *barcodeResultItem = result->GetItem(j);
                    fl_value_append_take(out, CreateBarcodeResultMap(barcodeResultItem));
                }
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

    /// Stores the call that the next exhausted capture round answers. If a
    /// previous call is still pending, it is released with an empty result so
    /// its Dart future does not hang (mirrors the macOS implementation).
    void SetMethodCall(FlMethodCall *method_call)
    {
        if (m_method_call)
        {
            RespondFlValue(m_method_call, fl_value_new_list());
            g_object_unref(m_method_call);
            m_method_call = nullptr;
        }
        if (method_call)
        {
            m_method_call = method_call;
            g_object_ref(m_method_call); // Retain the method call
        }
    }

    /// Discards the pending call without responding; used when starting a
    /// capture round failed and the error was already reported directly.
    void ClearMethodCall()
    {
        if (m_method_call)
        {
            g_object_unref(m_method_call);
            m_method_call = nullptr;
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

    void DecodeFile(FlMethodCall *method_call, const char *filename)
    {
        if (!handler || !fileFetcher)
        {
            RespondFlValue(method_call, fl_value_new_list());
            return;
        }

        // CFileFetcher reads and partitions the file (including multi-page
        // images) on the SDK capture thread.
        int ret = fileFetcher->SetFile(filename);
        if (ret != 0)
        {
            RespondFlValue(method_call, WrapError(ret, "Failed to set file"));
            return;
        }

        listener->SetMethodCall(method_call);
        char errorMsg[512] = {0};
        int errorCode = handler->StartCapturing("", false, errorMsg, 512);
        if (errorCode != 0)
        {
            printf("StartCapturing: %s\n", errorMsg);
            RespondFlValue(method_call, WrapError(errorCode, errorMsg));
            listener->ClearMethodCall();
        }
    }

    FlValue *DecodeFileBytes(const unsigned char *bytes, int size)
    {
        FlValue *results;
        if (!handler)
            return fl_value_new_list();

        CCapturedResult *capturedResult = handler->Capture(bytes, size);
        if (capturedResult->GetErrorCode())
        {
            results = WrapError(capturedResult->GetErrorCode(), capturedResult->GetErrorString());
        }
        else
        {
            results = WrapResults(capturedResult);
        }
        return results;
    }

    void DecodeImageBuffer(FlMethodCall *method_call, const unsigned char *buffer, int width, int height, int stride, int format, int rotation)
    {
        if (!handler)
            return;

        ImagePixelFormat pixelFormat = GetImagePixelFormat(format);
        CImageData *imageData = new CImageData(stride * height, buffer, width, height, stride, pixelFormat, rotation);
        fileFetcher->SetFile(imageData);
        delete imageData;

        char errorMsg[512] = {0};
        listener->SetMethodCall(method_call);
        int errorCode = handler->StartCapturing("", false, errorMsg, 512);
        if (errorCode != 0)
        {
            printf("StartCapturing: %s\n", errorMsg);
            RespondFlValue(method_call, WrapError(errorCode, errorMsg));
            listener->ClearMethodCall();
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