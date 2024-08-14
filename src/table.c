#include "table.h"
#include "evaluate_rpn.h"
#include "math.h"
#include "stdlib.h"

#define EPS 1E-5

struct screen initialize_screen(unsigned long width, unsigned long height,
                                wchar_t filler) {
  struct screen result;
  result.base_x = DEFAULT_BASE_X;
  result.base_y = DEFAULT_BASE_Y;
  result.diff_x = DEFAULT_DIFF_X;
  result.diff_y = DEFAULT_DIFF_Y;

  result.table = (wchar_t **)(calloc(width, sizeof(wchar_t *)));
  for (unsigned i = 0; i < width; ++i) {
    result.table[i] = (wchar_t *)(calloc(height, sizeof(wchar_t)));
  }
  for (unsigned i = 0; i < width; ++i) {
    for (unsigned j = 0; j < height; ++j) {
      result.table[i][j] = filler;
    }
  }
  result.width = width;
  result.height = height;

  return result;
}
void initialize_axes(struct screen *scr, long double base_x, long double base_y,
                     long double diff_x, long double diff_y, unsigned ord_freq,
                     unsigned abs_freq) {
  scr->base_x = base_x;
  scr->base_y = base_y;
  scr->diff_x = diff_x;
  scr->diff_y = diff_y;
  int ord_ax = -1, abs_ax = -1;
  for (unsigned i = 0; i < scr->width; ++i) {
    long double x_pos_prev = scr->base_x + (i - 1) * scr->diff_x;
    long double x_pos_next = scr->base_x + (i + 1) * scr->diff_x;
    if (x_pos_prev < 0 && 0 < x_pos_next)
      ord_ax = i;
  }
  for (unsigned j = 0; j < scr->height; ++j) {
    long double y_pos_prev = scr->base_y + (j - 1) * scr->diff_y;
    long double y_pos_next = scr->base_y + (j + 1) * scr->diff_y;
    if (y_pos_prev < 0 && 0 < y_pos_next)
      abs_ax = j;
  }
  if (ord_ax != -1)
    for (unsigned j = 0; j < scr->height; ++j) {
      if (ord_freq != 0 &&
          (int)round(fabsl(scr->base_y - scr->diff_y * j) / scr->diff_y) %
                  ord_freq ==
              0) {
        scr->table[ord_ax][j] = L'╂';
      } else {
        scr->table[ord_ax][j] = L'┃';
      }
    }
  if (abs_ax != -1)
    for (unsigned i = 0; i < scr->width; ++i) {
      if (abs_freq != 0 &&
          (int)round(fabsl(scr->base_x - scr->diff_x * i) / scr->diff_x) %
                  abs_freq ==
              0) {
        scr->table[i][abs_ax] = L'┿';
      } else {
        scr->table[i][abs_ax] = L'━';
      }
    }
  if (abs_ax != -1 && ord_ax != -1) {
    scr->table[ord_ax][abs_ax] = L'╋';
  }
}
void draw_graph(struct screen *scr, struct token *rpn_expr,
                unsigned long expr_len, const wchar_t *l_sign,
                unsigned long l_sign_len, const wchar_t *r_sign,
                unsigned long r_sign_len) {
  long double max_x = scr->base_x + scr->width * scr->diff_x - EPS;
  long double max_y = scr->base_y + scr->height * scr->diff_y - EPS;
  unsigned i = 0;
  long double l_step = scr->diff_y / l_sign_len;
  long double r_step = scr->diff_y / r_sign_len;
  long double prev_y = 0;
  for (long double x = scr->base_x; x < max_x && i < scr->width;
       x += scr->diff_x, ++i) {
    long double y = evaluate(x, rpn_expr, expr_len);
    if (isnan(y))
      continue;
    long double y_c = y;
    if (y >= scr->base_y && y < max_y) {
      unsigned p = 0;
      unsigned j = (y - scr->base_y) / scr->diff_y; // close position
      y -= scr->base_y + scr->diff_y * j;
      if (prev_y < y_c) {
        for (; y > 0 && p < l_sign_len - 1; y -= l_step) {
          ++p;
        }
        scr->table[i][j] = l_sign[p];
      } else {
        for (; y > 0 && p < r_sign_len - 1; y -= r_step) {
          ++p;
        }
        scr->table[i][j] = r_sign[p];
      }
    }
    prev_y = y_c;
  }
}

void print_screen(struct screen scr) {
  // PRINT TOP ORNAMENT
  wprintf(L"╔");
  for (unsigned i = 0; i < scr.width + 1; ++i) {
    wprintf(L"═");
  }
  wprintf(L"╗\n");
  // PRINT TOP ORNAMENT
  // PRINT TABLE
  for (int j = scr.height - 1; j >= 0; --j) {
    wprintf(L"║");
    for (unsigned i = 0; i < scr.width; ++i) {
      wprintf(L"%lc", scr.table[i][j]);
    }
    wprintf(L" ║ Y = %Lf\n", scr.base_y + scr.diff_y * j);
  }
  // PRINT TABLE
  // PRINT BOTTOM ORNAMENT
  wprintf(L"╚");
  for (unsigned i = 0; i < scr.width + 1; ++i) {
    wprintf(L"═");
  }
  wprintf(L"╝");
  wprintf(L"\n");
  // PRINT BOTTOM ORNAMENT
  wprintf(L"BASE X: %Lf\t", scr.base_x);
  wprintf(L"BASE Y: %Lf\n", scr.base_y);
  wprintf(L"DIFF X: %Lf\t", scr.diff_x);
  wprintf(L"DIFF Y: %Lf\n", scr.diff_y);
}

void free_screen(struct screen scr) {
  for (unsigned i = 0; i < scr.width; ++i)
    free(scr.table[i]);
  free(scr.table);
}
