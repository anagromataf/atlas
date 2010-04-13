/*
 *  test_shape_impl_equal.h
 *  atlas
 *
 *  Created by Peter Fenske on 4/6/10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST. All rights reserved.
 *
 *	This file is part of atlas.
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
#ifndef _TEST_SHAPE_IMPL_EQUAL_H_
#define _TEST_SHAPE_IMPL_EQUAL_H_

#include <check.h>
#include <lazy.h>
#include <stdint.h>
#include <stdio.h>
#include "atlas_shape_impl.h"

START_TEST (test_shape_impl_equal) {
    
	// create "empty" shape
	lz_obj shape1 = atlas_shape_create(0,0,0,0,0,^(int errno, const char *msg){});
	fail_if (shape1 == 0);
	lz_wait_for_completion();
	
	// create another empty shape
	lz_obj shape2 = atlas_shape_create(0,0,0,0,0,^(int errno, const char *msg){});
	fail_if (shape2 == 0);
	lz_wait_for_completion();
	
	// create a simple filled shape with only one coordinate
	struct atlas_shape_coordinate_s c31 = {1.1, 1.2};
	struct atlas_shape_coordinate_s coord_array_3[] = {c31};
	uint16_t pan_part_start_3[] = {0};
	enum atlas_shape_type_e type_array_3[] = {POINT};
	lz_obj shape3 = atlas_shape_create(1, 
									   sizeof(coord_array_3) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_3, 
									   type_array_3, 
									   coord_array_3, 
									   ^(int errno, const char *msg){});
	fail_if (shape3 == 0);
	lz_wait_for_completion();
	
	// create the another simple shape with the same content
	lz_obj shape4 = atlas_shape_create(1, 
									   sizeof(coord_array_3) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_3, 
									   type_array_3, 
									   coord_array_3, 
									   ^(int errno, const char *msg){});
	fail_if (shape4 == 0);
	lz_wait_for_completion();
	
	// create a more complex shape
	struct atlas_shape_coordinate_s c51 = {5.1, 5.2};
	struct atlas_shape_coordinate_s c52 = {5.3, 5.4};
	struct atlas_shape_coordinate_s c53 = {5.5, 5.6};
	struct atlas_shape_coordinate_s c54 = {5.7, 5.8};
	struct atlas_shape_coordinate_s c55 = {5.9, 5.0};
	struct atlas_shape_coordinate_s coord_array_5[] = {c51, c52, c53, c54, c55};
	uint16_t pan_part_start_5[] = {0,1,3};
	enum atlas_shape_type_e type_array_5[] = {POINT, ARC, ARC};
	lz_obj shape5 = atlas_shape_create(3, 
									   sizeof(coord_array_5) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_5, 
									   type_array_5, 
									   coord_array_5, 
									   ^(int errno, const char *msg){});
	fail_if (shape5 == 0);
	lz_wait_for_completion();
	
	// create another shape using the same data
	lz_obj shape6 = atlas_shape_create(3, 
									   sizeof(coord_array_5) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_5, 
									   type_array_5, 
									   coord_array_5, 
									   ^(int errno, const char *msg){});
	fail_if (shape6 == 0);
	lz_wait_for_completion();
	
	
	// create another shape, using the same data but different coordinates
	struct atlas_shape_coordinate_s c71 = {7.1, 7.2};
	struct atlas_shape_coordinate_s c72 = {7.3, 7.4};
	struct atlas_shape_coordinate_s c73 = {7.5, 7.6};
	struct atlas_shape_coordinate_s c74 = {7.7, 7.8};
	struct atlas_shape_coordinate_s c75 = {7.9, 7.0};
	struct atlas_shape_coordinate_s coord_array_7[] = {c71, c72, c73, c74, c75};
	lz_obj shape7 = atlas_shape_create(3, 
									   sizeof(coord_array_7) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_5, 
									   type_array_5, 
									   coord_array_7, 
									   ^(int errno, const char *msg){});
	fail_if (shape7 == 0);
	lz_wait_for_completion();
	
	
	// =====> =====> TODO !!! geht sicher kürzer !!! WIE ?
	// Tests
	/*
	 * 1 & 2, 3 & 4, and 5 & 6 are equal, all other combinations unequal
	 */
	fail_unless(atlas_shape_is_equal(shape1, shape2));
	fail_unless(atlas_shape_is_equal(shape3, shape4));
	fail_unless(atlas_shape_is_equal(shape5, shape6));
	
	fail_if(atlas_shape_is_equal(shape1, shape3));
	fail_if(atlas_shape_is_equal(shape1, shape4));
	fail_if(atlas_shape_is_equal(shape1, shape5));
	fail_if(atlas_shape_is_equal(shape1, shape6));
	fail_if(atlas_shape_is_equal(shape1, shape7));
	fail_if(atlas_shape_is_equal(shape2, shape3));
	fail_if(atlas_shape_is_equal(shape2, shape4));
	fail_if(atlas_shape_is_equal(shape2, shape5));
	fail_if(atlas_shape_is_equal(shape2, shape6));
	fail_if(atlas_shape_is_equal(shape2, shape7));
	fail_if(atlas_shape_is_equal(shape3, shape5));
	fail_if(atlas_shape_is_equal(shape3, shape6));
	fail_if(atlas_shape_is_equal(shape3, shape7));
	fail_if(atlas_shape_is_equal(shape4, shape5));
	fail_if(atlas_shape_is_equal(shape4, shape6));
	fail_if(atlas_shape_is_equal(shape4, shape7));
	fail_if(atlas_shape_is_equal(shape5, shape7));
	fail_if(atlas_shape_is_equal(shape6, shape7));
	
	fail_unless(atlas_shape_is_equal(shape1, shape1));
	fail_unless(atlas_shape_is_equal(shape2, shape2));
	fail_unless(atlas_shape_is_equal(shape3, shape3));
	fail_unless(atlas_shape_is_equal(shape4, shape4));
	fail_unless(atlas_shape_is_equal(shape5, shape5));
	fail_unless(atlas_shape_is_equal(shape6, shape6));
	fail_unless(atlas_shape_is_equal(shape7, shape7));
	
	
	
	lz_release(shape1);
	lz_release(shape2);
	lz_release(shape3);
	lz_release(shape4);
	lz_release(shape5);
	lz_release(shape6);
	lz_release(shape7);
	lz_wait_for_completion();
	
    
} END_TEST



#endif // _TEST_SHAPE_IMPL_EQUAL_H_