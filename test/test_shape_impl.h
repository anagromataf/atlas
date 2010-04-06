/*
 *  test_shape_impl.h
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

#ifndef _TEST_SHAPE_IMPL_H_
#define _TEST_SHAPE_IMPL_H_

#include <check.h>
#include <lazy.h>
#include <stdint.h>
#include <stdio.h>
#include "atlas_shape_impl.h"

START_TEST (test_shape_impl) {
    
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



#endif // _TEST_SHAPE_IMPL_H_
