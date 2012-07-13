/********************** (C) COPYRIGHT 2007-2010 RAISONANCE ********************
*
* File Name          :  Application.c
* Description        :  Circle_App CircleOS application template.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle_api.h"
#include "definitions.h"
#include <stdlib.h>

static const u16 splash[] = { 
   #include "si128128.png.h" 
};

static const u16 alien1[] = { 
   #include "alien1.png.h" 
};

static const u16 barrier[] = { 
   #include "barrier.png.h" 
};

static const u16 man[] = { 
   #include "man.png.h" 
};

static const u16 score_img[] = { 
   #include "score.png.h" 
};

static const u16 lives_img[] = { 
   #include "lives.png.h" 
};

static const u16 man_lives_img[] = { 
   #include "Man_lives.png.h" 
};

static const u16 gameOver[] = { 
   #include "gameOver.png.h" 
};


/*******************************************************************************
* Function Name  : Application_Ini
* Description    : Initialization function of Circle_App. This function will
*                  be called only once by CircleOS.
* Input          : None
* Return         : MENU_CONTINUE_COMMAND
*******************************************************************************/
enum MENU_code Application_Ini(void)
    {
    if(strcmp(UTIL_GetVersion(), NEEDEDVERSION) < 0)
        {
        return MsgVersion();
        }
    splash_scrn();
    LCD_FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB_BLACK); //clears screen
    LCD_SetOffset(OFFSET_OFF);
    reset_bullet();                                             //initialises bullet variables
    reset_alien_bullet();
    state = GAME_PLAY;
    LCD_DrawRect(x - 1, y - 1, MAN_WIDTH + 2, MAN_HEIGHT + 2, RGB_BLACK);
    LCD_FillRect(x, y, MAN_WIDTH, MAN_HEIGHT, RGB_GREEN);
    
    Initial_MEMS_X = ((MEMS_GetInfo())->OutX_F4)/4;
    int barrier_col;
    draw_barrier(barrier_col);
    
    
    MENU_SetAppliDivider(10);

    return MENU_CONTINUE_COMMAND;
    }

/*******************************************************************************
* Function Name  : Application_Handler
* Description    : Management of the Circle_App. This function will be called
*                  every millisecond by CircleOS while it returns MENU_CONTINUE.
* Input          : None
* Return         : MENU_CONTINUE
*******************************************************************************/
enum MENU_code Application_Handler(void)
{
    static int clk = 0;
    s16 Current_MEMS_X, delta;
    Current_MEMS_X = ((MEMS_GetInfo())->OutX_F4)/4;
    delta = (Current_MEMS_X - Initial_MEMS_X);
    if(state == GAME_PLAY){
        if((clk%(check_speed(Current_MEMS_X))) == 0){
            update_man(Current_MEMS_X);
        }
        
        if((clk % 10) == 0){
            alien_display();
        }
                
        if((clk % (alien_translate_speed())) == 0){
            alien_translate();
        }
        
        if(((BUTTON_GetState() == BUTTON_PUSHED) || (JOYSTICK_GetState() == JOYSTICK_UP) || (TOUCHSCR_IsPressed())) && (!bullet_active)){
            reset_bullet();
            bullet_active = 1;
            fire();
        }
        
        if(((clk%1) == 0) && (bullet_active)){
            update_bullet();
        }

        if((clk%100) == 0){
            check_all_aliens_killed();
        }

        if(((clk%5) == 0 )&& (bullet_active)){
            check_destroy();
            check_barrier_collision();
            check_alien_bullet_man_collision();
        }
        
        if(((clk%50) == 0) && (!alien_bullet_active)){
                alien_bullet_active = 1;
                random_alien_attack(clk);
        }
        
        if((clk%(alien_shoot_speed())) == 0 && (alien_bullet_active)){
            update_alien_bullet();
        }
    }
    
    if((state == GAME_OVER)){
        if((clk%2000) == 0){
            game_over();
        }else if((clk%2) == 0){
            check_quit();
            if(TOUCHSCR_IsPressed()){
                score = 0;
                game_duration = 1;
                alien_bullet_active = 0;
                lives = 3;
                state = GAME_PLAY;
                LCD_FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB_BLACK);
                reset_bullet();
                reset_alien_array();
                reset_alien_bullet();
                draw_barrier(barrier_col);
                alien_translation_x = 0;
                alien_translation_y = 0;
                dead_left_count = 0;
                dead_right_count = 0;
            }
        }
    }
    check_quit();
    clk++;
    return MENU_CONTINUE;
}

/*******************************************************************************
* Function Name  : splash_scrn
* Description    : Displays the splash screen until the for loop runs out. 
* Input          : None
* Return         : 0
*******************************************************************************/
int splash_scrn()
{
    DRAW_SetImage(splash, 0, 0, 128, 128);
    int clk_splash = 0;
    for(clk_splash = 0; clk_splash < SPLASH_DELAY; clk_splash++){}
    
    return 0;
}

/*******************************************************************************
* Function Name  : alien_shoot_speed
* Description    : Sets the speed of the alien bullets as an increasing function 
*                  of time.
* Input          : None
* Return         : (5 - (0.2)*game_duration))
*******************************************************************************/
int alien_shoot_speed()
{
    if(game_duration > 20){
        return 1;
    }
    return (5 - (((0.2)*game_duration)));
}

/*******************************************************************************
* Function Name  : alien_translate_speed
* Description    : Sets the value of the speed of the alien translation as an 
*                  increasing function of time,.
* Input          : None
* Return         : None
*******************************************************************************/
int alien_translate_speed()
{
    if(game_duration > 30){
        return 15;
    }
    return (30 - (((0.5)*game_duration)));
}

/*******************************************************************************
* Function Name  : update_score_lives
* Description    : Updates the score and lives by clearing it and redrawing the
*                  required ones. 
* Input          : None
* Return         : None
*******************************************************************************/
int update_score_lives()
{
    LCD_FillRect(0, SCREEN_HEIGHT - MAN_HEIGHT - 6, SCREEN_WIDTH, MAN_HEIGHT + 6, RGB_BLACK);
    DRAW_SetImage(score_img, 1, 118, 29, 9);
    UTIL_int2str(score_str, score, 3, 1);
    DRAW_DisplayStringWithMode(31, SCREEN_HEIGHT - 13, score_str, 4, 1, 1);
    DRAW_SetImage(lives_img, 68, 118, 30, 9);
    if(lives >= 1){
        DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 1, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
        if(lives >= 2){
            DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 9, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
            if(lives >= 3){
            DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 17, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
            }
        }
    }else{
        game_over();
    }
}

/*******************************************************************************
* Function Name  : show_score
* Description    : Updates the score by re-printing the value of score. 
* Input          : None
* Return         : None
*******************************************************************************/
int show_score()
{
    DRAW_SetImage(score_img, 1, 118, 29, 9);
    UTIL_int2str(score_str, score, 3, 1);
    DRAW_DisplayStringWithMode(31, SCREEN_HEIGHT - 13, score_str, 4, 1, 1);
}

/*******************************************************************************
* Function Name  : show_lives
* Description    : Updates the lives by redrawing the required ones.
* Input          : None
* Return         : None
*******************************************************************************/
int show_lives()
{
    
    DRAW_SetImage(lives_img, 68, 118, 30, 9);
    if(lives >= 1){
        DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 1, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
        if(lives >= 2){
            DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 9, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
            if(lives >= 3){
            DRAW_SetImage(man_lives_img, SCREEN_WIDTH - MAN_WIDTH - 17, SCREEN_HEIGHT - MAN_HEIGHT - 4, MAN_WIDTH, MAN_HEIGHT);
            }
        }
    }else{
        game_over();
    }
}

/*******************************************************************************
* Function Name  : check_quit
* Description    : Checks if the joystick is pushed down. If so, quits the game.
* Input          : None
* Return         : None
*******************************************************************************/
int check_quit()
{
    if(JOYSTICK_GetState() == JOYSTICK_DOWN) {
        JOYSTICK_WaitForRelease();
        return MENU_Quit();
        }
}

/*******************************************************************************
* Function Name  : reset_alien_bullet
* Description    : Resets the alien bullet by clearing its location and reseting 
*                  the variables associated with it, thus preparing it for reuse.
* Input          : None
* Return         : None
*******************************************************************************/
int reset_alien_bullet()
{
    LCD_FillRect(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation + 1, ALIEN_BULLET_WIDTH, ALIEN_BULLET_HEIGHT + 1, RGB_BLACK);
    alien_bullet_active = 0;
    alien_fire_point_x = 0;
    alien_fire_point_y = 0;  
    alien_bullet_translation = 0;
}

/*******************************************************************************
* Function Name  : update_alien_bullet
* Description    : updates the alien's bullet by checking its position for 
*                  destruction and redrawing it.
* Input          : None
* Return         : None
*******************************************************************************/
int update_alien_bullet()
{
    if(alien_bullet_translation > (alien_fire_point_y)){
        reset_alien_bullet();
    }else{
        alien_fire_draw();
        check_alien_bullet_barrier_collision();
        check_alien_bullet_man_collision();
        
    }
}

/*******************************************************************************
* Function Name  : alien_fire_draw
* Description    : Translates the alien bullet, by clearing and redrawing it. 
* Input          : None
* Return         : None
*******************************************************************************/
int random_alien_attack(int clk)
{
    alien_fire_row = (rand()%5);
    alien_fire_col = (rand()%7);
    if(alien_bullet_active){
        if(alien_array[alien_fire_row][alien_fire_col].alive)
        {
            alien_fire_point_x = ALIEN_START_X + (((alien_array[alien_fire_row][alien_fire_col].alien_pos_x)-1)*COLUMN_SIZE) + alien_translation_x + (ALIEN_WIDTH/2) - (ALIEN_BULLET_WIDTH/2);
            alien_fire_point_y = ALIEN_START_Y + (((alien_array[alien_fire_row][alien_fire_col].alien_pos_y)-1)*ROW_SIZE) + alien_translation_y - ALIEN_BULLET_HEIGHT;
            alien_fire_draw();
        }else{
            random_alien_attack(clk);
        }
    }
}

/*******************************************************************************
* Function Name  : alien_fire_draw
* Description    : Translates the alien bullet, by clearing and redrawing it. 
* Input          : None
* Return         : None
*******************************************************************************/
int alien_fire_draw()
{
    LCD_FillRect(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation + 1, ALIEN_BULLET_WIDTH, ALIEN_BULLET_HEIGHT + 1, RGB_BLACK);
    LCD_FillRect(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation, ALIEN_BULLET_WIDTH, ALIEN_BULLET_HEIGHT, RGB_BLUE);
    alien_bullet_translation++;
}

/*******************************************************************************
* Function Name  : check_speed
* Description    : Checks the amount of tilt of the MEMS sensor and returns a 
*                  value between 2 and 7, representing the speed of the man.
* Input          : int Current_MEMS_X
* Return         : An int between 2 and 7.
*******************************************************************************/
int check_speed(int Current_MEMS_X)
{
    if (lim_check_right(Current_MEMS_X)){
        return (7 - (Current_MEMS_X/100));
    }
    if (lim_check_left(Current_MEMS_X)){
        return (7 - (-(Current_MEMS_X/100)));
    }else{
        return 2;
    }
}

/*******************************************************************************
* Function Name  : check_all_aliens_killed
* Description    : Checks whether all of the aliens have been killed. If so, 
*                  resets the array and translation variables, and adds 10 to 
*                  the score.
* Input          : None
* Return         : None
*******************************************************************************/
int check_all_aliens_killed()
{
    for(alien_y = 0; alien_y < 4; alien_y++){
        for(alien_x = 0; alien_x < 6; alien_x++){
            if (!alien_array[alien_y][alien_x].alive){
                alive_count++;
            }
        }
    }

    if ((alive_count >= 24)){
        reset_alien_array();
        alien_translation_x = 0;
        alien_translation_y = 0;
        direction = 1;
        score += 10;
        update_score_lives();
    }
    alive_count = 0;
}

/*******************************************************************************
* Function Name  : reset_alien_array
* Description    : Sets the alive status of each alien to be 1 again (alive);
* Input          : None
* Return         : None
*******************************************************************************/
int reset_alien_array()
{
    for(alien_y = 0; alien_y < 4; alien_y++){
        for(alien_x = 0; alien_x < 6; alien_x++){
            alien_array[alien_y][alien_x].alive = 1;
        }
    }
}

/*******************************************************************************
* Function Name  : game_over
* Description    : Prints the gameOver background image and prints final score. 
* Input          : None
* Return         : 0
*******************************************************************************/
int game_over()
{
    LCD_FillRect(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT, RGB_BLACK);
    DRAW_SetImage(gameOver, 0, 0, 128, 128);
    UTIL_int2str(score_str2, score, 4, 0);
    DRAW_DisplayStringWithMode(60, 43, score_str, 4, 1, 0);
    state = GAME_OVER;
    
    return 0;
}

/*******************************************************************************
* Function Name  : check_dead_columns_left
* Description    : Checks whether the entirety of a column of aliens is killed. 
*                  If so, and if it is at the edge of the array of aliens, it
*                  returns a value to alien_translate() with a size 
*                  correcsponding to the number of pixels required to be 
*                  subtracted from the y limit. 
* Input          : None
* Return         : ((dead_left_count/4)*COLUMN_SIZE)
*******************************************************************************/
int check_dead_columns_left()
{
    dead_left_count = 0;
    for(alien_x = 0; alien_x < 6; alien_x++){
        for(alien_y = 0; alien_y < 4; alien_y++){
            if(!alien_array[alien_y][alien_x].alive){
                dead_left_count++;
            }else{
                return ((dead_left_count/4)*COLUMN_SIZE);
            }
        }
    }
    
}

/*******************************************************************************
* Function Name  : check_dead_columns_right
* Description    : Checks whether the entirety of a column of aliens is killed. 
*                  If so, and if it is at the edge of the array of aliens, it
*                  returns a value to alien_translate() with a size 
*                  correcsponding to the number of pixels required to be added 
*                  to the x limit. 
* Input          : None
* Return         : ((dead_right_count/4)*COLUMN_SIZE)
*******************************************************************************/
int check_dead_columns_right()
{
    dead_right_count = 0;
    for(alien_x = 5; alien_x >= 0; alien_x--){
        for(alien_y = 0; alien_y < 4; alien_y++){
            if(!alien_array[alien_y][alien_x].alive){
                dead_right_count++;
            }else{
                return ((dead_right_count/4)*COLUMN_SIZE);
            }
        }
    }
}

int check_rows()
{
    dead_row_count = 0;
    for(alien_y = 0; alien_y <= 4; alien_y++){
        for(alien_x = 0; alien_x < 5; alien_x++){
            if(!alien_array[alien_y][alien_x].alive){
                dead_row_count++;
            }else{
                return ((dead_row_count/6)*ROW_SIZE);
            }
        }
    }
}

/*******************************************************************************
* Function Name  : alien_translate
* Description    : Translates the entire alien array by incrementing 
*                  alien_translation. It also checks if the edge of the active 
*                  alien array is within the boudaries of the screen, by using 
*                  the check_dead_columns_right() and check_dead_columns_left()
*                  funcitions. 'direction', specifies the current direction of the 
*                  alien array. When the x limit is reached, the direciton changes.
*                  When the y limit is reached, it is reset; you loose a life; 
*                  and the conditions of each alien remains the same, until all 
*                  are killed.
* Input          : None
* Return         : None
*******************************************************************************/
int alien_translate()
{
    if(direction){
        alien_translation_x++;
        if(alien_translation_x > 30 + (check_dead_columns_right())){
            LCD_FillRect(0, ALIEN_START_Y - 22, SCREEN_WIDTH, 68, RGB_BLACK);
            alien_translation_y -= 5;
            direction = 0;
            game_duration += 1;
            if(alien_translation_y < - 31 - (check_rows())){
                LCD_FillRect(0, ALIEN_START_Y - 32, SCREEN_WIDTH, 78, RGB_BLACK);
                reposition_aliens();
                lives -= 1;
                update_score_lives();
                alien_display();
            }
        }   
    }else{
        alien_translation_x--;
        if(alien_translation_x < 1 - (check_dead_columns_left())){
            LCD_FillRect(0, ALIEN_START_Y - 32, SCREEN_WIDTH, 78, RGB_BLACK);
            alien_translation_y -= 5;
            direction = 1;
            game_duration += 1;
            if(alien_translation_y < - 31 - (check_rows())){
                LCD_FillRect(0, ALIEN_START_Y - 22, SCREEN_WIDTH, 68, RGB_BLACK);
                reposition_aliens();
                lives -= 1;
                update_score_lives();
                alien_display();
            }
        } 
    }
    alien_display();
}

/*******************************************************************************
* Function Name  : reposition_aliens
* Description    : Sets the position of the aliens to their original positions.  
* Input          : None
* Return         : None
*******************************************************************************/
int reposition_aliens()
{
    alien_translation_x = 0;
    alien_translation_y = 0;
    dead_right_count = 0;
    dead_left_count = 0;
    direction = 1;
}

/*******************************************************************************
* Function Name  : check_alien_bullet_man_collision
* Description    : Checks to see if the alien bullet collides with the man. If 
*                  so, it resets the alien bullet and decrements the mans lives. 
* Input          : None
* Return         : None
*******************************************************************************/
int check_alien_bullet_man_collision()
{
    if(((LCD_GetPixel(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation)) == 0xA3F8) || ((LCD_GetPixel(alien_fire_point_x + (ALIEN_BULLET_WIDTH/2), alien_fire_point_y - alien_bullet_translation)) == 0xA3F8)){
        lives -= 1;
        update_score_lives();
        reset_alien_bullet();
    }
}

/*******************************************************************************
* Function Name  : check_barrier_collision
* Description    : Checks to see if the bullet of the man collides with the 
*                  barrier. If so, causes damage by drawing a black box at the 
*                  point of collision. 
* Input          : None
* Return         : None
*******************************************************************************/
int check_barrier_collision()
{
    if(((LCD_GetPixel(bullet_init_x + (BULLET_WIDTH/2), bullet_init_y + BULLET_HEIGHT + bullet_translation + 1)) == 0xC567) || ((LCD_GetPixel(bullet_init_x, bullet_init_y + BULLET_HEIGHT + bullet_translation + 1)) == 0xC567)){
        LCD_FillRect((bullet_init_x), (bullet_init_y + BULLET_HEIGHT + bullet_translation + 1), BULLET_WIDTH, 1, RGB_BLACK);
        reset_bullet();
    }
}

/*******************************************************************************
* Function Name  : check_alien_bullet_barrier_collision
* Description    : Checks whether the alien bullet collides with the barriers. 
*                  If so, causes damage to the barrier by drawing a black box at 
*                  the point of impact.
* Input          : None
* Return         : None
*******************************************************************************/
int check_alien_bullet_barrier_collision()
{
    if(((LCD_GetPixel(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation)) == 0xC567) || ((LCD_GetPixel(alien_fire_point_x + (ALIEN_BULLET_WIDTH/2), alien_fire_point_y - alien_bullet_translation)) == 0xC567)){
        LCD_FillRect(alien_fire_point_x, alien_fire_point_y - alien_bullet_translation, BULLET_WIDTH, 1, RGB_BLACK);
        reset_alien_bullet();
    }
}

/*******************************************************************************
* Function Name  : check_destroy
* Description    : Uses the compare_distance() and alien_die() functions to 
*                  check if any aliens are destroyed. 
* Input          : None
* Return         : None
*******************************************************************************/
int check_destroy()
{   
    for(alien_y = 0; alien_y < 4; alien_y++){
            for(alien_x = 0; alien_x < 6; alien_x++){
                if (alien_array[alien_y][alien_x].alive){
                    if(compare_distance(alien_array[alien_y][alien_x].alien_pos_x, alien_array[alien_y][alien_x].alien_pos_y)){
                        alien_array[alien_y][alien_x].alive = 0;
                        alien_die(alien_array[alien_y][alien_x].alien_pos_x, alien_array[alien_y][alien_x].alien_pos_y);
                        score += 1;
                        update_score_lives();
                        reset_bullet();
                    }
                }
                if(!bullet_active){
                    break;
                }
            }
            if(!bullet_active){
                    break;
                }
        }
}

/*******************************************************************************
* Function Name  : reset_bullet
* Description    : Resets the state of the bullet to inactive and prepares it 
*                  for being used again.
* Input          : None
* Return         : None
*******************************************************************************/
int reset_bullet()
{
    LCD_FillRect(bullet_init_x, bullet_init_y + bullet_translation - 1, BULLET_WIDTH + 2, BULLET_HEIGHT + 2, RGB_BLACK);
    bullet_active = 0;
    bullet_init_x = 0;
    bullet_init_y = MAN_LEVEL + MAN_HEIGHT;  
    bullet_translation = 0;
}

/*******************************************************************************
* Function Name  : compare_distance
* Description    : Compares the position of the active bullet with all of the 
*                  active aliens, to check if they are hit.
* Input          : int alien_col, int alien_row
* Return         : 1, 0 (alien hit, alien missed)
*******************************************************************************/
int compare_distance(int alien_col, int alien_row)
{
    tot_x = ALIEN_START_X + ((alien_col - 1)*COLUMN_SIZE) + alien_translation_x;
    tot_y = ALIEN_START_Y + ((alien_row - 1)*ROW_SIZE) + alien_translation_y;
    centre_coord_bullet_x = bullet_init_x + (BULLET_WIDTH/2);
    centre_coord_bullet_y = bullet_init_y + (BULLET_HEIGHT/2) + bullet_translation;
    centre_coord_alien_x = tot_x + (ALIEN_WIDTH/2);
    centre_coord_alien_y = tot_y + (ALIEN_HEIGHT/2);
    
    if((centre_coord_alien_x - centre_coord_bullet_x) < ((ALIEN_WIDTH/2) + (BULLET_WIDTH/2)) && 
        ((centre_coord_alien_x - centre_coord_bullet_x)*-1) < ((ALIEN_WIDTH/2) + (BULLET_WIDTH/2))){
        if((centre_coord_alien_y - centre_coord_bullet_y) < ((ALIEN_HEIGHT/2) + (BULLET_HEIGHT/2)) && 
        ((centre_coord_alien_y - centre_coord_bullet_y)*-1) < ((ALIEN_HEIGHT/2) + (BULLET_HEIGHT/2))){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    } 
}

/*******************************************************************************
* Function Name  : alien_die
* Description    : Draws a black box over an alien location when it is hit.
* Input          : int alien_col, int alien_row
* Return         : None
*******************************************************************************/
int alien_die(int alien_col, int alien_row)
{
    tot_x = ALIEN_START_X + ((alien_col - 1)*COLUMN_SIZE) + alien_translation_x;
    tot_y = ALIEN_START_Y + ((alien_row - 1)*ROW_SIZE) + alien_translation_y;
    
    LCD_FillRect(tot_x-1, tot_y, ALIEN_WIDTH + 2, ALIEN_HEIGHT, RGB_BLACK);
}

/*******************************************************************************
* Function Name  : fire
* Description    : Called once when the user fires a bullet. Initialises bullet 
*                  location.
* Input          : None
* Return         : None
*******************************************************************************/
int fire()
{
    bullet_init_x = x + (MAN_WIDTH/2) - (BULLET_WIDTH/2);
    bullet_init_y = MAN_LEVEL + MAN_HEIGHT;
    LCD_FillRect(bullet_init_x, bullet_init_y + bullet_translation, BULLET_WIDTH, BULLET_HEIGHT, RGB_RED);
}

/*******************************************************************************
* Function Name  : update_bullet
* Description    : Translates the bullet of the man, by incrementing  
*                  bullet_translation and clearing/redrawing the bullet with the 
*                  new coordinates
* Input          : None
* Return         : None
*******************************************************************************/
int update_bullet()
{
    
    if(bullet_translation > (SCREEN_HEIGHT - bullet_init_y)){
        reset_bullet();
    }else{
        bullet_translation++;
        LCD_FillRect(bullet_init_x, bullet_init_y + bullet_translation - 2, BULLET_WIDTH, BULLET_HEIGHT + 2, RGB_BLACK);
        LCD_FillRect(bullet_init_x, bullet_init_y + bullet_translation, BULLET_WIDTH, BULLET_HEIGHT, RGB_RED);
    }
}

/*******************************************************************************
* Function Name  : alien_display
* Description    : Check's whether each alien is alive and if so, calls the 
                   draw_alien() function to draw it in it's correct location. 
* Input          : None
* Return         : 0
*******************************************************************************/
int alien_display()
{
    for(alien_y = 0; alien_y < 4; alien_y++){
        for(alien_x = 0; alien_x < 6; alien_x++){
            if (alien_array[alien_y][alien_x].alive){
                draw_alien(alien_array[alien_y][alien_x].alien_pos_x, alien_array[alien_y][alien_x].alien_pos_y);
            }
        }
    }
    return 0;
}

/*******************************************************************************
* Function Name  : draw_enemy
* Description    : Draws a single enemy at a specified location
* Input          : enemy_col, enemy_row, translation_x and translation_y
* Return         : 0
*******************************************************************************/
int draw_alien(int alien_col, int alien_row) //lower left at position a
{
    tot_x = ALIEN_START_X + ((alien_col - 1)*COLUMN_SIZE) + alien_translation_x;
    tot_y = ALIEN_START_Y + ((alien_row - 1)*ROW_SIZE) + alien_translation_y;
    
    LCD_FillRect(tot_x-1, tot_y, ALIEN_WIDTH + 2, ALIEN_HEIGHT, RGB_BLACK);
    DRAW_SetImage(alien1, tot_x, tot_y, 8, 8);
    return 0;
}
/*******************************************************************************
* Function Name  : draw_barrier
* Description    : Draws the barriers. Only executed one in application_ini()
* Input          : barrier_col (column of barriers (incremented in 
*                  application_ini()))
* Return         : 0
*******************************************************************************/
int draw_barrier(int barrier_col){
    
    for(barrier_col = 0; barrier_col < 4; barrier_col++){
        tot_barrier_x = BARRIER_START_X + (barrier_col*DISTANCE_BETWEEN_BARRIERS);
        tot_barrier_y = BARRIER_START_Y;
        DRAW_SetImage(barrier, tot_barrier_x, tot_barrier_y, BARRIER_WIDTH, BARRIER_HEIGHT);
    }
    
    
}

/*******************************************************************************
* Function Name  : update_man
* Description    : Checks if the conditions for movement are ok; if so, moves
*                  man in correct direction; else, does nothing. 
* Input          : Current_MEMS_X
* Return         : 0
*******************************************************************************/
int update_man(int Current_MEMS_X){
                   
    if (lim_check_right(Current_MEMS_X)){
        man_move_right();
    }
    if (lim_check_left(Current_MEMS_X)){
        man_move_left();
    }else{
        draw_man();
    }
}


/*******************************************************************************
* Function Name  : man_move_right
* Description    : Re-draws the man one pixel to the right of current position
* Input          : x, y (x and y positions of man)
* Return         : 0
*******************************************************************************/
int man_move_right()
{
    x = x + 1;
    draw_man();
    return 0;
}

/*******************************************************************************
* Function Name  : man_move_left
* Description    : Re-draws the man one pixel to the left of current position
* Input          : x
* Return         : 0
*******************************************************************************/
int man_move_left()
{
    x = x - 1;
    draw_man();
    return 0;
}

/*******************************************************************************
* Function Name  : draw_man
* Description    : Draws the moving man with updated position.  
* Input          : x, y (x and y positions of man)
* Return         : 0
*******************************************************************************/
int draw_man()
{
    LCD_DrawRect(x - 1, y - 1, MAN_WIDTH + 2, MAN_HEIGHT + 2, RGB_BLACK);
    DRAW_SetImage(man, x, y, MAN_WIDTH, MAN_HEIGHT);
    return 0;
}

/*******************************************************************************
* Function Name  : lim_check_right
* Description    : Checks if MEMS data is within range. Also checks boundaries. 
* Input          : Current_MEMS_X (int)
* Return         : 1 if MEMS > MIN_TILT_RIGHT, else 0.
*******************************************************************************/
int lim_check_right(int Current_MEMS_X){
    if ((x < (SCREEN_WIDTH - (5 + MAN_WIDTH))) && (Current_MEMS_X > 0 && Current_MEMS_X > MIN_TILT_RIGHT) ){
        return 1;
    }else{
        return 0;
    }
}

/*******************************************************************************
* Function Name  : lim_check_left
* Description    : Checks if MEMS data is within range. Also checks boundaries. 
* Input          : Current_MEMS_X (int)
* Return         : 1 if MEMS > MIN_TILT_LEFT, else 0.
*******************************************************************************/
int lim_check_left(int Current_MEMS_X){
    if ((x > 5) && (Current_MEMS_X < 0 && Current_MEMS_X < MIN_TILT_LEFT) ){
        return 1;
    }else{
        return 0;
    }
}

/*******************************************************************************
* Function Name  : MsgVersion
* Description    : Displays the current CircleOS version and the version needed
*                  exit to main menu after 4 seconds
* Input          : None
* Return         : MENU_REFRESH
*******************************************************************************/
static enum MENU_code MsgVersion(void)
    {
    u8 hh, mm, ss, ss2;

    DRAW_DisplayString( 5,  60, "CircleOS", 17 );
    DRAW_DisplayString( 80, 60, UTIL_GetVersion(), 6 );
    DRAW_DisplayString( 5,  34, NEEDEDVERSION, 6 );
    DRAW_DisplayString( 50, 34, " required", 12 );

    RTC_GetTime( &hh, &mm, &ss );
    ss = ss + 4;
    ss = ss % 60;

    do
        {
        RTC_GetTime( &hh, &mm, &ss2 );
        }
    while ( ss2 != ss );           // do while < 4 seconds

    DRAW_Clear();
    return MENU_REFRESH;
    }
