HANDLERS = {
  :builtin => {
    
    # Object commands
    0x00 => {
      0x01 => { :name => "RELEASE-OBJECT",
                :params => [:O, "object-id"] }
    },
    
    # Control Commands
    0x10 => {
      0x01 => { :name => "ALL-RESET" },
      0x02 => { :name => "FRAME-RESET" }
    },
    
    # Graphics - Screen
    0x40 => {
      0x01 => { :name => "CREATE-SCREEN",
                :params => [:L, "width", :L, "height", :C, "fullscreen"],
                :return => [:O, "screen-id"] },
      0x02 => { :name => "SET-ACTIVE-SCREEN",
                :params => [:O, "screen-id"] },
      0x03 => { :name => "CLEAR-SCREEN",
                :params => [:L, "color"] },
      0x04 => { :name => "FLIP" }
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
      0x06 => { :name => "SET-PEN-COLOR",
                :params => [:L, "argb"] },
      0x07 => { :name => "SET-FILL-COLOR",
                :params => [:L, "argb"] },
      0x08 => { :name => "DRAW-RECT",
                :params => [:f, "x", :f, "y", :f, "w", :f, "h"] },
      0x09 => { :name => "DRAW-CIRCLE",
                :params => [:f, "cx", :f, "cy", :f, "r"] },
      0x0A => { :name => "DRAW-LINE",
                :params => [:f, "x1", :f, "y1", :f, "x2", :f, "y2"] },
      0x0B => { :name => "DRAW-LINE-TO",
                :params => [:f, "x", :f, "y"] }
    },
    
    0xFF => {
      0x01 => { :name => "TEST-ECHO",
                :params => [:A, "string"] }
    }
    
  }
}