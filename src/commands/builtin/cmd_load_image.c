printf("load image requested (filename=%s)\n", filename);

obj_id_t image_id = create_image(filename);
if (OBJ_SUCCESS(image_id)) {
    // TODO: success
} else {
    // TODO: error
}