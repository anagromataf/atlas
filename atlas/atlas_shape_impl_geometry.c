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
#include "atlas_logging_impl.h"

const double PRECISION = 1.0E-15;

// ========================================
// http://math.rice.edu/~pcmi/sphere/
// http://www.gfy.ku.dk/~iag/HB2000/part4/groten.htm
// http://de.wikipedia.org/wiki/Geozentrische_Breite
// http://en.wikipedia.org/wiki/Latitude#Geocentric_latitude

const double FLATTENING = 1.0 / 298.257223563; 

struct atlas_shp_coord_vector_s {
	double x;
	double y;
	double z;
};

/*! Converts a degree value to radian.
 * 
 *  \param degree degree value
 *  \return radian value
 */
double 
degree_to_radian(double degree){
	return (degree * M_PI) / 180.0;
}

/*! Converts a radian value to degree.
 * 
 *  \param radian degree value
 *  \return degree value
 */
double 
radian_to_degree(double radian){
	return (radian * 180.0) / M_PI;
}

/*! Factor for flattening of earth.
 * 
 *  \return flattening factor
 */
double 
flattening_c(){
	return ( (1.0 - FLATTENING) * (1.0 - FLATTENING) );
}

/*! Converts a given geocentric latitude value to a geographic value.
 * 
 *  \param geocentric_lat geocentric latitude
 *  \return geographic latitude
 */
double 
geographic_latitude(double geocentric_lat){
	/*
	 * Division by zero is not possible. Flattening is a constant positive value.
	 * tan(x) returns useful values for x=90°, 
	 * same for atan(0);
	 */
	return atan( tan(geocentric_lat) / flattening_c() );
}

/*! Converts a given geographic latitude value to a geocentric value.
 * 
 *  \param geocentric_lat geographic latitude
 *  \return geocentric latitude
 */
double
geocentric_latitude(double geographic_lat){
	/*
	 * tan(x) returns useful values for x=90°, 
	 * same for atan(0);
	 */
	return atan( tan(geographic_lat) * flattening_c() );
}

/*! Calculates the antipodal point of the point given in cartesian coordinates.
 * 
 *  \param result pointer to target
 *  \param coord pointer to source
 */
void 
antipode(struct atlas_shp_coord_vector_s * result,
		 struct atlas_shp_coord_vector_s * coord) {
	result->x = -1.0 * coord->x;
	result->y = -1.0 * coord->y;
	result->z = -1.0 * coord->z;
}

/*! Converts a coordinate in degree to radian.
 * 
 *  \param source coordinate in degree
 *  \return coordinate in radian
 */
atlas_shp_coordinate_t
init_coord_rad(atlas_shp_coordinate_t * source) {
	atlas_shp_coordinate_t result;
	
	result.latitude = degree_to_radian(source->latitude);
	result.longitude = degree_to_radian(source->longitude);
	
	DBG("GCI (rad): lat=%f lon=%f\n", result.latitude, result.longitude);
	
	return result;
}

/*! Converts a spherical coordinate to a Cartesian coordinate.
 *
 * Trigonometric functions do not cause problems here, because sin and cos
 * are defined for [-inf,+inf]
 *
 * \param result pointer to vector to hold Cartesian coordinate
 * \param coord pointer to spherical coordinate
 */
void
sph_to_cart_gc(struct atlas_shp_coord_vector_s * result, 
			   atlas_shp_coordinate_t * coord) {
	
	double rlat = geocentric_latitude(coord->latitude);
	double c = cos(rlat);
	
	result->x = c * cos(coord->longitude);
	result->y = c * sin(coord->longitude);
	result->z = sin(rlat);
	
	DBG("GCI (sph-car): lat=%f lon=%f x=%f y=%f z=%f\n",
		   coord->latitude, coord->longitude, result->x, result->y, result->z);
}

/*! Converts a Cartesian coordinate to a spherical coordinate.
 *
 * atan2 is defined for all possible values.
 * asin would fail if the argument has an absolute value larger than 1.
 *
 * \param result pointer to vector to hold spherical coordinate
 * \param coord pointer to Cartesian coordinate
 *
 * \return 1 if argument for asin is larger than abs(1), 0 otherwise.
 */
int 
cart_to_sph_gc(atlas_shp_coordinate_t * result,
			   struct atlas_shp_coord_vector_s * coord) {
	
	if (fabs(coord->z) > 1.0) {
		return SHAPE_INVALID_PARAM;
	}
	result->latitude = geographic_latitude( asin(coord->z) );
	result->longitude = atan2(coord->y, coord->x);
	
	DBG("GCI (car-sph): lat=%f lon=%f x=%f y=%f z=%f\n",
		   result->latitude, result->longitude, coord->x, coord->y, coord->z);
	
	return 0;
}

/*! Calculates the cross product of two vectors and normalizes the result.
 *
 * Both vectors are given in Cartesian coordinates. The intermediate result, 
 * cross product of both, is then normalized by the length of the vector
 * representing the cross product.
 *
 * \param result pointer to vector holding the normalized cross product
 * \param coord1 vector 1
 * \param coord2 vector 2
 *
 * \return non-zero value if an error occured, zero otherwise
 */
int
cross_normalize(struct atlas_shp_coord_vector_s * result,
				struct atlas_shp_coord_vector_s * coord1,
				struct atlas_shp_coord_vector_s * coord2) {
	
	double x = (coord1->y * coord2->z) - (coord1->z * coord2->y);
	double y = (coord1->z * coord2->x) - (coord1->x * coord2->z);
	double z = (coord1->x * coord2->y) - (coord1->y * coord2->x);
	
	double L = sqrt( x*x + y*y + z*z );
	
	if (fabs(L) < PRECISION) {
		return SHAPE_DIV_BY_ZERO;
	}
	
	result->x = x / L;
	result->y = y / L;
	result->z = z / L;
	
	DBG("GCI (cross-l): l=%f \n", L);
	DBG("GCI (cross): x=%f y=%f z=%f\n",
		   result->x, result->y, result->z);
	
	return 0;
}

/*! Checks if a line/great circle defined by two coordinates is a meridian.
 *
 * If both latitudes are equal, the line/great circle is a meridian.
 * A circle passing the north pole, i.e. 80E 20N - 100W 20N is also a meridian.
 *
 * \param coord1 vector 1
 * \param coord2 vector 2
 *
 * \return 1 if meridian, 0 otherwise
 */
int
is_meridian(atlas_shp_coordinate_t * c1,
			atlas_shp_coordinate_t * c2) {
	if (c1->longitude == c2->longitude ||
		c1->longitude + 180.0 == c2->longitude ||
		c1->longitude == c2->longitude + 180.0) {
		return 1;
	} else {
		return 0;
	}

}

int
atlas_shape_lines_intersect_gc(atlas_shp_coordinate_t * result1,
							   atlas_shp_coordinate_t * result2,
							   atlas_shp_coordinate_t * l1_s,
							   atlas_shp_coordinate_t * l1_e,
							   atlas_shp_coordinate_t * l2_s,
							   atlas_shp_coordinate_t * l2_e) {
	
	
	// check, if both great circles are meridians
	if ( is_meridian(l1_s, l1_e) && is_meridian(l2_s, l2_e) ) {
		result1->latitude = 90.0;
		result1->longitude = 0.0;
		result2->latitude = -90.0;
		result2->longitude = 0.0;
		
		return 0;
	}
	
	// convert coordinates to radians
	atlas_shp_coordinate_t l1_s_rad = init_coord_rad(l1_s);
	atlas_shp_coordinate_t l1_e_rad = init_coord_rad(l1_e);
	atlas_shp_coordinate_t l2_s_rad = init_coord_rad(l2_s);
	atlas_shp_coordinate_t l2_e_rad = init_coord_rad(l2_e);
	
	// convert from spherical to cartesian coordinates
	struct atlas_shp_coord_vector_s l1_s_v;
	sph_to_cart_gc(&l1_s_v, &l1_s_rad);
	
	struct atlas_shp_coord_vector_s l1_e_v;
	sph_to_cart_gc(&l1_e_v, &l1_e_rad);
	
	struct atlas_shp_coord_vector_s l2_s_v;
	sph_to_cart_gc(&l2_s_v, &l2_s_rad);
	
	struct atlas_shp_coord_vector_s l2_e_v;
	sph_to_cart_gc(&l2_e_v, &l2_e_rad);
		
	
	// normalized cross product for first great circle		
	struct atlas_shp_coord_vector_s cross1;
	int cn1 = cross_normalize(&cross1, &l1_s_v, &l1_e_v);
	if (cn1 != 0) {
		return cn1;
	}
	
	// normalized cross product for second great circle
	struct atlas_shp_coord_vector_s cross2;
	int cn2 = cross_normalize(&cross2, &l2_s_v, &l2_e_v);
	if (cn2 != 0) {
		return cn2;
	}
		
	// normalized cross product of both great circles, gives intersection
	struct atlas_shp_coord_vector_s intersection1;
	int cn3 = cross_normalize(&intersection1, &cross1, &cross2);
	if (cn3 != 0) {
		/*
		 * If SHAPE_DIV_BY_ZERO 1 is returned here, the length of the vector
		 * was 0. In this case both great circles are equal.
		 */
		return cn3;
	}
	
	// antipodal point
	struct atlas_shp_coord_vector_s intersection2;
	antipode(&intersection2, &intersection1);
	
	// convert cartesian coordinates of intersections to spherical coordinates
	atlas_shp_coordinate_t intersect1_rad;
	int cs1 = cart_to_sph_gc(&intersect1_rad, &intersection1);
	if (cs1 != 0) {
		return cs1;
	}
		
	atlas_shp_coordinate_t intersect2_rad;
	int cs2 = cart_to_sph_gc(&intersect2_rad, &intersection2);
	if (cs2 != 0) {
		return cs2;
	}
	
	// copy coordinates of intersections to result1/2
	result1->latitude = radian_to_degree(intersect1_rad.latitude);
	result1->longitude = radian_to_degree(intersect1_rad.longitude);
	result2->latitude = radian_to_degree(intersect2_rad.latitude);
	result2->longitude = radian_to_degree(intersect2_rad.longitude);
	
	return 0;
}




// ========================================

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


