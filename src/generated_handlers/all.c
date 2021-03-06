/* This file is auto-generated (run `rake generate_handlers`) */

HANDLER_FN(cmd_all_reset) {
    
    /* stub handler */
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_frame_reset) {
    
    
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_load_image) {
    char* filename = READ_STRING();
    
    printf("load image requested (filename=%s)\n", filename);
    
    obj_id_t image_id = create_image(filename);
    if (OBJ_SUCCESS(image_id)) {
        // TODO: success
    } else {
        // TODO: error
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_image) {
    obj_id_t image_id = READ_FIXED_LENGTH(uint32_t, uint32);
    float x = READ_FIXED_LENGTH(float, float);
    float y = READ_FIXED_LENGTH(float, float);
    
    obj_image_t *image = OBJ_GET_SAFE(image_id, obj_image_t*, kObjectTypeImage);
    if (image) {
        al_draw_bitmap(image->bitmap, x, y, 0);
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_release_object) {
    obj_id_t object_id = READ_FIXED_LENGTH(uint32_t, uint32);
    
    obj_t *obj = obj_get(object_id);
    if (obj) {
        obj_release(obj);
        // TODO: success
    } else {
        // TODO: fail
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_load_tileset) {
    char* filename = READ_STRING();
    uint16_t tile_width = READ_FIXED_LENGTH(uint16_t, uint16);
    uint16_t tile_height = READ_FIXED_LENGTH(uint16_t, uint16);
    uint32_t mask_color = READ_FIXED_LENGTH(uint32_t, uint32);
    
    obj_id_t tileset_id = load_tileset_from_file(filename, tile_width, tile_height, mask_color);
    if (OBJ_SUCCESS(tileset_id)) {
        printf("tileset loaded, id = %d\n", tileset_id);
        // TODO: success
    } else {
        // TODO: error
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_create_tileset) {
    obj_id_t image_id = READ_FIXED_LENGTH(uint32_t, uint32);
    uint16_t tile_width = READ_FIXED_LENGTH(uint16_t, uint16);
    uint16_t tile_height = READ_FIXED_LENGTH(uint16_t, uint16);
    uint32_t mask_color = READ_FIXED_LENGTH(uint32_t, uint32);
    
    obj_id_t tileset_id = create_tileset_from_image(image_id, tile_width, tile_height, mask_color);
    if (OBJ_SUCCESS(tileset_id)) {
        // TODO: success
    } else {
        // TODO: error
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_tile) {
    obj_id_t tileset_id = READ_FIXED_LENGTH(uint32_t, uint32);
    uint16_t tile_number = READ_FIXED_LENGTH(uint16_t, uint16);
    float x = READ_FIXED_LENGTH(float, float);
    float y = READ_FIXED_LENGTH(float, float);
    
    obj_tileset_t *tileset = OBJ_GET_SAFE(tileset_id, obj_tileset_t*, kObjectTypeTileset);
    
    if (!tileset) {
        // TODO: signal error
        return;
    }
    
    if (tile_number >= tileset->tile_count) {
        // TODO: signal error
        return;
    }
    
    al_draw_bitmap_region(tileset->bitmap,
                          tileset->tiles[tile_number].x,
                          tileset->tiles[tile_number].y,
                          tileset->tile_width,
                          tileset->tile_height,
                          x,
                          y,
                          0);
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_test_echo) {
    char* string = READ_STRING();
    
    printf("len=> %d, string=> %s\n", strlen(string), string);
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_create_screen) {
    uint32_t width = READ_FIXED_LENGTH(uint32_t, uint32);
    uint32_t height = READ_FIXED_LENGTH(uint32_t, uint32);
    unsigned char fullscreen = READ_FIXED_LENGTH(unsigned char, byte);
    
    printf("new display requested (%dx%d,fullscreen=%d)\n", width, height, fullscreen);
    
    int flags = fullscreen ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED;
    obj_id_t screen_id = create_screen(width, height, flags);
    
    if (OBJ_SUCCESS(screen_id)) {
        // TODO: reply
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_set_active_screen) {
    obj_id_t screen_id = READ_FIXED_LENGTH(uint32_t, uint32);
    
    if (SUCCESS(activate_screen(screen_id))) {
        // TODO: success
    } else {
        // TODO: error
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_clear_screen) {
    uint32_t color = READ_FIXED_LENGTH(uint32_t, uint32);
    
    al_clear_to_color(COLOR_TO_ALLEGRO(color));
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_flip) {
    
    al_flip_display();
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_fill_off) {
    
    draw_state.flags &= ~DRAW_FILL_ON;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_line_to) {
    float x = READ_FIXED_LENGTH(float, float);
    float y = READ_FIXED_LENGTH(float, float);
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_line(draw_state.x, draw_state.y, x, y, draw_state.pen_color, 0);
    }
    draw_state.x = x;
    draw_state.y = y;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_set_pen_color) {
    uint32_t argb = READ_FIXED_LENGTH(uint32_t, uint32);
    
    draw_state.pen_color = COLOR_TO_ALLEGRO(argb);
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_set_drawing_mode) {
    unsigned char pen_enabled = READ_FIXED_LENGTH(unsigned char, byte);
    unsigned char fill_enabled = READ_FIXED_LENGTH(unsigned char, byte);
    
    draw_state.flags &= ~(DRAW_PEN_ON | DRAW_FILL_ON);
    
    if (pen_enabled)	draw_state.flags |= DRAW_PEN_ON;
    if (fill_enabled)	draw_state.flags |= DRAW_FILL_ON;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_set_fill_color) {
    uint32_t argb = READ_FIXED_LENGTH(uint32_t, uint32);
    
    draw_state.fill_color = COLOR_TO_ALLEGRO(argb);
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_pen_on) {
    
    draw_state.flags |= DRAW_PEN_ON;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_rect) {
    float x = READ_FIXED_LENGTH(float, float);
    float y = READ_FIXED_LENGTH(float, float);
    float w = READ_FIXED_LENGTH(float, float);
    float h = READ_FIXED_LENGTH(float, float);
    
    if (draw_state.flags & DRAW_FILL_ON) {
    	al_draw_filled_rectangle(x, y, x + w, y + h, draw_state.fill_color);
    }
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_rectangle(x, y, x + w, y + h, draw_state.pen_color, 0);
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_pen_off) {
    
    draw_state.flags &= ~DRAW_PEN_ON;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_circle) {
    float cx = READ_FIXED_LENGTH(float, float);
    float cy = READ_FIXED_LENGTH(float, float);
    float r = READ_FIXED_LENGTH(float, float);
    
    if (draw_state.flags & DRAW_FILL_ON) {
    	al_draw_filled_circle(cx, cy, r, draw_state.fill_color);
    }
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_circle(cx, cy, r, draw_state.pen_color, 0);
    }
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_fill_on) {
    
    draw_state.flags |= DRAW_FILL_ON;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_draw_line) {
    float x1 = READ_FIXED_LENGTH(float, float);
    float y1 = READ_FIXED_LENGTH(float, float);
    float x2 = READ_FIXED_LENGTH(float, float);
    float y2 = READ_FIXED_LENGTH(float, float);
    
    if (draw_state.flags & DRAW_PEN_ON) {
    	al_draw_line(x1, y1, x2, y2, draw_state.pen_color, 0);
    }
    draw_state.x = x2;
    draw_state.y = y2;
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_clear_clip) {
    
    al_reset_clipping_rectangle();
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_save_graphics_state) {
    
    save_graphics_state();
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_restore_graphics_state) {
    
    restore_graphics_state();
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

HANDLER_FN(cmd_set_clip_rect) {
    uint32_t x = READ_FIXED_LENGTH(uint32_t, uint32);
    uint32_t y = READ_FIXED_LENGTH(uint32_t, uint32);
    uint32_t w = READ_FIXED_LENGTH(uint32_t, uint32);
    uint32_t h = READ_FIXED_LENGTH(uint32_t, uint32);
    
    al_set_clipping_rectangle(x, y, w, h);
    
    return;
    
    param_error:
    fprintf(stderr, "parameter error!\n");
    return;
}

void handlers_init() {
    create_category(16, 2);
    install_handler(16, 1, cmd_all_reset);
    install_handler(16, 2, cmd_frame_reset);
    create_category(66, 2);
    install_handler(66, 1, cmd_load_image);
    install_handler(66, 2, cmd_draw_image);
    create_category(0, 1);
    install_handler(0, 1, cmd_release_object);
    create_category(67, 3);
    install_handler(67, 1, cmd_load_tileset);
    install_handler(67, 2, cmd_create_tileset);
    install_handler(67, 3, cmd_draw_tile);
    create_category(255, 1);
    install_handler(255, 1, cmd_test_echo);
    create_category(64, 4);
    install_handler(64, 1, cmd_create_screen);
    install_handler(64, 2, cmd_set_active_screen);
    install_handler(64, 3, cmd_clear_screen);
    install_handler(64, 4, cmd_flip);
    create_category(70, 11);
    install_handler(70, 5, cmd_fill_off);
    install_handler(70, 11, cmd_draw_line_to);
    install_handler(70, 6, cmd_set_pen_color);
    install_handler(70, 1, cmd_set_drawing_mode);
    install_handler(70, 7, cmd_set_fill_color);
    install_handler(70, 2, cmd_pen_on);
    install_handler(70, 8, cmd_draw_rect);
    install_handler(70, 3, cmd_pen_off);
    install_handler(70, 9, cmd_draw_circle);
    install_handler(70, 4, cmd_fill_on);
    install_handler(70, 10, cmd_draw_line);
    create_category(65, 6);
    install_handler(65, 6, cmd_clear_clip);
    install_handler(65, 1, cmd_save_graphics_state);
    install_handler(65, 2, cmd_restore_graphics_state);
    install_handler(65, 3, cmd_set_clip_rect);
}

