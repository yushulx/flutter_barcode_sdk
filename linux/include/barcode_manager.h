#ifndef BARCODE_MANAGER_H_
#define BARCODE_MANAGER_H_

#include "DynamsoftCaptureVisionRouter.h"
#include "DynamsoftUtility.h"
#include "template.h"

#include <vector>
#include <iostream>
#include <map>

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>

using namespace std;
using namespace dynamsoft::license;
using namespace dynamsoft::cvr;
using namespace dynamsoft::dbr;
using namespace dynamsoft::utility;
using namespace dynamsoft::basic_structures;

class MyCapturedResultReceiver : public CCapturedResultReceiver
{
public:
    vector<CDecodedBarcodesResult *> results;

public:
    void OnDecodedBarcodesReceived(CDecodedBarcodesResult *pResult) override
    {
        pResult->Retain();
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
    {
        m_router = router;
        m_receiver = receiver;
    }
    void OnImageSourceStateReceived(ImageSourceState state) override
    {
        if (state == ISS_EXHAUSTED)
        {
            m_router->StopCapturing();

            vector<CDecodedBarcodesResult *> results = m_router->results;

            FlValue *out = fl_value_new_list();

            for (int i = 0; i < results.size(); i++)
            {
                CDecodedBarcodesResult *result = results[i];
                if (!result || result->GetItemsCount() == 0)
                {
                    continue;
                }

                int barcodeResultItemCount = result->GetItemsCount();

                for (int j = 0; j < barcodeResultItemCount; j++)
                {
                    const CBarcodeResultItem *barcodeResultItem = result->GetItem(j);
                    const char *format = barcodeResultItem->GetFormatString();
                    const char *text = barcodeResultItem->GetText();
                    int angle = barcodeResultItem->GetAngle();
                    CPoint *points = barcodeResultItem->GetLocation().points;
                    unsigned char *raw = barcodeResultItem->GetBytes();

                    FlValue *map = fl_value_new_map();
                    fl_value_set_string_take(map, "format", format);
                    fl_value_set_string_take(map, "text", text);
                    fl_value_set_string_take(map, "x1", points[0][0]);
                    fl_value_set_string_take(map, "y1", points[0][1]);
                    fl_value_set_string_take(map, "x2", points[1][0]);
                    fl_value_set_string_take(map, "y2", points[1][1]);
                    fl_value_set_string_take(map, "x3", points[2][0]);
                    fl_value_set_string_take(map, "y3", points[2][1]);
                    fl_value_set_string_take(map, "x4", points[3][0]);
                    fl_value_set_string_take(map, "y4", points[3][1]);
                    fl_value_set_string_take(map, "angle", angle);
                    fl_value_set_string_take(map, "barcodeBytes", fl_value_new_uint8_list(raw, barcodeResultItem->GetBytesLength()));
                    fl_value_append_take(out, map);
                }

                result->Release();
            }

            fl_method_call_respond(m_method_call, fl_method_success_response_new(out), nullptr);
        }
    }

    void SetMethodCall(FlMethodCall *method_call)
    {
        m_method_call = method_call;
    }
};

class BarcodeManager
{
public:
    ~BarcodeManager()
    {
        if (handler != NULL)
        {
            delete handler;
            handler = NULL;
        }

        if (listener)
        {
            delete listener;
            listener = NULL;
        }

        if (fileFetcher)
        {
            delete fileFetcher;
            fileFetcher = NULL;
        }

        if (capturedReceiver)
        {
            delete capturedReceiver;
            capturedReceiver = NULL;
        }
    };

    FlValue *WrapResults(CCapturedResult *result)
    {
        FlValue *out = fl_value_new_list();
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

            FlValue *map = fl_value_new_map();
            fl_value_set_string_take(map, "format", format);
            fl_value_set_string_take(map, "text", text);
            fl_value_set_string_take(map, "x1", points[0][0]);
            fl_value_set_string_take(map, "y1", points[0][1]);
            fl_value_set_string_take(map, "x2", points[1][0]);
            fl_value_set_string_take(map, "y2", points[1][1]);
            fl_value_set_string_take(map, "x3", points[2][0]);
            fl_value_set_string_take(map, "y3", points[2][1]);
            fl_value_set_string_take(map, "x4", points[3][0]);
            fl_value_set_string_take(map, "y4", points[3][1]);
            fl_value_set_string_take(map, "angle", angle);
            fl_value_set_string_take(map, "barcodeBytes", fl_value_new_uint8_list(raw, barcodeResultItem->GetBytesLength()));
            fl_value_append_take(out, map);
        }

        barcodeResult->Release();
        result->Release();
        return out;
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
        FlValue *out = fl_value_new_list();
        if (handler == NULL)
            return out;

        CCapturedResult *result = handler->Capture(filename, "");
        return WrapResults(result);
    }

    FlValue *DecodeFileBytes(const unsigned char *bytes, int size)
    {
        FlValue *out = fl_value_new_list();
        if (handler == NULL)
            return out;
        CCapturedResult *result = handler->Capture(bytes, size);
        return WrapResults(result);
    }

    void DecodeImageBuffer(FlMethodCall *method_call, const unsigned char *buffer, int width, int height, int stride, int format)
    {
        if (handler == NULL)
            return;

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
        fileFetcher->SetFile(imageData);
        delete imageData, imageData = NULL;

        char errorMsg[512] = {0};
        listener->SetMethodCall(method_call);
        int errorCode = handler->StartCapturing("", true, errorMsg, 512);
        if (errorCode != 0)
        {
            printf("StartCapturing: %s\n", errorMsg);
        }
    }

    int SetFormats(unsigned long long formats)
    {
        if (reader == NULL)
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

    FlValue *GetParameters()
    {
        if (reader == NULL)
            return fl_value_new_string("");

        char *content = handler->OutputSettings("");
        FlValue *params = fl_value_new_string((const char *)content);
        CCaptureVisionRouter::FreeString(content);
        return params;
    }

    FlValue *SetParameters(const char *params)
    {
        if (handler == NULL)
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
    CImageSourceStateListener *listener;
    MyCapturedResultReceiver *capturedReceiver;
};

#endif