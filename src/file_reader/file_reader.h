#ifndef FILE_READER_H
#define FILE_READER_H

bool check_line (char* line, int step);
bool check_file (char* array[], char* file, int nb_lines, int create_step, int run_step, int exit_step);
void play_file (char* lines[], int run_step, int exit_step);
void read_file (char* file);
bool is_valid_cmd (char* line);
#endif