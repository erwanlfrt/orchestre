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
  display_position_menu(true);
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

void draw_compass (bool is_init) {
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
  char buf = current_direction + '0';
  char * buffer[1];
  buffer[0] = buf;
  if (!is_init) {
    assert(send(sockfd, &buffer, sizeof(buffer), 0)!=-1);
  }
}

void display_position_menu (bool is_init) {
  display_header();
  move(5, 0);
  printw("========= POSITION =========\n");
  draw_compass(is_init);
  char input;
  while (true) {
    input = getch();
    if ((int)input == 27 && !is_init) {
      listen_for_key_events();
      break;
    } else if ((int)input == 10 && is_init) {
      break;
    }
    else if (input == 'd') {
      current_direction++;
      if (current_direction > 7) {
        current_direction = 0;
      }
      draw_compass(is_init);
    } else if (input == 'q') {
      current_direction--;
      if (current_direction < 0) {
        current_direction = 7;
      }
      draw_compass(is_init);
    } else if (input == 'o') {
      current_direction = -1;
      draw_compass(is_init);
    }
  }
}
bool test_connection () {
  // sleep(5);
  // int check = send(sockfd, instrument, sizeof(instrument), 0);
  // printw("check = %i\n", check);
  // if (!check) {
  //   printw("problem pas de connexion");
  // } else {
  //   printw("tout est ok");
  // }
  // refresh();
  int error = 0;
  socklen_t len = sizeof (error);
  int retval = getsockopt (sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
  printw("retval = %i", retval);
  if (retval != 0) {
    /* there was a problem getting the error code */
    printw("error getting socket error code: %s\n", strerror(retval));
    return false;
  }

  if (error != 0) {
      /* socket has a non zero error status */
      printw("socket error: %s\n", strerror(error));
      return false;
  }
  refresh();
  return true;
}

void listen_for_key_events() {
  noecho();
  display_header();
  char input;
  char cmd[40];
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
        display_position_menu(false);
        break;
      } else if (position == DOWN) {
        strncpy(cmd, "stop", sizeof(cmd));
        assert(send(sockfd, cmd, sizeof(cmd), 0)!=-1);
        close(sockfd);
        break;
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
  echo();
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)>=0);

  // la connexion déclenche la création du thread distant
  assert(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);

  assert(send(sockfd, instrument, sizeof(instrument), 0)!=-1);
  refresh();
  sleep(2);
  char buf = current_direction + '0';
  char * buffer[1];
  buffer[0] = buf;
  assert(send(sockfd, &buffer, sizeof(buffer), 0)!=-1);
	// close(sockfd);
}

int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  get_connection();
  // test_connection();
  // while(1);
  listen_for_key_events();
  // print_menu();
  return 0;
}
