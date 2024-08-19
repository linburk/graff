#pragma once
#ifndef PARSER_H
#define PARSER_H
#include "token.h"

#define MAX_TOKEN_LENGTH 256

struct token *tokenize(const char *expr, unsigned expr_len, unsigned *size);

struct token *reverse_polish_notation(struct token *input, unsigned in_len,
                                      unsigned *out_len);

#endif
