#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_barcode_sdk.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_barcode_sdk'
  s.version          = '0.0.1'
  s.summary          = 'A flutter plugin of Dynamsoft Barcode Reader.'
  s.description      = <<-DESC
  A flutter plugin of Dynamsoft Barcode Reader.
                       DESC
  s.homepage         = 'https://www.dynamsoft.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Dynamsoft' => 'support@dynamsoft.com' }
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.dependency 'Flutter'
  s.vendored_frameworks = 'DynamsoftBarcodeReader.framework'
  
  s.platform = :ios, '9.0'
end
