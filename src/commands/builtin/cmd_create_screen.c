if (cmd_len != 9) {
	// TODO: handle error
	return;
}

uint32_t width 	= READ_UINT32();
uint32_t height = READ_UINT32();
char fullscreen	= READ_BYTE();

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
