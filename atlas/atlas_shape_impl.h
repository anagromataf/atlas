/*
 *  atlas_shape_impl.h
 *  atlas
 *
 *  Created by Peter Fenske on 25.03.10.
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
 * 
 *
 */


#ifndef _ATLAS_SHAPE_IMPL_H_
#define _ATLAS_SHAPE_IMPL_H_

#include <lazy.h>
#include <stdint.h>

struct atlas_shape_coordinate_s {
    double longitude;
    double latitude;
};

enum atlas_shape_type_e {
    POINT = 1,
    ARC = 2,
    POLYGON = 3
};


/*! \brief Create a shape with the coordinates and the type
 *
 *  \ingroup atlas_type_shp_shape
 *
 *  \param type Type of the shape.
 *  \param number_of_parts Number pof parts in this shape, has minimum of one
 *  \param number_of_coordinates Number of coordinates in this shape, has minimum of one (for a point)
 *  \param pan_part_start The list of zero based start vertices for the rings (parts) in this object.  
 *  The first should (not must) always be zero. 
 *  \param pan_part_type The type of each of the parts.
 *  \param coordinates The number of coordinates beeing passed.
 *  \param error_handler A block which is called if an error occured.
 */
lz_obj atlas_shape_create(uint16_t number_of_parts,
						  uint16_t number_of_coordinates,
						  uint16_t * pan_part_start,
						  enum atlas_shape_type_e * pan_part_type,
						  struct atlas_shape_coordinate_s * coordinates,
						  void (^error_handler)(int errno, const char *msg));


lz_obj atlas_shape_create_union(lz_obj obj1, lz_obj obj2);


#endif // _ATLAS_SHAPE_IMPL_H_



