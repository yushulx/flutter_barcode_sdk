package com.dynamsoft.flutter_barcode_sdk;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;

import com.dynamsoft.cvr.CapturedResult;
import com.dynamsoft.core.basic_structures.CapturedResultItem;
import com.dynamsoft.cvr.CaptureVisionRouter;
import com.dynamsoft.cvr.SimplifiedCaptureVisionSettings;
import com.dynamsoft.dbr.BarcodeResultItem;
import com.dynamsoft.license.LicenseManager;
import com.dynamsoft.core.basic_structures.ImageData;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.flutter.plugin.common.MethodChannel.Result;

public class BarcodeManager {
    private static final String TAG = "DynamsoftBarcodeReader";
    private CaptureVisionRouter mRouter;


    private void wrapResults(CapturedResult result, List<Map<String, Object>> out) {
        CapturedResultItem[] items = result.getItems();
        for (CapturedResultItem item : items) {
            Map<String, Object> data = new HashMap<>();
            BarcodeResultItem barcodeItem = (BarcodeResultItem)item;
            data.put("format", barcodeItem.getFormatString());
            data.put("text", barcodeItem.getText());
            Point[] points = barcodeItem.getLocation().points;
            data.put("x1", points[0].x);
            data.put("y1", points[0].y);
            data.put("x2", points[1].x);
            data.put("y2", points[1].y);
            data.put("x3", points[2].x);
            data.put("y3", points[2].y);
            data.put("x4", points[3].x);
            data.put("y4", points[3].y);
            data.put("angle", barcodeItem.getAngle());
            data.put("barcodeBytes", barcodeItem.getBytes());
            out.add(data);
        }
    }

    public int init(Context context) {
        if (mRouter == null) {
            try {
                mRouter = new CaptureVisionRouter(context);
            } catch (Exception e) {
                e.printStackTrace();
                return -1;
            }
        }

        return 0;
    }

    public void setLicense(String license, final Result result, Context context) {
        LicenseManager.initLicense(license, context, (isSuccess, error) -> {
            if (!isSuccess) {
                result.success(-1);
            }
            else {
                result.success(0);
            }
        });
    }

    public List<Map<String, Object>> decodeFile(String filename) {
        List<Map<String, Object>> ret = new ArrayList<Map<String, Object>>();
        try {
            CapturedResult results = mRouter.capture(filename, "");
            wrapResults(results, ret);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return ret;
    }

    public List<Map<String, Object>> decodeFileBytes(byte[] bytes) {
        List<Map<String, Object>> ret = new ArrayList<Map<String, Object>>();
        Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes != null ? bytes.length : 0);
        try {
            CapturedResult results = mRouter.capture(bitmap, "");
            wrapResults(results, ret);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

    public List<Map<String, Object>> decodeImageBuffer(byte[] bytes, int width, int height, int stride, int format) {
        List<Map<String, Object>> ret = new ArrayList<Map<String, Object>>();
        try {
            ImageData imageData = new ImageData();
            imageData.bytes = bytes;
            imageData.width = width;
            imageData.height = height;
            imageData.stride = stride;
            imageData.format = format;
            CapturedResult results = mRouter.capture(imageData, "");
            wrapResults(results, ret);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

    public int setBarcodeFormats(int formats) {
        try {
            SimplifiedCaptureVisionSettings settings = mRouter.getSimplifiedSettings("");
            settings.barcodeSettings.barcodeFormatIds = formats;
            mRouter.updateSettings("", settings);
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    public String getParameters() {
        try {
            return mRouter.outputSettings("");
        } catch (Exception e) {
            return e.toString();
        }
    }

    public int setParameters(String params) {
        try {
            mRouter.initSettings(params);
            return 0;
        } catch (Exception e) {
            return -1;
        }
    }
}
