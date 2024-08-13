#pragma once
#ifndef PARSER_H
#define PARSER_H
#include "token.h"

#define BASE_TOKEN_LENGTH 256
#define BASE_TOKEN_DATALEN 32
struct token *tokenize(const char *expr, unsigned long expr_len,
                       unsigned long *size);

struct token *reverse_polish_notation(struct token *input, unsigned long in_len,
                                      unsigned long *out_len);

void free_token(struct token tok);

#endif
