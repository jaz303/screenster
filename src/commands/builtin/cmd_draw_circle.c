if (draw_state.flags & DRAW_FILL_ON) {
	al_draw_filled_circle(cx, cy, r, draw_state.fill_color);
}

if (draw_state.flags & DRAW_PEN_ON) {
	al_draw_circle(cx, cy, r, draw_state.pen_color, 0);
}