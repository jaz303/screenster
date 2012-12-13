if (cmd_len != 1) {
	// TODO: error
	return;
}

char screen_id = READ_BYTE();

if (screen_id < 1 || screen_id > MAX_SCREENS || screens[screen_id] == 0) {
	// TODO: error
	return;
}

active_screen = screen_id;

// TODO: reply