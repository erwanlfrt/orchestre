#ifndef DIRECTION_H_
#define DIRECTION_H_

#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#define CENTER -1

#define NUMBER_OF_DIRECTIONS 9
char* available_directions[NUMBER_OF_DIRECTIONS];

int get_direction (char* direction);

#endif