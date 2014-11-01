/* @(#)hw.c
 *
 * 
 *  Copyright (C) 2013, GC3, University of Zurich. All rights
 *  reserved.
 *
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdio.h>

#include "cli.h"

#include "hwmath.h"

int main(int argc, char **argv){
    double result = 0, retcode;
    double *tmp = NULL;
    int i;

    retcode = parse_float_array(argc, argv, &tmp);
    if(retcode || NULL == tmp){
        if(retcode>=128){
            int idx = retcode-128;
            printf("Error in argument `%s' at position %d\n", argv[idx], idx);
        }
        printf("Error parsing command line.\nExiting.\n");
        return 1;
    }

    result = average(argc-1, tmp);

    printf("Your array is: ");
    for(i=0; i<argc-1; i++){
        printf("%g ", tmp[i]);
    }
    printf("\n");

    printf("This is the average of the numbers in your array: %g\n", result);
    return 0;
}
