/*
 *  atlas_shape_impl_geometry.c
 *  atlas
 *
 *  Created by Peter Fenske on 13.04.10.
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
#include <stdint.h>
#include <float.h>
#include <math.h>

#include "atlas_shape_impl.h"
#include "atlas_shape_impl_geometry.h"

const double PRECISION = 1.0E-15;


int
atlas_shape_lines_intersect(atlas_shp_coordinate_t * result,
                            atlas_shp_coordinate_t * l1_s,
                            atlas_shp_coordinate_t * l1_e,
                            atlas_shp_coordinate_t * l2_s,
                            atlas_shp_coordinate_t * l2_e) {
    
	// TODO: Adapt function for use with spheres.
	
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
	
	if ( ((l1_e->longitude - l1_s->longitude) == 0)  &&
         ((l2_e->longitude - l2_s->longitude) == 0)) {
        
		/*
		 * Both lines are vertical. In this case they are either on top of each 
		 * other or they never meet
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
		l2_m = (l2_e->latitude - l2_s->latitude) / (l2_e->longitude - l2_s->longitude);
		// y-intercept for line 2		
		l2_n = l2_s->latitude - l2_s->longitude*l2_m;
		
		intersect_y = l2_m * intersect_x + l2_n;
		
	} else if ((l2_e->longitude - l2_s->longitude) == 0) {
		// prevent division by zero, line 2 is vertical
		intersect_x = l2_e->longitude;
		
		// slope for line 1
		l1_m = (l1_e->latitude - l1_s->latitude) / (l1_e->longitude - l1_s->longitude);
		// y-intercept for line 1
		l1_n = l1_s->latitude - l1_s->longitude*l1_m;
		
		intersect_y = l1_m * intersect_x + l1_n;
		
	} else  {
		/*
		 * Last case, neither line 1 or line 2 are vertical
		 */
		
		// slope for line 1
		l1_m = (l1_e->latitude - l1_s->latitude) / (l1_e->longitude - l1_s->longitude);
		// y-intercept for line 1
		l1_n = l1_s->latitude - l1_s->longitude*l1_m;
		
		// slope for line 2
		l2_m = (l2_e->latitude - l2_s->latitude) / (l2_e->longitude - l2_s->longitude);
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
		
		/*
		 * solving equation for x: 
		 * l1_m*x + l1_n = l2_m*x + l2_n <==> x = (l2_n - l1_n) / (l1_m - l2_m)
		 */
		intersect_x = (l2_n - l1_n) / (l1_m - l2_m);
		
		// use intersect_x to calculate intersect_y
		intersect_y = l1_m * intersect_x + l1_n;
		// also determine intersect_y(_2) using other equation
		double intersect_y_2 = l2_m * intersect_x + l2_n;
		
		// check, if results are equal
		if (fabs( intersect_y - intersect_y_2 ) > PRECISION){
			return 0;
		}
	}
	
	/*
	 * Point has to be in "box".
	 * intersect_x has to be in between the start and end points of both lines. 
	 * Same for intercept_y. Otherwise both lines intercept, but that would be 
	 * at the extension of both lines.
	 */
	if (fmin(l1_s->longitude,l1_e->longitude) <= intersect_x &&
        intersect_x <= fmax(l1_s->longitude,l1_e->longitude) &&
        fmin(l2_s->longitude,l2_e->longitude) <= intersect_x &&
        intersect_x <= fmax(l2_s->longitude,l2_e->longitude) &&
        fmin(l1_s->latitude,l1_e->latitude) <= intersect_y &&
        intersect_y <= fmax(l1_s->latitude,l1_e->latitude) &&
        fmin(l2_s->latitude,l2_e->latitude) <= intersect_y &&
        intersect_y <= fmax(l2_s->latitude,l2_e->latitude)) {
		
        result->latitude = intersect_y;
		result->longitude = intersect_x;
        
		return 1;
	} else {	
        return 0;
    }
}

int
atlas_shape_point_equal(atlas_shp_coordinate_t * coord1, 
                        atlas_shp_coordinate_t * coord2){
	if ((fabs(coord1->latitude - coord2->latitude) > PRECISION) ||
        (fabs(coord1->longitude - coord2->longitude) > PRECISION)) {
		return 0;
	} else {
		return 1;
	}
}


int
atlas_shape_arc_equal(atlas_shp_coordinate_t * coords1, 
                      uint16_t num_coords1,
                      atlas_shp_coordinate_t * coords2,
                      uint16_t num_coords2){
	
	// TODO: Adapt for use with spheres (see lines_intersect)
	
	/*
	 * First case: The starting and ending point have to be equal, if not, 
	 no further checks are necessary
	 */
	if (!atlas_shape_point_equal(coords1, coords2) ||
        !atlas_shape_point_equal(coords1 + num_coords1-1, coords2 + num_coords2-1)) {
		return 0;
	}
	
	// create a temporary pointers back to where the coordinates begin
	atlas_shp_coordinate_t * tmp_c1_ptr = coords1;
	atlas_shp_coordinate_t * tmp_c2_ptr = coords2;
	
	// each point of the first arc must be on a line segment of the second arc, if not return 0
	for (int i = 0; i < num_coords1; i++) {
		for (int j = 0; j < num_coords2 - 1; j++) {
			// check, if the current point (i) is on line between j and j+1
			if (atlas_shape_pol(tmp_c1_ptr, tmp_c2_ptr, tmp_c2_ptr + 1) == 0) {
				// if not, done
				return 0;
			}
			// advance pointer to check next line segment
			tmp_c2_ptr++;
		}
		/*
		 * Once all line segments are checked with the first point, continue 
		 * with next point. The pointer for the coordinate pair to create a line 
		 * segment has to be set back to the start of the coordinate array.
		 */
		tmp_c2_ptr = coords2;
		// Once all inside loops are finished, advance pointer to next point
		tmp_c1_ptr++;
	}
	// also reset pointer after the outer loop is completed
	tmp_c1_ptr = coords1;
	
	
	/*
	 * The following lop is completely analog to the previous one. The 
	 * difference is, that the points are not taken from the first arc, but from 
	 * the second one. These points are checked to be on a line segment of arc 1.
	 */
	for (int i = 0; i < num_coords2; i++) {
		for (int j = 0; j < num_coords1 - 1; j++) {
			if (atlas_shape_pol(tmp_c2_ptr, tmp_c1_ptr, tmp_c1_ptr + 1) == 0) {
				return 0;
			}
			tmp_c1_ptr++;
		}
		tmp_c1_ptr = coords1;
		tmp_c2_ptr++;
	}
	tmp_c2_ptr = coords2;
	
	// if all checks passed, return 1
	return 1;
}	


int
atlas_shape_pol(atlas_shp_coordinate_t * point,
                atlas_shp_coordinate_t * l1,
                atlas_shp_coordinate_t * l2) {
	
	// TODO: adapt for use with spheres.
	
	if ((l1->longitude - l2->longitude) == 0 && point->longitude == l1->longitude){
		/*
		 * prevent division by zero, line is vertical, latitude of point does 
		 * not matter anymore, only the longtitude has to be equal
		 */
		return 1;
	}
	
	// slope for line
	double m = (l2->latitude - l1->latitude) / (l2->longitude - l1->longitude);
	// y-intercept for line
	double n = l1->latitude - l1->longitude*m;
	// subsitute x in line equation
	double f_of_x = m*point->longitude + n;
	// if result of function is equal to the y of the point, the point is on the line
	if (fabs(f_of_x - point->latitude) < PRECISION) {
		return 1;
	}
	
	return 0;
}


int
atlas_shape_polygon_equal(atlas_shp_coordinate_t * coords1, 
                          uint16_t num_coords1,
                          atlas_shp_coordinate_t * coords2,
                          uint16_t num_coords2) {
	return 0;
}


