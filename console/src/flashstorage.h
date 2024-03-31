void flash_init() {
}
inline int IJB_file() {
  return 0; // return _g.lastfile;
}
int IJB_save(int n, uint8* list, int size) {
  return 1;
}
int IJB_load(int n, uint8* list, int sizelimit, int init) { // ret:size if:-1 err
  return -1;
}
