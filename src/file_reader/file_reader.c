
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
 * @brief Check if line is a valid command line for RUN section
 * 
 * @param line line to check
 * @return true if line is a valid command line, false if not
 */
bool is_valid_cmd (char* line) {
  char * cmd = strsep(&line, " ");
  if (strcmp(cmd, "play") && strcmp(cmd, "stop") && strcmp(cmd, "wait") && strcmp(cmd, "direction")) {
    return false;
  } else if (!strcmp(cmd, "play") || !strcmp(cmd, "stop") || !strcmp(cmd, "wait")) {
    char* time_or_id_string = strsep(&line, " ");
    if (!is_int(time_or_id_string)) return false;
    if (!strcmp(cmd, "stop") || !strcmp(cmd, "play")) {
      int id = atoi(time_or_id_string);
      if(get_musician(id) == NULL) {
        printf("Unknown musician with id = %i\n", id);
        return false;
      }
    }
    return true;
  } else if (!strcmp(cmd, "direction")) {
    char* string_id = strsep(&line, " ");
    if (!is_int(string_id)) return false;
    int id = atoi(string_id);
    if(get_musician(id) == NULL) return false;
    char * direction = strsep(&line, " ");
    if (get_direction(direction) == -2) return false;
    return true;
  }
  return false;
}

/**
 * @brief Check if line is a valid command line
 * 
 * @param line line to check
 * @param step current step, possible value: CREATE_STEP, RUN_STEP, EXIT_STEP
 * @return true if line is a valid command line
 */
bool check_line (char* line, int step) {
  if (!strcmp(line, "")) return true; // return true if the line contains nothing
  if (step == 1) {
    if (!strcmp(line, "CREATE")) return true; // return true if line contains CREATE
    char * string_id = strsep(&line, " "); // get string id
    if (!is_int(string_id)) return false; // if string id is not a string the return false
    int id = atoi(string_id); // id value
    char * instrument = strsep(&line, " "); // get instrument
    if (!is_valid_instrument(instrument)) return false; // if instrument is not a valid one then return false
    char * direction =  strsep(&line, " "); // get direction
    if (get_direction(direction) == -2) return false; // if direction is not a valid one then return false
    create_musician(instrument, id, 0); // create the musician
    load(id); // load his partition
    set_position(id, get_direction(direction)); // set musician position
    return true;
  } else if (step == 2) {
    if (!strcmp(line, "RUN")) return true; 
    if (!is_valid_cmd(line)) return false; // if line is not a valid command, then return false
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
  int current_step = 1; // CREATE_STEP, RUN_STEP or EXIT_STEP
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
        // save the line if it corresponds to a valid command
        array[i] = malloc(100); 
        strcpy(array[i], line);
      } else {
        printf("Error in line %i : %s\n", i, line);
        file_ok = false;
        break;
      }
      i++;
      // update current step
      if (i >= run_step && i < exit_step ) {
        current_step = 2;
      } else if (i >= exit_step) {
        current_step = 3;
      }
  }
  fclose(fp); // close file
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
  // loop over the run section
  for (int i = run_step; i <= exit_step; i++) {
    char* cmd = strsep(&(lines[i]), " ");
    if (!strcmp(cmd, "play")) {
      // play command
      char* id_string = strsep(&lines[i], " ");
      if (!is_int(id_string)) return;
      int id = atoi(id_string);
      // if musician has been found then play his partition, else return
      if(get_musician(id) == NULL) return; 
      play(id);
    } else if(!strcmp(cmd, "stop")) {
      // stop command
      char* id_string = strsep(&lines[i], " ");
      if (!is_int(id_string)) return;
      int id = atoi(id_string);
      // if musician has been found then stop his partition, else return
      if(get_musician(id) == NULL) return;
      pause_sound(id);
    } else if(!strcmp(cmd, "wait")) {
      // wait command
      char* time_string = strsep(&lines[i], " ");
      if (!is_int(time_string)) return;
      int time = atoi(time_string);
      if (time > 0) {
        sleep(time); // sleep during the given time seconds
      }
    } else if (!strcmp(cmd, "direction")) {
      // direction command
      char* string_id = strsep(&lines[i], " ");
      if (!is_int(string_id)) return;
      int id = atoi(string_id);
      // check if the musician exists
      if(get_musician(id) == NULL);
      // get the direction
      char * direction_string = strsep(&lines[i], " ");
      int direction = get_direction(direction_string);
      if (direction == -2) return;
      // if valid position then set the position of the musician
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
      // read each line to find range of create, run and exit sections.
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
    // if sections have been found then check the file.
    char *lines[n_lines];
    if(check_file(lines, file, n_lines, create_step, run_step, exit_step)) {
      // if file is ok, then start playing it.
      printf("Start playing script %s\n", file);
      play_file(lines, run_step, exit_step);
    } else {
      printf("file error");
    }
  } else {
    printf("Error: structure of file is incorrect. It must contain a CREATE, RUN and EXIT parts.");
  }
}
