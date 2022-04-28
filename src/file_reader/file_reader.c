
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "../tools/tools.h"
#include "../instruments/instruments.h"
#include "../audio/audio.h"
#include "../direction/direction.h"
#include "../orchestra/orchestra.h"
#include <unistd.h>



/**
 * @brief Check if line is a valid command line
 * 
 * @param line line to check
 * @param step current step, possible value: CREATE_STEP, RUN_STEP, EXIT_STEP
 * @return true if line is a valid command line
 */
bool check_line (char* line, int step) {
  if (!strcmp(line, "")) return true;
  if (step == 1) {
    if (!strcmp(line, "CREATE")) return true;
    char * string_id = strsep(&line, " ");
    if (!is_int(string_id)) return false;
    int id = atoi(string_id);
    char * instrument = strsep(&line, " ");
    if (!is_valid_instrument(instrument)) return false;
    char * direction =  strsep(&line, " ");
    if (get_direction(direction) == -2) return false;
    create_musician(instrument, id, 0);
    load(id);
    set_position(id, get_direction(direction));
    return true;
  } else if (step == 2) {
    if (!strcmp(line, "RUN")) return true;
    if (!is_valid_cmd(line)) return false;
    return true;

  } else if (step == 3) {
    if (!strcmp("EXIT", line)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Check if a file is a valid script
 * 
 * @param array array in which commands will be stored
 * @param file file path
 * @param nb_lines number of lines commands
 * @param create_step index of create step into the number of lines
 * @param run_step  index of run_step
 * @param exit_step index of exit_step
 * @return true if file is correct, false if not
 */
bool check_file (char* array[], char* file, int nb_lines, int create_step, int run_step, int exit_step) {
  char * line = NULL;
  int i = 0;
  int current_step = 1;
  bool file_ok = true;
  FILE * fp;
  size_t len = 0;
  ssize_t read;
  fp = fopen(file, "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
      crop_newline(line);
      char copy_line[100];
      strcpy(copy_line, line);
      if (check_line (copy_line, current_step )) {
        array[i] = malloc(100);
        strcpy(array[i], line);
      } else {
        printf("Error in line %i : %s\n", i, line);
        file_ok = false;
        break;
      }
      i++;
      if (i >= run_step && i < exit_step ) {
        current_step = 2;
      } else if (i >= exit_step) {
        current_step = 3;
      }
  }
  fclose(fp);
  if (line) free(line);
  return file_ok;
}



/**
 * @brief Play a file, its commands have to be stored into an array.
 * 
 * @param lines array containing orders.
 * @param run_step run step index
 * @param exit_step exit step index
 */
void play_file (char* lines[], int run_step, int exit_step) {
  for (int i = run_step; i <= exit_step; i++) {
    char* cmd = strsep(&(lines[i]), " ");
    if (!strcmp(cmd, "play")) {
      char* id_string = strsep(&lines[i], " ");
      if (!is_int(id_string)) return;
      int id = atoi(id_string);
      if(get_musician(id) == NULL) return;
      play(id);
    } else if(!strcmp(cmd, "stop")) {
      char* id_string = strsep(&lines[i], " ");
      if (!is_int(id_string)) return;
      int id = atoi(id_string);
      if(get_musician(id) == NULL) return;
      pause_sound(id);
    } else if(!strcmp(cmd, "wait")) {
      char* time_string = strsep(&lines[i], " ");
      if (!is_int(time_string)) return;
      int time = atoi(time_string);
      if (time > 0) {
        sleep(time);
      }
    } else if (!strcmp(cmd, "direction")) {
      char* string_id = strsep(&lines[i], " ");
      if (!is_int(string_id)) return;
      int id = atoi(string_id);
      if(get_musician(id) == NULL);
      char * direction_string = strsep(&lines[i], " ");
      int direction = get_direction(direction_string);
      if (direction == -2) return;
      set_position(id, direction);
    } else if(!strcmp(cmd, "EXIT")) {
      exit_orchestra();
    }
  }
}



/**
 * @brief Read a file and then play the file if the file is correct.
 * 
 * @param file file path
 */
void read_file (char* file) {
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int create_step= -1;
  int run_step = -1;
  int exit_step = -1;
  int n_lines = 0;

  fp = fopen(file, "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
      crop_newline(line);
      int cmp = strcmp(line, "CREATE");
      if (!strcmp(line, "CREATE")) {
        create_step = n_lines;
      } else if (!strcmp(line, "RUN") && create_step > run_step) {
        run_step = n_lines;
      } else if (!strcmp(line, "EXIT") && run_step > exit_step) {
        exit_step = n_lines;
      }
      n_lines++;
  }
  fclose(fp);
  if (line) free(line);
  if (create_step >=0 && run_step >=0 && exit_step >=0) {
    char *lines[n_lines];
    if(check_file(lines, file, n_lines, create_step, run_step, exit_step)) {
      printf("Start playing script %s\n", file);
      play_file(lines, run_step, exit_step);
    } else {
      printf("file error");
    }
  } else {
    printf("Error: structure of file is incorrect. It must contain a CREATE, RUN and EXIT parts.");
  }
}
