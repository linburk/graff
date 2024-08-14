#pragma once
#ifndef TOKEN_H
#define TOKEN_H
enum token_id {
  ERROR,
  EMPTY,
  VARIABLE, // only x
  FUNCTION,
  NUMBER,
  L_PARENTHESIS,
  R_PARENTHESIS,
  SEPARATOR,
  TOKEN_TYPES_COUNT,
};

enum function_id {
  NOT_AN_FUNCTION,
  ADD,
  SUB, // always right-associavity
  MULT,
  DIV,
  EXP,
  SIN,
  COS,
  TAN,
  COT,
  ASIN,
  ACOS,
  ATAN,
  ACOT,
  LOG,
  DER,
  FUNCTIONS_COUNT,
};

struct token {
  enum token_id id;
  enum function_id op;
  long double val;
};
#endif
