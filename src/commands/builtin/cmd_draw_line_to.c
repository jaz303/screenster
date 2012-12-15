if (draw_state.flags & DRAW_PEN_ON) {
	al_draw_line(draw_state.x, draw_state.y, x, y, draw_state.pen_color, 0);
}
draw_state.x = x;
draw_state.y = y;