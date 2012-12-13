require 'socket'

$socket = TCPSocket.open("localhost", 32000)

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

send_msg 0xff, 0x01, "LLC", 1024, 768, 1
send_msg 0xff, 0x02


#send_msg 0x40, 0x01, "LLC",  1024, 768, 1

$socket.close