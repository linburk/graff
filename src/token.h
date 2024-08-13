#pragma once
#ifndef TOKEN_H
#define TOKEN_H
enum token_id {
  ERROR,
  VARIABLE, // only x
  OPERATOR,
  NUMBER,
  L_PARENTHESIS,
  R_PARENTHESIS,
  TOKEN_TYPES_COUNT,
};

enum operator_id {
  NOT_AN_OPERATOR,
  PLUS,
  MINUS, // always right-associavity
  MULTIPLY,
  DIVIDE,
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
  OPERATORS_COUNT,
};

struct token {
  enum token_id id;
  enum operator_id op;
  char *data;
  unsigned long data_len;
};
#endif
