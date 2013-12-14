#ifndef SCREEN_SCREEN_DATA_GUARD
#define SCREEN_SCREEN_DATA_GUARD
#include <stdbool.h>
/*
  Screen wrapper for arguments passed to asynchronous thread functions.
*/
typedef struct {
  bool **screen;
} ScreenData;
#endif

/*
  Screen size spec
*/
#define DISPLAY_WIDTH  38 
#define DISPLAY_LENGTH  76
#define UNIT_WIDTH 8
#define UNIT_LENGTH 76
