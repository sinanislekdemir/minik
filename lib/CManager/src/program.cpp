#include "program.hpp"
#include "error.hpp"
#include "helpers.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

extern error err;

program::program() {
  this->_line_count = -1;
  this->_sub_count = -1;
  this->sub_cursor = 0;
  this->pid = 0;
  this->exit_code = 0;
  this->err = NULL;
  for (uint i = 0; i < 16; i++)
    this->back_history[i] = -1;
  this->_sourcecode_cursor = 0;
  this->subs = NULL;
  this->source = NULL;
}

program::~program() { this->destroy(); }

void program::destroy() {
  for (uint i = 0; i < this->_sub_count; i++) {
    free(this->subs[i].name);
    for (uint j = 0; j < this->subs[i].instruction_count; j++) {
      destroy_command(this->subs[i].instructions[j]);
    }
    delete this->subs[i].instructions;
  }
  delete this->subs;
}

int program::previous_sub() {
  int res = -1;
  for (uint i = HISTORY_LIMIT - 1; i >= 0; i--) {
    if (this->back_history[i] > -1) {
      res = this->back_history[i];
      this->back_history[i] = -1;
      return res;
    }
  }
  return res;
}

void program::set_cmp_flag(uint flag) { this->_cmp_flag = flag; }

int program::previous_instruction(int index) {
  for (uint i = HISTORY_LIMIT - 1; i >= 0; i--) {
    if (this->subs[index].back_history[i] > -1) {
      int res = this->subs[index].back_history[i];
      this->subs[index].back_history[i] = -1;
      return res;
    }
  }
  return -1;
}

int program::find_sub(char *name) {
  for (uint i = 0; i < this->sub_count(); i++) {
    if (strcmp(this->subs[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

int program::append_to_history(int index, int jump_line) {
  for (uint i = 0; i < HISTORY_LIMIT; i++) {
    if (this->back_history[i] == -1) {
      this->back_history[i] = index;
      for (uint j = 0; j < HISTORY_LIMIT; j++) {
        if (this->subs[index].back_history[j] == -1) {
          this->subs[index].back_history[j] = jump_line;
          return 0;
        }
      }
      // TODO: Raise too many jumps
    }
  }
  return -1;
}

int program::step() {
  if (this->sub_cursor >= this->sub_count()) {
    return PROGRAM_END;
  }
  uint sc = this->sub_cursor;
  uint scc = this->subs[sc].cursor;

  // This sub routine has ended, it's time to go back to caller
  if (scc >= this->subs[sc].instruction_count) {
    int ps = this->previous_sub();
    if (ps == -1) {
      return PROGRAM_END;
    }
    sc = ps;
    scc = this->previous_instruction(sc);
    this->sub_cursor = sc;
    this->subs[sc].cursor = scc;
    if (scc == -1) {
      return PROGRAM_END;
    }
    return RUNNING;
  }

  int result = run(&this->subs[sc].instructions[scc], this);
  if (result == -1) {
    // TODO: Raise instruction error
    return PROGRAM_END;
  }
  if (sc == this->sub_cursor && scc == this->subs[sc].cursor) {
    this->subs[sc].cursor += 1;
  }
  return RUNNING;
}

int program::line_count() {
  if (this->_line_count > -1) {
    return this->_line_count;
  }
  this->_line_count = argc(this->source, '\n') + 1;
  return this->_line_count;
}

int program::sub_count() {
  int count = 0;
  if (this->_sub_count > -1) {
    return this->_sub_count;
  }

  for (uint i = 0; i < this->line_count(); i++) {
    char *buffer = extract(this->source, '\n', i, MAX_LINE_LENGTH);
    if (buffer == NULL) {
      break;
    }

    if (strlen(buffer) < 3 || buffer[0] == '#') {
      free(buffer);
      continue;
    }

    if (buffer[strlen(buffer) - 1] == ':') {
      count += 1;
    }

    free(buffer);
  }
  this->_sub_count = count;
  return count;
}

int program::sub_instruction_count(int index) {
  int count = 0;
  int sub_index = 0;
  bool begin = false;
  if (this->_sub_count <= index) {
    return -1;
  }
  for (uint i = 0; i < this->line_count(); i++) {
    char *buffer = extract(this->source, '\n', i, MAX_LINE_LENGTH);
    if (buffer == NULL) {
      break;
    }
    if (strlen(buffer) < 3 || buffer[0] == '#') {
      free(buffer);
      continue;
    }
    if (buffer[strlen(buffer) - 1] == ':') {
      if (sub_index == index) {
        begin = true;
        free(buffer);
        continue;
      } else {
        sub_index++;
      }
    }
    if (begin && strcmp(buffer, SUB_END) == 0) {
      free(buffer);
      return count;
    }
    if (begin) {
      count++;
    }
    free(buffer);
  }
  return count;
}

int program::compile_sub(int index) {
  if (index >= this->sub_count()) {
    return -1;
  }
  int instruction_count = this->sub_instruction_count(index);
  this->subs[index].instruction_count = instruction_count;
  this->subs[index].instructions = new command[instruction_count];

  int instruction_number = 0;
  uint p = this->_sourcecode_cursor;
  for (uint i = p; i < this->line_count(); i++) {
    char *buffer = extract(this->source, '\n', i, MAX_LINE_LENGTH);
    this->_sourcecode_cursor = i;
    if (strlen(buffer) < 3 || buffer[0] == '#') {
      free(buffer);
      continue;
    }
    if (buffer[strlen(buffer) - 1] == ':') {
      this->subs[index].name = (char *)malloc(strlen(buffer));
      memset(this->subs[index].name, 0, strlen(buffer));
      strncpy(this->subs[index].name, buffer, strlen(buffer) - 1);
      free(buffer);
      continue;
    }
    if (strcmp(buffer, SUB_END) == 0) {
      free(buffer);
      this->_sourcecode_cursor++;
      break;
    }
    command c = parse((const char *)buffer, this->pid);
    this->subs[index].instructions[instruction_number].argc = c.argc;
    this->subs[index].instructions[instruction_number].args = c.args;
    this->subs[index].instructions[instruction_number].command = c.command;
    this->subs[index].instructions[instruction_number].pid = this->pid;
    instruction_number++;
    free(buffer);
  }
  this->subs[index].instruction_count = instruction_number;
  for (uint i = 0; i < HISTORY_LIMIT; i++) {
    this->subs[index].back_history[i] = -1;
  }
  this->subs[index].cursor = 0;
  return 0;
}

int program::compile() {
  if (this->sub_count() == 0) {
    return -1;
  }
  for (uint i = 0; i < HISTORY_LIMIT; i++) {
    this->back_history[i] = -1;
  }
  int sub_count = this->sub_count();
  this->subs = new sub[sub_count];

  this->sub_cursor = 0;
  this->_sourcecode_cursor = 0;
  for (uint i = 0; i < sub_count; i++) {
    this->compile_sub(i);
  }
  this->sub_cursor = this->find_sub((char *)MAIN);
  this->subs[this->sub_cursor].cursor = 0;
  free(this->source);
  return 0;
}
