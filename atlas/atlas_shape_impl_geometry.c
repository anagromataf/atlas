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


// ========================================
// http://math.rice.edu/~pcmi/sphere/
// http://www.gfy.ku.dk/~iag/HB2000/part4/groten.htm
// http://de.wikipedia.org/wiki/Geozentrische_Breite
// http://en.wikipedia.org/wiki/Latitude#Geocentric_latitude

#pragma mark -
#pragma mark Constants for the flattening of earth

#define FLATTENING (1.0 / 298.257223563)
#define FLATTENING_C ((1.0 - FLATTENING) * (1.0 - FLATTENING))

#pragma mark -
#pragma mark Declaration of Private Functions

typedef struct atlas_shp_coord_vector_s {
	double x;
	double y;
	double z;
} atlas_shp_coord_vector_t;

// Private Functions
int lon_range_overlaps(atlas_shp_coordinate_t * coord11,
					   atlas_shp_coordinate_t * coord12,
					   atlas_shp_coordinate_t * coord21,
					   atlas_shp_coordinate_t * coord22);

int lat_range_overlaps(double * given_min_1,
					   double * given_max_1,
					   atlas_shp_coordinate_t * given_min_2,
					   atlas_shp_coordinate_t * given_max_2);

int lat_range_gc_seg(double * result_min,
					 double * result_max,
					 atlas_shp_coordinate_t * coord1,
					 atlas_shp_coordinate_t * coord2);

int initial_course(double * hdg_result,
				   atlas_shp_coordinate_t * coord1,
				   atlas_shp_coordinate_t * coord2);

int is_meridian(atlas_shp_coordinate_t * c1,
				atlas_shp_coordinate_t * c2);


int is_point_on_meridian_segment(atlas_shp_coordinate_t * point,
								 atlas_shp_coordinate_t * c1,
								 atlas_shp_coordinate_t * c2);


// Converter Functions
static inline double degree_to_radian(double degree);
static inline double radian_to_degree(double radian);

static inline double geographic_latitude(double geocentric_lat);
static inline double geocentric_latitude(double geographic_lat);


// Converter Functions between Spherical and Cartesian Coordinates
static int sph_to_cart_gc(atlas_shp_coord_vector_t * result,
                          atlas_shp_coordinate_t * coord);

static int cart_to_sph_gc(atlas_shp_coordinate_t * result,
                          atlas_shp_coord_vector_t * coord);


// Vector Operations
int is_vectors_equal(atlas_shp_coord_vector_t * p1, 
					 atlas_shp_coord_vector_t * p2);

static inline void antipode(atlas_shp_coord_vector_t * result,
                            atlas_shp_coord_vector_t * coord);

int is_antipodal(atlas_shp_coord_vector_t * p1, 
				 atlas_shp_coord_vector_t * p2);

static int cross_normalize(atlas_shp_coord_vector_t * result,
                           atlas_shp_coord_vector_t * coord1,
                           atlas_shp_coord_vector_t * coord2);

double vectors_angle(atlas_shp_coordinate_t * c1,
					 atlas_shp_coordinate_t * c2);



#pragma mark -
#pragma mark Spherical Operations

int
atlas_shape_gc_intersection(atlas_shp_coordinate_t * result1,
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
	
	// convert from spherical to cartesian coordinates
	struct atlas_shp_coord_vector_s l1_s_v;
	sph_to_cart_gc(&l1_s_v, l1_s);
	
	struct atlas_shp_coord_vector_s l1_e_v;
	sph_to_cart_gc(&l1_e_v, l1_e);
	
	struct atlas_shp_coord_vector_s l2_s_v;
	sph_to_cart_gc(&l2_s_v, l2_s);
	
	struct atlas_shp_coord_vector_s l2_e_v;
	sph_to_cart_gc(&l2_e_v, l2_e);
    
	
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
	
	// convert cartesian coordinates of intersections to spherical coordinates
	int cs1 = cart_to_sph_gc(result1, &intersection1);
	if (cs1 != 0) {
		return cs1;
	}
    
    // antipodal point
	struct atlas_shp_coord_vector_s intersection2;
	antipode(&intersection2, &intersection1);
    
	//atlas_shp_coordinate_t intersect2_rad;
	int cs2 = cart_to_sph_gc(result2, &intersection2);
	if (cs2 != 0) {
		return cs2;
	}
    
	return 0;
}


int
atlas_shape_gc_segments_intersect(atlas_shp_coordinate_t * result,
								  atlas_shp_coordinate_t * coord11,
								  atlas_shp_coordinate_t * coord12,
								  atlas_shp_coordinate_t * coord21,
								  atlas_shp_coordinate_t * coord22) {
	
	if (is_meridian(coord11, coord12) &&
		is_meridian(coord21, coord22) &&
		lon_range_overlaps(coord11, coord12, coord21, coord21) &&
		(coord11->longitude == coord21->longitude || 
		 coord11->longitude == coord21->longitude-180.0 || 
		 coord11->longitude == coord21->longitude+180.0) ) {
		/*
		 * Both segments are meridians and their longitude ranges overlap.
		 * The longitude ranges have to overlap. Otherwise there is no possible
		 * intersection.
		 * Also the longitudes must be equal or opposite (+- 180 deg) for
		 * both meridian segments to be on the same great circle.
		 */
		
		/*
		 * In case of two identical meridians the intersection of the two
		 * great circles returns both poles. However, now there will be a check,
		 * if one point of the first segment is on the other segment. If so,
		 * this point is returned.
		 */
		
		if (is_point_on_meridian_segment(coord11, coord21, coord22)) {
			*result = *coord11;
			return 1;
		} else if (is_point_on_meridian_segment(coord12, coord21, coord22)) {
			*result = *coord12;
			return 1;
		} else {
			return 0;
		}

	}
	
	// Variables to hold both points of intersection
	atlas_shp_coordinate_t intersection1;
	atlas_shp_coordinate_t intersection2;
	
	// Find both intersections of the two great circles.
	int intersect_result =
	atlas_shape_gc_intersection(&intersection1, &intersection2,
								coord11, coord12,
								coord21, coord22);
	
	/*
	 * Next four statements:
	 * Check, if the points of intersection are within the longitude range
	 * of both great circle segments.
	 */
	int lon_range_result_11 = lon_range_overlaps(coord11, coord12, 
												 &intersection1, &intersection1);
	
	int lon_range_result_12 = lon_range_overlaps(coord11, coord12, 
												 &intersection2, &intersection2);
	
	int lon_range_result_21 = lon_range_overlaps(coord21, coord22, 
												 &intersection1, &intersection1);
	
	int lon_range_result_22 = lon_range_overlaps(coord21, coord22, 
												 &intersection2, &intersection2);
	
	
	// Determine latitude range of first great circle
	double lat_range_min_1;
	double lat_range_max_1;
	lat_range_gc_seg(&lat_range_min_1, &lat_range_max_1, coord11, coord12);
	
	/*
	 * Check for both points of intersection, if they are within the
	 * latitude range.
	 */
	int lat_range_result_11 = lat_range_overlaps(&lat_range_min_1, 
												 &lat_range_max_1, 
												 &intersection1, 
												 &intersection1);
	
	int lat_range_result_12 = lat_range_overlaps(&lat_range_min_1, 
												 &lat_range_max_1, 
												 &intersection2, 
												 &intersection2);
	
	
	// Determine latitude range of second great circle
	double lat_range_min_2;
	double lat_range_max_2;
	lat_range_gc_seg(&lat_range_min_2, &lat_range_max_2, coord21, coord22);
	
	/*
	 * Check for both points of intersection, if they are within the
	 * latitude range.
	 */
	int lat_range_result_21 = lat_range_overlaps(&lat_range_min_2, 
												 &lat_range_max_2, 
												 &intersection1, 
												 &intersection1);
	
	int lat_range_result_22 = lat_range_overlaps(&lat_range_min_2, 
												 &lat_range_max_2, 
												 &intersection2, 
												 &intersection2);
	
	
	/*
	 * Depending on which ranges either of the points of intersection are in,
	 * the first XOR the second point are the intended result.
	 *
	 * Though the special case of meridians can still occur.
	 */
	int which_point = 0;		   
	
	if (lon_range_result_11 && 
		lon_range_result_21 &&
		lat_range_result_11 &&
		lat_range_result_21) {
		
		which_point = 1;		
		
	} else if (lon_range_result_12 && 
			   lon_range_result_22 &&
			   lat_range_result_12 &&
			   lat_range_result_22) {
		
		which_point = 2;
		
	}
	
	
	// Check for both segments if they are meridians
	int gc_seg_1_is_meridian = is_meridian(coord11, coord12);
	int gc_seg_2_is_meridian = is_meridian(coord21, coord22);	
	
	/*
	 * Next four statements:
	 * For each of the two points check, if they are on a meridian.
	 * This is necessary, because a point may be within the latitude range
	 * of a meridian passing a pole, but not necessarily within the segment.
	 */
	int p1_is_on_seg_1_meridian = 
	is_point_on_meridian_segment(&intersection1, coord11, coord12);
	
	int p2_is_on_seg_1_meridian = 
	is_point_on_meridian_segment(&intersection2, coord11, coord12);
	
	int p1_is_on_seg_2_meridian = 
	is_point_on_meridian_segment(&intersection1, coord21, coord22);
	
	int p2_is_on_seg_2_meridian = 
	is_point_on_meridian_segment(&intersection2, coord21, coord22);
	
	
	if ( (gc_seg_1_is_meridian == 1 && 
		  which_point == 1 && 
		  p1_is_on_seg_1_meridian == 1) || 
		(gc_seg_2_is_meridian == 1 && 
		 which_point == 1 && 
		 p1_is_on_seg_2_meridian == 1)) {
			/*
			 * first segment is meridian, first point, 
			 * and first point on first segment
			 * OR
			 * second segment is meridian, first point, 
			 * and first point on second segment
			 */
			
			*result = intersection1;
			return 1;
			
		} else if ( (gc_seg_1_is_meridian == 1 && 
					 which_point == 2 && 
					 p2_is_on_seg_1_meridian == 1) || 
				   (gc_seg_2_is_meridian == 1 && 
					which_point == 2 && 
					p2_is_on_seg_2_meridian == 1)) {
					   /*
						* first segment is meridian, second point, 
						* and second point on first segment
						* OR
						* second segment is meridian, second point, 
						* and second point on second segment
						*/
					   
					   *result = intersection2;
					   return 1;
					   
				   }  else if (p1_is_on_seg_1_meridian == 1 && 
							   p1_is_on_seg_2_meridian == 1 && 
							   which_point == 1) {
					   /*
						* Both segments are meridians,
						* and first point is intersection of both
						*/
					   
					   *result = intersection1;
					   return 1;
					   
				   } else if (p2_is_on_seg_1_meridian == 1 && 
							  p2_is_on_seg_2_meridian == 1 && 
							  which_point == 2) {
					   /*
						* Both segments are meridians,
						* and second point is intersection of both
						*/
					   
					   *result = intersection2;
					   return 1;
					   
				   } else if (gc_seg_1_is_meridian == 0 && 
							  gc_seg_2_is_meridian == 0 &&
							  which_point == 1) {
					   /*
						* Neither segment is a meridian, but before was determined, 
						* that the first point is an actual intersection.
						*/
					   
					   *result = intersection1;
					   return 1;
					   
				   } else if (gc_seg_1_is_meridian == 0 && 
							  gc_seg_2_is_meridian == 0 && 
							  which_point == 2) {
					   /*
						* Neither segment is a meridian, but before it was determined, 
						* that the second point is an actual intersection.
						*/
					   
					   *result = intersection2;
					   return 1;
				   }
	
	return 0;
}

/*! Checks if two coordinates are equal
 *
 * \param coord1 coordinate 1
 * \param coord2 coordinate 2
 *
 * \return 1 if equal, 0 otherwise
 */
int
atlas_shape_points_equal(atlas_shp_coordinate_t * coord1, 
                         atlas_shp_coordinate_t * coord2){
	if ((fabs(coord1->latitude - coord2->latitude) > SHAPE_PRECISION) ||
        (fabs(coord1->longitude - coord2->longitude) > SHAPE_PRECISION)) {
		return 0;
	} else {
		return 1;
	}
}

int
atlas_shape_arc_equal(atlas_shp_coordinate_t * coords1, 
                      uint16_t num_coords1,
                      atlas_shp_coordinate_t * coords2,
                      uint16_t num_coords2) {
	
	// TODO: Adapt for use with spheres (see lines_intersect)
	
	/*
	 * First case: The starting and ending point have to be equal, if not, 
	 no further checks are necessary
	 */
	if (!atlas_shape_points_equal(coords1, coords2) ||
        !atlas_shape_points_equal(coords1 + num_coords1-1, coords2 + num_coords2-1)) {
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
atlas_shape_polygon_equal(atlas_shp_coordinate_t * coords1, 
                          uint16_t num_coords1,
                          atlas_shp_coordinate_t * coords2,
                          uint16_t num_coords2) {
	// TODO: Implement
	return 0;
}


#pragma mark -
#pragma mark -
#pragma mark Private Functions


/*! Checks if two longitude ranges overlap.
 *
 * \param coord1 coordinate 1.1
 * \param coord2 coordinate 1.2
 * \param coord1 coordinate 2.1
 * \param coord2 coordinate 2.2
 *
 * \return zero (0) if no overlap, one (1) if overlap
 */
int
lon_range_overlaps(atlas_shp_coordinate_t * given_min_1,
				   atlas_shp_coordinate_t * given_max_1,
				   atlas_shp_coordinate_t * given_min_2,
				   atlas_shp_coordinate_t * given_max_2) {
	
	double min1 = given_min_1->longitude;
	double max1 = given_max_1->longitude;
	double min2 = given_min_2->longitude;
	double max2 = given_max_2->longitude;
	
	while (max1 < min1) {
		max1 = max1 + 360.0;
	}
	while (540.0 < min1 || 540.0 < max1) {
		min1 = min1 - 360.0;
		max1 = max1 - 360.0;
	}
	while (-180.0 > min1 || -180.0 > max1) {
		min1 = min1 + 360.0;
		max1 = max1 + 360.0;
	}
	
	while (max2 < min2) {
		max2 = max2 + 360.0;
	}
	while (540.0 < min2 || 540.0 < max2) {
		min2 = min2 - 360.0;
		max2 = max2 - 360.0;
	}
	while (-180.0 > min2 || -180.0 > max2) {
		min2 = min2 + 360.0;
		max2 = max2 + 360.0;
	}
	
	// Check for overlaps
	if ((min1 <= min2 && min2 <= max1) || 
		(min1 <= max2 && max2 <= max1) ||
		(min2 <= min1 && min1 <= max2) ||
		(min2 <= max1 && max1 <= max2) ) {
		return 1;
	}
	
	// Check for overlaps near the +/- 180 deg meridian
	if ((min1 <= min2+360.0 && min2+360.0 <= max1) || 
		(min1 <= max2+360.0 && max2+360.0 <= max1) ||
		(min2 <= min1+360.0 && min1+360.0 <= max2) ||
		(min2 <= max1+360.0 && max1+360.0 <= max2) ) {
		return 1;
	}
		
	return 0;
}

/*! Checks if two latitude ranges overlap.
 *
 * \param coord1 coordinate 1.1
 * \param coord2 coordinate 1.2
 * \param coord1 coordinate 2.1
 * \param coord2 coordinate 2.2
 *
 * \return zero (0) if no overlap, one (1) if overlap
 */
int
lat_range_overlaps(double * given_min_1,
				   double * given_max_1,
				   atlas_shp_coordinate_t * given_min_2,
				   atlas_shp_coordinate_t * given_max_2) {
	
	// TODO einer ist meridian über pol
	
	double min1 = *given_min_1;
	double max1 = *given_max_1;
	double min2 = given_min_2->latitude;
	double max2 = given_max_2->latitude;
	
	// Check for overlaps
	if ((min1 <= min2 && min2 <= max1) || 
		(min1 <= max2 && max2 <= max1) ||
		(min2 <= min1 && min1 <= max2) ||
		(min2 <= max1 && max1 <= max2) ) {
		return 1;
	} else {
		return 0;
	}	
}

/*! Computes the latitude range of a great circle segment
 *
 * \param result_min pointer to result variable, lowest latitude
 * \param result_max pointer to result variable, highest latitude
 * \param coord1 coordinate 1
 * \param coord2 coordinate 2
 *
 * \return zero (0) if no overlap, one (1) if overlap
 */
int 
lat_range_gc_seg(double * result_min,
				 double * result_max,
				 atlas_shp_coordinate_t * coord1,
				 atlas_shp_coordinate_t * coord2) {
	
	/*
	 * Convert both points from spherical to cartesian coordinates.
	 * This conversion is done here, because next there is a check if 
	 * both points are antipodal. Later this Cartesian coordinates are used
	 * for the vector cross product (in code near 'earths_axis').
	 */
	
	struct atlas_shp_coord_vector_s gc_start_v;
	sph_to_cart_gc(&gc_start_v, coord1);
	
	struct atlas_shp_coord_vector_s gc_end_v;
	sph_to_cart_gc(&gc_end_v, coord2);
	
	
	if (coord1->latitude == 0 && coord2->latitude == 0) {
		/*
		 * Check if great circle segment is segment of equator
		 */
		*result_min = 0;
		*result_max = 0;
		return 0;
		
	} else if (is_antipodal(&gc_start_v, &gc_end_v)) {
		/*
		 * If both points are antipodal, the imaginary plane representing the
		 * great circle could be rotated any way around the axis in between the
		 * two points. The minimum and maximum latitude would not be unique.
		 * Therefore the minimum and maximum latitude of the two points are
		 * used.
		 */
		*result_min = fmin(coord1->latitude, coord2->latitude);
		*result_max = fmax(coord1->latitude, coord2->latitude);
		return 0;
		
	} else if (is_meridian(coord1, coord2)) {
		
		if (coord1->longitude == coord2->longitude) {
			/*
			 * Case 1: Both points are on the same
			 * (half-circle) meridian. Then the min and max latitude correspond to 
			 * the latitudes of the points.
			 */
			/*
			 * the longitude range is the minimum and maximum latitude
			 */
			*result_min = fmin(coord1->latitude, coord2->latitude);
			*result_max = fmax(coord1->latitude, coord2->latitude);
			return 0;
		} else {
			/*
			 * Case 2: Both points are on opposite meridians, e.g. -10 and 170.
			 * In this case the segments pass a pole. Which one?
			 */
			
			// latitude range north pole to point 1
			double p1_diff_np = 90.0 - coord1->latitude;
			// latitude range north pole to point 2
			double p2_diff_np = 90.0 - coord2->latitude;
			
			// latitude range south pole to point 1
			double p1_diff_sp = fabs(-90.0 - coord1->latitude);
			// latitude range south pole to point 2
			double p2_diff_sp = fabs(-90.0 - coord2->latitude);
			
			if ((p1_diff_np + p2_diff_np) < 180.0) {
				/*
				 * Case 1: The sum of the differences in latitude to the north
				 * pole are less than 180 deg. This means, the great circle 
				 * segment passes over the north pole.
				 */
				*result_min = fmin(coord1->latitude, coord2->latitude);
				*result_max = 90.0;
				return 0;
				
			} else {
				/*
				 * Otherwise (if the sum of the differences in latitude to the 
				 * north pole are greater than 180 deg), the path across the 
				 * south pole is shorter.
				 */
				*result_min = -90.0;
				*result_max = fmax(coord1->latitude, coord2->latitude);
				return 0;
			}
		}		
	} // end of if (is_meridian(coord1, coord2))
	
	
	/*
	 * Define a vector (0,1,0) to represent the Earth's axis
	 */
	
	atlas_shp_coord_vector_t earths_axis = { 0.0 , 0.0 , 1.0 };
	
	/*
	 * Get normalized cross product of both points. This plane represents the great circle
	 * defined by both points.
	 */	
	struct atlas_shp_coord_vector_s cross_gc;
	int cn_gc = cross_normalize(&cross_gc, &gc_start_v, &gc_end_v);
	if (cn_gc != 0) {
		return cn_gc;
	}
	
	
	/*
	 * To find the angle in between the great circle and the Earth's axis
	 * both are intersected.
	 * For readability the numerator and the expressions within the denominator 
	 * are evaluated seperately.
	 */
	
	double numerator 
	= (cross_gc.x * earths_axis.x) + 
	(cross_gc.y * earths_axis.y) + 
	(cross_gc.z * earths_axis.z);
	
	double denom1 
	= sqrt(earths_axis.x * earths_axis.x + 
		   earths_axis.y * earths_axis.y + 
		   earths_axis.z * earths_axis.z);
	
	double denom2
	= sqrt(cross_gc.x * cross_gc.x + 
		   cross_gc.y * cross_gc.y + 
		   cross_gc.z * cross_gc.z);
	
	/*
	 * Phi is the angle in between the plane and the earth's axis.
	 */
	double phi = acos(numerator / (denom1 * denom2));
	
	phi = geographic_latitude(phi);
	
	// Convert phi back to degree
	phi = radian_to_degree(phi);
	
	//printf("phi = %f \n",phi);
		
	/*
	 * The following steps are used to determine the initial heading on the 
	 * great circle segment from each of the two points. Both initial headings 
	 * are used to see, if the point is the max/min latitude or if the angle
	 * great circle-earth's axis is the min/max latitude.
	 */
	
	double hdg1;
	int h1 = initial_course(&hdg1, coord1, coord2);
	//printf("hdg1 = %f \n",hdg1);

	double hdg2;
	int h2 = initial_course(&hdg2, coord2, coord1);
	//printf("hdg2 = %f \n",hdg2);
		
	
	if ((hdg1 < 90 || hdg1 > 270) && (hdg2 < 90 || hdg2 > 270)) {
		// hdg1 points north, hdg2 points north
		//printf("NORTH-NORTH \n");
		*result_min = fmin(coord1->latitude, coord2->latitude);
		*result_max = phi;
		
	} else if ((90 < hdg1 && hdg1 < 270) && (90 < hdg2 && hdg2 < 270)) {
		// hdg1 points south, hdg2 points south
		//printf("SOUTH-SOUTH \n");
		*result_min = -1 * phi;
		*result_max = fmax(coord1->latitude, coord2->latitude);
		
	} else if (
			   ((hdg1 < 90 || hdg1 > 270) && (90 < hdg2 && hdg2 < 270)) ||
			   ((90 < hdg1 && hdg1 < 270) && (hdg2 < 90 || hdg2 > 270))
				) {
		/*
		 * hdg1 points north, hdg2 points south
		 * OR
		 * hdg1 points south, hdg2 points north
		 */
		//printf("NORTH-SOUTH || SOUTH-NORTH \n");
		*result_min = fmin(coord1->latitude, coord2->latitude);
		*result_max = fmax(coord1->latitude, coord2->latitude);
		
	} 
	
	return 0;
}

/*! Determines the initial heading between two points.
 *
 * \param hdg_result pointer to variable for initial heading result
 * \param coord1 point 1
 * \param coord2 point 2
 *
 * \return zero (0)
 */
int 
initial_course(double * hdg_result,
			   atlas_shp_coordinate_t * coord1,
			   atlas_shp_coordinate_t * coord2) {
	
	// Convert coordinates to radians
	double lat1_rad = geocentric_latitude(degree_to_radian(coord1->latitude));
	double lon1_rad = degree_to_radian(coord1->longitude);
	double lat2_rad = geocentric_latitude(degree_to_radian(coord2->latitude));
	double lon2_rad = degree_to_radian(coord2->longitude);
	
	/*
	 * The arguments for the atan2 operation are evaluated seperately for
	 * readability.
	 */
	
	double y = sin(lon2_rad - lon1_rad) * cos(lat2_rad);
	
	double x = cos(lat1_rad) * sin(lat2_rad) 
				- sin(lat1_rad) * cos(lat2_rad) * cos(lon2_rad - lon1_rad);
	
	double hdg = fmod(atan2(y, x), 2 * M_PI);
		
	// Convert result back to degree
	hdg = radian_to_degree(hdg);
	
	// In case of a negative result make it positive.
	if (hdg < 0) {
		hdg = hdg + 360.0;
	}
	
	// Write result.
	*hdg_result = hdg;
	
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

/*! Checks if a point is on a great circle segment which is a meridian.
 *
 * \param point point to check
 * \param c1 point 1 of meridian
 * \param c2 point 2 of meridian
 *
 * \return 1 if meridian, 0 otherwise
 */
int
is_point_on_meridian_segment(atlas_shp_coordinate_t * point,
							 atlas_shp_coordinate_t * c1,
							 atlas_shp_coordinate_t * c2){
	
	if (!is_meridian(c1, c2)) {
		// if line is not a meridian, return 0
		return 0;
	}
	
	if (fabs(c1->longitude - c2->longitude) < SHAPE_PRECISION) {
		/*
		 * if both longitudes are equal, the meridian segment
		 * does not cross a pole
		 */
		
		double min_lat;
		double max_lat;
		
		min_lat = fmin(c1->latitude, c2->latitude);
		max_lat = fmax(c1->latitude, c2->latitude);
		
		/*
		 * Since no pole is crossed, the point's latitude must be within
		 * the range in between the two latitudes.
		 */
		if (point->latitude <= max_lat && point->latitude >= min_lat) {
			return 1;
		} else {
			return 0;
		}
	} else {
		/*
		 * In this case the meridian passes a pole. Which one does not matter,
		 * since by the use of the absolute value the northern hemisphere is 
		 * used.
		 */
		
		double point_angle = fabs(point->latitude);
		double gc_lat1 = fabs(c1->latitude);
		double gc_lat2 = fabs(c2->latitude);
		
		/*
		 * The latitude of the point in questions needs to be in between the 
		 * north pole and the latitude of the great circle segment endpoint
		 * for which the longitude is equal.
		 */
		if (fabs(c1->longitude - point->longitude) < SHAPE_PRECISION && 
			point_angle >= gc_lat1) {
			return 1;
		} else if (fabs(c2->longitude - point->longitude) < SHAPE_PRECISION && 
				   point_angle >= gc_lat2) {
			return 1;
		} else if (fabs(90.0 - point_angle) < SHAPE_PRECISION) {
			return 1;
		} else {
			return 0;
		}
	}	
}
										

#pragma mark -
#pragma mark Converter Functions

/*! Converts a degree value to radian.
 * 
 *  \param degree degree value
 *  \return radian value
 */
static inline double 
degree_to_radian(double degree) {
	return (degree * M_PI) / 180.0;
}


/*! Converts a radian value to degree.
 * 
 *  \param radian degree value
 *  \return degree value
 */
static inline double 
radian_to_degree(double radian) {
	return (radian * 180.0) / M_PI;
}


/*! Converts a given geocentric latitude value to a geographic value.
 * 
 *  \param geocentric_lat geocentric latitude
 *  \return geographic latitude
 */
static inline double 
geographic_latitude(double geocentric_lat) {
	/*
	 * Division by zero is not possible. Flattening is a constant positive value.
	 * tan(x) returns useful values for x=90°, 
	 * same for atan(0);
	 */
	return atan( tan(geocentric_lat) / FLATTENING_C );
}


/*! Converts a given geographic latitude value to a geocentric value.
 * 
 *  \param geocentric_lat geographic latitude
 *  \return geocentric latitude
 */
static inline double
geocentric_latitude(double geographic_lat) {
	/*
	 * tan(x) returns useful values for x=90°, 
	 * same for atan(0);
	 */
	return atan( tan(geographic_lat) * FLATTENING_C );
}


#pragma mark -
#pragma mark Converter Functions between Spherical and Cartesian Coordinates


/*! Converts a spherical coordinate to a Cartesian coordinate.
 *
 * Trigonometric functions do not cause problems here, because sin and cos
 * are defined for [-inf,+inf]
 *
 * \param result pointer to vector to hold Cartesian coordinate
 * \param coord pointer to spherical coordinate
 */
static int
sph_to_cart_gc(struct atlas_shp_coord_vector_s * result, 
			   atlas_shp_coordinate_t * coord) {
	
    double latitude_rad = degree_to_radian(coord->latitude);
    double longitude_rad = degree_to_radian(coord->longitude);
    
	double rlat = geocentric_latitude(latitude_rad);
	double c = cos(rlat);
	
	result->x = c * cos(longitude_rad);
	result->y = c * sin(longitude_rad);
	result->z = sin(rlat);
    
    return 0;
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
static int 
cart_to_sph_gc(atlas_shp_coordinate_t * result,
			   struct atlas_shp_coord_vector_s * coord) {
	
	if (fabs(coord->z) > 1.0) {
		return SHAPE_INVALID_PARAM;
	}
    
    double latitude_rad = geographic_latitude( asin(coord->z) );
    double longitude_rad = atan2(coord->y, coord->x);
    
	result->latitude = radian_to_degree(latitude_rad);
	result->longitude = radian_to_degree(longitude_rad);
	
	return 0;
}


#pragma mark -
#pragma mark Vector Operations

/*! Checks if two vectors are equal
 *
 * This can be used for vectors and therefore points on a sphere. If the 
 * components of vector 1 and the components of vector 2
 * are each equal, the vectors are equal.
 *
 * \param p1 vector 1
 * \param p2 vector 2
 *
 * \return 1 if equal, 0 otherwise
 */
int 
is_vectors_equal(atlas_shp_coord_vector_t * p1, atlas_shp_coord_vector_t * p2) {
	
	if (fabs(p1->x - p2->x) < SHAPE_PRECISION &&
		fabs(p1->y - p2->y) < SHAPE_PRECISION &&
		fabs(p1->z - p2->z) < SHAPE_PRECISION) {
		return 1;
	} else {
		return 0;
	}
	
}

/*! Calculates the antipodal point of the point given in cartesian coordinates.
 * 
 *  \param result pointer to target
 *  \param coord pointer to source
 */
void static inline 
antipode(struct atlas_shp_coord_vector_s * result,
		 struct atlas_shp_coord_vector_s * coord) {
	result->x = -1.0 * coord->x;
	result->y = -1.0 * coord->y;
	result->z = -1.0 * coord->z;
}

/*! Checks if two vectors are opposite
 *
 * This can be used for vectors and therefore points on a sphere. If the 
 * components of vector 1 multiplied by -1 and the components of vector 2
 * are each equal, the vectors are opposite.
 *
 * \param p1 vector 1
 * \param p2 vector 2
 *
 * \return 1 if opposite, 0 otherwise
 */
int 
is_antipodal(atlas_shp_coord_vector_t * p1, atlas_shp_coord_vector_t * p2) {
	
	atlas_shp_coord_vector_t p2_temp = { -1.0 * p2->x, -1.0 * p2->y, -1.0 * p2->z};
	
	if (fabs(p1->x - p2_temp.x) < SHAPE_PRECISION &&
		fabs(p1->y - p2_temp.y) < SHAPE_PRECISION &&
		fabs(p1->z - p2_temp.z) < SHAPE_PRECISION) {
		return 1;
	} else {
		return 0;
	}
	
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
static int
cross_normalize(struct atlas_shp_coord_vector_s * result,
				struct atlas_shp_coord_vector_s * coord1,
				struct atlas_shp_coord_vector_s * coord2) {
	
	double x = (coord1->y * coord2->z) - (coord1->z * coord2->y);
	double y = (coord1->z * coord2->x) - (coord1->x * coord2->z);
	double z = (coord1->x * coord2->y) - (coord1->y * coord2->x);
	
	double l = sqrt( x*x + y*y + z*z );
	
	if (fabs(l) < SHAPE_PRECISION) {
		return SHAPE_DIV_BY_ZERO;
	}
	
	result->x = x / l;
	result->y = y / l;
	result->z = z / l;
	
	return 0;
}

/*! Determines the angle in between a line/plane and a line/plane.
 *
 * Both a line and a plane can be given by their normal. This is used here.
 * Therefore the angle in between lines and planes can be determined.
 *
 * \param phi angle in between line/plane and line/plane
 * \param c1 normal 1
 * \param c2 normal 2
 *
 * \return Phi
 */
double 
vectors_angle(atlas_shp_coordinate_t * c1,
			  atlas_shp_coordinate_t * c2) {
	
	struct atlas_shp_coord_vector_s v1;
	sph_to_cart_gc(&v1, c1);
	
	struct atlas_shp_coord_vector_s v2;
	sph_to_cart_gc(&v2, c2);
	
	double numerator 
	= (v1.x * v2.x) + 
	(v1.y * v2.y) + 
	(v1.z * v2.z);
	
	double denom1 
	= sqrt(v2.x * v2.x + 
		   v2.y * v2.y + 
		   v2.z * v2.z);
	
	double denom2
	= sqrt(v1.x * v1.x + 
		   v1.y * v1.y + 
		   v1.z * v1.z);
	
	double phi = acos(numerator / (denom1 * denom2));
	
	// Convert phi back to degree
	phi = radian_to_degree(phi);
	printf("Phi (deg) = %f\n", phi);
	
	return phi;
}




#pragma mark -
#pragma mark Operations on Lines (not Great Circles)

int
atlas_shape_lines_intersection(atlas_shp_coordinate_t * result,
                               atlas_shp_coordinate_t * l1_s,
                               atlas_shp_coordinate_t * l1_e,
                               atlas_shp_coordinate_t * l2_s,
                               atlas_shp_coordinate_t * l2_e) {
    
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
		if (fabs( intersect_y - intersect_y_2 ) > SHAPE_PRECISION){
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
	if (fabs(f_of_x - point->latitude) < SHAPE_PRECISION) {
		return 1;
	}
	
	return 0;
}

