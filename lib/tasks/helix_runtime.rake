require 'helix_runtime/build_task'

HelixRuntime::BuildTask.new("glare")

task :default => :build
