// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "flutter_barcode_sdk_ios",
    platforms: [
        .iOS(.v11)
    ],
    products: [
        .library(name: "flutter-barcode-sdk-ios", targets: ["flutter_barcode_sdk_ios"])
    ],
    dependencies: [
        .package(url: "https://github.com/Dynamsoft/barcode-reader-spm", from: "11.6.1000")
    ],
    targets: [
        .target(
            name: "flutter_barcode_sdk_ios",
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
