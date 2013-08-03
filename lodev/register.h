#ifndef REGISTER_COLCLK_GUARD
#define REGISTER_COLCLK_GUARD
void colclk(void);
#endif

#ifndef REGISTER_ROWCLK_GUARD
#define REGISTER_ROWCLK_GUARD
void rowclk(void);
#endif

#ifndef REGISTER_LATCH_GUARD
#define REGISTER_LATCH_GUARD
void latch(void);
#endif

#ifndef REGISTER_ROW_IN_HIGH_GUARD
#define REGISTER_ROW_IN_HIGH_GUARD
void rowInHigh(void);
#endif

#ifndef REGISTER_ROW_IN_LOW_GUARD
#define REGISTER_ROW_IN_LOW_GUARD
void rowInLow(void);
#endif

#ifndef REGISTER_COL_IN_HIGH_GUARD
#define REGISTER_COL_IN_HIGH_GUARD
void colInHigh(void);
#endif

#ifndef REGISTER_COL_IN_LOW_GUARD
#define REGISTER_COL_IN_LOW_GUARD
void colInLow(void);
#endif

#ifndef REGISTER_INITIALIZE_PINS_GUARD
#define REGISTER_INITIALIZE_PINS_GUARD
void initializePins(void);
#endif

