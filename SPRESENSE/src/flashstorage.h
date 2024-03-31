#include <File.h>

extern SDClass theSD;

void flash_init() {
}
inline int IJB_file() {
	return _g.lastfile;
}
int IJB_save(int n, uint8* list, int size) { // success:0, err: 1
	if (n < 0 || n > 9) {
		return -1;
	}
	char fn[10];
	fn[0] = '0' + n;
	fn[1] = '.';
	fn[2] = 'b';
	fn[3] = 'i';
	fn[4] = 'n';
	fn[5] = '\0';
  File file = theSD.open(fn, FILE_WRITE);
	if (file == NULL) {
		return -1;
	}
	//size_t len = file.write(list, size);
	file.seek(0);
	size_t len = file.write(list, 1024);
	file.close();
	//if (len != size) {
	if (len != 1024) {
		return -1;
	}
	_g.lastfile = n;
	return 0;
}
int IJB_load(int n, uint8* list, int sizelimit, int init) { // ret:size if:-1 err
	if (n < 0 || n > 9) {
		return -1;
	}
	if (init) {
		_g.lastfile = n;
	}
	char fn[10];
	fn[0] = '0' + n;
	fn[1] = '.';
	fn[2] = 'b';
	fn[3] = 'i';
	fn[4] = 'n';
	fn[5] = '\0';
  File file = theSD.open(fn, FILE_READ);
	if (file == NULL) {
		return -1;
	}
	for (int i = 0; i < sizelimit; i++) {
		list[i] = 0;
	}
	size_t len = file.read(list, sizelimit);
	file.close();
	return sizelimit;
}
