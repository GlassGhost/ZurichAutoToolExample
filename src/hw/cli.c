/* @(#)cli.c
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
#include <stdlib.h>
#include <string.h>

#include "cli.h"

int parse_float_array(int argc, char **argv, double **result){
    int i;
    char *endptr = NULL;
    double *array;

    *result = calloc(argc-1, sizeof(double));
    array = *result;

    for(i=0; i<argc-1; i++){
        array[i] = strtod(argv[i+1], &endptr);
        if(argv[i+1] == endptr || strlen(endptr) > 0){
            /* Error converting float */
            return 128+i+1;
        }
    }

    if(i == 1){
        /* No conversion happened */
        return 2;
    }
            
    return 0;
}
