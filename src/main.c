#include "evaluate_rpn.h"
#include "interface/interface.h"
#include "locale.h"
#include "parser.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "wchar.h"

int scan_expr(FILE *params, struct graph_params *graph) {
  fscanf(params, "%u\n", &graph->graph_count);
  if (graph->graph_count > MAX_EXPR_COUNT) {
    fprintf(stderr, "EXPRESSION LIMIT EXCEED\n");
    return 1;
  }
  for (unsigned i = 0; i < graph->graph_count; ++i) {
    char str_expr[MAX_EXPR_LENGTH];
    fgets(str_expr, MAX_EXPR_LENGTH, params);
    unsigned expr_len = 0;
    struct token *expr = tokenize(str_expr, strlen(str_expr), &expr_len);
    if (expr == NULL) {
      fprintf(stderr, "TOKENIZE ERROR\n");
      return 1;
    }
    graph->rpn_expr[i] =
        reverse_polish_notation(expr, expr_len, &graph->rpn_expr_len[i]);
    free(expr);
    if (graph->rpn_expr[i] == NULL) {
      fprintf(stderr, "RPN ERROR\n");
      for (unsigned j = 0; j < i; ++j)
        free(graph->rpn_expr[j]);
      return 1;
    }
  }
  return 0;
}

int scan_window_params(FILE *params, struct graph_params *graph) {
  fscanf(params,
         "width: %u\n"
         "height: %u\n"
         "base x: %Lf\n"
         "base y: %Lf\n"
         "step x: %Lf\n"
         "step y: %Lf\n"
         "frequency of ord. axe marks(0 to off): %u\n"
         "frequency of abs. axe marks(0 to off): %u\n",
         &graph->width, &graph->height, &graph->base_x, &graph->base_y,
         &graph->diff_x, &graph->diff_y, &graph->ord_freq, &graph->abs_freq);
  if (graph->width > MAX_WIDTH) {
    fprintf(stderr, "WIDTH EXCEED\n");
    return 1;
  }
  if (graph->height > MAX_HEIGHT) {
    fprintf(stderr, "HEIGHT EXCEED\n");
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  const struct lconv *loc = localeconv();
  loc->decimal_point[0] = '.';
  struct graph_params graph;
  if (argc > 1) {
    FILE *params = fopen(argv[1], "r");
    if (params == NULL) {
      fprintf(stderr, "FILE OPEN ERROR\n");
      return 0;
    }
    if (scan_window_params(params, &graph) == 1)
      abort();
    if (scan_expr(params, &graph) == 1)
      abort();
    fclose(params);
    tui(graph);
  } else {
    char config_path[128];
    memset(config_path, 0, 128);
    printf("Enter relative path to config:\n");
    scanf("%s", config_path);
    FILE *params = fopen(config_path, "r");
    if (params == NULL) {
      fprintf(stderr, "FILE OPEN ERROR\n");
      return 0;
    }
    if (scan_expr(params, &graph) == 1)
      abort();
    fclose(params);
    graph.base_x = COMMON_BASE_X;
    graph.base_y = COMMON_BASE_Y;
    graph.diff_x = COMMON_DIFF_X;
    graph.diff_y = COMMON_DIFF_Y;
    gui(graph);
  }
  for (unsigned i = 0; i < graph.graph_count; ++i) {
    free(graph.rpn_expr[i]);
  }
  return 0;
}
