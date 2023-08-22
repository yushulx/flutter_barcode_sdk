#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_barcode_sdk.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_barcode_sdk'
  s.version          = '2.0.0'
  s.summary          = 'A flutter plugin of Dynamsoft Barcode Reader.'
  s.description      = <<-DESC
  A flutter plugin of Dynamsoft Barcode Reader.
                       DESC
  s.homepage         = 'https://github.com/yushulx/flutter_barcode_sdk'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'yushulx' => 'lingxiao1002@gmail.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'

  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
  s.vendored_libraries = ['lib/libDynamsoftBarcodeReader.dylib', 'lib/libDynamsoftLicenseClient.dylib', 'lib/libDynamicPdf.dylib']
end
