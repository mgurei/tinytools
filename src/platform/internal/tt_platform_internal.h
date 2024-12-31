/**
 * @file tt_platform_internal.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_PLATFORM_INTERNAL_H_
#define TT_PLATFORM_INTERNAL_H_

#include "../../../include/tt_platform.h"

/**
 * @brief Platform specific initialization functions
 * @param info Pointer to platform info
 * @return TT_ERROR on success, error code otherwise
 */
tt_error_t tt_platform_linux_init(tt_platform_info_t *info);
tt_error_t tt_platform_arduino_init(tt_platform_info_t *info);
tt_error_t tt_platform_freertos_init(tt_platform_info_t *info);

/**
 * @brief Platform specific clean up functions
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_linux_cleanup(void);
tt_error_t tt_platform_arduino_cleanup(void);
tt_error_t tt_platform_freertos_cleanup(void);

#endif // TT_PLATFORM_INTERNAL_H_
