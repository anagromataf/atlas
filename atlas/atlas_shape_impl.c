/*
 *  atlas_shape_impl.c
 *  atlas
 *
 *  Created by Peter Fenske on 25.03.10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST. All rights reserved.
 *
 */

#include "atlas_shape_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lazy.h>
#include <stdint.h>
#include <assert.h>

struct atlas_shape_static_data_s{
	uint16_t number_of_parts;
	uint16_t number_of_coordinates;
	char data[];
};

lz_obj atlas_shape_create(uint16_t number_of_parts,
						  uint16_t number_of_coordinates,
						  uint16_t * pan_part_start,
						  enum atlas_shape_type_e * pan_part_type,
						  struct atlas_shape_coordinate_s * coordinates,
						  void (^error_handler)(int errno, const char *msg)) {
	
	
	// TODO Daten validieren, z.b. Koordinaten sinnvoll, selfintersecting
	
	
	
	
	/*
	 * Calculate total necessary length of data to allocate.
	 * The length is two uint_16 for the number of parts and the number of coordinates, for the number of parts times the size of uint16 for holding the indices of the pan part starts, same for the type (enum) and the number of coordinates times the size of the struct for a coordinate tuple.
	 */
	int length_of_data = sizeof(uint16_t) * 2
					   + number_of_parts * sizeof(uint16_t)
					   + number_of_parts * sizeof(enum atlas_shape_type_e)
					   + number_of_coordinates * sizeof(struct atlas_shape_coordinate_s);
	
	// Declare pointer to begin of memory location
	struct atlas_shape_static_data_s * static_data = malloc(length_of_data);
	
	// if memory was successfully allocated
	if (static_data){
		// assign number of parts
		static_data->number_of_parts = number_of_parts;
		// assign number of coordinates
		static_data->number_of_coordinates = number_of_coordinates;

		if (number_of_parts > 0) {
			// if the number of parts would be 0, then there exists no data to copy. Otherwise copy pan part start indices.
			memcpy(static_data->data,
				   pan_part_start,
				   number_of_parts * sizeof(uint16_t));
			
			// Also only copy the types, if there is one part or more
			memcpy(static_data->data + number_of_parts * sizeof(uint16_t),
				   pan_part_type,
				   number_of_parts * sizeof(enum atlas_shape_type_e));
		}
		
		if (number_of_coordinates > 0) {
			// If there is at least one coordinate (struct) copy coordinates
			memcpy(static_data->data + number_of_parts * ( sizeof(uint16_t) + sizeof(enum atlas_shape_type_e)),
				   coordinates,
				   number_of_coordinates * sizeof(struct atlas_shape_coordinate_s));
		}
		
		// create lazy object
		lz_obj result = lz_obj_new(static_data, length_of_data, ^{
			free(static_data);
		}, 0);
		
		// return result;
		return result;
		
	}  else {
		// Error allocating memory
		return 0;
	}
}

int atlas_shape_get_number_of_parts(lz_obj obj){
	// ... sync 
	//__block uint16_t result;
//	lz_obj_sync(lz_obj, ^(void * data, uint32_t size){
//		result = &((uint16_t *)data);
//	});
//	return result;

	
	__block uint16_t number_of_parts = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_static_data_s * data_s = data;
		// read number of coordinates
		number_of_parts = data_s->number_of_parts;	
	});
	
	printf("TEST: Num of parts read is %i\n", number_of_parts);
	
	return number_of_parts;
}


int atlas_shape_get_number_of_coords(lz_obj obj){
	__block uint16_t number_of_coords = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_static_data_s * data_s = data;
		// read number of coordinates
		number_of_coords = data_s->number_of_coordinates;	
	});
	
	printf("TEST: Num of coordinates read is %i\n", number_of_coords);
	
	return number_of_coords;
}


enum atlas_shape_type_e atlas_shape_get_type_of_part(lz_obj obj, int part){
	
	
	__block enum atlas_shape_type_e result;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		struct atlas_shape_static_data_s * shape = data;
		
		// Check if part number (as in index) is smaller than
		// the number of parts specified in the object
		assert(part < shape->number_of_parts);

		
		enum atlas_shape_type_e * types = (void *)(shape->data) + sizeof(uint16_t) * shape->number_of_parts;
		result = types[part];
		
		
//		struct xyz * x = (struct xyz *)(shape->data);
//		t = x[part].type

	});
	
	return result;
}

