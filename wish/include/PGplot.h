/****************************
  PGplot Information
****************************/

#define SUB_APPS 12
#define NUM_PIXELS 72
#define NUM_SLOPES 12
#define NUM_ACTUATORS 336

/*
  Structure to hold plot information
*/
typedef struct _PGplot_Spots_Data {
  int values;
  int pixels_per_sub;
  float used_subapps;
  float image_scale;
  float image_transform[6];
  float pupil_scale;
  float pupil_transform[6];
  float min;
  float min_lower;
  float min_upper;
  float max;
  float max_lower;
  float max_upper;
  float min_pupil;
  float min_pupil_lower;
  float min_pupil_upper;
  float max_pupil;
  float max_pupil_lower;
  float max_pupil_upper;
  float average_fraction;
  float *current;
  float *average;
  float *pupil;
  int *slope_grid;
  float *x_slopes_factor;
  float *x_slopes;
  float *y_slopes_factor;
  float *y_slopes;
  float tip;
  float tilt;
  float *focus_sin;
  float *focus_cos;
  float *x_focus;
  float *y_focus;
  float focus;
} PGplot_Spots_Data;

typedef struct _PGplot_Slopes_Data {
  int values;
  float used_subapps;
  float scale;
  float transform[6];
  float min;
  float min_lower;
  float min_upper;
  float max;
  float max_lower;
  float max_upper;
  float average_fraction;
  float *current_x;
  float *current_y;
  float *average_x;
  float *average_y;
  float tip;
  float tilt;
} PGplot_Slopes_Data;

typedef struct _PGplot_DM_Info {
  int n_actuators;
  int n_rings;
  int *n_act_ring;
  float *angle_ring;
  float *angle_act0;
  float curv_radius;
  float in_radius;
  float out_radius;
  float *current;
  float *average;
} PGplot_DM_Info;

typedef struct _PGplot_DM_Data {
  PGplot_DM_Info *dm;
  float min;
  float min_lower;
  float min_upper;
  float max;
  float max_lower;
  float max_upper;
  float average_fraction;
  float *current;
  float *average;
} PGplot_DM_Data;

typedef struct _PGplot_Info {
  char name[STRING_LENGTH];
  char path[STRING_LENGTH];
  int id;
  PGplot_Spots_Data *spots;
  PGplot_Slopes_Data *slopes;
  PGplot_DM_Data *rtr;
  PGplot_DM_Data *cmd;
  PGplot_DM_Data *cur;
  PGplot_DM_Data *pos;
} PGplot_Info;

/* Prototypes */
int PGplot_ColorTable( PGplot_Info *Info, int color, int debug);
int PGplot_Open( PGplot_Info *Info, char *path, int debug);
int PGplot_SetupInfo( PGplot_Info *Info, char *name, int debug);
int PGplot_TestDraw( PGplot_Info *Info, double x_min, double x_max, int debug);
int PGplot_SlopesCalculate( PGplot_Info *Slopes_Info, short *Slopes_Vector, int debug);
int PGplot_SlopesSetup( PGplot_Info *Slopes_Info, int debug);
int PGplot_SlopesUpdate( PGplot_Info *Slopes_Info, int Slopes_Type, int Slopes_Arrow, int debug);
int PGplot_SpotsCalculate( PGplot_Info *Spots_Info, short *Spots_Array, int debug);
int PGplot_SpotsPattern( PGplot_Info *Info, short **WFSC_Array, int debug);
int PGplot_SpotsSetup( PGplot_Info *Info, int debug);
int PGplot_SpotsUpdate( PGplot_Info *Spots_Info, int Spots_Type, int Spots_Slopes, int Spots_Arrow, int debug);
