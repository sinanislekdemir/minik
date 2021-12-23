#include "tasks.hpp"

task tasks = {NULL, 0, 0, 0, 0, NULL};
daemon_task daemons = {NULL, NULL, 0, 0, 0};

void add_task(program *_p, unsigned int priority) {
  if (tasks.prog == NULL) {
    tasks.prog = _p;
    tasks.priority = priority;
    tasks.start_time = millis();
    tasks.end_time = 0;
    tasks.exit_code = RUNNING;
    tasks.next = NULL;
    return;
  }
  task *t = (task *)malloc(sizeof(task));
  t->prog = _p;
  t->priority = priority;
  t->start_time = millis();
  t->end_time = 0;
  t->exit_code = RUNNING;
  t->next = NULL;

  task *head = &tasks;
  while (true) {
    if (head->next == NULL) {
      head->next = t;
      break;
    }
    head = head->next;
  }
}

bool step_tasks() {
  daemon_task *dhead = &daemons;
  while (dhead != NULL) {
    if (dhead->exit_code != RUNNING) {
      dhead = dhead->next;
      continue;
    }
    int status = dhead->task->process();
    if (status != RUNNING) {
      dhead->exit_code = status;
      dhead->end_time = millis();
    }
    dhead = dhead->next;
  }

  task *head = &tasks;
  while (head != NULL) {
    if (head->exit_code != RUNNING) {
      head = head->next;
      continue;
    }
    for (unsigned int i = 0; i < head->priority; i++) {
      int status = head->prog->step();
      if (status != RUNNING) {
        head->exit_code = status;
        head->end_time = millis();
      }
    }
    head = head->next;
  }
  return true;
}
