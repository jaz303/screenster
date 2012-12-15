/* This file is auto-generated (run `rake generate_handlers`) */

HANDLER_FN(cmd_all_reset) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    /* stub handler */
}

HANDLER_FN(cmd_frame_reset) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    

}

HANDLER_FN(cmd_create_screen) {
    if (cmd_len != 9) {
        // TODO: handle error
        return;
    }
    
    uint32_t width = READ_UINT32();
    uint32_t height = READ_UINT32();
    unsigned char fullscreen = READ_BYTE();
    
    printf("new display requested (%dx%d,fullscreen=%d)\n", width, height, fullscreen);
    
    int created = 0;
    
    for (int i = 1; i <= MAX_SCREENS; ++i) {
    	if (screens[i] == 0) {
    
    		printf("creating display (id=%d)\n", i);
    		
    		int flags = fullscreen ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED;
    		
    		al_set_new_display_flags(flags);
    		screens[i] = al_create_display(width, height);
    
    		if (screens[i]) {
    			created = 1;
    		}
    		
    		break;
    	
    	}
    }
    
    if (created) {
    	// TODO: reply
    }
}

HANDLER_FN(cmd_destroy_screen) {
    if (cmd_len != 1) {
        // TODO: handle error
        return;
    }
    
    unsigned char screen_id = READ_BYTE();
    
    printf("destroy screen requested (id=%d)\n", screen_id);
    
    if (screen_id < 1 || screen_id > MAX_SCREENS || screens[screen_id] == 0) {
    	printf("invalid screen id\n", screen_id);
    	// TODO: handle error
    	return;
    }
    
    if (active_screen == screen_id) {
    	active_screen = 0;
    	al_set_target_bitmap(NULL);
    }
    
    al_destroy_display(screens[screen_id]);
    screens[screen_id] = NULL;
    
    // TODO: reply
}

HANDLER_FN(cmd_set_active_screen) {
    if (cmd_len != 1) {
        // TODO: handle error
        return;
    }
    
    unsigned char screen_id = READ_BYTE();
    
    if (screen_id < 1 || screen_id > MAX_SCREENS || screens[screen_id] == 0) {
    	// TODO: error
    	return;
    }
    
    active_screen = screen_id;
    al_set_target_backbuffer(screens[active_screen]);
    
    // TODO: reply
}

HANDLER_FN(cmd_clear_screen) {
    if (cmd_len != 4) {
        // TODO: handle error
        return;
    }
    
    uint32_t color = READ_UINT32();
    
    al_clear_to_color(COLOR_TO_ALLEGRO(color));
}

HANDLER_FN(cmd_flip) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    al_flip_display();
}

HANDLER_FN(cmd_save_graphics_state) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    save_graphics_state();
}

HANDLER_FN(cmd_restore_graphics_state) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    restore_graphics_state();
}

HANDLER_FN(cmd_set_clip_rect) {
    if (cmd_len != 16) {
        // TODO: handle error
        return;
    }
    
    uint32_t x = READ_UINT32();
    uint32_t y = READ_UINT32();
    uint32_t w = READ_UINT32();
    uint32_t h = READ_UINT32();
    
    al_set_clipping_rectangle(x, y, w, h);
}

HANDLER_FN(cmd_clear_clip) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    al_reset_clipping_rectangle();
}

HANDLER_FN(cmd_set_drawing_mode) {
    if (cmd_len != 2) {
        // TODO: handle error
        return;
    }
    
    unsigned char pen_enabled = READ_BYTE();
    unsigned char fill_enabled = READ_BYTE();
    
    draw_state.flags &= ~(DRAW_PEN_ON | DRAW_FILL_ON);
    
    if (pen_enabled)	draw_state.flags |= DRAW_PEN_ON;
    if (fill_enabled)	draw_state.flags |= DRAW_FILL_ON;
}

HANDLER_FN(cmd_pen_on) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    draw_state.flags |= DRAW_PEN_ON;
}

HANDLER_FN(cmd_pen_off) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    draw_state.flags &= ~DRAW_PEN_ON;
}

HANDLER_FN(cmd_fill_on) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    draw_state.flags |= DRAW_FILL_ON;
}

HANDLER_FN(cmd_fill_off) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    draw_state.flags &= ~DRAW_FILL_ON;
}

HANDLER_FN(cmd_set_pen_color) {
    if (cmd_len != 4) {
        // TODO: handle error
        return;
    }
    
    uint32_t argb = READ_UINT32();
    
    draw_state.pen_color = COLOR_TO_ALLEGRO(argb);
}

HANDLER_FN(cmd_set_fill_color) {
    if (cmd_len != 4) {
        // TODO: handle error
        return;
    }
    
    uint32_t argb = READ_UINT32();
    
    draw_state.fill_color = COLOR_TO_ALLEGRO(argb);
}

HANDLER_FN(cmd_draw_rect) {
    if (cmd_len != 16) {
        // TODO: handle error
        return;
    }
    
    float x = READ_FLOAT();
    float y = READ_FLOAT();
    float w = READ_FLOAT();
    float h = READ_FLOAT();
    
    if (draw_state.flags & DRAW_FILL_ON) {
    	al_draw_filled_rectangle(x, y, x + w, y + h, draw_state.fill_color);
    }
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_rectangle(x, y, x + w, y + h, draw_state.pen_color, 0);
    }
}

HANDLER_FN(cmd_draw_circle) {
    if (cmd_len != 12) {
        // TODO: handle error
        return;
    }
    
    float cx = READ_FLOAT();
    float cy = READ_FLOAT();
    float r = READ_FLOAT();
    
    if (draw_state.flags & DRAW_FILL_ON) {
    	al_draw_filled_circle(cx, cy, r, draw_state.fill_color);
    }
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_circle(cx, cy, r, draw_state.pen_color, 0);
    }
}

HANDLER_FN(cmd_draw_line) {
    if (cmd_len != 16) {
        // TODO: handle error
        return;
    }
    
    float x1 = READ_FLOAT();
    float y1 = READ_FLOAT();
    float x2 = READ_FLOAT();
    float y2 = READ_FLOAT();
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_line(x1, y1, x2, y2, draw_state.pen_color, 0);
    }
    draw_state.x = x2;
    draw_state.y = y2;
}

HANDLER_FN(cmd_draw_line_to) {
    if (cmd_len != 8) {
        // TODO: handle error
        return;
    }
    
    float x = READ_FLOAT();
    float y = READ_FLOAT();
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_line(draw_state.x, draw_state.y, x, y, draw_state.pen_color, 0);
    }
    draw_state.x = x;
    draw_state.y = y;
}

HANDLER_FN(cmd_hello) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    /* stub handler */
}

HANDLER_FN(cmd_goodbye) {
    if (cmd_len != 0) {
        // TODO: handle error
        return;
    }
    
    
    /* stub handler */
}

void handlers_init() {
    create_category(0, 2);
    install_handler(0, 1, cmd_all_reset);
    install_handler(0, 2, cmd_frame_reset);
    create_category(64, 5);
    install_handler(64, 1, cmd_create_screen);
    install_handler(64, 2, cmd_destroy_screen);
    install_handler(64, 3, cmd_set_active_screen);
    install_handler(64, 4, cmd_clear_screen);
    install_handler(64, 5, cmd_flip);
    create_category(65, 6);
    install_handler(65, 1, cmd_save_graphics_state);
    install_handler(65, 2, cmd_restore_graphics_state);
    install_handler(65, 3, cmd_set_clip_rect);
    install_handler(65, 6, cmd_clear_clip);
    create_category(70, 11);
    install_handler(70, 1, cmd_set_drawing_mode);
    install_handler(70, 2, cmd_pen_on);
    install_handler(70, 3, cmd_pen_off);
    install_handler(70, 4, cmd_fill_on);
    install_handler(70, 5, cmd_fill_off);
    install_handler(70, 6, cmd_set_pen_color);
    install_handler(70, 7, cmd_set_fill_color);
    install_handler(70, 8, cmd_draw_rect);
    install_handler(70, 9, cmd_draw_circle);
    install_handler(70, 10, cmd_draw_line);
    install_handler(70, 11, cmd_draw_line_to);
    create_category(255, 2);
    install_handler(255, 1, cmd_hello);
    install_handler(255, 2, cmd_goodbye);
}

