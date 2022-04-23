#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include "../tools/colors.h"
#include "../instruments/instruments.h"
#include "../direction/direction.h"
#define NUMBER_OF_CMD 2
#define port 1234
#define NUMBER_OF_CMD 2
#define port 1234

#define UP 8
#define MIDDLE 14
#define DOWN 15

char instrument[40];



char * available_cmd[2] = {
  "play",
  "pause"
};

char instrument[40];

int current_direction = CENTER;

bool is_playing = false;
int sockfd;

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

void display_header () {
  clear();
  yellow();
  printw("======================================================\n");
  printw("=                    MUSICIAN                        =\n");
  printw("======================================================\n");
  white();
}

void display_help () {
  int i;
  printw("available instruments : \n");
  for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
    printw("%s\n", music_instruments[i]);
  }
}

bool is_valid_instrument (char * instrument) {
  int i;
  if (strcmp("", instrument) && strcmp("help", instrument) && strcmp("h", instrument) ) {
    for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
      if (strcmp(music_instruments[i], instrument) == 0) {
        return true;
      }
    }
    red();
    printf("%s is not a valid instrument.\n", instrument);
    white();
  }
  return false;
}

bool is_valid_cmd (char* cmd) {
  int i;
  if (!strcmp("", cmd)) {
    return true;
  }
  return false;
}

void get_instrument () {
  strncpy(instrument, "", sizeof(instrument));
  while (!is_valid_instrument(instrument)) {
    printw("\nWhat is your instrument ? ");
    refresh();
    scanw("%s", instrument);
    if(!strcmp("help", instrument) || !strcmp("h", instrument)) {
      display_help();
    }
  }
}


void display_welcome_screen () {
  initscr();
  init();
  display_header();
  green();
  printw("To see the available instrument, type \"help\" or \"h\"\n");
  white();
  refresh();
}

void draw_compass () {
  move(6,0);
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
}

void display_position_menu () {
  display_header();
  move(5, 0);
  printw("========= POSITION =========\n");
  draw_compass();
  char input;
  while (true) {
    input = getch();
    if ((int)input == 27) {
      listen_for_key_events();
      break;
    } 
    else if (input == 'd') {
      current_direction++;
      if (current_direction > 7) {
        current_direction = 0;
      }
      draw_compass();
    } else if (input == 'q') {
      current_direction--;
      if (current_direction < 0) {
        current_direction = 7;
      }
      draw_compass();
    } else if (input == 'o') {
      current_direction = -1;
      draw_compass();
    }
  }
}

void listen_for_key_events() {
  noecho();
  display_header();
  char input;
  char cmd[40];
  // printw("Type \"play\" or \"pause\"\n");
  display_pause();
  move(MIDDLE, 0);
  int position = MIDDLE;
  printw("> Set sound position\n  Exit\n");
  refresh();
  while(true) {
    input = getch();
    refresh();
    if (input == '\n') {
      if (position == UP) {
        is_playing = !is_playing;
        if (is_playing) {
          strncpy(cmd, "play", sizeof(cmd));
          display_play();
        } else {
          strncpy(cmd, "pause", sizeof(cmd));
          display_pause();
        }
        assert(send(sockfd, cmd, sizeof(cmd), 0)!=-1);
        sleep(2);
      } else if (position == MIDDLE) {
        display_position_menu();
        break;
      } else if (position == DOWN) {
      }
      
    } else if(input == 'z') {   
        if (position == DOWN) {
          move(DOWN,0);
          printw(" ");
          move(MIDDLE,0);
          printw(">");
          position = MIDDLE;
        } else if (position == MIDDLE) {
          move(MIDDLE,0);
          printw(" ");
          move(UP, 0);
          position = UP;
        }
    } else if(input == 's') {   
      if (position == UP) {
        move(MIDDLE,0);
        printw(">");
        position = MIDDLE;
      } else if (position == MIDDLE) {
        move(MIDDLE,0);
        printw(" ");
        move(DOWN,0);
        printw(">");
        position = DOWN;
      }
    }
  }
}

void get_connection () {
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)>=0);

  // la connexion déclenche la création du thread distant
  assert(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);


  assert(send(sockfd, instrument, sizeof(instrument), 0)!=-1);
  sleep(2);
	// close(sockfd);
}


int print_menu () {
  clear();
  display_header();
  char input;;
  char c;
  int position = MIDDLE;
  printw("\n\n> Quit Game without saving?\n  Save Game ▶!\n");
  for ( ;; )
  {
    input = getch();
    if(input == 'z')
    {   
        move(MIDDLE,0);
        printw(" ");
        move(DOWN,0);
        printw(">");
        position = MIDDLE;
    }
    if(input == 's')
    {   
        move(MIDDLE,0);
        printw(" ");
        move(DOWN,0);
        printw(">");
        position = MIDDLE;
    }
  }
  endwin();
  return 0;
}




int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  get_connection();
  listen_for_key_events();
  // print_menu();
  return 0;
}
