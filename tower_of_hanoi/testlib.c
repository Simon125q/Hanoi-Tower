
#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define PEGS 3
#define BLOCKS 9
#define TRUE 1
#define FALSE 0
#define DELAY 10
#define MOVEMENT_SPEED 3
#define FLOOR_HEIGHT 50
#define PEG_WIDTH 10
#define MIN_BLOCK_WIDTH (PEG_WIDTH + 10)
#define X1 0
#define Y1 1
#define X2 2
#define Y2 3
#define MAGNITUDE 4
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define PEG_HEIGHT (0.8 * SCREEN_HEIGHT)
#define BLOCK_HEIGHT ((PEG_HEIGHT - FLOOR_HEIGHT) / (BLOCKS + 1))
#define MAX_BLOCK_WIDTH ((0.8 * SCREEN_WIDTH) / (PEGS + 1))
// how much to add or subctract form the biggest block width while blocks get smaller
#define BLOCKS_DIF (((MAX_BLOCK_WIDTH - MIN_BLOCK_WIDTH) / (BLOCKS - 1)) / 2)
#define CENTER_X_COORD ((0.9 * SCREEN_WIDTH / (PEGS + 1)) + PEG_WIDTH / 2)
#define Y_ABOVE_FLOOR (SCREEN_HEIGHT - FLOOR_HEIGHT)
#define MOVEMENT_HEIGHT (PEG_HEIGHT + 20)

int play = TRUE;
int top = FALSE;
// 1-x1 coord, 2-y1 coord, 3-x2 coord, 4-y2coord, 5-magnitude
int block_position[PEGS][BLOCKS][5] = {0};
int moving_block[5] = {0};

void starting_blocks();
void controls();
void move_up(int peg_nember);
void move_to_side(int destination);
void move_down(int peg_number);
void move(int pressed_key_number);
void draw();
void check_win();

int main()
{

    if (gfx_init())
    {
        exit(3);
    }

    starting_blocks();

    while (play)
    {

        draw();

        controls();

        SDL_Delay(DELAY);

        check_win();

        if (gfx_isKeyDown(SDLK_ESCAPE))
        {
            play = FALSE;
        }
    }

    return 0;
}
void controls()
{
    
    if (gfx_isKeyDown(SDLK_1))
    {
        move(1);
    }
    if (gfx_isKeyDown(SDLK_2))
    {
        move(2);
    }
    if (gfx_isKeyDown(SDLK_3))
    {
        move(3);
    }
    if (gfx_isKeyDown(SDLK_4))
    {
        move(4);
    }
    if (gfx_isKeyDown(SDLK_5))
    {
        move(5);
    }
    if (gfx_isKeyDown(SDLK_6))
    {
        move(6);
    }
    if (gfx_isKeyDown(SDLK_7))
    {
        move(7);
    }
    if (gfx_isKeyDown(SDLK_8))
    {
        move(8);
    }
    if (gfx_isKeyDown(SDLK_9))
    {
        move(9);
    }
    if (gfx_isKeyDown(SDLK_0))
    {
        move(10);
    }
}

void starting_blocks()
{
    double primary_x;
    double primary_y;
    int magnitude = BLOCKS;
    for (int block = 0; block < BLOCKS; block++)
    {
        primary_x = CENTER_X_COORD - MAX_BLOCK_WIDTH / 2;
        primary_y = Y_ABOVE_FLOOR - block * BLOCK_HEIGHT;
        block_position[0][block][X1] = primary_x + block * BLOCKS_DIF;
        block_position[0][block][Y1] = primary_y - BLOCK_HEIGHT;
        block_position[0][block][X2] = primary_x + MAX_BLOCK_WIDTH - block * BLOCKS_DIF;
        block_position[0][block][Y2] = primary_y;
        block_position[0][block][MAGNITUDE] = magnitude;
        magnitude--;
    }
}

void move_up(int peg_number)
{
    int block = BLOCKS - 1;
    int move = TRUE;
    while (block_position[peg_number - 1][block][Y1] == 0 && block != -1)
    {
        block--;
    }
    while (block > -1 && move)
    {
        block_position[peg_number - 1][block][Y1] -= MOVEMENT_SPEED;
        block_position[peg_number - 1][block][Y2] -= MOVEMENT_SPEED;

        if (block_position[peg_number - 1][block][Y2] <= SCREEN_HEIGHT - MOVEMENT_HEIGHT)
        {
            move = FALSE;
        }
        draw();

        SDL_Delay(DELAY);

        if (gfx_isKeyDown(SDLK_ESCAPE))
        {
            move = FALSE;
            play = FALSE;
        }
    }
    if(block != -1)
    {
        for (int coord = 0; coord < 5; coord++)
        {
            moving_block[coord] = block_position[peg_number - 1][block][coord];
            block_position[peg_number - 1][block][coord] = 0;
        }
        top = TRUE;
    }
}

void move_to_side(int destination)
{

    int dest_coord_x = CENTER_X_COORD * destination;
    int x_center = (moving_block[X1] + moving_block[X2]) / 2;
    int moving_direction;
    int move = TRUE;
    if (x_center < dest_coord_x)
    {
        moving_direction = 1;
    }
    else
    {
        moving_direction = -1;
    }
    while ((x_center <= (dest_coord_x - MOVEMENT_SPEED) || x_center >= (dest_coord_x + MOVEMENT_SPEED)) && move)
    {
        moving_block[0] += MOVEMENT_SPEED * moving_direction;
        moving_block[2] += MOVEMENT_SPEED * moving_direction;
        x_center = (moving_block[X1] + moving_block[X2]) / 2;
        draw();
        SDL_Delay(DELAY);
        if (gfx_isKeyDown(SDLK_ESCAPE))
        {
            move = FALSE;
            play = FALSE;
        }
    }
    move_down(destination);
}

void move_down(int peg_number)
{
    int first_empty_position = BLOCKS - 1;

    while (block_position[peg_number - 1][first_empty_position][X1] == 0 && first_empty_position != -1)
    {
        first_empty_position--;
    }
    first_empty_position += 1;
    if (moving_block[4] < block_position[peg_number - 1][first_empty_position - 1][4] || first_empty_position == 0)
    {
        for (int coord = 0; coord < 5; coord++)
        {
            block_position[peg_number - 1][first_empty_position][coord] = moving_block[coord];
            moving_block[coord] = 0;
        }

        while ((first_empty_position == 0 || block_position[peg_number - 1][first_empty_position][Y2] <= block_position[peg_number - 1][first_empty_position - 1][Y1] - MOVEMENT_SPEED) &&
               block_position[peg_number - 1][first_empty_position][Y2] <= Y_ABOVE_FLOOR - MOVEMENT_SPEED)
        {
            block_position[peg_number - 1][first_empty_position][Y1] += MOVEMENT_SPEED;
            block_position[peg_number - 1][first_empty_position][Y2] += MOVEMENT_SPEED;
            draw();
            SDL_Delay(DELAY);
        }
        top = FALSE;
    }
}

void move(int pressed_key_number)
{
    if(PEGS >= pressed_key_number)
    {
        if (top == FALSE)
        {
            move_up(pressed_key_number);
        }
        else if (top == TRUE)
        {
            move_to_side(pressed_key_number);
        }
    }
}

void draw()
{
    int block_color;
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    for (int peg = 1; peg <= PEGS; peg++)
    {
        gfx_filledRect(peg * CENTER_X_COORD - PEG_WIDTH / 2, SCREEN_HEIGHT - PEG_HEIGHT, peg * CENTER_X_COORD + PEG_WIDTH / 2, SCREEN_HEIGHT, GREEN);
    }
    gfx_filledRect(0, SCREEN_HEIGHT - FLOOR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, YELLOW);

    for (int peg = 0; peg < PEGS; peg++)
    {
        for (int block = 0; block < BLOCKS; block++)
        {
            if (block_position[peg][block][MAGNITUDE] % 2 == 0)
            {
                block_color = RED;
            }
            else
            {
                block_color = BLUE;
            }
            gfx_filledRect(block_position[peg][block][X1], block_position[peg][block][Y1], block_position[peg][block][X2], block_position[peg][block][Y2], block_color);
        }
    }
    if (moving_block[MAGNITUDE] % 2 == 0)
    {
        block_color = RED;
    }
    else
    {
        block_color = BLUE;
    }

    gfx_filledRect(moving_block[X1], moving_block[Y1], moving_block[X2], moving_block[Y2], block_color);

    gfx_updateScreen();
}

void check_win()
{
    int winning_block = block_position[PEGS - 1][BLOCKS - 1][4];
    if (winning_block == 1)
    {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
        gfx_textout(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "YOU WON!!", GREEN);
        gfx_updateScreen();
        gfx_getkey();
        exit(2);
    }
}