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
#define SHAPE_PRECISION 1.0E-15
#endif

// Error constants
// TODO: Define these constant in a separate file

#define SHAPE_DIV_BY_ZERO 1
#define SHAPE_INVALID_PARAM 2

/*! Checks for intersection of lines
 *
 *  Checks, if two lines intersect. Returns the point (single coordinate) if they 
 *  intersect, 1 if they lie on top of each other, 0 if they don't
 *
 *  \param result A pointer to where the coordinate of intersection is saved.
 *
 *  \param l1_s Starting coordinate of line 1.
 *	\param l1_e Ending coordinate of line 1.
 *
 *	\param l2_s Starting coordinate of line 2.
 *	\param l2_e Ending coordinate of line 2.
 *
 *  \return 0 if parallel and not on top of each other, 1 if intersecting or
 *			on top of each other
 */
int
atlas_shape_lines_intersection(atlas_shp_coordinate_t * result,
                               atlas_shp_coordinate_t * l1_s,
                               atlas_shp_coordinate_t * l1_e,
                               atlas_shp_coordinate_t * l2_s,
                               atlas_shp_coordinate_t * l2_e);


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


/*! Checks, if a point is located on a line.
 *
 *  \param point Coordinate for point.
 *	\param l1 Starting coordinate for line.
 *	\param l2 Ending coordinate for line.
 *
 *  \return 1 if point is on line, 0 otherwise.
 */
int
atlas_shape_pol(atlas_shp_coordinate_t * point,
                atlas_shp_coordinate_t * l1,
                atlas_shp_coordinate_t * l2);


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


#endif // _ATLAS_SHAPE_IMPL_GEOMETRY_H_


