HANDLERS = {
  :builtin => {
    
    # Control Commands
    0x00 => {
      0x01 => { :name => "ALL-RESET" },
      0x02 => { :name => "FRAME-RESET" }
    },
    
    # Graphics - Screen
    0x40 => {
      0x01 => { :name => "CREATE-SCREEN",
                :params => [:L, "width", :L, "height", :C, "fullscreen"],
                :return => [:L, "screen-id"] },
      0x02 => { :name => "DESTROY-SCREEN",
                :params => [:L, "screen-id"] }
    },
    
  },
  
  :test => {
    
    0xFF => {
      0x01 => { :name => "HELLO" },
      0x02 => { :name => "GOODBYE" }
    }
    
  }
}