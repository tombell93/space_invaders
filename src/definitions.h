/* Private defines -----------------------------------------------------------*/

// The following should be the minimal CircleOS version needed by your application
#define NEEDEDVERSION "V 4.0"
#define MAN_LEVEL 5
#define INCREMENT 1
#define MAN_WIDTH 9
#define MAN_HEIGHT 5
#define START_POS (SCREEN_WIDTH-MAN_WIDTH)/2
#define MIN_TILT_RIGHT 50
#define MIN_TILT_LEFT -50
#define COLUMN_SIZE 16
#define ROW_SIZE 12
#define SCORE_POS_X 0
#define SCORE_POS_Y 118
#define DISTANCE_BETWEEN_BARRIERS (SCREEN_WIDTH-(BARRIER_WIDTH + 2*(BARRIER_START_X)))/3
#define BARRIER_START_X 10
#define BARRIER_START_Y 23
#define BARRIER_WIDTH 12
#define BARRIER_HEIGHT 8
#define ALIEN_START_X 5
#define ALIEN_START_Y 70
#define ALIEN_WIDTH 8
#define ALIEN_HEIGHT 8
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 3
#define ALIEN_BULLET_WIDTH 2
#define ALIEN_BULLET_HEIGHT 3
#define GAME_PLAY 1
#define GAME_WON 3
#define GAME_OVER 2
#define SPLASH_DELAY 3000000
#define PHYS_SCREEN_HEIGHT 162

/* Private functions ---------------------------------------------------------*/
static enum MENU_code MsgVersion(void);

/* Public variables ----------------------------------------------------------*/

/* Public Alien Variables -----------------------------------------------------------*/
int alien_col = 0;
int alien_row = 0;
int alien_x = 0;
int alien_y = 0;

/* Public Man Variables -----------------------------------------------------------*/
int y = MAN_LEVEL;
int x = START_POS;

/* Public Barrier Variables -----------------------------------------------------------*/
const char Application_Name[8+1] = {"SpaceInv"};      // Max 8 characters
s16 Initial_MEMS_X;

struct alien {
    bool alive;
    short int alien_pos_x;
    short int alien_pos_y;
};

struct bullet{
    short int bullet_pos_x;
    short int bullet_pos_y;
};

struct alien alien_array[4][6] = {{{1,1,1},{1,2,1},{1,3,1},{1,4,1},{1,5,1},{1,6,1}},
                                  {{1,1,2},{1,2,2},{1,3,2},{1,4,2},{1,5,2},{1,6,2}},
                                  {{1,1,3},{1,2,3},{1,3,3},{1,4,3},{1,5,3},{1,6,3}},
                                  {{1,1,4},{1,2,4},{1,3,4},{1,4,4},{1,5,4},{1,6,4}}};


short int tot_x = 0;
short int tot_y = 0;
short int man_speed;
short int alien_translation_x = 0;
short int alien_translation_y = 0;
short int barrier_col = 0;
short int tot_barrier_x = 0;
short int tot_barrier_y = 0;
short int bullet_translation = 0;
short int bullet_init_x = 0;
short int bullet_init_y = 0;      
short int bullet_active = 0;
short int centre_coord_bullet_x;
short int centre_coord_bullet_y;
short int centre_coord_alien_x;
short int centre_coord_alien_y;
short int alien_fire_point_x = 50;
short int alien_fire_point_y = 0;
short int alien_bullet_translation = 0;
short int alien_bullet_active = 0;
short int alien_fire_col = 0;
short int alien_fire_row = 0;
short int direction = 1;

short int enable_fire = 0;
short int alive_count = 0;
short int end = 0;
short int state;
short int alien_fire_point_x;
short int alien_fire_point_y;
short int lives = 3;
int score = 0;
short int game_duration = 1;
char score_str[3];
char score_str2[4];
int dead_right_count = 0;
int dead_left_count = 0;
int dead_row_count = 0;
int result_right = 0;
int result_left = 0;