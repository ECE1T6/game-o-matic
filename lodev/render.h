#ifndef RENDER_FLUSH_GUARD
#define RENDER_FLUSH_GUARD
void flush(void);
#endif

#ifndef RENDER_RESET_ROWS_GUARD
#define RENDER_RESET_ROWS_GUARD
void resetRows(void);
#endif

#ifndef RENDER_REFRESH_GUARD
#define RENDER_REFRESH_GUARD
void refresh(bool **screen);
#endif

#ifndef RENDER_RENDER_GUARD
#define RENDER_RENDER_GUARD
void *render(void *args);
#endif

