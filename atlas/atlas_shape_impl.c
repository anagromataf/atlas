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

/*
 * This struct represents the data written to memory for a shape. It holds the number of parts and coordinates and the dynamic length data.
 */
struct atlas_shape_data_s{
	uint16_t number_of_parts;
	uint16_t number_of_coordinates;
	/*
	 * data will later hold number_of_parts*atlas_shape_part_info_s and all coordinates
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
						  void (^error_handler)(int errno, const char *msg)) {
	
	
	// TODO Daten validieren
	/*
	 * Koordinaten sinnvoll?
	 * 
	 * Polygon: is_simple() => A polygon is simple if edges don't intersect, except consecutive edges, which intersect in their common vertex. == selfintersecting
	 *
	 * Lines: Überschneidungen erlaubt?
	 */
	
	// END TODO Daten validieren
	
	
	
	
	/*
	 * Calculate total necessary length of data to allocate.
	 * The length is two uint_16 for the number of parts and the number of coordinates. Next the number of parts times the size of the atlas_shape_part_info_s struct. Finally the number of coordinates times the size of the struct for a coordinate tuple.
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
			// if the number of parts would be 0, then there exists no data to copy. Otherwise copy pan part start indices.
			
			struct atlas_shape_part_info_s * part_infos = (struct atlas_shape_part_info_s *) shape_data->data;
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

/*
 * Returns the number of parts in a shape
 */
int atlas_shape_get_number_of_parts(lz_obj obj){
	// ... sync 
	//__block uint16_t result;
	// lz_obj_sync(lz_obj, ^(void * data, uint32_t size){
	//		result = &((uint16_t *)data);
	//	});
	//	return result;
	
	
	__block uint16_t number_of_parts = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_data_s * data_s = data;
		// read number of coordinates
		number_of_parts = data_s->number_of_parts;	
	});
	
	printf("TEST: Num of parts read is %i\n", number_of_parts);
	
	return number_of_parts;
}

/*
 * Returns the number of coordinate tuples in a shape
 */
int atlas_shape_get_number_of_coords(lz_obj obj){
	__block uint16_t number_of_coords = 0;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		// read data content from lazy object
		struct atlas_shape_data_s * data_s = data;
		// read number of coordinates
		number_of_coords = data_s->number_of_coordinates;	
	});
	
	printf("TEST: Num of coordinates read is %i\n", number_of_coords);
	
	return number_of_coords;
}

/*
 * Returns the type of the n-th part (count starts at 0) in a shape
 */
enum atlas_shape_type_e atlas_shape_get_type_of_part(lz_obj obj, int part){
	__block enum atlas_shape_type_e type_of_shape;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		struct atlas_shape_data_s * shape = data;
		
		// Check if part number (as in index) is smaller than the number of parts specified in the object
		assert(part < shape->number_of_parts);
		
		struct atlas_shape_part_info_s * part_info = (struct atlas_shape_part_info_s *)(shape->data);
		type_of_shape = part_info[part].pan_part_type;
		
		printf("TEST: Shape type is %i\n", type_of_shape);
		
		
		
	});
	return type_of_shape;
}

/*
 * Returns the coordinate start index of the n-th part (count starts at 0) in a shape‚
 */
int atlas_shape_get_start_of_part(lz_obj obj, int part){
	__block uint16_t start_of_part;
	
	lz_obj_sync(obj, ^(void * data, uint32_t size){
		struct atlas_shape_data_s * shape = data;
		
		// Check if part number (as in index) is smaller than the number of parts specified in the object
		assert(part < shape->number_of_parts);
		
		struct atlas_shape_part_info_s * part_info = (struct atlas_shape_part_info_s *)(shape->data);
		start_of_part = part_info[part].pan_part_start;
		
		printf("TEST: Start is %i\n", start_of_part);
		
	});
	return start_of_part;
}


/*
 * Create the intersection of two shapes.
 */
lz_obj atlas_shape_create_intersection(){
	return 0;
}

/*
 * Create the union of two shapes.
 */
lz_obj atlas_shape_create_union(lz_obj obj1, lz_obj obj2){
	__block struct atlas_shape_data_s * shape1;
	__block struct atlas_shape_data_s * shape2;
	
	// read first shape
	lz_obj_sync(obj2, ^(void * data, uint32_t size){
		shape1 = data;		
	});
	
	// read second shape
	lz_obj_sync(obj2, ^(void * data, uint32_t size){
		shape2 = data;
	});
	
	uint16_t number_of_parts_union = shape1->number_of_parts + shape2->number_of_parts;
	uint16_t number_of_coordinates_union = shape1->number_of_coordinates + shape2->number_of_coordinates;
	
	int length_of_data_union = sizeof(uint16_t) * 2
	+ number_of_parts_union * sizeof(struct atlas_shape_part_info_s)
	+ number_of_coordinates_union * sizeof(struct atlas_shape_coordinate_s);
	
	
	// Declare pointer to begin of memory location
	struct atlas_shape_data_s * shape_data_union = malloc(length_of_data_union);
	
	if (shape_data_union){
		
		
		shape_data_union->number_of_parts = number_of_parts_union;
		shape_data_union->number_of_coordinates = number_of_coordinates_union;
		
		
		/*
		 * take structs holding part information from second shape, increment pan_part_starts by number of coordinates from first shape, and attach them to the first
		 */
		
		// copies shape information from shape1 to new shape
		memcpy(shape_data_union->data,
			   shape1->data,
			   sizeof(struct atlas_shape_part_info_s) * shape1->number_of_parts);
		
		
		// copies shape information from shape2 to new shape, destination is the pointer sum of the part info from shape1 within data, the source is the data part of shape2 and the length to copy is the info size
		memcpy(shape_data_union->data + shape1->number_of_parts * sizeof(struct atlas_shape_part_info_s),
			   shape2->data,
			   sizeof(struct atlas_shape_part_info_s) * shape2->number_of_parts);
		
		// create part info pointer to data of new shape
		struct atlas_shape_part_info_s * part_infos = (struct atlas_shape_part_info_s *) shape_data_union->data;
		// Move pointer forward, to where the infos form shape2 start
		part_infos = part_infos + shape1->number_of_parts;
		for (int i = 0; i < shape2->number_of_parts; i++){
			/*
			 * Iterate over the number of parts in shape2 and increase the pan_part_start by the number of coordinates from shape1
			 */
			part_infos[i].pan_part_start = part_infos[i].pan_part_start + shape1->number_of_coordinates;
		}
		
		
		/*
		 * copy coodinates from shape1 and shape2 to new shape
		 */
				
		if (shape1->number_of_coordinates > 0 && shape2->number_of_coordinates > 0) {
			// If there is at least one coordinate (struct) in both shapes copy coordinates
			
			/*
			 * copy coordinates from shape1
			 * source is data in shape1 plus the length of the part information, thus the beginning of the coordinates, destination is the target data plus the length the part information for the new shape, and the length is the size of the coordinates from shape1
			 */
			memcpy(shape_data_union->data + number_of_parts_union * sizeof(struct atlas_shape_part_info_s),
				   shape1->data + shape1->number_of_parts * sizeof(struct atlas_shape_part_info_s),
				   shape1->number_of_coordinates * sizeof(struct atlas_shape_coordinate_s));
			
			// temporary data pointer, source for copying coordinates from shape2
			int tmp_data_index = number_of_parts_union * sizeof(struct atlas_shape_part_info_s)
			+  shape1->number_of_coordinates * sizeof(struct atlas_shape_coordinate_s);
			
			/*
			 * copy coordinates from shape2
			 * source is data in shape2 plus the length of the part information, thus the beginning of the coordinates, destination is the tmp_data_index (see above), and the length is the size of the coordinates from shape2
			 */
			memcpy(shape_data_union->data + tmp_data_index,
				   shape2->data + shape2->number_of_parts * sizeof(struct atlas_shape_part_info_s),
				   shape2->number_of_coordinates * sizeof(struct atlas_shape_coordinate_s));
		}
		
		
		// create lazy object
		lz_obj result = lz_obj_new(shape_data_union, length_of_data_union, ^{
			free(shape_data_union);
		}, 0);
		
		// return result;
		return result;
	} else {
		// Error allocating memory
		return 0;
	}
}

/*
 * Create the difference of two shapes.
 */
lz_obj atlas_shape_create_difference(){
	return 0;
}

/*
 * check if the two shapes are equal
 * returns 0 if shapes are unequal, returns 1 if both shapes are equal
 */
// =====> =====> TODO !!! gelten zwei shapes als gleich, wenn deren interne struktur gleich ist, oder müssen die parts paarweise geprüft werden, also z.b. point an "stelle 7" und point an "stelle 25" und wenn alle passen, dann erst gleich? 

// =====> =====> TODO !!! neue funktion für semantische gleichheit .... diese funktion umbennen

int atlas_shape_is_equal(lz_obj obj1, lz_obj obj2){
	__block struct atlas_shape_data_s * shape1;
	__block struct atlas_shape_data_s * shape2;
	
	// read first shape
	lz_obj_sync(obj1, ^(void * data, uint32_t size){
		shape1 = data;		
	});
	
	// read second shape
	lz_obj_sync(obj2, ^(void * data, uint32_t size){
		shape2 = data;		
	});
	
	// Check, if number of parts is unequal
	if (shape1->number_of_parts != shape2->number_of_parts) {
		return 0;
	}
	
	// Check, if number of coordinates is unequal
	if (shape1->number_of_coordinates != shape2->number_of_coordinates){
		return 0;
	}
	
	
	int length_of_data = shape1->number_of_parts * sizeof(struct atlas_shape_part_info_s)
	+ shape1->number_of_coordinates * sizeof(struct atlas_shape_coordinate_s);
	
	
	/*
	 * Check, if data content (part information and coordinates) is unequal for both shapes.
	 * memcmp returns 0 when all bytes by the specified length are equal. A non-zero value otherwise.
	 */
	if (0 != memcmp((struct atlas_shape_part_info_s *)(shape1->data), 
					(struct atlas_shape_part_info_s *)(shape2->data), 
					length_of_data )) {
		return 0;
	}
	
	return 1;
}

/*
 * check if the two shapes are intersecting
 */
int atlas_shape_intersects (){
	return 0;
}


/*
 * check if shp1 is inside shp2
 */
int atlas_shape_is_inside (){
	return 0;
}


/*
 * check if shp1 is outsid of shp2
 */
int atlas_shape_is_outside (){
	return 0;
}


/*
 * check if the two shapes are adjacent
 */
int atlas_shape_is_adjacent (){
	return 0;
}
