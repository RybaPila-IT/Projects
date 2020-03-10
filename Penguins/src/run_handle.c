#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "run_handle.h"
#include "err.h"

#define PLACEMENT 1
#define MOVEMENT  2
#define ERR      -1
#define TRUE      1
#define FALSE     0

#define PHASE_LEN 6
#define PENGU_LEN 9


const char* phase = "phase=";
const char* placement = "placement";
const char* movement = "movement";
const char* penguins = "penguins=";

static int get_integer(const char* penguins, int beginning) {

  int i = beginning;
  int sum = 0;

  while(penguins[i] != '\0' && isdigit(penguins[i])) {

      sum *= 10;
      sum += penguins[i] - '0';
      i++;

  }

  if(sum == 0 || penguins[i] != '\0')
    return ERR;

  return sum;
}

static int string_includes(const char* longer, const char* shorter, int beginning) {

  int i = beginning;
  int j = 0;

  while(shorter[j] != '\0' && longer[i] != '\0' && shorter[j] == longer[i]) {
    i++;
    j++;
  }

  if(shorter[j] != '\0')
    return FALSE;

  return TRUE;
}


static int get_phase(const char* program_arg) {

  if(!string_includes(program_arg, phase, 0))
    return ERR;

  if(string_includes(program_arg, movement, PHASE_LEN))
    return MOVEMENT;

  if(string_includes(program_arg, placement, PHASE_LEN))
    return PLACEMENT;

  return ERR;
}


static int get_penguins(const char* program_arg) {

  if(!string_includes(program_arg, penguins, 0))
    return ERR;

  return get_integer(program_arg, PENGU_LEN);
}



static int get_all(int argc, const char* argv[], int* penguins) {

  int phase_read;

  if(argc != 4 && argc != 5)
    return ERR;

  if(argc == 4)
    phase_read  = get_phase(argv[1]) == MOVEMENT ? MOVEMENT : ERR;

  else {
    
    phase_read = get_phase(argv[1]) == PLACEMENT ? PLACEMENT : ERR;
    (*penguins) = get_penguins(argv[2]);
  }

  return phase_read;
}

int decide_phase(int argc, const char* argv[], int* penguins) {

  int phase = get_all(argc, argv, penguins);

  if( (*penguins) == ERR || phase == ERR)
    input_args_err();

  return phase;
}
