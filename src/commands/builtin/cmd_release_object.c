obj_t *obj = obj_get(object_id);
if (obj) {
    obj_release(obj);
    // TODO: success
} else {
    // TODO: fail
}