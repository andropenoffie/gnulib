/* Formatted output to a stream.
   Copyright (C) 2004, 2006-2017 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* Specification.  */
#include "unistdio.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "fseterr.h"

/* Print formatted output to the stream FP.
   Return string length of formatted string.  On error, return a negative
   value.  */
int
ulc_vfprintf (FILE *fp, const char *format, va_list args)
{
  char buf[2000];
  char *output;
  size_t len;
  size_t lenbuf = sizeof (buf);

  output = ulc_vasnprintf (buf, &lenbuf, format, args);
  len = lenbuf;

  if (!output)
    {
      fseterr (fp);
      return -1;
    }

  if (fwrite (output, 1, len, fp) < len)
    {
      if (output != buf)
        {
          int saved_errno = errno;
          free (output);
          errno = saved_errno;
        }
      return -1;
    }

  if (len > INT_MAX)
    {
      errno = EOVERFLOW;
      fseterr (fp);
      return -1;
    }

  return len;
}
