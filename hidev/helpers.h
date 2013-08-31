#ifndef HELPERS_FREEZEFRAME_GUARD
#define HELPERS_FREEZEFRAME_GUARD
void freezeFrame(unsigned int frames);
#endif

#ifndef HELPERS_PRINTSCORE_GUARD
#define HELPERS_PRINTSCORE_GUARD
void printScore(int score, char *position);
#endif

#ifndef HELPERS_MAKEARRAY_GUARD
#define HELPERS_MAKEARRAY_GUARD
bool** makeArray(float HEIGHT, float WIDTH);
#endif

#ifndef HELPERS_FILLARRAY_GUARD
#define HELPERS_FILLARRAY_GUARD
void fill2DArray(bool** ledArray, float HEIGHT, float WIDTH, bool lightsOn);
#endif

#ifndef HELPERS_FREEARRAY_GUARD
#define HELPERS_FREEARRAY_GUARD
free2DArray(bool** ledArray);
#endif

#ifndef HELPERS_DRAWRECTANGLE_GUARD
#define HELPERS_DRAWRECTANGLE_GUARD
void drawRectangle(bool** ledArray, bool lightsOn, int topY, int leftX, int HEIGHT, int WIDTH);
#endif

#ifndef HELPERS_GETJOYSTICK_GUARD
#define HELPERS_GETJOYSTICK_GUARD
int getJoystick(unsigned char player);
#endif

#ifndef HELPERS_GETLEFTBUTTON_GUARD
#define HELPERS_GETLEFTBUTTON_GUARD
bool getLeftButton(unsigned char player);
#endif

#ifndef HELPERS_GETRIGHTBUTTON_GUARD
#define HELPERS_GETRIGHTBUTTON_GUARD
bool getRightButton(unsigned char player);
#endif

#ifndef HELPERS_PRINTTEST_GUARD
#define HELPERS_PRINTTEST_GUARD
void printTest(bool** ledArray);
#endif

#ifndef HELPERS_FRAMETEST_GUARD
#define HELPERS_FRAMETEST_GUARD
void frameTest(bool** ledArray, float TOP_MARGIN, float LEFT_MARGIN, float BOT_END, float RIGHT_END);
#endif
