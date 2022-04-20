#include "colors.h"
#include <stdio.h>


const char * BLACK = "\033[0;30m";
const char * RED = "\033[0;31m";
const char * GREEN = "\033[0;32m";
const char * YELLOW = "\033[0;33m";
const char * BLUE = "\033[0;34m";
const char * PURPLE = "\033[0;35m";
const char * CYAN = "\033[0;36m";
const char * WHITE = "\033[0;37m";


void black() {
  printf("%s", BLACK);
}
void red(){
  printf("%s", RED);
}
void green(){
  printf("%s", GREEN);
}
void yellow(){
  printf("%s", YELLOW);
}
void blue(){
  printf("%s", BLUE);
}
void purple(){
  printf("%s", PURPLE);
}
void cyan(){
  printf("%s", CYAN);
}
void white(){
  printf("%s", WHITE);
}