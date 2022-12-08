package com.dynamsoft.flutter_barcode_sdk;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.dynamsoft.dbr.BarcodeReaderException;
import com.dynamsoft.dbr.DBRLicenseVerificationListener;
import com.dynamsoft.dbr.EnumBarcodeFormat;
import com.dynamsoft.dbr.EnumConflictMode;
import com.dynamsoft.dbr.EnumImagePixelFormat;
import com.dynamsoft.dbr.EnumIntermediateResultType;
import com.dynamsoft.dbr.IntermediateResult;
import com.dynamsoft.dbr.LocalizationResult;
import com.dynamsoft.dbr.PublicRuntimeSettings;
import com.dynamsoft.dbr.TextResult;
import com.dynamsoft.dbr.BarcodeReader;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.flutter.plugin.common.MethodChannel.Result;

public class BarcodeManager {
    private static final String TAG = "DynamsoftBarcodeReader";
    private BarcodeReader mBarcodeReader;

    // public BarcodeManager() {
    // try {
    // mBarcodeReader = new BarcodeReader();
    // //Best Coverage settings
    // //mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestCoverage\",\"DeblurLevel\":9,\"ExpectedBarcodesCount\":512,\"ScaleDownThreshold\":100000,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"},{\"Mode\":\"LM_STATISTICS\"},{\"Mode\":\"LM_LINES\"},{\"Mode\":\"LM_STATISTICS_MARKS\"}],\"GrayscaleTransformationModes\":[{\"Mode\":\"GTM_ORIGINAL\"},{\"Mode\":\"GTM_INVERTED\"}]}}",
    // EnumConflictMode.CM_OVERWRITE);
    // //Best Speed settings
    // //mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestSpeed\",\"DeblurLevel\":3,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_SCAN_DIRECTLY\"}],\"TextFilterModes\":[{\"MinImageDimension\":262144,\"Mode\":\"TFM_GENERAL_CONTOUR\"}]}}",
    // EnumConflictMode.CM_OVERWRITE);
    // //Balance settings
    // mBarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"Balance\",\"DeblurLevel\":5,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"}]}}",
    // EnumConflictMode.CM_OVERWRITE);
    // PublicRuntimeSettings settings = mBarcodeReader.getRuntimeSettings();
    // settings.intermediateResultTypes =
    // EnumIntermediateResultType.IRT_TYPED_BARCODE_ZONE;
    // settings.barcodeFormatIds = EnumBarcodeFormat.BF_ONED |
    // EnumBarcodeFormat.BF_DATAMATRIX | EnumBarcodeFormat.BF_QR_CODE |
    // EnumBarcodeFormat.BF_PDF417;
    // settings.barcodeFormatIds_2 = 0;
    // mBarcodeReader.updateRuntimeSettings(settings);
    // } catch (Exception e) {
    // e.printStackTrace();
    // }
    // }

    private void wrapResults(TextResult[] results, List<Map<String, Object>> out) {
        if (results != null) {

            for (TextResult result : results) {
                Map<String, Object> data = new HashMap<>();
                data.put("format", result.barcodeFormatString);
                // data.put("text", result.barcodeText);
                data.put("x1", result.localizationResult.resultPoints[0].x);
                data.put("y1", result.localizationResult.resultPoints[0].y);
                data.put("x2", result.localizationResult.resultPoints[1].x);
                data.put("y2", result.localizationResult.resultPoints[1].y);
                data.put("x3", result.localizationResult.resultPoints[2].x);
                data.put("y3", result.localizationResult.resultPoints[2].y);
                data.put("x4", result.localizationResult.resultPoints[3].x);
                data.put("y4", result.localizationResult.resultPoints[3].y);
                data.put("angle", result.localizationResult.angle);
                data.put("barcodeBytes", result.barcodeBytes);
                out.add(data);
            }
        }
    }

    public int init() {
        if (mBarcodeReader == null) {
            try {
                mBarcodeReader = new BarcodeReader();
                // Best Coverage settings
                // mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestCoverage\",\"DeblurLevel\":9,\"ExpectedBarcodesCount\":512,\"ScaleDownThreshold\":100000,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"},{\"Mode\":\"LM_STATISTICS\"},{\"Mode\":\"LM_LINES\"},{\"Mode\":\"LM_STATISTICS_MARKS\"}],\"GrayscaleTransformationModes\":[{\"Mode\":\"GTM_ORIGINAL\"},{\"Mode\":\"GTM_INVERTED\"}]}}",
                // EnumConflictMode.CM_OVERWRITE);
                // Best Speed settings
                // mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestSpeed\",\"DeblurLevel\":3,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_SCAN_DIRECTLY\"}],\"TextFilterModes\":[{\"MinImageDimension\":262144,\"Mode\":\"TFM_GENERAL_CONTOUR\"}]}}",
                // EnumConflictMode.CM_OVERWRITE);
                // Balance settings
                mBarcodeReader.initRuntimeSettingsWithString(
                        "{\"ImageParameter\":{\"Name\":\"Balance\",\"DeblurLevel\":5,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"}]}}",
                        EnumConflictMode.CM_OVERWRITE);
                PublicRuntimeSettings settings = mBarcodeReader.getRuntimeSettings();
                settings.intermediateResultTypes = EnumIntermediateResultType.IRT_TYPED_BARCODE_ZONE;
                settings.barcodeFormatIds = EnumBarcodeFormat.BF_ONED | EnumBarcodeFormat.BF_DATAMATRIX
                        | EnumBarcodeFormat.BF_QR_CODE | EnumBarcodeFormat.BF_PDF417;
                settings.barcodeFormatIds_2 = 0;
                mBarcodeReader.updateRuntimeSettings(settings);
            } catch (Exception e) {
                e.printStackTrace();
                return -1;
            }
        }

        return 0;
    }

    public void setLicense(String license, final Result result) {
        BarcodeReader.initLicense(
            license,
                new DBRLicenseVerificationListener() {
                    @Override
                    public void DBRLicenseVerificationCallback(boolean isSuccessful, Exception e) {
                        if (isSuccessful)
                        {
                            result.success(0);
                        }
                        else {
                            result.success(-1);
                        }
                    }
                });
    }

    public List<Map<String, Object>> decodeFile(String filename) {
        List<Map<String, Object>> ret = new ArrayList<Map<String, Object>>();
        try {
            TextResult[] results = mBarcodeReader.decodeFile(filename, "");
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
            TextResult[] results = mBarcodeReader.decodeBufferedImage(bitmap, "");
            wrapResults(results, ret);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

    public List<Map<String, Object>> decodeImageBuffer(byte[] bytes, int width, int height, int stride, int format) {
        List<Map<String, Object>> ret = new ArrayList<Map<String, Object>>();
        try {
            TextResult[] results = mBarcodeReader.decodeBuffer(bytes, width, height, stride, format, "");
            wrapResults(results, ret);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

    public int setBarcodeFormats(int formats) {
        try {
            PublicRuntimeSettings settings = mBarcodeReader.getRuntimeSettings();
            settings.barcodeFormatIds = formats;
            mBarcodeReader.updateRuntimeSettings(settings);
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    public String getParameters() {
        try {
            return mBarcodeReader.outputSettingsToString("currentRuntimeSettings");
        } catch (Exception e) {
            return e.toString();
        }
    }

    public int setParameters(String params) {
        try {
            PublicRuntimeSettings settings = mBarcodeReader.getRuntimeSettings();
            mBarcodeReader.initRuntimeSettingsWithString(params, EnumConflictMode.CM_OVERWRITE);
            return 0;
        } catch (Exception e) {
            return -1;
        }
    }
}
