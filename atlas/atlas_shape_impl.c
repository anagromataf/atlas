/*
 *  atlas_shape_impl.c
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
 */

#include "atlas_shape_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lazy.h>
#include <stdint.h>
#include <assert.h>

/*
 * This struct represents the data written to memory for a shape. 
 * It holds the number of parts and coordinates and the dynamic length data.
 */
struct atlas_shape_data_s{
	uint16_t number_of_parts;
	uint16_t number_of_coordinates;
	/*
	 * data will later hold number_of_parts*atlas_shape_part_info_s 
	 * and all coordinates
	 */
	char data[];
};


/*
 * This struct holds the information for a single pan_part as part of a shape.
 */
struct atlas_shape_part_info_s{
	// start index for coordinates of shape
	uint16_t pan_part_start;
	enum atlas_shape_type_e pan_part_type;
};


lz_obj atlas_shape_create(uint16_t number_of_parts,
						  uint16_t number_of_coordinates,
						  uint16_t * pan_part_start,
						  enum atlas_shape_type_e * pan_part_type,
						  struct atlas_shape_coordinate_s * coordinates,
						  atlas_error_handler err) {
	
	// TODO: Validate input data for consistency (coordinates, simple polygons)
	
	/*
	 * Calculate total necessary length of data to allocate.
	 * The length is two uint_16 for the number of parts and the number of 
	 * coordinates. Next the number of parts times the size of the 
	 * atlas_shape_part_info_s struct. Finally the number of coordinates times 
	 * the size of the struct for a coordinate tuple.
	 */
	
	int length_of_data = sizeof(uint16_t) * 2
	+ number_of_parts * sizeof(struct atlas_shape_part_info_s)
	+ number_of_coordinates * sizeof(struct atlas_shape_coordinate_s);
	
	
	// Declare pointer to begin of memory location
	struct atlas_shape_data_s * shape_data = malloc(length_of_data);
	
	
	// if memory was successfully allocated
	if (shape_data){
		// assign number of parts
		shape_data->number_of_parts = number_of_parts;
		// assign number of coordinates
		shape_data->number_of_coordinates = number_of_coordinates;
		
		if (number_of_parts > 0) {
			/*
			 * if the number of parts would be 0, then there exists no data to copy. 
			 * Otherwise copy pan part start indices.
			 */
			
			struct atlas_shape_part_info_s * part_infos 
				= (struct atlas_shape_part_info_s *) shape_data->data;
			
			// for the number of parts
			for (int i = 0; i < number_of_parts; i++){
				// copy part information from parameter
				part_infos[i].pan_part_start = pan_part_start[i];
				part_infos[i].pan_part_type = pan_part_type[i];
			}
		}
		
		if (number_of_coordinates > 0) {
			// If there is at least one coordinate (struct) copy coordinates
			
			// copy coordinates
			memcpy(shape_data->data + number_of_parts * sizeof(struct atlas_shape_part_info_s),
				   coordinates,
				   number_of_coordinates * sizeof(struct atlas_shape_coordinate_s));
		}
		
		// create lazy object
		lz_obj result = lz_obj_new(shape_data, length_of_data, ^{
			free(shape_data);
		}, 0);
		
		// return result;
		return result;
		
	}  else {
		// Error allocating memory
		return 0;
	}
}

int atlas_shape_get_number_of_parts(lz_obj obj){
	__block uint16_t number_of_parts = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_data_s * data_s = data;
		// read number of coordinates
		number_of_parts = data_s->number_of_parts;	
	});
	
	return number_of_parts;
}


int atlas_shape_get_number_of_coords(lz_obj obj){
	__block uint16_t number_of_coords = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_data_s * data_s = data;
		// read number of coordinates
		number_of_coords = data_s->number_of_coordinates;	
	});
	
	return number_of_coords;
}


enum atlas_shape_type_e atlas_shape_get_type_of_part(lz_obj obj, int part){
	__block enum atlas_shape_type_e type_of_shape;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		struct atlas_shape_data_s * shape = data;
		
		// Check if part number (as in index) is smaller than the number of parts specified in the object
		assert(part < shape->number_of_parts);
		
		struct atlas_shape_part_info_s * part_info = (struct atlas_shape_part_info_s *)(shape->data);
		type_of_shape = part_info[part].pan_part_type;		
		
	});
	return type_of_shape;
}


int atlas_shape_get_start_of_part(lz_obj obj, int part){
	__block uint16_t start_of_part;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		struct atlas_shape_data_s * shape = data;
		
		// Check if part number (as in index) is smaller than the number of parts specified in the object
		assert(part < shape->number_of_parts);
		
		struct atlas_shape_part_info_s * part_info = (struct atlas_shape_part_info_s *)(shape->data);
		start_of_part = part_info[part].pan_part_start;
		
	});
	return start_of_part;
}

lz_obj atlas_shape_create_intersection(lz_obj obj1, 
									   lz_obj obj2,
									   atlas_error_handler err){
	return 0;
}


lz_obj atlas_shape_create_union(lz_obj obj1, 
								lz_obj obj2,
								atlas_error_handler err){
	return 0;
}


lz_obj atlas_shape_create_difference(lz_obj obj1, 
									 lz_obj obj2,
									 atlas_error_handler err){
	return 0;
}


int atlas_shape_is_equal(lz_obj obj1, lz_obj obj2){
	return 0;
}


int atlas_shape_intersects (lz_obj obj1, lz_obj obj2){
	return 0;
}


int atlas_shape_is_inside (lz_obj obj1, lz_obj obj2){
	return 0;
}


int atlas_shape_is_outside (lz_obj obj1, lz_obj obj2){
	return 0;
}


int atlas_shape_is_adjacent (lz_obj obj1, lz_obj obj2){
	return 0;
}
