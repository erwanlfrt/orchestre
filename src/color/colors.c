#include "colors.h"
#include <stdio.h>
#include <ncurses.h>

#define BLACK_PAIR 1
#define RED_PAIR 2
#define GREEN_PAIR 3 
#define YELLOW_PAIR 4
#define BLUE_PAIR 5
#define CYAN_PAIR 6
#define WHITE_PAIR 7

const char * BLACK = "\033[0;30m";
const char * RED = "\033[0;31m";
const char * GREEN = "\033[0;32m";
const char * YELLOW = "\033[0;33m";
const char * BLUE = "\033[0;34m";
const char * PURPLE = "\033[0;35m";
const char * CYAN = "\033[0;36m";
const char * WHITE = "\033[0;37m";

/* =======================================

Put some colors into the terminal

========================================== */


void init() {
  start_color();
  init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(BLACK_PAIR, COLOR_BLACK, COLOR_BLACK);
  init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
  init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
  init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
  init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
}

void black() {
  attron(COLOR_PAIR(BLACK_PAIR));
}

void red(){
  attron(COLOR_PAIR(RED_PAIR));
}

void green(){
  attron(COLOR_PAIR(GREEN_PAIR));
}

void yellow(){
  attron(COLOR_PAIR(YELLOW_PAIR));
}

void blue(){
  attron(COLOR_PAIR(BLUE_PAIR));
}

void cyan(){
  attron(COLOR_PAIR(CYAN_PAIR));
}

void white(){
  attron(COLOR_PAIR(WHITE_PAIR));
}


void printf_black() {
  printf("%s", BLACK);
}
void printf_red(){
  printf("%s", RED);
}
void printf_green(){
  printf("%s", GREEN);
}
void printf_yellow(){
  printf("%s", YELLOW);
}
void printf_blue(){
  printf("%s", BLUE);
}
void printf_purple(){
  printf("%s", PURPLE);
}
void printf_cyan(){
  printf("%s", CYAN);
}
void printf_white(){
  printf("%s", WHITE);
}