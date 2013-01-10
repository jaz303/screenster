printf("new display requested (%dx%d,fullscreen=%d)\n", width, height, fullscreen);

int flags = fullscreen ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED;
obj_id_t screen_id = create_screen(width, height, flags);

if (OBJ_SUCCESS(screen_id)) {
    // TODO: reply
}