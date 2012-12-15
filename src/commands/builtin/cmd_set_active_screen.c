if (screen_id < 1 || screen_id > MAX_SCREENS || screens[screen_id] == 0) {
	// TODO: error
	return;
}

active_screen = screen_id;
al_set_target_backbuffer(screens[active_screen]);

// TODO: reply