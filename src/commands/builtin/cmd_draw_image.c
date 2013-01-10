obj_image_t *image = OBJ_GET_SAFE(image_id, obj_image_t*, kObjectTypeImage);
if (image) {
    al_draw_bitmap(image->bitmap, x, y, 0);
}