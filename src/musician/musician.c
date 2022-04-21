#include <stdio.h>
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
#define NUMBER_OF_CMD 2
#define port 1234

char instrument[40];



char * available_cmd[2] = {
  "play",
  "pause"
};

void display_help () {
  int i;
  printf("available instruments : \n");
  for (i = 0; i < NUMBER_OF_INSTRUMENTS; i++) {
    printf("%s\n", music_instruments[i]);
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
  if (strcmp("", cmd)) {
    for (i = 0; i < NUMBER_OF_CMD; i++) {
      if (strcmp(available_cmd[i], cmd) == 0) {
        return true;
      }
    }
    red();
    printf("\n%s is not a valid command.\n", cmd);
    white();
  }
  return false;
}

void get_instrument () {
  strncpy(instrument, "", sizeof(instrument));
  while (!is_valid_instrument(instrument)) {
    printf("\nWhat is your instrument ? ");
    scanf("%s", instrument);
    if(!strcmp("help", instrument) || !strcmp("h", instrument)) {
      display_help();
    }
  }
}


void display_welcome_screen () {
  yellow();
  printf("======================================================\n");
  printf("=                    MUSICIAN                        =\n");
  printf("======================================================\n");
  green();
  printf("To see the available instrument, type \"help\" or \"h\"\n");
  white();
}

void listen_for_key_events(int sockfd) {
  char cmd[40];
  printf("Type \"play\" or \"pause\"\n");
  while(true) {
    scanf(" %s", cmd);
    if (is_valid_cmd(cmd)) {
      assert(send(sockfd, cmd, sizeof(instrument), 0)!=-1);
      sleep(2);
    }
  }
}

int get_connection () {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  assert(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)>=0);

  // la connexion déclenche la création du thread distant
  assert(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr))>=0);


  assert(send(sockfd, instrument, sizeof(instrument), 0)!=-1);
  sleep(2);
	// close(sockfd);
  return sockfd;
}




int main(int argc, char* argv[]) {
  display_welcome_screen();
  get_instrument();
  int sockfd = get_connection();
  listen_for_key_events(sockfd);
  return 0;
}
