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
  s.homepage         = 'https://github.com/yushulx/flutter_barcode_sdk/tree/main/packages/flutter_barcode_sdk_macos'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'yushulx' => 'lingxiao1002@gmail.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.vendored_libraries = 'Libraries/*.dylib'
  # The Dynamsoft SDK locates its preset templates and models in Templates/
  # and Models/ directories next to its dylibs (i.e. Contents/Frameworks in
  # the built app). CocoaPods offers no way for a pod to inject files there,
  # so the app target must copy them at build time — see the "Copy Dynamsoft
  # Resources" build phase in the example's Runner target. preserve_paths
  # keeps the Resources folder in the pod sandbox so that step always finds
  # it. Do NOT use resource_bundles here: the files would end up buried in a
  # .bundle inside the plugin framework where the SDK cannot find them.
  s.preserve_paths = 'Resources'
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