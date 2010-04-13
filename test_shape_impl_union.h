/*
 *  test_shape_impl_union.h
 *  atlas
 *
 *  Created by Peter Fenske on 4/13/10.
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

#ifndef _TEST_SHAPE_IMPL_UNION_H_
#define _TEST_SHAPE_IMPL_UNION_H_

#include <check.h>
#include <lazy.h>
#include "atlas_shape_impl.h"

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



#endif // _TEST_SHAPE_IMPL_UNION_H_

