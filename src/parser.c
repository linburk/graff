#include "parser.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "token.h"

struct token scan_next_token(const char *expr, unsigned expr_len, unsigned *i) {
  struct token result;
  result.id = ERROR;
  result.op = NOT_AN_FUNCTION;
  for (; *i < expr_len && isspace(expr[*i]); ++(*i))
    ; // skip minor chars
  if (*i >= expr_len) {
    result.id = EMPTY;
    return result;
  }
  if (isdigit(expr[*i])) {
    result.id = NUMBER;
    unsigned c_scanned = 0;
    sscanf(&expr[*i], "%Lf%n", &result.val, &c_scanned);
    *i += c_scanned;
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
  if (expr[*i] == ',') {
    result.id = SEPARATOR;
    ++(*i);
    return result;
  }
  if (expr[*i] == 'x') {
    result.id = VARIABLE;
    ++(*i);
    return result;
  }
  if (*i + 2 && expr[*i] == 'A' && expr[*i + 1] == 'D' && expr[*i + 2] == 'D') {
    result.id = FUNCTION;
    result.op = ADD;
    *i += 3;
    return result;
  }
  if (*i + 2 && expr[*i] == 'S' && expr[*i + 1] == 'U' && expr[*i + 2] == 'B') {
    result.id = FUNCTION;
    result.op = SUB;
    *i += 3;
    return result;
  }
  if (*i + 3 && expr[*i] == 'M' && expr[*i + 1] == 'U' && expr[*i + 2] == 'L' &&
      expr[*i + 3] == 'T') {
    result.id = FUNCTION;
    result.op = MULT;
    *i += 4;
    return result;
  }
  if (*i + 2 && expr[*i] == 'D' && expr[*i + 1] == 'I' && expr[*i + 2] == 'V') {
    result.id = FUNCTION;
    result.op = DIV;
    *i += 3;
    return result;
  }
  if (*i + 2 && expr[*i] == 'E' && expr[*i + 1] == 'X' && expr[*i + 2] == 'P') {
    result.id = FUNCTION;
    result.op = EXP;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'S' && expr[*i + 1] == 'I' &&
      expr[*i + 2] == 'N') {
    result.id = FUNCTION;
    result.op = SIN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'C' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'S') {
    result.id = FUNCTION;
    result.op = COS;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'T' && expr[*i + 1] == 'A' &&
      expr[*i + 2] == 'N') {
    result.id = FUNCTION;
    result.op = TAN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'C' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'T') {
    result.id = FUNCTION;
    result.op = COT;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'L' && expr[*i + 1] == 'O' &&
      expr[*i + 2] == 'G') {
    result.id = FUNCTION;
    result.op = LOG;
    *i += 3;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'C' &&
      expr[*i + 2] == 'O' && expr[*i + 3] == 'S') {
    result.id = FUNCTION;
    result.op = ACOS;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'S' &&
      expr[*i + 2] == 'I' && expr[*i + 3] == 'N') {
    result.id = FUNCTION;
    result.op = ASIN;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'T' &&
      expr[*i + 2] == 'A' && expr[*i + 3] == 'N') {
    result.id = FUNCTION;
    result.op = ATAN;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'A' && expr[*i + 1] == 'C' &&
      expr[*i + 2] == 'O' && expr[*i + 3] == 'T') {
    result.id = FUNCTION;
    result.op = ACOT;
    *i += 4;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'D' && expr[*i + 1] == 'E' &&
      expr[*i + 2] == 'R') {
    result.id = FUNCTION;
    result.op = DER;
    *i += 3;
    return result;
  }
  return result;
}

struct token *tokenize(const char *expr, unsigned expr_len, unsigned *size) {
  struct token *result =
      (struct token *)(calloc(MAX_TOKEN_LENGTH, sizeof(struct token)));
  if (result == NULL) {
    fprintf(stderr, "CALLOC ERROR\n");
    abort();
  }
  for (unsigned i = 0; i < expr_len;) {
    struct token next_token = scan_next_token(expr, expr_len, &i);
    if (next_token.id == EMPTY)
      continue;
    if (next_token.id == ERROR) {
      free(result);
      return NULL;
    }
    result[(*size)++] = next_token;
  }
  return result;
}

struct token *reverse_polish_notation(struct token *input, unsigned in_len,
                                      unsigned *out_len) {
  struct token *stack = (struct token *)(calloc(in_len, sizeof(struct token)));
  if (stack == NULL) {
    fprintf(stderr, "CALLOC ERROR\n");
    abort();
  }
  unsigned stack_size = 0;
  struct token *output = (struct token *)(calloc(in_len, sizeof(struct token)));
  if (output == NULL) {
    fprintf(stderr, "CALLOC ERROR\n");
    abort();
  }
  for (unsigned i = 0; i < in_len; ++i) {
    switch (input[i].id) {
    case VARIABLE:
    case NUMBER:
      output[(*out_len)++] = input[i];
      break;
    case FUNCTION:
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
      while (stack_size && stack[stack_size - 1].id != L_PARENTHESIS) {
        output[(*out_len)++] = stack[--stack_size];
        if (stack_size == 0) {
          free(stack);
          free(output);
          return NULL;
        }
      }
      --stack_size;
      if (stack_size && stack[stack_size - 1].id == FUNCTION) {
        output[(*out_len)++] = stack[--stack_size];
      }
      break;
    case SEPARATOR:
      while (stack_size && stack[stack_size - 1].id != L_PARENTHESIS) {
        output[(*out_len)++] = stack[--stack_size];
      }
      if (stack_size == 0 || stack[stack_size - 1].id != L_PARENTHESIS) {
        free(stack);
        free(output);
        return NULL;
      }
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
