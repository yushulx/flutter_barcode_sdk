package com.dynamsoft.flutter_barcode_sdk;

import android.util.Log;

import com.dynamsoft.barcode.BarcodeReader;
import com.dynamsoft.barcode.BarcodeReaderException;
import com.dynamsoft.barcode.EnumBarcodeFormat;
import com.dynamsoft.barcode.EnumIntermediateResultType;
import com.dynamsoft.barcode.PublicRuntimeSettings;
import com.dynamsoft.barcode.TextResult;

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
        catch (BarcodeReaderException e) {
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
        } catch (BarcodeReaderException e) {
            e.printStackTrace();
        }
        return "No Barcode Detected.";
    }
}
