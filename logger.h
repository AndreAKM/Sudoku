/*
 * logger.h
 *
 *  Created on: Nov 1, 2018
 *      Author: oem
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef DEBUG
#define  LOG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#endif /* LOGGER_H_ */
