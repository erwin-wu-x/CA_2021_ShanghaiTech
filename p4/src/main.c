#include "lcd/lcd.h"
#include <string.h>
#include <stdlib.h>
#include "snake.h"
#include "utils.h"

int x_max = 152; /* range(x)=(0,159) */
int y_max = 67;  /* range(y)=(0,74) */
int x = 0;
int y = 0;
int randseed = 1;
int food = 0;

int diff = 2000;
int state = 0;
int option = 0;
int diffoption = 1;
int direction = 0;       /* 0:up 2:down 1:left 3:right */
int foodx[] = {0, 0, 0}; /* green yellow red */
int foody[] = {0, 0, 0};
bool alive = TRUE;
bool green_exist = FALSE;
bool yellow_exist = FALSE;
bool red_exist = FALSE;

/* snake functions */

void init_snake(snake *s)
{
    s->len = 0;
    s->head = malloc(sizeof(body));
    s->head->x = 80;
    s->head->y = 40;
    s->head->next = NULL;
    s->head->prev = NULL;
    s->tail = s->head;
    snakehead(s->head->x, s->head->y);
}
void delete_tail(snake *s)
{
    body *temp = s->tail;
    if (s->tail->prev)
    {
        s->tail->prev->next = NULL;
        killsnake(s->tail->x, s->tail->y);
        s->tail = s->tail->prev;
    }
    else
        killhead(s->tail->x, s->tail->y);
    free(temp);
}
void check_alive(snake *s)
{
    if (s->head->x <= 2 || s->head->x >= 155 || s->head->y <= 2 || s->head->y >= 72)
        func_endmenu(s->len);
    body *temp = s->head;
    while (temp != s->tail)
    {
        temp = temp->next;
        if (temp->x == s->head->x && temp->y == s->head->y)
            func_endmenu(s->len);
    }
}
void add_head(snake *s)
{
    body *temp = malloc(sizeof(body));
    temp->x = s->head->x;
    temp->y = s->head->y;
    temp->prev = NULL;
    temp->next = s->head;
    s->head->prev = temp;
    s->head = temp;
    if (direction == 0)
        temp->y -= 4;
    if (direction == 1)
        temp->x += 4;
    if (direction == 2)
        temp->y += 4;
    if (direction == 3)
        temp->x -= 4;
    check_alive(s);
}
void move_snake(snake *s, bool instr) /* control the snake lenth */
{
    button();
    delay_1ms(diff / 5);
    change_direction();
    add_head(s);
    int this_food = check_foodxy(s->head->x, s->head->y);
    // LCD_ShowNum(90, 15, foodx[0], 3, 0xFFFFFFFF);
    // LCD_ShowNum(90, 30, foody[0], 3, 0xFFFFFFFF);
    // LCD_ShowNum(90, 45, s->head->x, 3, 0xFFFFFFFF);
    // LCD_ShowNum(90, 60, s->head->y, 3, 0xFFFFFFFF);
    // LCD_ShowNum(70, 60, this_food, 3, 0xFFFFFFFF);
    if (this_food == 0 || instr)
    {
        if (this_food == 0)
        {
            LCD_DrawCircle(foodx[0], foody[0], 2, 0x0);
            foodx[0] = 0;
            foody[0] = 0;
            green_exist = FALSE;
        }
        s->len++;
    }
    else if (this_food == 1)
    {
        // LCD_ShowNum(90, 15, foodx[1], 3, 0xFFFFFFFF);
        // LCD_ShowNum(90, 30, foody[1], 3, 0xFFFFFFFF);
        LCD_DrawPoint_big(foodx[1], foody[1], 0x0);
        foodx[1] = 0;
        foody[1] = 0;
        // LCD_ShowNum(90, 15, foodx[1], 3, 0xFFFFFFFF);
        // LCD_ShowNum(90, 30, foody[1], 3, 0xFFFFFFFF);
        yellow_exist = FALSE;
        s->len--;
        // LCD_ShowNum(90, 30, s->len, 3, 0xFFFFFFFF);
        if (s->len <= 1)
            func_endmenu(2);
        delete_tail(s);
        delete_tail(s);
    }
    else if (this_food == 2)
    {
        LCD_DrawPoint_big(foodx[2], foody[2], 0x0);
        foodx[2] = 0;
        foody[2] = 0;
        red_exist = FALSE;
        func_endmenu(s->len);
    }
    else
        delete_tail(s);
    snakehead(s->head->x, s->head->y);
    killhead(s->head->next->x, s->head->next->y);
    snakebody(s->head->next->x, s->head->next->y);
}

/* snake functions */

/*  button function */
int button()
{
    if (Get_Button(0) == 1 && Get_Button(1) == 1) //still
        state = 0;
    else if (Get_Button(0) == 1 && Get_Button(1) == 0) //0
        state = 1;
    else if (Get_Button(0) == 0 && Get_Button(1) == 1) //1
        state = 2;
    else
        state = 0;
    return state;
}
/*  button function */

/* assitant funcion*/
int change_option()
{
    if (option)
        option = 0;
    else
        option = 1;
    return option;
}
void change_direction()
{
    if (state == 1)
        direction++;
    else if (state == 2)
        direction--;
    if (direction == 4)
        direction = 0;
    if (direction == -1)
        direction = 3;
}
int check_foodxy(int x, int y)
{
    for (int i = 0; i < 3; i++)
        if (abs(foodx[i] - x) < 2 && abs(foody[i] - y) < 2)
            return i;
    return -1;
}
/* assitant funcion*/

void Inp_init(void)
{
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
    RCU_CFG0 |= (0b10 << 14) | (1 << 28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0) |= ADC_CTL1_ADCON;
}

void IO_init(void)
{
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
}

/* menu functions */
void func_startmenu()
{
    delay_1ms(400);
    startmenu();
    choice(option, 1);
    while (button() != 1)
    {
        if (state == 2)
        {
            choice(option, 0);
            choice(change_option(), 1);
            delay_1ms(300);
        }
    }
    if (option)
        func_helpmenu();
    else
        func_diffmenu();
}

void func_helpmenu()
{
    delay_1ms(400);
    helpmenu();
    while (button() == 0)
        continue;
    func_startmenu();
}

void func_playmenu()
{
    LCD_Clear(0);
    snake mysnake;
    init_snake(&mysnake);

    int temp = 0;
    direction = 0;
    green_exist = FALSE;
    yellow_exist = FALSE;
    red_exist = FALSE;
    srand(randseed++);

    while (alive)
    {
        while (check_foodxy(x, y) != -1)
        {
            x = rand() % x_max + 4;
            y = rand() % y_max + 4;
        }
        button();
        delay_1ms(diff / 5);
        x = x / 4 * 4;
        y = y / 4 * 4;
        if (!green_exist)
        {
            button();
            delay_1ms(diff / 5);
            food0(x, y);
            green_exist = TRUE;
            foodx[0] = x;
            foody[0] = y;
        }
        else if (!yellow_exist)
        {
            button();
            delay_1ms(diff / 5);
            food1(x, y);
            yellow_exist = TRUE;
            foodx[1] = x;
            foody[1] = y;
        }
        else if (!red_exist)
        {
            button();
            delay_1ms(diff / 5);
            food2(x, y);
            red_exist = TRUE;
            foodx[2] = x;
            foody[2] = y;
        }
        else
        {
            button();
            delay_1ms(diff / 5);
        }
        button();
        delay_1ms(diff / 5);
        if (temp < 2)
            move_snake(&mysnake, TRUE);
        move_snake(&mysnake, FALSE);
        delay_1ms(diff / 5);
        temp++;
    }
    func_endmenu(mysnake.len);
}
void func_diffmenu()
{
    delay_1ms(400);

    difficulty();
    otherchoice(diffoption, 1);
    while (button() != 1)
    {
        if (state == 2)
        {
            otherchoice(diffoption, 0);
            diffoption++;
            if (diffoption == 4)
                diffoption = 0;
            otherchoice(diffoption, 1);
            delay_1ms(300);
        }
    }
    if (diffoption == 0)
        diff = 1000;
    else if (diffoption == 1)
        diff = 600;
    else if (diffoption == 2)
        diff = 300;
    else
        diff = 100;
    func_playmenu();
}
void func_endmenu(int score)
{
    delay_1ms(400);
    score -= 2;
    scoreboard(score);
    choice(option, 1);
    while (button() != 1)
    {
        if (state == 2)
        {
            choice(option, 0);
            choice(change_option(), 1);
            delay_1ms(300);
        }
    }
    if (option)
        func_startmenu();
    else
        func_diffmenu();
}
/* menu functions */

int main(void)
{
    IO_init(); // init OLED

    func_startmenu();

    return 0;
}
