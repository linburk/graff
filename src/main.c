#include "evaluate_rpn.h"
#include "locale.h"
#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "table.h"
#define SIGN_BUFFER_SIZE 32
#define LINE_BUFFER_SIZE 512

int main() {
  setlocale(LC_ALL, "");
  const struct lconv *loc = localeconv();
  loc->decimal_point[0] = '.';
  FILE *graph_settings;
  unsigned width, height;
  long double base_x, base_y, diff_x, diff_y;
  unsigned expr_count;
  graph_settings = fopen("%INPUT PATH TO CONFIG%", "r");
  wchar_t blank;
  wchar_t l_sign[SIGN_BUFFER_SIZE], r_sign[SIGN_BUFFER_SIZE];
  unsigned long l_sign_len, r_sign_len;
  unsigned ord_freq, abs_freq;
  fwscanf(graph_settings,
          L"width: %u\n"
          L"height: %u\n"
          L"base x: %Lf\n"
          L"base y: %Lf\n"
          L"step x: %Lf\n"
          L"step y: %Lf\n"
          L"increasing symbols: %ls\n"
          L"decreasing symbols: %ls\n"
          L"background char: %lc\n"
          L"frequency of ord. axe marks(0 to off): %u\n"
          L"frequency of abs. axe marks(0 to off): %u\n"
          L"number of expressions: %u\n",
          &width, &height, &base_x, &base_y, &diff_x, &diff_y, l_sign, r_sign,
          &blank, &ord_freq, &abs_freq, &expr_count);
  l_sign_len = wcslen(l_sign);
  r_sign_len = wcslen(r_sign);
  struct screen frame = initialize_screen(width, height, blank);
  initialize_axes(&frame, base_x, base_y, diff_x, diff_y, ord_freq, abs_freq);
  for (unsigned i = 0; i < expr_count; ++i) {
    char expr[LINE_BUFFER_SIZE];
    fgets(expr, LINE_BUFFER_SIZE, graph_settings);
    unsigned long len = 0;
    struct token *res = tokenize(expr, strlen(expr), &len);
    if (res == NULL) {
      printf("Wrong expression\n");
      return 0;
    }
    unsigned long slen = 0;
    struct token *out = reverse_polish_notation(res, len, &slen);
    if (out == NULL) {
      printf("Wrong expression\n");
      return 0;
    }
    draw_graph(&frame, out, slen, l_sign, l_sign_len, r_sign, r_sign_len);
    free(out);
    free(res);
  }
  fclose(graph_settings);
  print_screen(frame);
  free_screen(frame);
}
