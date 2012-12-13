/* This file is auto-generated (run `rake generate_handlers`) */

HANDLER_FN(cmd_hello) {
    uint32_t a1 = READ_UINT32();
    uint32_t a2 = READ_UINT32();
    uint32_t a3 = READ_BYTE();
    printf("hello! %d %d %d\n", a1, a2, a3);
}

HANDLER_FN(cmd_goodbye) {
    printf("goodbye!\n");
}

HANDLER_FN(cmd_all_reset) {
    /* stub handler */
}

HANDLER_FN(cmd_frame_reset) {
    /* stub handler */
}

HANDLER_FN(cmd_create_screen) {
    /* stub handler */
}

HANDLER_FN(cmd_destroy_screen) {
    /* stub handler */
}

void handlers_init() {
    create_category(255, 2);
    install_handler(255, 1, cmd_hello);
    install_handler(255, 2, cmd_goodbye);
    create_category(0, 2);
    install_handler(0, 1, cmd_all_reset);
    install_handler(0, 2, cmd_frame_reset);
    create_category(64, 2);
    install_handler(64, 1, cmd_create_screen);
    install_handler(64, 2, cmd_destroy_screen);
}

