/**
 * @file unity_config.h
 * @author Mihai Gurei <mihai.gurei@protonmail.com>
 * @date 2024-12-27
 * @brief
 * @copyright Copyright (c) 2024 AnAlphaBeta. All rights reserved.
 */

#ifndef UNITY_CONFIG_H_
#define UNITY_CONFIG_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

// Unity output configuration
void unity_output_char(char c);
void unity_output_start(void);
void unity_output_complete(void);

// Define any custom Unity configurations here
#define UNITY_OUTPUT_CHAR(c) unity_output_char(c)
#define UNITY_OUTPUT_START() unity_output_start()
#define UNITY_OUTPUT_COMPLETE() unity_output_complete()

#endif // UNITY_CONFIG_H_
