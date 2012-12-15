require 'socket'

require File.dirname(__FILE__) + '/handlers'

module Screenster
  class Client

    def initialize(io)
      @io = io
    end

    HANDLERS.values.each do |group|
      group.each do |category,commands|
        commands.each do |cmd,cmd_def|
          method_name = cmd_def[:name].downcase.gsub('-', '_')

          param_names, pack_string = [], ""
          
          ix = 0
          while ix < (cmd_def[:params] || []).length
            pack_string << cmd_def[:params][ix].to_s
            param_names << cmd_def[:params][ix+1].gsub('-', '_')
            ix += 2
          end
          
          param_names = param_names.join(',')
          arg_names = param_names
          arg_names = (',' + arg_names) if arg_names.length > 0
          
          class_eval <<-CODE
            def #{method_name}(#{param_names})
              send_msg(#{category}, #{cmd}, "#{pack_string}" #{arg_names})
            end
          CODE
        end
      end
    end

    private

    def send_msg(cat, msg, param_types, *params)
      payload = params.pack(param_types)
      msg = [cat, msg, payload.length].pack("CCS") + payload
      @io.write msg
    end

  end
end

$client = Screenster::Client.new(TCPSocket.open("localhost", 32000))

$client.create_screen 800, 600, 0
$client.set_active_screen 1
$client.draw_line 10.0, 10.0, 50.0, 10.0
$client.flip

gets
$client.destroy_screen 1

exit


# uint64    Q
# uint32    L
# uint16    S
# uint8     C

# int64     q
# int32     l
# int16     s
# int8      c

# bool
# string      

def send_msg(cat, msg, param_types = "", *params)
  payload = params.pack(param_types)
  msg = [cat, msg, payload.length].pack("CCS") + payload
  p msg.bytes.to_a
  $socket.write msg
end

# $bridge.all_reset
# $bridge.create_screen 1024, 768, 1

#send_msg 0x40, 0x02, "LLC", 800, 600, 0
#send_msg 0x40, 0x01, "LLC", 800, 600, 0
send_msg 0x40, 0x02, "C", 1

$socket.close
