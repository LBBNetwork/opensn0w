/*
 * More Functions
 */

#ifndef _util_h_
#define _util_h_

bool file_exists(const char *fileName);
char *mode_to_string(int mode);
size_t writeData(void *ptr, size_t size, size_t mem, FILE * stream);

#endif
