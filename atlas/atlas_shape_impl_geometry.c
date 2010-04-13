/*
 *  atlas_shape_impl_geometry.c
 *  atlas
 *
 *  Created by Peter Fenske on 4/13/10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST.
 *
 *  This file is part of atlas.
 *	
 *	atlas is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	atlas is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with atlas.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <float.h>
#include <math.h>

#include "atlas_shape_impl.h"
#include "atlas_shape_impl_geometry.h"

/*
 * Checks, if two lines intersect. Returns the point (single coordinate) if they intersect, 1 if they lie on top of each other, 0 if they don't
 */
int atlas_shape_lines_intersect(struct atlas_shape_coordinate_s * result,
								struct atlas_shape_coordinate_s * l1_s,
								struct atlas_shape_coordinate_s	* l1_e,
								struct atlas_shape_coordinate_s * l2_s,
								struct atlas_shape_coordinate_s	* l2_e){
	/* 
	 * latitude = y
	 * longtitude = x
	 * e = end = 2
	 * s = start = 1
	 * l1 = line1 , l2 = line 2
	 * y = m*x + n
	 */
	
	// Variables for intersection
	double intersect_x = DBL_MAX;
	double intersect_y = DBL_MAX;
	
	// function parameters for y = m*x + n
	double l1_m, l1_n, l2_m, l2_n;
	
	if (((l1_e->longitude - l1_s->longitude) == 0) && ((l2_e->longitude - l2_s->longitude) == 0)){
		/*
		 * Both lines are vertical. In this case they are either on top of each other or they never meet
		 */
		if (l1_e->longitude == l2_e->longitude){
			// they are on top of each other
			return 1;
		} else {
			// they run in parallel and never meet
			return 0;
		}		
		
	} else if ((l1_e->longitude - l1_s->longitude) == 0){
		// prevent division by zero, line 1 is vertical
		intersect_x = l1_e->longitude;
		
		// slope for line 2
		l2_m = (l2_e->latitude - l2_s->latitude)/(l2_e->longitude - l2_s->longitude);
		// y-intercept for line 2		
		l2_n = l2_s->latitude - l2_s->longitude*l2_m;
		
		intersect_y = l2_m * intersect_x + l2_n;
		
		
	} else if ((l2_e->longitude - l2_s->longitude) == 0) {
		// prevent division by zero, line 2 is vertical
		intersect_x = l2_e->longitude;
		
		// slope for line 1
		l1_m = (l1_e->latitude - l1_s->latitude)/(l1_e->longitude - l1_s->longitude);
		// y-intercept for line 1
		l1_n = l1_s->latitude - l1_s->longitude*l1_m;
		
		intersect_y = l1_m * intersect_x + l1_n;
		
	} else  {
		/*
		 * Last case, neither line 1 or line 2 are vertical
		 */
		
		// slope for line 1
		l1_m = (l1_e->latitude - l1_s->latitude)/(l1_e->longitude - l1_s->longitude);
		// y-intercept for line 1
		l1_n = l1_s->latitude - l1_s->longitude*l1_m;
		
		// slope for line 2
		l2_m = (l2_e->latitude - l2_s->latitude)/(l2_e->longitude - l2_s->longitude);
		// y-intercept for line 2
		l2_n = l2_s->latitude - l2_s->longitude*l2_m;
		
		if (fabs(l1_m) == fabs(l2_m)) {
			// lines are in parallel, check if absolute value of slope is equal
			if (l1_n == l2_n) {
				// lines are on top of each other, if y-intercept is equal
				return 1;
			} else {
				// lines horizontally run in parallel and never meet
				return 0;
			}
			
		}
		
		// solving equation for x: l1_m*x + l1_n = l2_m*x + l2_n <==> x = (l2_n - l1_n) / (l1_m - l2_m)
		intersect_x = (l2_n - l1_n) / (l1_m - l2_m);
		
		// use intersect_x to calculate intersect_y
		intersect_y = l1_m * intersect_x + l1_n;
		// also determine intersect_y(_2) using other equation
		double intersect_y_2 = l2_m * intersect_x + l2_n;
		
		// check, if results are equal
		if (fabs( intersect_y - intersect_y_2 ) > 1.0E-10){
			return 0;
		}
	}
	
	printf("TEST: intersect_x is %f\n", intersect_x);
	printf("TEST: intersect_y is %f\n", intersect_y);
	
	/*
	 * Point has to be in "box".
	 * intersect_x has to be in between the start and end points of both lines. Same for intercept_y. Otherwise both lines intercept, but that would be at the extension of both lines.
	 */
	if (
		fmin(l1_s->longitude,l1_e->longitude) <= intersect_x
		&& intersect_x <= fmax(l1_s->longitude,l1_e->longitude)
		&& fmin(l2_s->longitude,l2_e->longitude) <= intersect_x
		&& intersect_x <= fmax(l2_s->longitude,l2_e->longitude)
		&& fmin(l1_s->latitude,l1_e->latitude) <= intersect_y
		&& intersect_y <= fmax(l1_s->latitude,l1_e->latitude)
		&& fmin(l2_s->latitude,l2_e->latitude) <= intersect_y
		&& intersect_y <= fmax(l2_s->latitude,l2_e->latitude)
		) {
		
		result->latitude = intersect_y;
		result->longitude = intersect_x;
		return 1;
	} 
	
	return 0;
}
