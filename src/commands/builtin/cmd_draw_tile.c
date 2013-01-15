obj_tileset_t *tileset = OBJ_GET_SAFE(tileset_id, obj_tileset_t*, kObjectTypeTileset);

if (!tileset) {
    // TODO: signal error
    return;
}

if (tile_number >= tileset->tile_count) {
    // TODO: signal error
    return;
}

al_draw_bitmap_region(tileset->bitmap,
                      tileset->tiles[tile_number].x,
                      tileset->tiles[tile_number].y,
                      tileset->tile_width,
                      tileset->tile_height,
                      x,
                      y,
                      0);
