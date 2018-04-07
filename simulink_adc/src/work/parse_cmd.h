/*
 * parse_cmd.h
 *
 *  Created on: Sep 4, 2017
 *      Author: user52
 */

#ifndef SRC_HOST_UTIL_PARSE_CMD_H_
#define SRC_HOST_UTIL_PARSE_CMD_H_

#include "brd.h"

//! Get integer value from command line
int GetFromCommnadLine(int argc, BRDCHAR **argv, const BRDCHAR* name, int defValue);

//! Get string value from command line
int GetStrFromCommnadLine(int argc, BRDCHAR **argv, const BRDCHAR* name, BRDCHAR* defValue, BRDCHAR* dst, int dstLen );



#endif /* SRC_HOST_UTIL_PARSE_CMD_H_ */
