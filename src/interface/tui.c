#include "../evaluate_rpn.h"
#include "interface.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "wchar.h"

struct tui_viewport {
  wchar_t **table;
  unsigned width;
  unsigned height;
};

#define EPS 1E-5

// graph symbols
const unsigned L_SIGN_LEN = 7;
const unsigned R_SIGN_LEN = 7;
const wchar_t *L_SIGN = L"⣀⡠⠤⠔⠒⠊⠉";
const wchar_t *R_SIGN = L"⣀⢄⠤⠢⠒⠑⠉";
const wchar_t *V_SIGN = L"⠇⠸";
// background
const wchar_t background_char = L'⠀';
// axes symbols
const wchar_t LINE_ABS = L'━';
const wchar_t LINE_ORD = L'┃';
const wchar_t MARK_ABS = L'┿';
const wchar_t MARK_ORD = L'╂';
const wchar_t CROSS = L'╋';
// ornament symbols
const wchar_t LEFT_ANGLE_TOP_ORNAMENT = L'╔';
const wchar_t RIGHT_ANGLE_TOP_ORNAMENT = L'╗';
const wchar_t LEFT_ANGLE_BOTTOM_ORNAMENT = L'╚';
const wchar_t RIGHT_ANGLE_BOTTOM_ORNAMENT = L'╝';
const wchar_t HORIZONTAL_ORNAMENT = L'═';
const wchar_t VERTICAL_ORNAMENT = L'║';
//

void initialize_screen(struct tui_viewport *scr, struct graph_params graph) {
  scr->width = graph.width;
  scr->height = graph.height;
  // alloc
  scr->table = (wchar_t **)(calloc(graph.width, sizeof(wchar_t *)));
  if (scr->table == NULL) {
    fprintf(stderr, "CALLOC ERROR\n");
    abort();
  }
  for (unsigned i = 0; i < graph.width; ++i) {
    scr->table[i] = (wchar_t *)(calloc(graph.height, sizeof(wchar_t)));
    if (scr->table[i] == NULL) {
      fprintf(stderr, "CALLOC ERROR\n");
      abort();
    }
  }
  for (unsigned i = 0; i < graph.width; ++i) {
    for (unsigned j = 0; j < graph.height; ++j) {
      scr->table[i][j] = background_char;
    }
  }
  // find axes
  int ord_ax = -1, abs_ax = -1;
  for (unsigned i = 0; i < graph.width; ++i) {
    long double x_pos_prev = graph.base_x + (i - 1) * graph.diff_x;
    long double x_pos_next = graph.base_x + (i + 1) * graph.diff_x;
    if (x_pos_prev < 0 && 0 < x_pos_next)
      ord_ax = i;
  }
  for (unsigned j = 0; j < graph.height; ++j) {
    long double y_pos_prev = graph.base_y + (j - 1) * graph.diff_y;
    long double y_pos_next = graph.base_y + (j + 1) * graph.diff_y;
    if (y_pos_prev < 0 && 0 < y_pos_next)
      abs_ax = j;
  }
  // set axes
  if (ord_ax != -1)
    for (unsigned j = 0; j < graph.height; ++j) {
      if (graph.ord_freq != 0 &&
          (int)round(fabsl(graph.base_y - graph.diff_y * j) / graph.diff_y) %
                  graph.ord_freq ==
              0) {
        scr->table[ord_ax][j] = MARK_ORD;
      } else {
        scr->table[ord_ax][j] = LINE_ORD;
      }
    }
  if (abs_ax != -1)
    for (unsigned i = 0; i < graph.width; ++i) {
      if (graph.abs_freq != 0 &&
          (int)round(fabsl(graph.base_x - graph.diff_x * i) / graph.diff_x) %
                  graph.abs_freq ==
              0) {
        scr->table[i][abs_ax] = MARK_ABS;
      } else {
        scr->table[i][abs_ax] = LINE_ABS;
      }
    }
  if (abs_ax != -1 && ord_ax != -1) {
    scr->table[ord_ax][abs_ax] = CROSS;
  }
}

void draw_graph(struct tui_viewport scr, struct graph_params graph) {
  long double max_x = graph.base_x + graph.width * graph.diff_x - EPS;
  long double max_y = graph.base_y + graph.height * graph.diff_y - EPS;

  long double L_SIGN_step = graph.diff_y / L_SIGN_LEN;
  long double R_SIGN_step = graph.diff_y / R_SIGN_LEN;
  for (unsigned num = 0; num < graph.graph_count; ++num) {
    long double prev_y = 0;
    signed prev_j = -1;
    for (unsigned i = 0; i < graph.width; ++i) {
      long double x = graph.base_x + graph.diff_x * i;
      long double y = evaluate(x, graph.rpn_expr[num], graph.rpn_expr_len[num]);
      if (isnan(y)) {
        prev_j = -1;
        continue;
      }
      long double y_c = y;
      unsigned p = 0;
      signed j = (y - graph.base_y) / graph.diff_y; // close position
      j = j < 0 ? 0 : j;
      j = j > graph.height ? graph.height : j;
      if (y >= graph.base_y && y < max_y) {
        y -= graph.base_y + graph.diff_y * j;
        if (prev_y < y_c) {
          for (; y > 0 && p < L_SIGN_LEN - 1; y -= L_SIGN_step) {
            ++p;
          }
          scr.table[i][j] = L_SIGN[p];
        } else {
          for (; y > 0 && p < R_SIGN_LEN - 1; y -= R_SIGN_step) {
            ++p;
          }
          scr.table[i][j] = R_SIGN[p];
        }
      }
      if (prev_j != -1) {
        signed CURR = j < graph.height ? j : graph.height;
        signed PREV = prev_j > 0 ? prev_j : 0;
        PREV = PREV > graph.height ? graph.height : PREV;
        if (CURR < PREV) {
          for (signed t_j = CURR + 1; t_j < PREV; ++t_j) {
            scr.table[i][t_j] = V_SIGN[0];
          }
        } else {
          for (signed t_j = PREV + 1; t_j < CURR; ++t_j) {
            scr.table[i - 1][t_j] = V_SIGN[1];
          }
        }
      }
      prev_j = j;
      prev_y = y_c;
    }
  }
}

void print_screen(struct tui_viewport scr, struct graph_params graph) {
  wprintf(L"%lc", LEFT_ANGLE_TOP_ORNAMENT);
  for (unsigned i = 0; i < scr.width + 1; ++i) {
    wprintf(L"%lc", HORIZONTAL_ORNAMENT);
  }
  wprintf(L"%lc\n", RIGHT_ANGLE_TOP_ORNAMENT);
  for (int j = scr.height - 1; j >= 0; --j) {
    wprintf(L"%lc", VERTICAL_ORNAMENT);
    for (unsigned i = 0; i < scr.width; ++i) {
      wprintf(L"%lc", scr.table[i][j]);
    }
    wprintf(L" %lc Y = %Lf\n", VERTICAL_ORNAMENT,
            graph.base_y + j * graph.diff_x);
  }
  wprintf(L"%lc", LEFT_ANGLE_BOTTOM_ORNAMENT);
  for (unsigned i = 0; i < scr.width + 1; ++i) {
    wprintf(L"%lc", HORIZONTAL_ORNAMENT);
  }
  wprintf(L"%lc\n", RIGHT_ANGLE_BOTTOM_ORNAMENT);
}

void free_table(struct tui_viewport scr) {
  for (unsigned i = 0; i < scr.width; ++i)
    free(scr.table[i]);
  free(scr.table);
}

void tui(struct graph_params graph) {
  struct tui_viewport scr;
  initialize_screen(&scr, graph);
  draw_graph(scr, graph);
  print_screen(scr, graph);
  free_table(scr);
}