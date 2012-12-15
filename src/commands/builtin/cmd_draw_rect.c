if (draw_state.flags & DRAW_FILL_ON) {
	al_draw_filled_rectangle(x, y, x + w, y + h, draw_state.fill_color);
}

if (draw_state.flags & DRAW_PEN_ON) {
	al_draw_rectangle(x, y, x + w, y + h, draw_state.pen_color, 0);
}