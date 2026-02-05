// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "flutter_barcode_sdk",
    platforms: [
        .iOS(.v11)
    ],
    products: [
        .library(name: "flutter-barcode-sdk", targets: ["flutter_barcode_sdk"])
    ],
    dependencies: [
        .package(url: "https://github.com/Dynamsoft/barcode-reader-spm", from: "11.2.5000")
    ],
    targets: [
        .target(
            name: "flutter_barcode_sdk",
            dependencies: [
                .product(name: "DynamsoftBarcodeReaderBundle", package: "barcode-reader-spm")
            ],
            path: "Classes",
            resources: [
                .process("../Assets")
            ]
        )
    ]
)
