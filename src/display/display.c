#include <stdbool.h>
#include "../direction/direction.h"
#include "../musician/musician.h"
#include "../instruments/instruments.h"
#include "../color/colors.h"
#include <ncurses.h>
#include <assert.h>
#include <sys/socket.h>
#include <stdlib.h>

extern int current_direction;

/**
 * @brief display pause into terminal
 * 
 */
void display_pause () {
  green();
  move(5,0);
  printw("========= PLAYER =========\n");
  printw("           $\n");
  printw("           $$$\n");
  printw("           $$$$$\n");
  printw("           $$$\n");
  printw("           $\n");
  printw("==========================\n");
  move(8,0);
  white();
  refresh();
}

/**
 * @brief display "play" into terminal
 * 
 */
void display_play () {
  green();
  move(5,0);
  printw("========= PLAYER =========\n");
  printw("           $$ $$\n");
  printw("           $$ $$\n");
  printw("           $$ $$\n");
  printw("           $$ $$\n");
  printw("           $$ $$\n");
  printw("==========================\n");
  move(8,0);
  white();
  refresh();
}

/**
 * @brief display header into terminal
 * 
 */
void display_header () {
  clear();
  yellow();
  printw("======================================================\n");
  printw("=                    MUSICIAN                        =\n");
  printw("======================================================\n");
  white();
}

/**
 * @brief display help screen
 * 
 */
void display_help () {
  int i;
  printw("available instruments : \n");
  // print all the instruments
  for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
    printw("%s\n", music_instruments[i]);
  }
}



/**
 * @brief Display welcome screen
 * 
 */
void display_welcome_screen () {
  initscr();
  init();
  display_header();
  green();
  printw("To see the available instrument, type \"help\" or \"h\"\n");
  white();
  refresh();
}

/**
 * @brief draw compass with green hightlighted current position
 * 
 * @param is_init true if the compass is for initialization, false if not
 */
void draw_compass (bool is_init) {
  move(6,0);
  //print step by step the compass, print in green color the current_direction
  if (current_direction == N) {
    green();
  }
  printw("         N\n");
  printw("         |\n");
  white();
  if (current_direction == NW) {
    green();
  }
  printw("     NW");
  white();
  if (current_direction == N) {
    green();
  }
  printw("  |");
  white();
  if(current_direction == NE) {
    green();
  }
  printw("  NE\n");
  white();

  //line 4
  if (current_direction == NW) {
    green();
  }
  printw("       \\");
  white();
  if (current_direction == N) {
    green();
  }
  printw(" | ");
  white();
  if(current_direction == NE) {
    green();
  }
  printw("/\n");
  white();

  //line 5
  if (current_direction == W) {
    green();
  }
  printw("W--------");
  white();
  if (current_direction == CENTER) {
    green();
  }
  printw("o");
  white();
  if(current_direction == E) {
    green();
  }
  printw("--------E\n");
  white();

  // line 6
  if (current_direction == SW) {
    green();
  }
  printw("       / ");
  white();
  if (current_direction == S) {
    green();
  }
  printw("| ");
  white();
  if(current_direction == SE) {
    green();
  }
  printw("\\\n");
  white();

  // line 7
  if (current_direction == SW) {
    green();
  }
  printw("     SW");
  white();
  if (current_direction == S) {
    green();
  }
  printw("  |  ");
  white();
  if(current_direction == SE) {
    green();
  }
  printw("SE\n");
  white();

  // line 8
  if (current_direction == S) {
    green();
  }
  printw("         |\n");
  white();
  if (current_direction == S) {
    green();
  }
  printw("         S\n");
  white();
  printw("                          \n");
  refresh();
  char buf = current_direction + '0';
  char * buffer = malloc(sizeof(buf));
  buffer[0] = buf;
  if (!is_init) {
    assert(send(sockfd, buffer, sizeof(buffer), 0)!=-1); // send the current_direction if we are already connected to the orchestra
  }
}

/**
 * @brief display position menu
 * 
 * @param is_init true if the position menu is for initialization, false if not
 */
void display_position_menu (bool is_init) {
  noecho();
  display_header();
  move(5, 0);
  printw("========= POSITION =========\n");
  draw_compass(is_init);
  char input;
  while (true) {
    input = getch();
    if ((int)input == 27 && !is_init) {
      listen_for_key_events(); // if 'esc' key pressed, then go back to previous menu
      break;
    } else if ((int)input == 10 && is_init) {
      break; // if enter is pressed during init, then stop waiting for getting the init direction
    }
    else if (input == 'd') {
      current_direction++; // select the next direction
      if (current_direction > 7) {
        current_direction = 0; // new turn
      }
      draw_compass(is_init);
    } else if (input == 'q') {
      current_direction--; // select the previous direction
      if (current_direction < 0) {
        current_direction = 7; // new turn
      }
      draw_compass(is_init);
    } else if (input == 'o') {
      current_direction = -1; // set the direction to 'center'
      draw_compass(is_init);
    }
  }
}
