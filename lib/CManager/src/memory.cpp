/**
   We are reserving some capacity for the variables
   array. This way, we will do less memory swaps
   and memory copies.
 */
#include "memory.hpp"
#include <Arduino.h>

unsigned int variable_count = 0;
unsigned int capacity = VARIABLE_CAPACITY_STEP;

variable *variables = new variable[capacity];

variable *find_variable(const char *name, unsigned int pid) {
  for (unsigned int i = 0; i < variable_count; i++) {
    if (strcmp(variables[i].name, name) == 0 && pid == variables[i].pid) {
      if (variables[i].type == TYPE_VARIABLE) {
        return find_variable(variables[i].data, pid);
      }
      return &variables[i];
    }
  }
  return NULL;
}

void free_variable(const char *name, unsigned int pid) {
  for (unsigned int i = 0; i < variable_count; i++) {
    if (strcmp(variables[i].name, name) == 0 && pid == variables[i].pid) {
      free(variables[i].data);
      free(variables[i].name);
      variables[i].datasize = 0;
      variables[i].pid = 0;
      variables[i].deleted = true;
    }
  }
}

void expand_capacity() {
  capacity += VARIABLE_CAPACITY_STEP;
  variable *temp = new variable[capacity];
  for (unsigned int i = 0; i < variable_count; i++) {
    copy_variable(&temp[i], &variables[i]);
    free(variables[i].data);
    free(variables[i].name);
  }
  free(variables);
  variables = temp;
}

void new_variable(variable *v) {
  for (unsigned int i = 0; i < variable_count; i++) {
    if (variables[i].deleted) {
      copy_variable(&variables[i], v);
      return;
    }
  }
  copy_variable(&variables[variable_count], v);
  variable_count++;
  if (variable_count == capacity) {
    expand_capacity();
  }
}

void defrag_variables() {}

void copy_variable(variable *dest, variable *src) {
  dest->data = (char *)malloc(src->datasize);
  memcpy(dest->data, src->data, src->datasize);
  dest->datasize = src->datasize;
  dest->name = (char *)malloc(strlen(src->name) + 1);
  memset(dest->name, 0, strlen(src->name) + 1);
  memcpy(dest->name, src->name, strlen(src->name));
  dest->pid = src->pid;
  dest->type = src->type;
  dest->deleted = src->deleted;
}
