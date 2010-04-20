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

/*! Checks for intersection of lines
 *
 * Checks, if two lines intersect. Returns the point (single coordinate) if they 
 * intersect, 1 if they lie on top of each other, 0 if they don't
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
int atlas_shape_lines_intersect(struct atlas_shape_coordinate_s * result,
								struct atlas_shape_coordinate_s * l1_s,
								struct atlas_shape_coordinate_s	* l1_e,
								struct atlas_shape_coordinate_s * l2_s,
								struct atlas_shape_coordinate_s	* l2_e);

/*! Checks two points for equality
 * 
 *  \param coord1 Coordinate for point 1.
 *	\param coord2 Coordinate for point 2.
 *
 *  \return 1 if equal, 0 otherwise
 */
int atlas_shape_point_equal(struct atlas_shape_coordinate_s * coord1, 
							struct atlas_shape_coordinate_s * coord2);

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
int atlas_shape_arc_equal(struct atlas_shape_coordinate_s * coords1, 
						  uint16_t num_coords1,
						  struct atlas_shape_coordinate_s * coords2,
						  uint16_t num_coords2);

/*! Checks, if a point is located on a line.
 *
 *  \param point Coordinate for point.
 *	\param l1 Starting coordinate for line.
 *	\param l2 Ending coordinate for line.
 *
 *  \return 1 if point is on line, 0 otherwise.
 */
int atlas_shape_pol(struct atlas_shape_coordinate_s * point,
					struct atlas_shape_coordinate_s * l1,
					struct atlas_shape_coordinate_s * l2);

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
int atlas_shape_polygon_equal(struct atlas_shape_coordinate_s * coords1, 
							  uint16_t num_coords1,
							  struct atlas_shape_coordinate_s * coords2,
							  uint16_t num_coords2);


#endif // _ATLAS_SHAPE_IMPL_GEOMETRY_H_