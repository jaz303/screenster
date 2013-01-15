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

          param_names, pack_string, args = [], "", []
          
          ix = 0
          while ix < (cmd_def[:params] || []).length
            param_type = cmd_def[:params][ix]
            param_name = cmd_def[:params][ix+1].gsub('-', '_')
            
            param_names << param_name
            
            param_type = :L if param_type == :O
              
            case param_type
            when :A
              pack_string << "SA\#{#{param_name}.length + 1}"
              args << "#{param_name}.length"
              args << param_name
            else
              pack_string << param_type.to_s
              args << param_name
            end
            
            ix += 2
          end
          
          param_names = param_names.join(',')
          
          args = args.join(',')
          args = (',' + args) if args.length > 0
          
          method = <<-CODE
            def #{method_name}(#{param_names})
              send_msg(#{category}, #{cmd}, "#{pack_string}" #{args})
            end
          CODE
          
          class_eval method
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

$client.test_echo "This is a test!\n"

$client.load_tileset "guardian.jpg", 30, 37, 0

x, y = 0, 0

100.times do
  x += 1
  y += 1
  
  $client.clear_screen 0xff0000
  $client.draw_line 10.0, 10.0, 50.0, 10.0
  $client.draw_rect x, y, 100, 200
  $client.draw_tile 3, x % 80, 200, 200
  $client.flip
  sleep 0.04
end

gets
$client.release_object 1

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
