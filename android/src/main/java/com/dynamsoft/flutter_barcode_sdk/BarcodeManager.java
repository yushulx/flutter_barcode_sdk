package com.dynamsoft.flutter_barcode_sdk;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.dynamsoft.dbr.DBRLTSLicenseVerificationListener;
import com.dynamsoft.dbr.DMLTSConnectionParameters;
import com.dynamsoft.dbr.EnumBarcodeFormat;
import com.dynamsoft.dbr.EnumConflictMode;
import com.dynamsoft.dbr.EnumImagePixelFormat;
import com.dynamsoft.dbr.EnumIntermediateResultType;
import com.dynamsoft.dbr.IntermediateResult;
import com.dynamsoft.dbr.LocalizationResult;
import com.dynamsoft.dbr.PublicRuntimeSettings;
import com.dynamsoft.dbr.TextResult;
import com.dynamsoft.dbr.BarcodeReader;

public class BarcodeManager {
    private static final String TAG = "DynamsoftBarcodeReader";
    private BarcodeReader mBarcodeReader;

    public BarcodeManager() {
        try {
            mBarcodeReader = new BarcodeReader();
            DMLTSConnectionParameters parameters = new DMLTSConnectionParameters();
            parameters.organizationID = "200001";
            mBarcodeReader.initLicenseFromLTS(parameters, new DBRLTSLicenseVerificationListener() {
                @Override
                public void LTSLicenseVerificationCallback(boolean isSuccess, Exception error) {
                    if (!isSuccess) {
                        error.printStackTrace();
                    }
                }
            });
            //Best Coverage settings
            //mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestCoverage\",\"DeblurLevel\":9,\"ExpectedBarcodesCount\":512,\"ScaleDownThreshold\":100000,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"},{\"Mode\":\"LM_STATISTICS\"},{\"Mode\":\"LM_LINES\"},{\"Mode\":\"LM_STATISTICS_MARKS\"}],\"GrayscaleTransformationModes\":[{\"Mode\":\"GTM_ORIGINAL\"},{\"Mode\":\"GTM_INVERTED\"}]}}", EnumConflictMode.CM_OVERWRITE);
            //Best Speed settings
            //mbarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestSpeed\",\"DeblurLevel\":3,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_SCAN_DIRECTLY\"}],\"TextFilterModes\":[{\"MinImageDimension\":262144,\"Mode\":\"TFM_GENERAL_CONTOUR\"}]}}", EnumConflictMode.CM_OVERWRITE);
            //Balance settings
            mBarcodeReader.initRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"Balance\",\"DeblurLevel\":5,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"}]}}", EnumConflictMode.CM_OVERWRITE);
            PublicRuntimeSettings settings = mBarcodeReader.getRuntimeSettings();
            settings.intermediateResultTypes = EnumIntermediateResultType.IRT_TYPED_BARCODE_ZONE;
            settings.barcodeFormatIds = EnumBarcodeFormat.BF_ONED | EnumBarcodeFormat.BF_DATAMATRIX | EnumBarcodeFormat.BF_QR_CODE | EnumBarcodeFormat.BF_PDF417;
            settings.barcodeFormatIds_2 = 0;
            mBarcodeReader.updateRuntimeSettings(settings);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String decodeFile(String filename) {
        try {
            TextResult[] results = mBarcodeReader.decodeFile(filename, "");
            if (results != null) {
                String ret = "";
                for (TextResult result: results) {
                    ret += "Format: " + result.barcodeFormatString + ", ";
                    ret += "Text: " + result.barcodeText + ". ";
                }

                if (ret.equals(""))
                    return "No Barcode Detected";
                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "No Barcode Detected.";
    }

    public String decodeFileBytes(byte[] bytes) {
        Bitmap bitmap = BitmapFactory.decodeByteArray(bytes , 0, bytes != null ? bytes.length : 0);
        try {
            TextResult[] results = mBarcodeReader.decodeBufferedImage(bitmap, "");
            if (results != null) {
                String ret = "";
                for (TextResult result: results) {
                    ret += "Format: " + result.barcodeFormatString + ", ";
                    ret += "Text: " + result.barcodeText + ". ";
                }

                if (ret.equals(""))
                    return "No Barcode Detected";
                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "No Barcode Detected."; 
    }

    public String decodeImageBuffer(byte[] bytes, int width, int height, int stride, int format) {
        int pixelFormat = EnumImagePixelFormat.IPF_BGR_888;
        switch(format) {
            case 0:
                pixelFormat = EnumImagePixelFormat.IPF_GRAYSCALED;
                break;
            case 1:
                pixelFormat = EnumImagePixelFormat.IPF_ARGB_8888;
                break;
        }

        try {
            TextResult[] results = mBarcodeReader.decodeBuffer(bytes, width, height, stride, pixelFormat, "");
            if (results != null) {
                String ret = "";
                for (TextResult result: results) {
                    ret += "Format: " + result.barcodeFormatString + ", ";
                    ret += "Text: " + result.barcodeText + ". ";
                }

                if (ret.equals(""))
                    return "No Barcode Detected";
                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "No Barcode Detected.";
    }
}
