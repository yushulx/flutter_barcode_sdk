package com.dynamsoft.flutter_barcode_sdk;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.dynamsoft.dbr.EnumImagePixelFormat;
import com.dynamsoft.dbr.IntermediateResult;
import com.dynamsoft.dbr.LocalizationResult;
import com.dynamsoft.dbr.TextResult;
import com.dynamsoft.dbr.BarcodeReader;

public class BarcodeManager {
    private static final String TAG = "DynamsoftBarcodeReader";
    private BarcodeReader mBarcodeReader;

    public BarcodeManager() {
        try {
            mBarcodeReader = new BarcodeReader();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void initLicense(String license) {
        try {
            Log.i(TAG, license);
            mBarcodeReader.initLicense(license);
        }
        catch (Exception e) {
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

                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "No Barcode Detected.";
    }

    public String decodeBytes(byte[] bytes) {
        Bitmap bitmap = BitmapFactory.decodeByteArray(bytes , 0, bytes != null ? bytes.length : 0);
        try {
            TextResult[] results = mBarcodeReader.decodeBufferedImage(bitmap, "");
            if (results != null) {
                String ret = "";
                for (TextResult result: results) {
                    ret += "Format: " + result.barcodeFormatString + ", ";
                    ret += "Text: " + result.barcodeText + ". ";
                }

                return ret;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "No Barcode Detected."; 
    }
}
