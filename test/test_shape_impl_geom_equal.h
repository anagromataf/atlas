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
	// Checks for point equality
	struct atlas_shape_coordinate_s c_11 = {3.0, 1.0};
	struct atlas_shape_coordinate_s c_12 = {3.0, 1.0};
	fail_unless(atlas_shape_point_equal(&c_11, &c_12) == 1);
	
	struct atlas_shape_coordinate_s c_21 = {(1.0/3.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_22 = {(1.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_21, &c_22) == 1);
	
	struct atlas_shape_coordinate_s c_31 = {(1.0/3.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_32 = {(2.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_31, &c_32) == 0);
	
	struct atlas_shape_coordinate_s c_41 = {(1.0/9999999.0), (4.0/3.0)};
	struct atlas_shape_coordinate_s c_42 = {(1.0/9999998.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_41, &c_42) == 0);
		
	// Checks for point on line
	struct atlas_shape_coordinate_s c_pol_11 = {5.0, 5.0};
	struct atlas_shape_coordinate_s c_pol_12 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_pol_13 = {9.0, 9.0};
	fail_unless(atlas_shape_pol(&c_pol_11, &c_pol_12, &c_pol_13) == 1);
	
	struct atlas_shape_coordinate_s c_pol_21 = {5.00000000000001, 5.0};
	struct atlas_shape_coordinate_s c_pol_22 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_pol_23 = {9.0, 9.0};
	fail_unless(atlas_shape_pol(&c_pol_21, &c_pol_22, &c_pol_23) == 0);
	
	// Checks for arc equality
	struct atlas_shape_coordinate_s c_arc_111 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_arc_112 = {9.0, 9.0};
	struct atlas_shape_coordinate_s coords_arc_11[] = {c_arc_111, c_arc_112};
	struct atlas_shape_coordinate_s c_arc_121 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_arc_122 = {5.0, 5.0};
	struct atlas_shape_coordinate_s c_arc_123 = {9.0, 9.0};
	struct atlas_shape_coordinate_s coords_arc_12[] = {c_arc_121, c_arc_122, c_arc_123};
	fail_unless(atlas_shape_arc_equal(&coords_arc_11, 2, &coords_arc_12, 3) == 1);
	
	
	// Checks for arc equality
	struct atlas_shape_coordinate_s c_arc_211 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_arc_212 = {5.0, 5.0};
	struct atlas_shape_coordinate_s c_arc_213 = {6.0, 6.0};
	struct atlas_shape_coordinate_s c_arc_214 = {7.0, 7.0};
	struct atlas_shape_coordinate_s c_arc_215 = {8.0, 8.0};
	struct atlas_shape_coordinate_s c_arc_216 = {9.0, 9.0};
	struct atlas_shape_coordinate_s coords_arc_21[] = {c_arc_211, c_arc_212, c_arc_213, c_arc_214, c_arc_215, c_arc_216};
	struct atlas_shape_coordinate_s c_arc_221 = {0.0, 0.0};
	struct atlas_shape_coordinate_s c_arc_222 = {1.0, 1.0};
	struct atlas_shape_coordinate_s c_arc_223 = {2.0, 2.0};
	struct atlas_shape_coordinate_s c_arc_224 = {3.5, 3.5};
	struct atlas_shape_coordinate_s c_arc_225 = {5.0, 5.0};
	struct atlas_shape_coordinate_s c_arc_226 = {9.0, 9.0};
	struct atlas_shape_coordinate_s coords_arc_22[] = {c_arc_221, c_arc_222, c_arc_223, c_arc_224, c_arc_225, c_arc_226};
	fail_unless(atlas_shape_arc_equal(&coords_arc_21, 6, &coords_arc_22, 6) == 1);
	
	// modify a coordinate and check again
	coords_arc_22[1].latitude = 1.000001;
	fail_unless(atlas_shape_arc_equal(&coords_arc_21, 6, &coords_arc_22, 6) == 0);
    
} END_TEST



#endif // _TEST_SHAPE_IMPL_GEOM_EQUAL_H_

