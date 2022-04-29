#include <stdbool.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Check if a string is an integer
 * 
 * @param string string to check
 * @return true if string contains an integer, false if not
 */
bool is_int(char* string) {
  char * endPtr;
  long value = strtol( string, &endPtr, 10 ); 
  return endPtr != string;
}


/**
 * @brief Crop the newline character in a string
 * 
 * @param string string to crop
 */
void crop_newline(char* string) {
  size_t ln = strlen(string) - 1;
  if (*string && string[ln] == '\n') {
    string[ln] = '\0'; // replace the newline character by the termination character
  }
}
