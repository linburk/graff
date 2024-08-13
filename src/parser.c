#include "parser.h"
#include "ctype.h"
#include "stdlib.h"

const unsigned operator_priority[OPERATORS_COUNT] = {
    /* NOT_AN_OPERATOR */ 0,
    /* PLUS */ 10,
    /* MINUS */ 40,
    /* MULTIPLY */ 20,
    /* DIVIDE */ 20,
    /* EXP */ 30,
    /* SIN */ 40,
    /* COS */ 40,
    /* TAN */ 40,
    /* COT */ 40,
    /* ASIN */ 40,
    /* ACOS */ 40,
    /* ATAN */ 40,
    /* ACOT */ 40,
    /* LOG */ 40,
    /* DER */ 40,
};

int is_left_associative(enum operator_id id) {
  if (id == PLUS || id == MULTIPLY || id == DIVIDE | id == EXP)
    return 1;
  return 0;
}

int is_function(enum operator_id id) {
  if (id == SIN || id == COS || id == TAN || id == COT || id == LOG ||
      id == ASIN || id == ACOS || id == ATAN || id == ACOT)
    return 1;
  return 0;
}

int scan_number(const char *expr, unsigned long expr_len, unsigned *i,
                char *buffer, unsigned *buffer_size) {
  for (char WAS_DOT = 0;
       *i < expr_len && (expr[*i] == '.' || isdigit(expr[*i])); ++(*i)) {
    buffer[(*buffer_size)++] = expr[*i];
    if (expr[*i] == '.') {
      if (WAS_DOT) {
        return -1;
      }
      WAS_DOT = 1;
    }
  }
  return 0;
}
struct token scan_next_token(const char *expr, unsigned long expr_len,
                             unsigned *i) {
  struct token result;
  char buffer[BASE_TOKEN_DATALEN];
  unsigned buffer_size = 0;
  result.id = ERROR;
  result.op = NOT_AN_OPERATOR;
  result.data = NULL;
  result.data_len = 0;
  for (; *i < expr_len && (iscntrl(expr[*i]) || isspace(expr[*i])); ++(*i))
    ; // skip minor chars
  if (isdigit(expr[*i]) || expr[*i] == '.') {
    int err = scan_number(expr, expr_len, i, buffer, &buffer_size);
    if (err == -1) {
      return result;
    }
    result.id = NUMBER;
    result.data_len = buffer_size;
    result.data = (char *)(calloc(BASE_TOKEN_DATALEN, sizeof(char)));
    for (unsigned j = 0; j < buffer_size; ++j) {
      result.data[j] = buffer[j];
    }
    return result;
  }
  if (expr[*i] == '(' || expr[*i] == ')') {
    if (expr[*i] == '(')
      result.id = L_PARENTHESIS;
    if (expr[*i] == ')')
      result.id = R_PARENTHESIS;
    ++(*i);
    return result;
  }
  if (expr[*i] == 'x') {
    result.id = VARIABLE;
    ++(*i);
    return result;
  }
  if (expr[*i] == '+') {
    result.id = OPERATOR;
    result.op = PLUS;
    ++(*i);
    return result;
  }
  if (expr[*i] == '-') {
    result.id = OPERATOR;
    result.op = MINUS;
    ++(*i);
    return result;
  }
  if (expr[*i] == '*') {
    result.id = OPERATOR;
    result.op = MULTIPLY;
    ++(*i);
    return result;
  }
  if (expr[*i] == '/') {
    result.id = OPERATOR;
    result.op = DIVIDE;
    ++(*i);
    return result;
  }
  if (expr[*i] == '^') {
    result.id = OPERATOR;
    result.op = EXP;
    ++(*i);
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'S' && expr[*i + 1] == 'I' &&
      expr[*i + 2] == 'N') {
    result.id = OPERATOR;
    result.op = SIN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'C' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'S') {
    result.id = OPERATOR;
    result.op = COS;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'T' && expr[*i + 1] == 'A' &&
      expr[*i + 2] == 'N') {
    result.id = OPERATOR;
    result.op = TAN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'C' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'T') {
    result.id = OPERATOR;
    result.op = COT;
    *i += 3;
    return result;
  }
  if (*i + 4 < expr_len && expr[*i] == 'L' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'G' && expr[*i + 3] == '_') {
    *i += 4;
    if (expr[*i] == 'x') {
      buffer[buffer_size++] = 'x';
      ++(*i);
    } else {
      int err = scan_number(expr, expr_len, i, buffer, &buffer_size);
      if (err == -1) {
        return result;
      }
    }
    result.id = OPERATOR;
    result.op = LOG;
    result.data_len = buffer_size;
    result.data = (char *)(calloc(BASE_TOKEN_DATALEN, sizeof(char)));
    for (unsigned j = 0; j < buffer_size; ++j) {
      result.data[j] = buffer[j];
    }
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'C' &&
      expr[*i + 2] == 'O' && expr[*i + 3] == 'S') {
    result.id = OPERATOR;
    result.op = ACOS;
    *i += 3;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'S' &&
      expr[*i + 2] == 'I' && expr[*i + 3] == 'N') {
    result.id = OPERATOR;
    result.op = ASIN;
    *i += 3;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'T' &&
      expr[*i + 2] == 'A' && expr[*i + 3] == 'N') {
    result.id = OPERATOR;
    result.op = ATAN;
    *i += 3;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'C' &&
      expr[*i + 2] == 'O' && expr[*i + 3] == 'T') {
    result.id = OPERATOR;
    result.op = ACOT;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'D' && expr[*i + 1] == 'E' &&
      expr[*i + 2] == 'R') {
    result.id = OPERATOR;
    result.op = DER;
    *i += 3;
    return result;
  }

  return result;
}

struct token *tokenize(const char *expr, unsigned long expr_len,
                       unsigned long *size) {
  struct token *result =
      (struct token *)(calloc(BASE_TOKEN_LENGTH, sizeof(struct token)));
  for (unsigned i = 0; i < expr_len;) {
    struct token next_token = scan_next_token(expr, expr_len, &i);
    if (next_token.id == ERROR) {
      free(result);
      return NULL;
    }
    result[(*size)++] = next_token;
  }
  return result;
}

struct token *reverse_polish_notation(struct token *input, unsigned long in_len,
                                      unsigned long *out_len) {
  struct token *stack = (struct token *)(calloc(in_len, sizeof(struct token)));
  unsigned stack_size = 0;
  struct token *output = (struct token *)(calloc(in_len, sizeof(struct token)));
  for (unsigned i = 0; i < in_len; ++i) {
    switch (input[i].id) {
    case VARIABLE:
    case NUMBER:
      output[(*out_len)++] = input[i];
      break;
    case OPERATOR:
      while (stack_size && stack[stack_size - 1].id == OPERATOR &&
             (operator_priority[stack[stack_size - 1].op] >=
                  operator_priority[input[i].op] ||
              is_left_associative(stack[stack_size - 1].op) &&
                  operator_priority[stack[stack_size - 1].op] ==
                      operator_priority[input[i].op])) {
        output[(*out_len)++] = stack[--stack_size];
      }
      stack[stack_size++] = input[i];
      break;
    case L_PARENTHESIS:
      stack[stack_size++] = input[i];
      break;
    case R_PARENTHESIS:
      if (stack_size == 0) {
        free(stack);
        free(output);
        return NULL;
      }
      while (stack[stack_size - 1].id != L_PARENTHESIS) {
        output[(*out_len)++] = stack[--stack_size];
        if (stack_size == 0) {
          free(stack);
          free(output);
          return NULL;
        }
      }
      --stack_size;
      break;
    case ERROR:
    default:
      free(stack);
      free(output);
      return NULL;
      break;
    }
  }
  while (stack_size) {
    if (stack[stack_size - 1].id == L_PARENTHESIS) {
      free(stack);
      free(output);
      return NULL;
    }
    output[(*out_len)++] = stack[--stack_size];
  }
  free(stack);
  return output;
}

void free_token(struct token tok) { free(tok.data); }
