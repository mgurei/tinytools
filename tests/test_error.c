/**
 * @file test_error.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_error.h"
#include <stdio.h>

int main(void) {
  printf("Testing error handling...\n");

  // Test error string conversion
  printf("TT_SUCCESS: %s\n", tt_error_to_string(TT_SUCCESS));
  printf("TT_ERROR_INVALID_PARAM: %s\n",
         tt_error_to_string(TT_ERROR_INVALID_PARAM));
  printf("TT_ERROR_NULL_POINTER: %s\n",
         tt_error_to_string(TT_ERROR_NULL_POINTER));
  printf("TT_ERROR_BUFFER_EMPTY: %s\n",
         tt_error_to_string(TT_ERROR_BUFFER_EMPTY));
}
