#include "constants.hpp"

const char *constants[] = {"OUTPUT", "HIGH", "LOW", "STACK", "MILLIS"};

const char *type_tostr(int type) {
  switch (type) {
  case TYPE_NONE:
    return "NONE";
    break;
  case TYPE_STR:
    return "STRING";
    break;
  case TYPE_NUM:
    return "NUMBER";
    break;
  case TYPE_CONSTANT:
    return "CONSTANT";
    break;
  case TYPE_VARIABLE:
    return "VARIABLE";
    break;
  case TYPE_LABEL:
    return "LABEL";
    break;
  case TYPE_REGISTER:
    return "REGISTER";
    break;
  case TYPE_FILE:
    return "FILE";
    break;
  default:
    return "UNKNOWN";
  }
}
