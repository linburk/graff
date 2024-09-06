#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H
#include "../token.h"

#define MAX_EXPR_COUNT 16
#define MAX_EXPR_LENGTH 256
#define MAX_HEIGHT 5000
#define MAX_WIDTH 5000

#define COMMON_BASE_X -3.6
#define COMMON_BASE_Y -2.1
#define COMMON_DIFF_X 0.005
#define COMMON_DIFF_Y 0.005

struct graph_params {
  int width;
  int height;
  unsigned abs_freq;
  unsigned ord_freq;
  long double base_x;
  long double base_y;
  long double diff_x;
  long double diff_y;
  struct token *rpn_expr[MAX_EXPR_COUNT];
  unsigned rpn_expr_len[MAX_EXPR_COUNT];
  unsigned graph_count;
};

void tui(struct graph_params);

void gui(struct graph_params);

#endif