/*
 *  test_atlas_shape_impl.c
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

#include <check.h>
#include <lazy.h>

#include <stdint.h>
#include <stdio.h>

#include "atlas_shape_impl.h"

START_TEST (test_shape_impl_create) {
    
	// FIRST SHAPE
	lz_obj lzo1 = atlas_shape_create(0,0,0,0,0,^(int errno, const char *msg){});
	fail_if (lzo1 == 0);
	lz_release(lzo1);
	lz_wait_for_completion();
	
	
	// SECOND SHAPE
	struct atlas_shape_coordinate_s c1 = {1.9,2.9};
	struct atlas_shape_coordinate_s c2 = {3.9,4.9};
	struct atlas_shape_coordinate_s coord_array[] = {c1, c2, c1, c2, c1, c2};
	
	uint16_t pan_part_start[] = {0, 4};
	
	enum atlas_shape_type_e type_array[] = {ARC, POINT};
	
	lz_obj lzo2 = atlas_shape_create(2, 
									 sizeof(coord_array) / sizeof(struct atlas_shape_coordinate_s), 
									 pan_part_start, 
									 type_array, 
									 coord_array, 
									 ^(int errno, const char *msg){});
	fail_if (lzo2 == 0);
	
	fail_unless(atlas_shape_get_number_of_parts(lzo2) == 2);
	fail_unless(atlas_shape_get_number_of_coords(lzo2) == 6);
	
	fail_unless(atlas_shape_get_type_of_part(lzo2, 0) == ARC);
	fail_unless(atlas_shape_get_start_of_part(lzo2, 0) == 0);
	
	fail_unless(atlas_shape_get_type_of_part(lzo2, 1) == POINT);
	fail_unless(atlas_shape_get_start_of_part(lzo2, 1) == 4);
	
	
	lz_release(lzo2);
	lz_wait_for_completion();
	
	
	// ANOTHER SHAPE
	struct atlas_shape_coordinate_s c71 = {7.1, 7.2};
	struct atlas_shape_coordinate_s c72 = {7.3, 7.4};
	struct atlas_shape_coordinate_s c73 = {7.5, 7.6};
	struct atlas_shape_coordinate_s c74 = {7.7, 7.8};
	struct atlas_shape_coordinate_s c75 = {7.9, 7.0};
	struct atlas_shape_coordinate_s coord_array_7[] = {c71, c72, c73, c74, c75};
	uint16_t pan_part_start_7[] = {0,1,3};
	enum atlas_shape_type_e type_array_7[] = {POINT, ARC, ARC};
	lz_obj shape7 = atlas_shape_create(3, 
									   sizeof(coord_array_7) / sizeof(struct atlas_shape_coordinate_s), 
									   pan_part_start_7, 
									   type_array_7, 
									   coord_array_7, 
									   ^(int errno, const char *msg){});
	fail_if (shape7 == 0);
	fail_unless(atlas_shape_get_number_of_parts(shape7) == 3);
	fail_unless(atlas_shape_get_number_of_coords(shape7) == 5);
	
	fail_unless(atlas_shape_get_start_of_part(shape7, 0) == 0);
	fail_unless(atlas_shape_get_start_of_part(shape7, 1) == 1);
	fail_unless(atlas_shape_get_start_of_part(shape7, 2) == 3);
	
	fail_unless(atlas_shape_get_type_of_part(shape7, 0) == POINT);
	fail_unless(atlas_shape_get_type_of_part(shape7, 1) == ARC);
	fail_unless(atlas_shape_get_type_of_part(shape7, 2) == ARC);
	
	
	lz_release(shape7);
	lz_wait_for_completion();
	
	
} END_TEST


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


START_TEST (test_shape_impl_union) {
	
	struct atlas_shape_coordinate_s c11 = {1.1,1.9};
	struct atlas_shape_coordinate_s c12 = {1.9,1.9};
	struct atlas_shape_coordinate_s coord_array_1[] = {c11, c12};
	uint16_t pan_part_start_1[] = {0};
	enum atlas_shape_type_e type_array_1[] = {ARC};
	lz_obj lzo1 = atlas_shape_create(1, 
									 sizeof(coord_array_1) / sizeof(struct atlas_shape_coordinate_s), 
									 pan_part_start_1, 
									 type_array_1, 
									 coord_array_1, 
									 ^(int errno, const char *msg){});
	fail_if (lzo1 == 0);
	lz_wait_for_completion();
	
	struct atlas_shape_coordinate_s c21 = {2.1,2.9};
	struct atlas_shape_coordinate_s c22 = {2.9,2.9};
	struct atlas_shape_coordinate_s coord_array_2[] = {c21, c22};
	uint16_t pan_part_start_2[] = {0};
	enum atlas_shape_type_e type_array_2[] = {ARC};
	lz_obj lzo2 = atlas_shape_create(1, 
									 sizeof(coord_array_2) / sizeof(struct atlas_shape_coordinate_s), 
									 pan_part_start_2, 
									 type_array_2,
									 coord_array_2,
									 ^(int errno, const char *msg){});
	fail_if (lzo2 == 0);
	lz_wait_for_completion();
	
	lz_obj lzo_union_12 = atlas_shape_create_union(lzo1, lzo2);
	fail_if (lzo_union_12 == 0);
	
	fail_unless(atlas_shape_get_number_of_parts(lzo_union_12) == 2);
	fail_unless(atlas_shape_get_number_of_coords(lzo_union_12) == 4);
	fail_unless(atlas_shape_get_type_of_part(lzo_union_12, 0) == ARC);
	fail_unless(atlas_shape_get_start_of_part(lzo_union_12, 0) == 0);
	fail_unless(atlas_shape_get_type_of_part(lzo_union_12, 1) == ARC);
	fail_unless(atlas_shape_get_start_of_part(lzo_union_12, 1) == 2);	
	
	lz_wait_for_completion();
	lz_release(lzo1);
	lz_release(lzo2);
	lz_release(lzo_union_12);
	
} END_TEST


#pragma mark -
#pragma mark Fixtures

static void setup() {
    printf(">>>\n");
}

static void teardown() {
    printf("<<<\n");
}


#pragma mark -
#pragma mark Shape Impl Suites

Suite * shape_impl_suite(void) {
    
    Suite *s = suite_create("Shape Impl");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
	
    tcase_add_test(tc_create, test_shape_impl_create);
	//tcase_add_test(tc_create, test_shape_impl_equal);
	//tcase_add_test(tc_create, test_shape_impl_union);
    
    suite_add_tcase(s, tc_create);
    
    return s;
}