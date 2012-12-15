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
                :params => [:C, "screen-id"] },
      0x03 => { :name => "SET-ACTIVE-SCREEN",
                :params => [:C, "screen-id"] }
    },

    # Graphics - General
    0x41 => {
      0x01 => { :name => "SAVE-GRAPHICS-STATE" },
      0x02 => { :name => "RESTORE-GRAPHICS-STATE" },
      0x03 => { :name => "SET-CLIP-RECT",
                :params => [:L, "x", :L, "y", :L, "w", :L, "h"] },
      #0x04 SET-CLIP-CIRCLE
      #0x05 SET-CLIP-POLYGON
      0x06 => { :name => "CLEAR-CLIP" }
    },

    # Graphics - Primitives
    0x46 => {
      0x01 => { :name => "SET-DRAWING-MODE",
                :params => [:C, "pen-enabled", :C, "fill-enabled"] },
      0x02 => { :name => "PEN-ON" },
      0x03 => { :name => "PEN-OFF" },
      0x04 => { :name => "FILL-ON" },
      0x05 => { :name => "FILL-OFF" },
      0x06 => { :name => "DRAW-RECT",
                :params => [:L, "x", :L, "y", :L, "w", :L, "h"] },
      0x07 => { :name => "DRAW-CIRCLE",
                :params => [:L, "cx", :L, "cy", :L, "r"] },
      0x08 => { :name => "DRAW-LINE",
                :params => [:L, "x1", :L, "y1", :L, "x2", :L, "y2"] },
      0x09 => { :name => "DRAW-LINE-TO",
                :params => [:L, "x", :L, "y"] }
    }
    
  },
  
  :test => {
    
    0xFF => {
      0x01 => { :name => "HELLO" },
      0x02 => { :name => "GOODBYE" }
    }
    
  }
}