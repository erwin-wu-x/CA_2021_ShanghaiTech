// snake.h
#ifndef SNAKE_H
#define SNAKE_H
#endif

typedef struct body
{
    int x;
    int y;
    struct body *next;
    struct body *prev;
} body;

typedef struct snake
{
    int len;
    body *head;
    body *tail;
} snake;
