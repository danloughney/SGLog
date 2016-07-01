Pod::Spec.new do |s|
  s.name                        = "SGLog"
  s.version                     = "0.0.2"
  s.summary                     = "ObjectiveC logging macros"
  s.description                 = <<-DESC
        SGLog is a macro based logger which allows you to embed significant logging code in your
        projects. You can categorize your log messages and write them out conditionally. Finally when
        you are ready to ship, all of your logging messages are compiled out of your archive protecting
        your proprietary info and preventing your app from spamming the iOS console.
        DESC
  s.homepage                    = "https://github.com/danloughney/SGLog"
  s.license                     = { :type => "MIT", :file => "LICENSE" }
  s.author                      = { "Daniel Loughney" => "danloughney@gmail.com" }
  s.social_media_url            = 'http://twitter.com/dcloughney'
  s.platform                    = :ios
  s.ios.deployment_target       = '8.0'
  # s.watchos.deployment_target = "2.0"
  # s.tvos.deployment_target    = "9.0"
  s.source                      = { :git => "https://github.com/danloughney/SGLog.git", :tag => "0.0.2" }
  s.source_files                = "Classes", "SGLog/*.{h,m}"
  s.public_header_files         = "SGLog/*.h"
  s.framework                   = "UIKit"
  s.requires_arc                = true

  #s.exclude_files               = "Classes/Exclude"
  # s.xcconfig = { "HEADER_SEARCH_PATHS" => "$(SDKROOT)/usr/include/libxml2" }
  # s.dependency "JSONKit", "~> 1.4"

end
