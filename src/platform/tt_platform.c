/**
 * @file tt_platform.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "tt_platform.h"
#include "internal/tt_platform_internal.h"
#include "tt_types.h"
#include <string.h>

static tt_platform_info_t platform_info;
static bool platform_initalized = false;

tt_error_t tt_platfor_init() {
  if (platform_initalized) {
    return TT_ERROR_ALREADY_INITIALIZED;
  }

  tt_error_t err;

#if defined(TT_PLATFORM_LINUX)
  err = tt_platform_linux_init(&platform_info);
#elif defined(TT_PLATFORM_ARDUINO)
  err = tt_platform_arduino_init(&platform_info);
#elif defined(TT_PLATFORM_FREERTOS)
  err = tt_platform_freertos_init(&platform_info);
#else
  err = TT_ERROR_PLATFORM_NOT_SUPPORTED;
#endif

  if (err != TT_SUCCESS) {
    return err;
  }

  platform_initalized = true;
  return TT_SUCCESS;
}

tt_error_t tt_platform_cleanup(void) {
  if (!platform_initalized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  tt_error_t err;

#if defined(TT_PLATFORM_LINUX)
  err = tt_platform_linux_cleanup();
#elif defined(TT_PLATFORM_ARDUINO)
  err = tt_platform_arduino_cleanup();
#elif defined(TT_PLATFORM_FREERTOS)
  err = tt_platform_freertos_cleanup();
#else
  err = TT_ERROR_PLATFORM_NOT_SUPPORTED;
#endif

  if (err != TT_SUCCESS) {
    return err;
  }

  memset(&platform_info, 0, sizeof(tt_platform_info_t));
  platform_initalized = false;
  return TT_SUCCESS;
}

tt_error_t tt_platform_get_info(tt_platform_info_t *info) {
  if (!platform_initalized) {
    return TT_ERROR_NOT_INITIALIZED;
  }

  if (info == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  memcpy(info, &platform_info, sizeof(tt_platform_info_t));
  return TT_SUCCESS;
}
