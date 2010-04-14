/*
 *  atlas_shape_impl_geom_equal.h
 *  atlas
 *
 *  Created by Peter Fenske on 4/14/10.
 *  Copyright 2010 Fraunhofer Institut für Software- und Systemtechnik ISST.
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

#ifndef _TEST_SHAPE_IMPL_GEOM_EQUAL_H_
#define _TEST_SHAPE_IMPL_GEOM_EQUAL_H_

#include <check.h>
#include "atlas_shape_impl_geometry.h"

START_TEST (test_shape_impl_geom_equal) {
	struct atlas_shape_coordinate_s c_11 = {3.0, 1.0};
	struct atlas_shape_coordinate_s c_12 = {3.0, 1.0};
	fail_unless(atlas_shape_point_equal(&c_11, &c_12) == 1);
	
	struct atlas_shape_coordinate_s c_21 = {(1.0/3.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_22 = {(1.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_21, &c_22) == 1);
	
	struct atlas_shape_coordinate_s c_31 = {(1.0/3.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_32 = {(2.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_31, &c_32) == 0);
	
	struct atlas_shape_coordinate_s c_41 = {(1.0/99999999.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_42 = {(1.0/99999998.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_41, &c_42) == 0);
		
	
    
} END_TEST



#endif // _TEST_SHAPE_IMPL_GEOM_EQUAL_H_

