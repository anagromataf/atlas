/*
 *  shape.h
 *  atlas
 *
 *  Created by Peter Fenske on 20.04.10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST.
 *
 *  This file is part of atlas.
 *	
 *  atlas is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *	
 *  atlas is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with atlas.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ATLAS_GEO_SHAPE_H_
#define _ATLAS_GEO_SHAPE_H_

#include <atlas/base.h>

#include <lazy.h>
#include <stdint.h>

/*! Handle for an Atlas Shape
 */
typedef lz_obj atlas_shp_t;

/*! Type for a coordinate
 */
typedef struct atlas_shp_coordinate_s {
    double longitude;
    double latitude;
} atlas_shp_coordinate_t;

/*! Type of a part in a shape
 */
typedef enum atlas_shp_type_e {
    POINT = 1,
    ARC = 2,
    POLYGON = 3
} atlas_shp_type_t;

#pragma mark -
#pragma mark Create a Shape

/*! Create a shape with the coordinates and the type
 *
 *  \param number_of_parts Number pof parts in this shape, has minimum of one
 *  \param number_of_coordinates Number of coordinates in this shape, has minimum of one (for a point)
 *  \param pan_part_start The list of zero based start vertices for the rings (parts) in this object.  
 *						The first should (not must) always be zero. 
 *  \param pan_part_type The type of each of the parts.
 *  \param coordinates The number of coordinates beeing passed.
 *  \param err A block which is called if an error occured.
 *
 *	\return
 */
atlas_shp_t
atlas_shape_create(uint16_t number_of_parts,
                   uint16_t number_of_coordinates,
                   uint16_t * pan_part_start,
                   atlas_shp_type_t * pan_part_type,
                   atlas_shp_coordinate_t * coordinates,
                   atlas_error_handler err);


/*! Create the intersection of two shapes.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * \param err A block which is called if an error occured.
 * 
 * \return A new shape containing the intersection of the two shapes.
 */
atlas_shp_t
atlas_shape_create_intersection(atlas_shp_t shape1, 
                                atlas_shp_t shape2,
                                atlas_error_handler err);


/*! Create the union of two shapes.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * \param err A block which is called if an error occured
 * 
 * \return A new shape containing the union of the two shapes.
 */
atlas_shp_t
atlas_shape_create_union(atlas_shp_t shape1, 
                         atlas_shp_t shape2, 
                         atlas_error_handler err);


/*! Create the difference of two shapes.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * \param err A block which is called if an error occured
 * 
 * \return A new shape containing the difference of the two shapes.
 */
atlas_shp_t
atlas_shape_create_difference(atlas_shp_t shape1, 
                              atlas_shp_t shape2,
                              atlas_error_handler err);


#pragma mark -
#pragma mark Access Details of a Shape

/*! Returns the number of parts in a shape
 * 
 * \param shape a shape
 * \return Number of parts the shape
 */
int
atlas_shape_get_number_of_parts(atlas_shp_t shape);


/*! Returns the type of the n-th part in a shape
 * 
 * \param shape a shape
 * \param Number of part (count starts at 0)
 * 
 * \return Type of part
 */
atlas_shp_type_t
atlas_shape_get_type_of_part(atlas_shp_t shape,
                             int part);


/*! Returns the number of coordinate tuples in a shape
 * 
 * FIXME: This function should not be public
 *
 * \param shape a shape
 * 
 * \return Number coordinates in shape
 */
int
atlas_shape_get_number_of_coords(atlas_shp_t shape);


/*! Returns the coordinate's start index of the n-th part in a shape.
 * 
 * FIXME: This function should not be public
 *
 * \param shape a shape
 * \param Number of part (count starts at 0)
 * 
 * \return Type of part.
 */
int
atlas_shape_get_start_of_part(atlas_shp_t shape,
                              int part);


#pragma mark -
#pragma mark Predicates with two Shapes

/*! Checks if the two shapes are equal.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * 
 * \return 1 if both shapes represent the same information, 0 otherwise.
 */
int
atlas_shape_is_equal(atlas_shp_t shape1,
                     atlas_shp_t shape2);


/*! Check if the two shapes are intersecting.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * 
 * \return 1 if both shapes intersect, 0 otherwise.
 */
int
atlas_shape_intersects(atlas_shp_t shape1,
                       atlas_shp_t shape2);


/*! Check if shape 1 is inside shape 2.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * 
 * \return 1 if shape 1 is inside of shape 2, 0 otherwise.
 */
int
atlas_shape_is_inside(atlas_shp_t shape1,
                      atlas_shp_t shape2);


/*! Check if shape 1 is outside of shape 2.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * 
 * \return 1 if shape 1 is outside of shape 2, 0 otherwise.
 */
int
atlas_shape_is_outside(atlas_shp_t shape1,
                       atlas_shp_t shape2);


/*! Check if the two shapes are adjacent.
 * 
 * \param shape1 first shape
 * \param shape2 second shape
 * 
 * \return 1 if shapes are adjacent, 0 otherwise.
 */
int
atlas_shape_is_adjacent(atlas_shp_t shape1,
                        atlas_shp_t shape2);


#endif // _ATLAS_GEO_SHAPE_H_


