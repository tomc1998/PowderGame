#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int load_from_path(const char* path, char** data, long* data_len) {
  FILE* f = NULL;

  /* Open file */
  f = fopen(path, "r");
  if (!f) {
    return 2;
  }

  /* Get file size */
  if (fseek(f, 0, SEEK_END)) {
    return 2;
  }
  *data_len = ftell(f)-1;
  if (*data_len == -1) {
    return 2;
  }
  if (fseek(f, 0, SEEK_SET)) {
    return 2;
  }

  /* Allocate memory */
  *data = (char*)malloc(*data_len);
  if (*data == 0) {
    return 4;
  }

  /* Load from file */
  if (!fread(*data, *data_len, 1, f)) {
    return 3;
  }

  /* Close file */
  fclose(f);

  return 0;
}
