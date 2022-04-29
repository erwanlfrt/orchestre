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
#include <stdlib.h>
#include "../color/colors.h"
#include "../instruments/instruments.h"
#include "../direction/direction.h"
#include "../display/display.h"
#include "./musician.h"
#define NUMBER_OF_CMD 2
#define port 1234
#define NUMBER_OF_CMD 2
#define port 1234
#define UP 8
#define MIDDLE 14
#define DOWN 15

int current_direction = CENTER;
bool is_playing = false;
char instrument[40];
char * available_cmd[2] = {
  "play",
  "pause"
};

/**
 * @brief Ask which instrument the user want.
 * 
 */
void get_instrument () {
  strncpy(instrument, "", sizeof(instrument));
  while (!is_valid_instrument(instrument)) {
    printw("\nWhat is your instrument ? ");
    refresh();
    scanw("%s", instrument); // ask the instrument while the user doesn't give a valid one.
    if(!strcmp("help", instrument) || !strcmp("h", instrument)) {
      display_help(); // display help if "help" or "h" is entered
    }
  }
  display_position_menu(true); // display position menu to ask for init position
}


/**
 * @brief Listen for key events
 * 
 */
void listen_for_key_events() {
  noecho();
  display_header();
  char input;
  char cmd[40];
  if (is_playing) {
    display_play(); // display play if musician is playing
  } else {
    display_pause(); // display pause if musician is not playing
  }
  move(MIDDLE, 0); // move on terminal
  int position = MIDDLE;
  printw("> Set sound position\n  Exit\n"); // print menu options
  refresh();
  while(true) {
    input = getch(); // get input value
    refresh();
    if (input == '\n') {
      // on enter, get the position and do the appropriate action
      if (position == UP) {
        // play/pause
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
        // display compass
        display_position_menu(false);
        break;
      } else if (position == DOWN) {
        // exit the orchestra
        strncpy(cmd, "stop", sizeof(cmd));
        assert(send(sockfd, cmd, sizeof(cmd), 0)!=-1);
        close(sockfd);
        break;
      }
      
    } else if(input == 'z') {   
        // select the lower option in menu
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
      // select the upper option in menu 
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


/**
 * @brief Get a connection to the orchestra.
 * 
 */
bool get_connection () {
  echo();
  // socket configuration
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)>=0);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    // not connected
    red();
    printw("Orchestra doesn't want new musicians for now. Try again later :)\n");
    white();
    refresh();
    return false;
  } else {
    //connected, send the instrument and the init position
    assert(send(sockfd, instrument, sizeof(instrument), 0)!=-1);
    refresh();
    sleep(2);
    char buf = current_direction + '0';
    char * buffer = malloc(sizeof(buf));
    buffer[0] = buf;
    assert(send(sockfd, buffer, sizeof(buffer), 0)!=-1);
    return true;
  }
  
}

/**
 * @brief main function
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @return int EXIT_SUCCESS
 */

int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  if(get_connection()) {
    listen_for_key_events(); // display menu if connected
  }
  return 0;
}
