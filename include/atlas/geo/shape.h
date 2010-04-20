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

#ifndef _ATLAS_SHAPE_H_
#define _ATLAS_SHAPE_H_

#include <lazy.h>
#include <stdint.h>
#include <atlas/base.h>

struct atlas_shape_coordinate_s {
    double longitude;
    double latitude;
};

enum atlas_shape_type_e {
    POINT = 1,
    ARC = 2,
    POLYGON = 3
};


/*! Create a shape with the coordinates and the type
 *
 *  \param type Type of the shape.
 *  \param number_of_parts Number pof parts in this shape, has minimum of one
 *  \param number_of_coordinates Number of coordinates in this shape, has minimum of one (for a point)
 *  \param pan_part_start The list of zero based start vertices for the rings (parts) in this object.  
 *						The first should (not must) always be zero. 
 *  \param pan_part_type The type of each of the parts.
 *  \param coordinates The number of coordinates beeing passed.
 *  \param error_handler A block which is called if an error occured.
 *
 *	\return
 */
lz_obj atlas_shape_create(uint16_t number_of_parts,
						  uint16_t number_of_coordinates,
						  uint16_t * pan_part_start,
						  enum atlas_shape_type_e * pan_part_type,
						  struct atlas_shape_coordinate_s * coordinates,
						  atlas_error_handler err);

/*
 *! Returns the number of parts in a shape
 * 
 * \param obj LazyObject containing the shape.
 * 
 * \return Number of parts in shape.
 */
int atlas_shape_get_number_of_parts(lz_obj obj);


/*
 *! Returns the number of coordinate tuples in a shape
 * 
 * \param LazyObject containing the shape.
 * 
 * \return Number coordinates in shape.
 */
int atlas_shape_get_number_of_coords(lz_obj obj);


/*
 *! Returns the type of the n-th part in a shape
 * 
 * \param LazyObject containing the shape.
 * \param Number of part (count starts at 0)
 * 
 * \return Type of part.
 */
enum atlas_shape_type_e atlas_shape_get_type_of_part(lz_obj obj, int part);


/*
 *! Returns the coordinate's start index of the n-th part in a shape.
 * 
 * \param LazyObject containing the shape.
 * \param Number of part (count starts at 0)
 * 
 * \return Type of part.
 */
int atlas_shape_get_start_of_part(lz_obj obj, int part);


/*
 *! Create the intersection of two shapes.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return A new lazyObject containing the intersection of the two shapes.
 */
lz_obj atlas_shape_create_intersection(lz_obj obj1, lz_obj obj2);


/*
 *! Create the union of two shapes.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape. 
 * 
 * \return A new lazyObject containing the union of the two shapes.
 */
lz_obj atlas_shape_create_union(lz_obj obj1, lz_obj obj2);


/*
 *! Create the difference of two shapes.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return A new lazyObject containing the difference of the two shapes.
 */
lz_obj atlas_shape_create_difference(lz_obj obj1, lz_obj obj2);


/*
 *! Checks if the two shapes are equal.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return 1 if both shapes represent the same information, 0 otherwise.
 */
int atlas_shape_is_equal(lz_obj obj1, lz_obj obj2);


/*
 *! Check if the two shapes are intersecting.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return 1 if both shapes intersect, 0 otherwise.
 */
int atlas_shape_intersects (lz_obj obj1, lz_obj obj2);


/*
 *! Check if shape 1 is inside shape 2.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return 1 if shape 1 is inside of shape 2, 0 otherwise.
 */
int atlas_shape_is_inside (lz_obj obj1, lz_obj obj2);


/*
 *! Check if shape 1 is outside of shape 2.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return 1 if shape 1 is outside of shape 2, 0 otherwise.
 */
int atlas_shape_is_outside (lz_obj obj1, lz_obj obj2);


/*
 *! Check if the two shapes are adjacent.
 * 
 * \param LazyObject containing the first shape.
 * \param LazyObject containing the second shape.
 * 
 * \return 1 if shapes are adjacent, 0 otherwise.
 */
int atlas_shape_is_adjacent (lz_obj obj1, lz_obj obj2);


#endif // _ATLAS_SHAPE_H_