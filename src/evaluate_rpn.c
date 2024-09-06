#include "evaluate_rpn.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
double EPS;

long double evaluate(long double x, struct token *rpn_expr,
                     unsigned long expr_len) {
  long double *stack = (long double *)(calloc(expr_len, sizeof(long double)));
  if (stack == NULL) {
    fprintf(stderr, "CALLOC ERROR\n");
    abort();
  }
  unsigned stack_size = 0;
  long double first, second;
  for (unsigned i = 0; i < expr_len; ++i) {
    switch (rpn_expr[i].id) {
    case NUMBER:
      stack[stack_size++] = rpn_expr[i].val;
      break;
    case VARIABLE:
      stack[stack_size++] = x;
      break;
    case FUNCTION:
      switch (rpn_expr[i].op) {
      case ADD:
        if (stack_size < 2) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = second + first;
        break;
      case SUB:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = second - first;
        break;
      case MULT:
        if (stack_size < 2) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = second * first;
        break;
      case DIV:
        if (stack_size < 2) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = first != 0 ? (second / first) : NAN;
        break;
      case EXP:
        if (stack_size < 2) {
          free(stack);
          return NAN;
        }
        EPS = 1E-7;
        first = stack[--stack_size];
        second = stack[--stack_size];
        int pow_r = round(first);
        if (first + EPS > pow_r && first - EPS < pow_r) {
          stack[stack_size++] = pow(second, pow_r);
        } else {
          stack[stack_size++] = second > 0 ? pow(second, first) : NAN;
        }
        break;
      case SIN:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        stack[stack_size++] = sin(first);
        break;
      case COS:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        stack[stack_size++] = cos(first);
        break;
      case TAN:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        stack[stack_size++] = cos(first) != 0 ? tan(first) : NAN;
        break;
      case COT:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        stack[stack_size++] = sin(first) != 0 ? 1 / tan(first) : NAN;
        break;
      case LOG:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        first = stack[--stack_size];
        second = stack[--stack_size];
        stack[stack_size++] = first > 0 && second > 0 && second != 1
                                  ? log2(first) / log2(second)
                                  : NAN;
        break;
      case DER:
        if (stack_size < 1) {
          free(stack);
          return NAN;
        }
        EPS = 1E-7;
        first = stack[--stack_size];
        second = evaluate(x + EPS, rpn_expr, i);
        stack[stack_size++] = (second - first) / EPS;
        break;
      default:
        break;
      }
    default:
      break;
    }
  }
  long double result =
      stack_size > 0 && stack_size <= expr_len ? stack[stack_size - 1] : NAN;
  free(stack);
  return result;
}
