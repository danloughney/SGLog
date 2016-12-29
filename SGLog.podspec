Pod::Spec.new do |s|
  s.name                        = "SGLog"
  s.version                     = "0.0.8"
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
  s.social_media_url            = "http://twitter.com/dcloughney"
  s.platform                    = :ios
  s.ios.deployment_target       = "8.0"
  s.source                      = { :git => "https://github.com/danloughney/SGLog.git", :tag => "0.0.7" }
  s.source_files                = "Classes", "SGLog/*.{h,m}"
  s.public_header_files         = "SGLog/*.h"
  s.requires_arc                = true
end
