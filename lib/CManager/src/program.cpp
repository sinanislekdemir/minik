#include "program.hpp"
#include "helpers.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

extern error err;

program::program(int pid) {
  if (pid == 0) {
    pid = 1;
  }
  this->_line_count = -1;
  this->main = NULL;
  this->cursor = NULL;
  this->back_history.item = NULL;
  this->back_history.next = NULL;
  this->_sourcecode_cursor = 0;

  this->pid = pid;
  this->exit_code = 0;
  this->source = NULL;

  this->_sleep = false;
  this->_sleep_start = 0;
  this->_sleep_duration = 0;
}

program::~program() { this->destroy(); }

void _free_instructions(command *head) {
  command *tmp;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    for (unsigned int i = 0; i < tmp->argc; i++) {
      free(tmp->args[i].data);
      free(tmp->args[i].name);
    }
    free(tmp->cmd);
    free(tmp->args);
    free(tmp);
  }
}

void _free_jump_history(command_history *head) {
  command_history *tmp;
  while (head != NULL) {
    tmp = head->next;
    free(head);
    head = tmp;
  }
}

void _free_subs(sub *head) {
  sub *tmp;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp->name);
    _free_jump_history(tmp->back_history.next);
    _free_instructions(tmp->root_instruction);
    free(tmp);
  }
}

void _free_sub_history(sub_history *head) {
  sub_history *tmp;
  while (head != NULL) {
    tmp = head->next;
    free(head);
    head = tmp;
  }
}

void program::destroy() {
  _free_sub_history(this->back_history.next);
  _free_subs(this->main);
  free_program(this->pid);
}

sub *program::previous_sub() {
  sub_history *node = &back_history;
  sub_history *prev = &back_history;

  sub *tmp;
  while (true) {
    if (node->next != NULL) {
      prev = node;
      node = node->next;
      continue;
    }
    if (node->next == NULL) {
      tmp = node->item;
      free(node);
      prev->next = NULL;
      return tmp;
    }
  }
  return NULL; // unreachable code
}

void program::set_cmp_flag(unsigned int flag) { this->_cmp_flag = flag; }

command *program::previous_instruction() {
  if (this->cursor == NULL) {
    return NULL;
  }
  command_history *node = &this->cursor->back_history;
  command_history *prev = &this->cursor->back_history;

  command *tmp;
  while (true) {
    if (node->next != NULL) {
      prev = node;
      node = node->next;
      continue;
    }
    if (node->next == NULL) {
      tmp = node->item;
      free(node);
      prev->next = NULL;
      return tmp;
    }
  }

  return NULL; // unreachable code
}

sub *program::find_sub(char *name) {
  sub *node = this->main;
  while (node != NULL) {
    if (strcmp(node->name, name) == 0) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

void program::append_to_history(sub *cursor, command *instruction) {
  sub_history *node = &this->back_history;
  sub_history *n = (sub_history *)malloc(sizeof(sub_history));

  n->item = cursor;
  n->next = NULL;
  while (true) {
    if (node->next == NULL) {
      node->next = n;
      break;
    }
    node = node->next;
  }

  command_history *c_node = &cursor->back_history;
  command_history *c_n = (command_history *)malloc(sizeof(command_history));
  c_n->item = instruction;
  c_n->next = NULL;

  while (true) {
    if (c_node->next == NULL) {
      c_node->next = c_n;
      break;
    }
    c_node = c_node->next;
  }
}

int program::step() {
  if (this->_sleep) {
    unsigned long now = millis();
    if (now - this->_sleep_start >= this->_sleep_duration) {
      this->_sleep = false;
      this->_sleep_start = 0;
      this->_sleep_duration = 0;
    } else {
      return RUNNING;
    }
  }

  if (this->cursor == NULL) {
    return PROGRAM_END;
  }

  if (this->cursor->cursor == NULL) {
    this->cursor = this->previous_sub();
    return RUNNING;
  }

  int result = run(this->cursor->cursor, this);
  if (result == -1) {
    // TODO: raise
    Serial.println(this->cursor->cursor->cmd);
    return PROGRAM_END;
  }
  if (result == 1) {
    // Cursor is explicitly set by the instruction
    return RUNNING;
  }

  if (this->cursor->cursor != NULL && this->cursor->cursor->next == NULL) {
    // end of the instructions, head back to caller;
    this->cursor = this->previous_sub();
    if (this->cursor == NULL) {
      return PROGRAM_END;
    }
  }
  if (this->cursor->cursor == NULL) {
    this->cursor->cursor = this->cursor->root_instruction;
  } else {
    this->cursor->cursor = this->cursor->cursor->next;
  }
  return RUNNING;
}

unsigned int program::line_count() {
  if (this->_line_count >= 0) {
    return this->_line_count;
  }
  this->_line_count = argc(this->source, '\n');
  return this->_line_count;
}

sub *program::compile_next_sub() {
  unsigned int p = this->_sourcecode_cursor;
  if (p == this->line_count() - 1) {
    return NULL;
  }
  sub *result = (sub *)malloc(sizeof(sub));
  result->cursor = NULL;
  command *prev = NULL;

  for (unsigned int i = p; i < this->line_count(); i++) {
    char *buffer = extract(this->source, '\n', i, MAX_LINE_LENGTH);

    this->_sourcecode_cursor = i;
    if (strlen(buffer) < 3 || buffer[0] == '#') {
      free(buffer);
      continue;
    }
    if (buffer[strlen(buffer) - 1] == ':') {
      result->name = (char *)malloc(strlen(buffer));
      memset(result->name, 0, strlen(buffer));
      strncpy(result->name, buffer, strlen(buffer) - 1);
      free(buffer);
      continue;
    }
    if (strcmp(buffer, SUB_END) == 0) {
      free(buffer);
      this->_sourcecode_cursor++;
      break;
    }
    command *c = parse((const char *)buffer, this->pid);
    if (prev == NULL) {
      prev = c;
      result->root_instruction = c;
    } else {
      prev->next = c;
      prev = c;
    }
  }
  result->back_history.item = NULL;
  result->back_history.next = NULL;
  result->cursor = result->root_instruction;
  result->next = NULL;

  return result;
}

int program::compile() {
  this->_sourcecode_cursor = 0;

  sub *prev = NULL;
  while (this->_sourcecode_cursor < this->line_count()) {
    sub *s = this->compile_next_sub();
    if (s == NULL) {
      break;
    }
    if (prev == NULL) {
      prev = s;
      this->main = s;
      this->cursor = s;
    } else {
      prev->next = s;
      prev = s;
    }
  }

  free(this->source);
  return 0;
}
