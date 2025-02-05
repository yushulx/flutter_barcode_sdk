#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint flutter_barcode_sdk.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'flutter_barcode_sdk'
  s.version          = '3.0.0'
  s.summary          = 'A flutter plugin for Dynamsoft Barcode Reader.'
  s.description      = <<-DESC
  A flutter plugin for Dynamsoft Barcode Reader.
                       DESC
  s.homepage         = 'https://github.com/yushulx/flutter_barcode_sdk'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'yushulx' => 'lingxiao1002@gmail.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.swift_version = '5.0'
  s.platform = :osx, '10.11'
  s.dependency 'DCV'
  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    # 'FRAMEWORK_SEARCH_PATHS' => '"$(PODS_TARGET_SRCROOT)/frameworks"',
    # 'OTHER_LDFLAGS' => '-framework DCV',
    # 'LD_RUNPATH_SEARCH_PATHS' => '@executable_path/../Frameworks'
  }
  
  # s.vendored_frameworks = ['frameworks/DCV.framework']
end
