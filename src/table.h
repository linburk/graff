#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include "token.h"
#include "wchar.h"

#define EXTRA_LINES 6
#define DEFAULT_BASE_X 0
#define DEFAULT_BASE_Y 0
#define DEFAULT_DIFF_X 1
#define DEFAULT_DIFF_Y 1

struct screen {
  wchar_t **table;
  unsigned long width;
  unsigned long height;
  long double base_x;
  long double base_y;
  long double diff_x;
  long double diff_y;
};

struct screen initialize_screen(unsigned long width, unsigned long height,
                                wchar_t filler);

void initialize_axes(struct screen *scr, long double base_x, long double base_y,
                     long double diff_x, long double diff_y);

void draw_graph(struct screen *scr, struct token *rpn_expr,
                unsigned long expr_len, const wchar_t *sign,
                unsigned long sign_len);

void print_screen(struct screen scr);

void free_screen(struct screen scr);

#endif
