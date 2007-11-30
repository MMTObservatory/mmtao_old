/* Power.h
 */
#define POWER_LOOPER_WAIT	1000000

#define POWER_MAXLINE	16

#define POWER_NCHAN	8

// Prototypes
int Power_On ( Socket_Info *, int, int );
int Power_Off ( Socket_Info *, int, int );
int Power_Info ( Socket_Info *, char *, int );
int Power_PrintDevice ( Socket_Info *, Device_Data * );
int Power_Status ( Socket_Info *, Device_Data *, int );

void * Power_Looper ( void * );
int Power_Process ( Device_Info *, char * );

/* THE END */
