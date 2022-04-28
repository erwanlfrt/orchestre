#include <stdbool.h>

#ifndef DISPLAY_H_
#define DISPLAY_H_

void display_pause();
void display_play ();
void display_header ();
void display_help ();
void display_welcome_screen ();
void draw_compass (bool is_init);
void display_position_menu (bool is_init);
void display_orchestra_header();

#endif