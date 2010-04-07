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

#include <float.h>
#include <math.h>

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
	
	
	// TODO Daten validieren, z.b. Koordinaten sinnvoll, selfintersecting
	
	
	
	
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
			
//			memcpy(static_data->data,
//				   pan_part_start,
//				   number_of_parts * sizeof(uint16_t));
//			
//			// Also only copy the types, if there is one part or more
//			memcpy(static_data->data + number_of_parts * sizeof(uint16_t),
//				   pan_part_type,
//				   number_of_parts * sizeof(enum atlas_shape_type_e));
			
			
			// =====> =====> TODO !!! geht sicher kürzer !!! WIE ?
			
			// for the number of parts
			for (int i = 0; i < number_of_parts; i++){
				// allocate memory to hold a struct for the information of a part
				struct atlas_shape_part_info_s * part_info = malloc(sizeof(struct atlas_shape_part_info_s));
				
				// copy part information from parameter
				part_info->pan_part_start = pan_part_start[i];
				part_info->pan_part_type = pan_part_type[i];
				
				// copy information to dynamic data part of shape 
				memcpy(shape_data->data + i * sizeof(struct atlas_shape_part_info_s), 
					   part_info, 
					   sizeof(struct atlas_shape_part_info_s));
				
				// release memory, since it was only temporarily needed
				free(part_info);
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

		
		// struct xyz * x = (struct xyz *)(shape->data);
		// t = x[part].type
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
lz_obj atlas_shape_create_union(){
	return 0;
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

/*
 * Checks, if two lines intersect. Returns 1 if they intersect, 0 if they don't
 */
int lines_intersect(struct atlas_shape_coordinate_s l1_s,
					struct atlas_shape_coordinate_s	l1_e,
					struct atlas_shape_coordinate_s l2_s,
					struct atlas_shape_coordinate_s	l2_e){
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
	
	if (((l1_e.longitude - l1_s.longitude) == 0) && ((l2_e.longitude - l2_s.longitude) == 0)){
		/*
		 * Both lines are vertical. In this case they are either on top of each other or they never meet
		 */
		if (l1_e.longitude == l2_e.longitude){
			// they are on top of each other
			return 1;
		} else {
			// they run in parallel and never meet
			return 0;
		}		
		
	} else if ((l1_e.longitude - l1_s.longitude) == 0){
		// prevent division by zero, line 1 is vertical
		intersect_x = l1_e.longitude;
		
		// slope for line 2
		l2_m = (l2_e.latitude - l2_s.latitude)/(l2_e.longitude - l2_s.longitude);
		// y-intercept for line 2		
		l2_n = l2_s.latitude - l2_s.longitude*l2_m;
		
		intersect_y = l2_m * intersect_x + l2_n;
		
		
	} else if ((l2_e.longitude - l2_s.longitude) == 0) {
		// prevent division by zero, line 2 is vertical
		intersect_x = l2_e.longitude;
		
		// slope for line 1
		l1_m = (l1_e.latitude - l1_s.latitude)/(l1_e.longitude - l1_s.longitude);
		// y-intercept for line 1
		l1_n = l1_s.latitude - l1_s.longitude*l1_m;
		
		intersect_y = l1_m * intersect_x + l1_n;
		
	} else  {
		/*
		 * Last case, neither line 1 or line 2 are vertical
		 */
		
		// slope for line 1
		l1_m = (l1_e.latitude - l1_s.latitude)/(l1_e.longitude - l1_s.longitude);
		// y-intercept for line 1
		l1_n = l1_s.latitude - l1_s.longitude*l1_m;
		
		// slope for line 2
		l2_m = (l2_e.latitude - l2_s.latitude)/(l2_e.longitude - l2_s.longitude);
		// y-intercept for line 2
		l2_n = l2_s.latitude - l2_s.longitude*l2_m;
				
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
		
		// solving equation for x: l1_m*x + l1_n = l2_m*x + l2_n <==> x = (l2_n - l1_n) / (l1_m - l2_m)
		intersect_x = (l2_n - l1_n) / (l1_m - l2_m);
		
		// use intersect_x to calculate intersect_y
		intersect_y = l1_m * intersect_x + l1_n;
		// also determine intersect_y(_2) using other equation
		double intersect_y_2 = l2_m * intersect_x + l2_n;
		
		// check, if results are equal
		// =====> =====> TODO !!! besser? WIE ?
		if (fabs( intersect_y - intersect_y_2 ) > 1.0E-10){
			return 0;
		}
	}
	
	printf("TEST: intersect_x is %f\n", intersect_x);
	printf("TEST: intersect_y is %f\n", intersect_y);
	
	/*
	 * Point has to be in "box".
	 * intersect_x has to be in between the start and end points of both lines. Same for intercept_y. Otherwise both lines intercept, but that would be at the extension of both lines.
	 */
	if (
		fmin(l1_s.longitude,l1_e.longitude) <= intersect_x
		&& intersect_x <= fmax(l1_s.longitude,l1_e.longitude)
		&& fmin(l2_s.longitude,l2_e.longitude) <= intersect_x
		&& intersect_x <= fmax(l2_s.longitude,l2_e.longitude)
		&& fmin(l1_s.latitude,l1_e.latitude) <= intersect_y
		&& intersect_y <= fmax(l1_s.latitude,l1_e.latitude)
		&& fmin(l2_s.latitude,l2_e.latitude) <= intersect_y
		&& intersect_y <= fmax(l2_s.latitude,l2_e.latitude)
		) {
		return 1;
	} 
	
	return 0;
}

