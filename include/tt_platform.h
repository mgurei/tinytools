/**
 * @file tt_platform.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-31
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef TT_PLATFORM_H_
#define TT_PLATFORM_H_

#include "tt_types.h"

#if defined(TT_CAP_MUTEX)
#include "tt_mutex.h"
#endif /* TT_CAP_MUTEX*/

/**
 * @brief Platform types
 */
typedef enum {
  TT_PLATFORM_UNKNOWN = 0,
#ifdef TT_TARGET_LINUX
  TT_PLATFORM_LINUX,
#endif
  TT_PLATFORM_WINDOWS,
  TT_PLATFORM_MACOS,
#ifdef TT_TARGET_ARDUINO
  TT_PLATFORM_ARDUINO,
#endif
#ifdef TT_TARGET_FREERTOS
  TT_PLATFORM_FREERTOS,
#endif
  TT_PLATFORM_BAREMETAL,
  // Add other embedded platforms as needed
} tt_platform_type_t;

/**
 * @brief CPU architecture typed
 */
typedef enum {
  TT_ARCH_UNKNOWN = 0,
  TT_ARCH_X86,
  TT_ARCH_X64,
  TT_ARCH_ARM,
  TT_ARCH_ARM64,
  TT_ARCH_AVR,
  TT_ARCH_RISCV,
  // Add other architectures as needed
} tt_arch_type_t;

/**
 * @brief Platform capabilities flags
 */
typedef enum {
  TT_PLATFORM_CAP_NONE = 0,
  TT_PLATFORM_CAP_THREADS = (1 << 0),
  TT_PLATFORM_CAP_MUTEX = (1 << 1),
  TT_PLATFORM_CAP_SEMAPHORE = (1 << 2),
  TT_PLATFORM_CAP_TLS = (1 << 3), // Thread local storage
  TT_PLATFORM_CAP_ATOMIC = (1 << 4),
  TT_PLATFORM_CAP_IRQ = (1 << 5), // Interept handling
  TT_PLATFORM_CAP_TIMERS = (1 << 6),
  TT_PLATFORM_CAP_DMA = (1 << 7)
} tt_platform_caps_t;

/**
 * @brief Platform, CPU and thread capabilities information
 */
typedef struct {
  // Platform identification
  tt_platform_type_t platform_type;
  tt_arch_type_t arch_type;
  uint32_t capabilities; // Bitmap of tt_platform_caps_t

  // System information
  struct {
    size_t cpu_frequency; /**< CPU frequency in Hz*/
    size_t ram_size;      /**< RAM size in bytes*/
    size_t flash_size;    /**< Flash size in bytes - for embedded*/
    uint8_t core_count;   /**< Number of CPU cores*/
  } system;

  // Thread capabilities (when TT_CAP_THREADS is set)
  struct {
    size_t min_stack_size;
    size_t max_stack_size;
    int max_priority;
    int min_priority;
  } thread;

  // Interrupt capabilites (when TT_CAP_IRQ is set)
  struct {
    uint8_t irq_levels; /**< Number of interrupt priority levels*/
    bool nested_irq;    /**< Supports nested interrupts*/
  } irq;

} tt_platform_info_t;

/**
 * @brief Platform initialization
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_init(void);

/**
 * @brief Platform cleanup
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_cleanup(void);

/**
 * @brief Get platform information
 * @param info Pointer to platform information structure
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_get_info(tt_platform_info_t *info);

// Platform-specific thread operations (when TT_CAP_THREADS)
#if defined(TT_CAP_THREADS)
/**
 * @brief Create a new platform specific thread
 *
 * Detailed description
 *
 * @param thread Pointer to thread handle
 * @param attr Thread attributes
 * @param func Thread function
 * @param arg Pointer to thread function arguments
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_thread_create(tt_thread_t *thread,
                                     const tt_thread_attr_t *attr,
                                     tt_thread_func_t func, void *arg);
/**
 * @brief Join with a terminated platform specific thread
 * @param thread Pointer to thread handle
 * @param retval Pointer to store thread return value
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_thread_join(tt_thread_t *thread, void **retval);

/**
 * @brief Detach Platform-specific Thread
 * @param thread Pointer to thread handle
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_thread_detach(tt_thread_t *thread);

/**
 * @brief Exit platform-specific thread
 *
 * Detailed description
 *
 * @param retval Pointer to store thread return value
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_thread_exit(void *retval);
#endif

// Platform-specific synchronization primitives (when TT_CAP_MUTEX is supported)
#if defined(TT_CAP_MUTEX)
/**
 * @brief Initialization of platform-specifi mutex
 * @param mutex Pointer to mutex
 * @param attr Pointer to mutex attributes
 * @return TT_SUCCESS on success, error code otherwise
 */
/* tt_error_t tt_platform_mutex_init(tt_mutex_t *mutex, */
/*                                   const tt_mutex_attr_t *attr); */
tt_error_t tt_platform_mutex_init(tt_mutex_t *mutex);

/**
 * @brief Destroy platform-specific mutesx
 * @param mutex Pointer to mutex
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_mutex_destroy(tt_mutex_t *mutex);

/**
 * @brief Lock platform-specific mutex
 * @param mutex Pointer to mutex
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_mutex_lock(tt_mutex_t *mutex);

/**
 * @brief Unlock platform-specific mutex
 * @param mutex Pointer to mutex
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_mutex_unlock(tt_mutex_t *mutex);

/**
 * @brief Try lock platform-specific mutex without blocking
 * @param mutex Pointer to mutex
 * @return TT_SUCCESS on success, TT_ERROR_BUSY if mutex is already locked
 */
tt_error_t tt_platform_mutex_trylock(tt_mutex_t *mutex);

/**
 * @brief Check if mutex is locked
 * @param mutex Pointer to mutex structure
 * @return true if mutex is locked, false otherwise
 */
bool tt_platform_mutex_is_locked(tt_mutex_t *mutex);

#endif /* TT_CAP_MUTEX */

// Interrupt handling (when TT_CAP_IRQ is supported)
#if defined(TT_CAP_IRQ)
/**
 * @brief Enable platform-specific IRQ
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_irq_enable(void);

/**
 * @brief Disable platform-specific IRQ
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_irq_disable(void);

/**
 * @brief Set priority on platform-specific IRQ
 * @param irq_num Number of IRQ
 * @param prioriry Priority level
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_irq_set_priority(uint8_t irq_num, uint8_t priority);
#endif /* TT_CAP_IRQ */

// Platform specific memory operations
/**
 * @brief Initialize memory on platform
 * @return TT_SUCCESS on success, error code otherwise
 */
tt_error_t tt_platform_mem_init(void);

/**
 * @brief Free memory on platform
 * @return Return free memory
 */
size_t tt_platform_mem_get_free(void);

#endif // TT_PLATFORM_H_
