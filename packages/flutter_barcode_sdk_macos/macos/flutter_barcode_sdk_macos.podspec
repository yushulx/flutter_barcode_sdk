#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_barcode_sdk_macos.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_barcode_sdk_macos'
  s.version          = '1.0.0'
  s.summary          = 'A macOS flutter plugin for Dynamsoft Barcode Reader.'
  s.description      = <<-DESC
  A macOS flutter plugin for Dynamsoft Barcode Reader.
                       DESC
  s.homepage         = 'https://github.com/yushulx/flutter_barcode_sdk_macos'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'yushulx' => 'lingxiao1002@gmail.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.vendored_libraries = 'Libraries/*.dylib'
  s.resource_bundles = { 'flutter_barcode_sdk_macos' => ['Resources/**/*'] }
  s.dependency 'FlutterMacOS'
  s.platform         = :osx, '12.0'
  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    'HEADER_SEARCH_PATHS' => '"$(PODS_TARGET_SRCROOT)/Libraries"',
    'LIBRARY_SEARCH_PATHS' => '"$(PODS_TARGET_SRCROOT)/Libraries"',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++',
    'SWIFT_VERSION' => '5.0'
  }
end