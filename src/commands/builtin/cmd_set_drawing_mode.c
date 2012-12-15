draw_state.flags &= ~(DRAW_PEN_ON | DRAW_FILL_ON);

if (pen_enabled)	draw_state.flags |= DRAW_PEN_ON;
if (fill_enabled)	draw_state.flags |= DRAW_FILL_ON;