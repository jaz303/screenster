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
    
    
    /* stub handler */
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
    
    al_destroy_display(screens[screen_id]);
    screens[screen_id] = 0;
    
    if (active_screen == screen_id) {
    	active_screen = 0;
    }
    
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
    
    // TODO: reply
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
    create_category(64, 3);
    install_handler(64, 1, cmd_create_screen);
    install_handler(64, 2, cmd_destroy_screen);
    install_handler(64, 3, cmd_set_active_screen);
    create_category(255, 2);
    install_handler(255, 1, cmd_hello);
    install_handler(255, 2, cmd_goodbye);
}

