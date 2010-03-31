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
#include "atlas_shape_impl.h"
#include <stdint.h>

#include <stdio.h>

START_TEST (test_shape_impl) {
    
	lz_obj lzo1 = atlas_shape_create(0,0,0,0,0,^(int errno, const char *msg){});
	fail_if (lzo1 == 0);
	lz_release(lzo1);
	lz_wait_for_completion();
	
	
	
	
	
	
	
	struct atlas_shape_coordinate_s c1 = {52.1,13.9};
	struct atlas_shape_coordinate_s c2 = {22.1,23.9};
	struct atlas_shape_coordinate_s coord_array[] = {c1, c2};
	
	uint16_t pan_part_start[] = {0};

	enum atlas_shape_type_e type_array[] = {ARC};
	
	lz_obj lzo2 = atlas_shape_create(1, 
									 sizeof(coord_array) / sizeof(struct atlas_shape_coordinate_s), 
									 pan_part_start, 
									 type_array, 
									 coord_array, 
									 ^(int errno, const char *msg){});
	fail_if (lzo2 == 0);

	fail_unless(atlas_shape_get_number_of_parts(lzo2) == 1);
	fail_unless(atlas_shape_get_number_of_coords(lzo2) == 2);
	fail_unless(atlas_shape_get_type_of_part(lzo2, 0) == ARC);
	
	
	lz_release(lzo2);
	lz_wait_for_completion();
	
	
	
	
	
	
//	lz_obj_sync(lzo, ^(void * data, uint32_t size){
//        fail_unless(strcmp(data, text) == 0);
//    });
	
	
    
} END_TEST



#endif // _TEST_SHAPE_IMPL_H_
