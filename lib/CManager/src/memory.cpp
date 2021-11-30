/**
   We are storing variables at global in a linked list.
   With linked list, we are avoiding memory swaps while
   extending arrays. Keep the memory requirements at minimum.
   Also all memory is shared and accessible.
 */
#include "memory.hpp"
#include <Arduino.h>

variable root_variable = {(char *)"", (char *)"", 0, 0, 0, false, NULL};

variable *find_variable(const char *name, unsigned int pid) {
  variable *node = &root_variable;
  while (true) {
    if (node->name != NULL && strcmp(node->name, name) == 0 &&
        pid == node->pid) {
      if (node->type == TYPE_VARIABLE && node->data != NULL) {
        return find_variable(node->data, pid);
      }
      return node;
    }
    node = node->next;
    if (node == NULL) {
      break;
    }
  }

  return NULL;
}

void mem_dump() {
  variable *node = &root_variable;
  while (node != NULL) {
    Serial.print("N: ");
    Serial.print(node->name);
    Serial.print(" T: ");
    Serial.print(node->type);
    Serial.print(" S: ");
    Serial.print(node->datasize);
    Serial.print("bytes PID: ");
    Serial.println(node->pid);
    node = node->next;
  }
}

void free_variable(const char *name, unsigned int pid) {
  /**
     Free variables allocated data and name,
     and mark the entry as deleted for future use.
     But this doesn't actually remove the item totally from the linked list.
     You need some sort of defrag.
   */
  variable *node = &root_variable;
  while (true) {
    if (node->name != NULL && strcmp(node->name, name) == 0 &&
        pid == node->pid) {
      free(node->data);
      free(node->name);
      node->datasize = 0;
      node->pid = 0;
      node->deleted = true;
      break;
    }
    node = node->next;
    if (node == NULL) {
      break;
    }
  }
}

void new_variable(variable *v) {
  /**
     Add the variable to the linked list.
   */
  variable *node = &root_variable;
  while (true) {
    if (node->next == NULL) {
      node->next = v;
      break;
    }
    node = node->next;
    if (node == NULL) {
      // TODO: Raise errro
      break;
    }
  }
}

void defrag_variables() {}
