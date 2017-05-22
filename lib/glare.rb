require "helix_runtime"

begin
  require "glare/native"
rescue LoadError
  warn "Unable to load glare/native. Please run `rake build`"
end
