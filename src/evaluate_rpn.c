#include "evaluate_rpn.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

long double evaluate(long double x, struct token *rpn_expr,
                     unsigned long expr_len) {
  long double *stack = (long double *)(calloc(expr_len, sizeof(long double)));
  unsigned stack_size = 0;
  for (unsigned i = 0; i < expr_len; ++i) {
    switch (rpn_expr[i].id) {
    case NUMBER:
      sscanf(rpn_expr[i].data, "%Lf", &stack[stack_size++]);
      break;
    case VARIABLE:
      stack[stack_size++] = x;
      break;
    case OPERATOR:
      switch (rpn_expr[i].op) {
        long double first, second;
      case PLUS:
        if (stack_size < 2) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = first + second;
        break;
      case MINUS:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        stack[stack_size++] = -first;
        break;
      case MULTIPLY:
        if (stack_size < 2) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = first * second;
        break;
      case DIVIDE:
        if (stack_size < 2) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = first != 0 ? second / first : 0;
        break;
      case EXP:
        if (stack_size < 2) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = second > 0 ? pow(second, first) : 0;
        break;
      case SIN:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        stack[stack_size++] = sin(first);
        break;
      case COS:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        stack[stack_size++] = cos(first);
        break;
      case TAN:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        stack[stack_size++] = cos(first) != 0 ? tan(first) : 0;
        break;
      case COT:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        first = stack[--stack_size];
        stack[stack_size++] = sin(first) != 0 ? 1 / tan(first) : 0;
        break;
      case LOG:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        if (rpn_expr[i].data[0] == 'x') {
          second = x;
        } else {
          sscanf(rpn_expr[i].data, "%Lf", &second);
        }
        first = stack[--stack_size];
        stack[stack_size++] = first > 0 && second > 0 && second != 1
                                  ? log2(first) / log2(second)
                                  : 0;
        break;
      case DER:
        if (stack_size < 1) {
          free(stack);
          return 0;
        }
        const double EPS = 0.125;
        first = stack[--stack_size];
        second = evaluate(x + EPS, rpn_expr, i);
        stack[stack_size++] = (second - first) / EPS;
        break;
      }
    default:
      break;
    }
  }
  long double result = stack[0];
  free(stack);
  return result;
}
