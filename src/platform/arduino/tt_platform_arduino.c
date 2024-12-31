/**
 * @file tt_platform_arduino.c
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#include "../internal/tt_platform_internal.h"

// Arduino specific defines - typically these would come from Arduino.h
#ifndef F_CPU
#define F_CPU 16000000UL // Default to 16MHz if not defined
#endif

#ifndef RAMEND
#define RAMEND 0x8FF // Default Arduino Uno RAM end
#endif

#ifndef FLASHEND
#define FLASHEND 0x7FFF // Default Arduino Uno flash end
#endif

tt_error_t tt_platform_arduino_init(tt_platform_info_t *info) {
  if (info == NULL) {
    return TT_ERROR_NULL_POINTER;
  }

  // Basic Arduino platform info
  info->platform_type = TT_PLATFORM_ARDUINO;
  info->arch_type = TT_ARCH_AVR;
  info->capabilities = TT_CAP_IRQ | TT_CAP_TIMERS;

  // System information
  info->system.cpu_frequency = F_CPU;
  info->system.ram_size = RAMEND + 1;
  info->system.flash_size = FLASHEND + 1;
  info->system.core_count = 1;

  // IRQ capabilities
  info->irq.irq_levels = 1;
  info->irq.nested_irq = false;

  return TT_SUCCESS;
}

tt_error_t tt_platform_arduino_cleanup(void) { return TT_SUCCESS; }
