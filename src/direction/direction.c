#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include "./direction.h"

char * available_directions[NUMBER_OF_DIRECTIONS] = { 
  "CENTER", 
  "N", 
  "NE", 
  "E", 
  "SE", 
  "S", 
  "SW",
  "W", 
  "NW" 
};

/**
 * @brief Get the direction value from string
 * 
 * @param direction string direction
 * @return direction value, -2 if not a valid string direction
 */
int get_direction(char* direction) {
  int i;
  if (!strcmp(direction, "CENTER")) {
    return CENTER;
  } else if (!strcmp(direction, "N")) {
    return N;
  } else if (!strcmp(direction, "NE")) {
    return NE ;
  } else if (!strcmp(direction, "E")) {
    return E ;
  } else if (!strcmp(direction, "SE")) {
    return SE;
  } else if (!strcmp(direction, "S")) {
    return S;
  } else if (!strcmp(direction, "SW")) {
    return SW;
  } else if (!strcmp(direction, "W")) {
    return W;
  } else if (!strcmp(direction, "NW")) {
    return NW;
  }
  return -2;
}