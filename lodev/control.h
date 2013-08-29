#ifndef INIT_CTLDATA_GUARD
#define INIT_CTLDATA_GUARD
typedef struct{
		unsigned char player1;
		unsigned char player2;
} ControlData;
#endif

#ifndef CONTROL_GETCONTROL_GUARD
#define CONTROL_GETCONTROL_GUARD
void getControl(ControlData *tdata);
#endif

#ifndef CONTROL_CONTROL_GUARD
#define CONTROL_CONTROL_GUARD
void *control(void *args);
#endif
