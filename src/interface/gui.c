#include "../evaluate_rpn.h"
#include "SDL.h"
#include "interface.h"
#include "math.h"
#include "stdlib.h"

#define ERR(S, CRITICAL)                                                       \
  do {                                                                         \
    fprintf(stderr, "[%s:%d] %s error: { %s } \n", __FILE__, __LINE__, #S,     \
            SDL_GetError());                                                   \
    if (CRITICAL)                                                              \
      abort();                                                                 \
  } while (0)

struct gui_viewport {
  unsigned width;
  unsigned height;
  SDL_Window *window;
  SDL_Renderer *renderer;
};

#define ZOOM 1.25
#define SHIFT 20
#define FPS 25

#define BLACK 0x00, 0x00, 0x00, 0x00
#define WHITE 0xff, 0xff, 0xff, 0xff
#define RED 0xff, 0x00, 0x00, 0x01

void renderGraph(SDL_Renderer *renderer, const struct graph_params graph) {
  {
    if (SDL_SetRenderDrawColor(renderer, WHITE) != 0)
      ERR(SDL_SetRenderDrawColor, 0);
    if (SDL_RenderClear(renderer) != 0)
      ERR(SDL_RenderClear, 0);
  }
  {
    if (SDL_SetRenderDrawColor(renderer, RED) != 0)
      ERR(SDL_SetRenderDrawColor, 0);
    SDL_Point axis_x[2], axis_y[2];
    axis_x[0].x = 0;
    axis_x[1].x = MAX_WIDTH;
    axis_x[0].y = axis_x[1].y =
        graph.height - 1 - (int)(-graph.base_y / graph.diff_y);
    axis_y[0].y = 0;
    axis_y[1].y = MAX_HEIGHT;
    axis_y[0].x = axis_y[1].x = (int)(-graph.base_x / graph.diff_x);
    if (SDL_RenderDrawLines(renderer, axis_x, 2) != 0)
      ERR(SDL_RenderDrawLines, 0);
    if (SDL_RenderDrawLines(renderer, axis_y, 2) != 0)
      ERR(SDL_RenderDrawLines, 0);
  }
  {
    SDL_Point *points = (SDL_Point *)(calloc(graph.width, sizeof(SDL_Point)));
    if (points == NULL) {
      fprintf(stderr, "CALLOC ERROR\n");
      abort();
    }
    if (SDL_SetRenderDrawColor(renderer, BLACK) != 0)
      ERR(SDL_SetRenderDrawColor, 0);
    for (unsigned num = 0; num < graph.graph_count; ++num) {
      unsigned ord = 0;
      for (int i = 0; i < graph.width; ++i, ++ord) {
        long double x = graph.base_x + i * graph.diff_x;
        long double y =
            evaluate(x, graph.rpn_expr[num], graph.rpn_expr_len[num]);
        if (isnan(y)) {
          if (ord > 1 && SDL_RenderDrawLines(renderer, points, ord) != 0)
            ERR(SDL_RenderDrawLines, 0);
          ord = -1;
          continue;
        }
        points[ord].x = i;
        points[ord].y =
            graph.height - 1 - (int)((y - graph.base_y) / graph.diff_y);
      }
      if (ord > 1 && SDL_RenderDrawLines(renderer, points, ord) != 0)
        ERR(SDL_RenderDrawLines, 0);
    }
  }
}

void gui(struct graph_params graph) {
  struct gui_viewport g;
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    ERR(SDL_Init(), 1);
  atexit(SDL_Quit);
  g.width = graph.width;
  g.height = graph.height;
  g.window = SDL_CreateWindow("graff", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, g.width, g.height,
                              SDL_WINDOW_RESIZABLE);
  if (g.window == NULL)
    ERR(SDL_CreateWindow(), 1);
  g.renderer = SDL_CreateRenderer(
      g.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (g.renderer == NULL)
    ERR(SDL_CreateRenderer(), 1);
  for (SDL_Event event;;) {
    SDL_GetWindowSize(g.window, &graph.width, &graph.height);
    Uint32 start = SDL_GetTicks();
    Uint32 elapsed;
    Uint32 estimated = 1000 / FPS;
    renderGraph(g.renderer, graph);
    SDL_RenderPresent(g.renderer);
    elapsed = SDL_GetTicks() - start;
    if (elapsed < estimated)
      SDL_Delay(estimated - elapsed);
    if (SDL_PollEvent(&event) == 0)
      continue;
    switch (event.type) {
    case SDL_QUIT:
      goto end;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        goto end;
      case SDLK_q:
        graph.diff_x *= ZOOM;
        graph.diff_y *= ZOOM;
        break;
      case SDLK_e:
        graph.diff_x /= ZOOM;
        graph.diff_y /= ZOOM;
        break;
      case SDLK_w:
        graph.base_y += SHIFT * graph.diff_y;
        break;
      case SDLK_a:
        graph.base_x -= SHIFT * graph.diff_x;
        break;
      case SDLK_s:
        graph.base_y -= SHIFT * graph.diff_y;
        break;
      case SDLK_d:
        graph.base_x += SHIFT * graph.diff_x;
        break;
      }
      break;
    default:
      break;
    }
  }
end:
  SDL_DestroyRenderer(g.renderer);
  SDL_DestroyWindow(g.window);
}