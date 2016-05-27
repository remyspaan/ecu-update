/*
 * File:    test/fail.c
 * Author:  Michael Hutter, Peter Schwabe
 * Version: Thu Jul 24 14:04:59 2014 +0200
 * Public Domain
 */

#include <stdlib.h>
#include "fail.h"

void fail(const char *error)
{
  /* can do something here with the error, like print it */
  exit(-1);
}
