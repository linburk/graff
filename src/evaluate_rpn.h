#pragma once
#ifndef EVAL_RPN_H
#define EVAL_RPN_H
#include "token.h"
long double evaluate(long double x, struct token *rpn_expr,
                     unsigned long expr_len);

#endif
