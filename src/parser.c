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
    *i += 1;
    return result;
  }
  if (expr[*i] == ',') {
    result.id = SEPARATOR;
    *i += 1;
    return result;
  }
  if (expr[*i] == 'x') {
    result.id = VARIABLE;
    *i += 1;
    return result;
  }
  if (expr[*i] == '+') {
    result.id = FUNCTION;
    result.op = ADD;
    *i += 1;
    return result;
  }
  if (expr[*i] == '-') {
    result.id = FUNCTION;
    result.op = SUB;
    *i += 1;
    return result;
  }
  if (expr[*i] == '*') {
    result.id = FUNCTION;
    result.op = MULT;
    *i += 1;
    return result;
  }
  if (expr[*i] == '/') {
    result.id = FUNCTION;
    result.op = DIV;
    *i += 1;
    return result;
  }
  if (expr[*i] == '^') {
    result.id = FUNCTION;
    result.op = EXP;
    *i += 1;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 's' && expr[*i + 1] == 'i' &&
      expr[*i + 2] == 'n') {
    result.id = FUNCTION;
    result.op = SIN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'c' && expr[*i + 1] == 'o' &&
      expr[*i + 2] == 's') {
    result.id = FUNCTION;
    result.op = COS;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 't' && expr[*i + 1] == 'a' &&
      expr[*i + 2] == 'n') {
    result.id = FUNCTION;
    result.op = TAN;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'c' && expr[*i + 1] == 'o' &&
      expr[*i + 2] == 't') {
    result.id = FUNCTION;
    result.op = COT;
    *i += 3;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'l' && expr[*i + 1] == 'o' &&
      expr[*i + 2] == 'g') {
    result.id = FUNCTION;
    result.op = LOG;
    *i += 3;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'a' && expr[*i + 1] == 'c' &&
      expr[*i + 2] == 'o' && expr[*i + 3] == 's') {
    result.id = FUNCTION;
    result.op = ACOS;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'a' && expr[*i + 1] == 's' &&
      expr[*i + 2] == 'i' && expr[*i + 3] == 'n') {
    result.id = FUNCTION;
    result.op = ASIN;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'a' && expr[*i + 1] == 't' &&
      expr[*i + 2] == 'a' && expr[*i + 3] == 'n') {
    result.id = FUNCTION;
    result.op = ATAN;
    *i += 4;
    return result;
  }
  if (*i + 3 < expr_len && expr[*i] == 'a' && expr[*i + 1] == 'c' &&
      expr[*i + 2] == 'o' && expr[*i + 3] == 't') {
    result.id = FUNCTION;
    result.op = ACOT;
    *i += 4;
    return result;
  }
  if (*i + 2 < expr_len && expr[*i] == 'd' && expr[*i + 1] == 'e' &&
      expr[*i + 2] == 'r') {
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
