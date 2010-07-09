/*
 *  atlas_shape_impl_geometry.h
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

#ifndef _ATLAS_SHAPE_IMPL_GEOMETRY_H_
#define _ATLAS_SHAPE_IMPL_GEOMETRY_H_

#ifndef SHAPE_PRECISION
#define SHAPE_PRECISION 1.0E-12
#endif

// Error constants
// TODO: Define these constant in a separate file

#define SHAPE_INVALID_PARAM 2
#define SHAPE_DIV_BY_ZERO 3




/*! Checks two points for equality
 * 
 *  \param coord1 Coordinate for point 1.
 *	\param coord2 Coordinate for point 2.
 *
 *  \return 1 if equal, 0 otherwise
 */
int
atlas_shape_points_equal(atlas_shp_coordinate_t * coord1, 
                         atlas_shp_coordinate_t * coord2);


/*! Checks for equality of two arcs.
 *
 *  \param coords1 Array of coordinates for arc 1.
 *	\param num_coords1 Number of coordinates for arc 1.
 *
 *  \param coords2 Array of coordinates for arc 2.
 *	\param num_coords2 Number of coordinates for arc 2.
 *
 *  \return 1 if equal, 0 otherwise.
 */
int
atlas_shape_arc_equal(atlas_shp_coordinate_t * coords1, 
                      uint16_t num_coords1,
                      atlas_shp_coordinate_t * coords2,
                      uint16_t num_coords2);



/*! Checks for equality of two polygons.
 *
 *  \param coords1 Array of coordinates for polygon 1.
 *	\param num_coords1 Number of coordinates for polygon 1.
 *
 *  \param coords2 Array of coordinates for polygon 2.
 *	\param num_coords2 Number of coordinates for polygon 2.
 *
 *  \return 1 if equal, 0 otherwise.
 */
int
atlas_shape_polygon_equal(atlas_shp_coordinate_t * coords1, 
                          uint16_t num_coords1,
                          atlas_shp_coordinate_t * coords2,
                          uint16_t num_coords2);


/*! Intersects two great circles defined by two coordinates (degree, spherical) each.
 *
 * When intersecting great circles two points are returned.
 *
 * \param result1 pointer for first result
 * \param result2 pointer for second result
 *
 * \param l1_s starting coordinate for first great circle
 * \param l1_e ending coordinate for first great circle
 *
 * \param l2_s starting coordinate for second great circle
 * \param l2_e ending coordinate for second great circle
 *
 * \return	non-zero value if an error occured or
 *			if both circles are on top of each other, 
 *			zero otherwise
 */
int
atlas_shape_gc_intersection(atlas_shp_coordinate_t * result1,
                            atlas_shp_coordinate_t * result2,
                            atlas_shp_coordinate_t * l1_s,
                            atlas_shp_coordinate_t * l1_e,
                            atlas_shp_coordinate_t * l2_s,
                            atlas_shp_coordinate_t * l2_e);


/*! Checks if two great circle segments intersect.
 *
 * If there is an intersection, 1 is returned, otherwise 0. If there is an 
 * intersection of two segments (one is not a meridian) the resulting point
 * of intersection is written to the result variable passed. If both segments
 * do not intersect, no value is written to the result variable.
 *
 * If both segments are segments of meridians on the identical great circle
 * and they overlap, 1 is returned and either the first or second coordinate
 * of the first segment which is located on the other segment is written to
 * the result. This means: if c11 is on (c21-c22) is returned, 
 * else if c12 is on (c21-c22) c12 is returned.
 *
 * \param result pointer to variable holding the point of intersection
 * of the two segments if this point exists. It is set to (0.0, 0.0) if
 * the segments do not intersect.
 * \param c11 point 1 for first segment
 * \param c12 point 2 for first segment
 * \param c21 point 1 for second segment
 * \param c22 point 2 for second segment
 *
 * \return 1 if the segments intersect, 0 otherwise
 */
int
atlas_shape_gc_segments_intersection(atlas_shp_coordinate_t * result,
									 atlas_shp_coordinate_t * coord11,
									 atlas_shp_coordinate_t * coord12,
									 atlas_shp_coordinate_t * coord21,
									 atlas_shp_coordinate_t * coord22);


/*! Checks if two longitude ranges overlap.
 *
 * \param origin_1 coordinate 1.1
 * \param destination_1 coordinate 1.2
 * \param origin_2 coordinate 2.1
 * \param destination_2 coordinate 2.2
 *
 * \return zero (0) if no overlap, one (1) if overlap
 */
int 
atlas_lon_range_overlaps(atlas_shp_coordinate_t * origin_1,
						 atlas_shp_coordinate_t * destination_1,
						 atlas_shp_coordinate_t * origin_2,
						 atlas_shp_coordinate_t * destination_2);

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
atlas_lat_range_overlaps(double * given_min_1,
						 double * given_max_1,
						 atlas_shp_coordinate_t * given_min_2,
						 atlas_shp_coordinate_t * given_max_2);

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
atlas_lat_range_gc_seg(double * result_min,
					   double * result_max,
					   atlas_shp_coordinate_t * coord1,
					   atlas_shp_coordinate_t * coord2);

/*! Determines the initial heading between two points.
 *
 * \param hdg_result pointer to variable for initial heading result
 * \param coord1 point 1
 * \param coord2 point 2
 *
 * \return zero (0)
 */
int 
atlas_gc_initial_course(double * hdg_result,
						atlas_shp_coordinate_t * coord1,
						atlas_shp_coordinate_t * coord2);


#endif // _ATLAS_SHAPE_IMPL_GEOMETRY_H_


