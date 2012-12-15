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