if (draw_state.flags & DRAW_PEN_ON) {
	al_draw_line(x1, y1, x2, y2, draw_state.pen_color, 0);
}
draw_state.x = x2;
draw_state.y = y2;