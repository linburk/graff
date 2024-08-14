#include "locale.h"
#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "table.h"

int main() {
  setlocale(LC_ALL, "");
  FILE *graph_settings;
  unsigned width, height;
  long double base_x, base_y, diff_x, diff_y;
  unsigned expr_count;
  graph_settings = fopen("%ENTER PATH TO CONFIG FILE%", "r");
  wchar_t blank;
  wchar_t l_sign[32], r_sign[32];
  unsigned long l_sign_len, r_sign_len;
  unsigned ord_freq, abs_freq;
  fscanf(graph_settings,
         "width: %u\n"
         "height: %u\n"
         "base x: %Lf\n"
         "base y: %Lf\n"
         "step x: %Lf\n"
         "step y: %Lf\n"
         "increasing symbols: %ls\n"
         "decreasing symbols: %ls\n"
         "background char: %lc\n"
         "frequency of ord. axe marks(0 to off): %u\n"
         "frequency of abs. axe marks(0 to off): %u\n"
         "number of expressions: %u\n",
         &width, &height, &base_x, &base_y, &diff_x, &diff_y, l_sign, r_sign,
         &blank, &ord_freq, &abs_freq, &expr_count);
  l_sign_len = wcslen(l_sign);
  r_sign_len = wcslen(r_sign);
  struct screen frame = initialize_screen(width, height, blank);
  initialize_axes(&frame, base_x, base_y, diff_x, diff_y, ord_freq, abs_freq);
  for (unsigned i = 0; i < expr_count; ++i) {
    char expr[512];
    fscanf(graph_settings, "%s", expr);
    unsigned long len = 0;
    printf("%s\n", expr);
    struct token *res = tokenize(expr, strlen(expr), &len);
    unsigned long slen = 0;
    struct token *out = reverse_polish_notation(res, len, &slen);
    draw_graph(&frame, out, slen, l_sign, l_sign_len, r_sign, r_sign_len);
    for (unsigned i = 0; i < len; ++i) {
      free_token(res[i]);
    }
    free(out);
    free(res);
  }
  fclose(graph_settings);
  print_screen(frame);
  free_screen(frame);
}
