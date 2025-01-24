#include <iostream>

#ifndef TEMPLATE_H
#define TEMPLATE_H

const std::string jsonString = R"(
    {
        "CaptureVisionTemplates": [
            {
                "Name": "ReadBarcodes_Default",
                "ImageROIProcessingNameArray": [
                    "roi-read-barcodes"
                ]
            },
            {
                "Name": "ReadBarcodes_ReadRateFirst",
                "ImageROIProcessingNameArray": [
                    "roi-read-barcodes-read-rate"
                ],
                "Timeout": 100000
            },
            {
                "Name": "ReadBarcodes_SpeedFirst",
                "ImageROIProcessingNameArray": [
                    "roi-read-barcodes-speed-first"
                ]
            },
            {
                "Name": "ReadSingleBarcode",
                "ImageROIProcessingNameArray": [
                    "roi-read-single-barcode"
                ]
            }
        ],
        "TargetROIDefOptions": [
            {
                "Name": "roi-read-barcodes",
                "TaskSettingNameArray": [
                    "task-read-barcodes"
                ]
            },
            {
                "Name": "roi-read-barcodes-read-rate",
                "TaskSettingNameArray": [
                    "task-read-barcodes-read-rate"
                ]
            },
            {
                "Name": "roi-read-barcodes-speed-first",
                "TaskSettingNameArray": [
                    "task-read-barcodes-speed-first"
                ]
            },
            {
                "Name": "roi-read-single-barcode",
                "TaskSettingNameArray": [
                    "task-read-single-barcode"
                ]
            }
        ],
        "BarcodeReaderTaskSettingOptions": [
            {
                "Name": "task-read-barcodes",
                "ExpectedBarcodesCount": 0,
                "BarcodeFormatIds": [
                    "BF_DEFAULT"
                ],
                "BarcodeFormatSpecificationNameArray": [
                    "bfs1",
                    "bfs2"
                ],
                "SectionImageParameterArray": [
                    {
                        "Section": "ST_REGION_PREDETECTION",
                        "ImageParameterName": "ip-read-barcodes"
                    },
                    {
                        "Section": "ST_BARCODE_LOCALIZATION",
                        "ImageParameterName": "ip-read-barcodes"
                    },
                    {
                        "Section": "ST_BARCODE_DECODING",
                        "ImageParameterName": "ip-read-barcodes"
                    }
                ]
            },
            {
                "Name": "task-read-barcodes-read-rate",
                "ExpectedBarcodesCount": 999,
                "BarcodeFormatIds": [
                    "BF_DEFAULT"
                ],
                "LocalizationModes": [
                    {
                        "Mode": "LM_CONNECTED_BLOCKS"
                    },
                    {
                        "Mode": "LM_LINES"
                    },
                    {
                        "Mode": "LM_STATISTICS"
                    }
                ],
                "DeblurModes": null,
                "BarcodeFormatSpecificationNameArray": [
                    "bfs1-read-rate-first",
                    "bfs2-read-rate-first"
                ],
                "SectionImageParameterArray": [
                    {
                        "Section": "ST_REGION_PREDETECTION",
                        "ImageParameterName": "ip-read-barcodes-read-rate"
                    },
                    {
                        "Section": "ST_BARCODE_LOCALIZATION",
                        "ImageParameterName": "ip-read-barcodes-read-rate"
                    },
                    {
                        "Section": "ST_BARCODE_DECODING",
                        "ImageParameterName": "ip-read-barcodes-read-rate"
                    }
                ]
            },
            {
                "Name": "task-read-barcodes-speed-first",
                "ExpectedBarcodesCount": 0,
                "BarcodeFormatIds": [
                    "BF_DEFAULT"
                ],
                "LocalizationModes": [
                    {
                        "Mode": "LM_CONNECTED_BLOCKS"
                    }
                ],
                "DeblurModes": [
                    {
                        "Mode": "DM_BASED_ON_LOC_BIN"
                    },
                    {
                        "Mode": "DM_THRESHOLD_BINARIZATION"
                    },
                    {
                        "Mode": "DM_DIRECT_BINARIZATION"
                    }
                ],
                "BarcodeFormatSpecificationNameArray": [
                    "bfs1-speed-first",
                    "bfs2-speed-first"
                ],
                "SectionImageParameterArray": [
                    {
                        "Section": "ST_REGION_PREDETECTION",
                        "ImageParameterName": "ip-read-barcodes-speed-first"
                    },
                    {
                        "Section": "ST_BARCODE_LOCALIZATION",
                        "ImageParameterName": "ip-read-barcodes-speed-first"
                    },
                    {
                        "Section": "ST_BARCODE_DECODING",
                        "ImageParameterName": "ip-read-barcodes-speed-first"
                    }
                ]
            },
            {
                "Name": "task-read-single-barcode",
                "ExpectedBarcodesCount": 1,
                "BarcodeFormatIds": [
                    "BF_DEFAULT"
                ],
                "LocalizationModes": [
                    {
                        "Mode": "LM_SCAN_DIRECTLY"
                    },
                    {
                        "Mode": "LM_CONNECTED_BLOCKS"
                    }
                ],
                "DeblurModes": [
                    {
                        "Mode": "DM_BASED_ON_LOC_BIN"
                    },
                    {
                        "Mode": "DM_THRESHOLD_BINARIZATION"
                    }
                ],
                "BarcodeFormatSpecificationNameArray": [
                    "bfs1-single-barcode",
                    "bfs2-single-barcode"
                ],
                "SectionImageParameterArray": [
                    {
                        "Section": "ST_REGION_PREDETECTION",
                        "ImageParameterName": "ip-read-single-barcode"
                    },
                    {
                        "Section": "ST_BARCODE_LOCALIZATION",
                        "ImageParameterName": "ip-read-single-barcode"
                    },
                    {
                        "Section": "ST_BARCODE_DECODING",
                        "ImageParameterName": "ip-read-single-barcode"
                    }
                ]
            }
        ],
        "BarcodeFormatSpecificationOptions": [
            {
                "Name": "bfs1",
                "BarcodeFormatIds": [
                    "BF_PDF417",
                    "BF_QR_CODE",
                    "BF_DATAMATRIX",
                    "BF_AZTEC",
                    "BF_MICRO_QR",
                    "BF_MICRO_PDF417",
                    "BF_DOTCODE"
                ],
                "MirrorMode": "MM_BOTH"
            },
            {
                "Name": "bfs2",
                "BarcodeFormatIds": [
                    "BF_ALL"
                ],
                "MirrorMode": "MM_NORMAL"
            },
            {
                "Name": "bfs1-read-rate-first",
                "BarcodeFormatIds": [
                    "BF_PDF417",
                    "BF_QR_CODE",
                    "BF_DATAMATRIX",
                    "BF_AZTEC",
                    "BF_MICRO_QR",
                    "BF_MICRO_PDF417",
                    "BF_DOTCODE"
                ],
                "MirrorMode": "MM_BOTH"
            },
            {
                "Name": "bfs2-read-rate-first",
                "BarcodeFormatIds": [
                    "BF_ALL"
                ],
                "MirrorMode": "MM_NORMAL"
            },
            {
                "Name": "bfs1-speed-first",
                "BarcodeFormatIds": [
                    "BF_PDF417",
                    "BF_QR_CODE",
                    "BF_DATAMATRIX",
                    "BF_AZTEC",
                    "BF_MICRO_QR",
                    "BF_MICRO_PDF417",
                    "BF_DOTCODE"
                ],
                "MirrorMode": "MM_BOTH"
            },
            {
                "Name": "bfs2-speed-first",
                "BarcodeFormatIds": [
                    "BF_ALL"
                ],
                "MirrorMode": "MM_NORMAL"
            },
            {
                "Name": "bfs1-single-barcode",
                "BarcodeFormatIds": [
                    "BF_PDF417",
                    "BF_QR_CODE",
                    "BF_DATAMATRIX",
                    "BF_AZTEC",
                    "BF_MICRO_QR",
                    "BF_MICRO_PDF417",
                    "BF_DOTCODE"
                ],
                "MirrorMode": "MM_BOTH"
            },
            {
                "Name": "bfs2-single-barcode",
                "BarcodeFormatIds": [
                    "BF_ALL"
                ],
                "MirrorMode": "MM_NORMAL"
            }
        ],
        "ImageParameterOptions": [
            {
                "Name": "ip-read-barcodes",
                "TextDetectionMode": {
                    "Mode": "TTDM_LINE",
                    "Direction": "UNKNOWN",
                    "Sensitivity": 3
                },
                "IfEraseTextZone": 1
            },
            {
                "Name": "ip-read-barcodes-read-rate",
                "TextDetectionMode": {
                    "Mode": "TTDM_LINE",
                    "Direction": "UNKNOWN",
                    "Sensitivity": 3
                },
                "IfEraseTextZone": 1,
                "GrayscaleTransformationModes": [
                    {
                        "Mode": "GTM_ORIGINAL"
                    },
                    {
                        "Mode": "GTM_INVERTED"
                    }
                ],
                "ScaleDownThreshold": 100000
            },
            {
                "Name": "ip-read-barcodes-speed-first",
                "TextDetectionMode": {
                    "Mode": "TTDM_LINE",
                    "Direction": "UNKNOWN",
                    "Sensitivity": 3
                },
                "IfEraseTextZone": 1,
                "BinarizationModes": [
                    {
                        "Mode": "BM_LOCAL_BLOCK",
                        "BlockSizeX": 0,
                        "BlockSizeY": 0,
                        "EnableFillBinaryVacancy": 0
                    }
                ]
            },
            {
                "Name": "ip-read-single-barcode",
                "TextDetectionMode": {
                    "Mode": "TTDM_LINE",
                    "Direction": "UNKNOWN",
                    "Sensitivity": 3
                },
                "IfEraseTextZone": 1,
                "BinarizationModes": [
                    {
                        "Mode": "BM_LOCAL_BLOCK",
                        "BlockSizeX": 71,
                        "BlockSizeY": 71,
                        "EnableFillBinaryVacancy": 0
                    }
                ],
                "GrayscaleTransformationModes": [
                    {
                        "Mode": "GTM_ORIGINAL"
                    },
                    {
                        "Mode": "GTM_INVERTED"
                    }
                ]
            }
        ]
    })";

#endif