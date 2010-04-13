/*
 *  test_shape_impl_line_intersect.h
 *  atlas
 *
 *  Created by Peter Fenske on 4/7/10.
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
#ifndef _TEST_SHAPE_IMPL_LINE_INTERSECT_H_
#define _TEST_SHAPE_IMPL_LINE_INTERSECT_H_

#include <check.h>
#include "atlas_shape_impl.h"
#include "atlas_shape_impl_geometry.h"

START_TEST (test_shape_impl_line_intersect) {
	
//	printf("----1------\n");
//	printf("case with two vertical lines that don't meet\n");
    // case with two vertical lines that don't meet
	struct atlas_shape_coordinate_s c_l1s_1 = {3.0, -1.0};
	struct atlas_shape_coordinate_s c_l1e_1 = {3.0, 5.0};
	struct atlas_shape_coordinate_s c_l2s_1 = {7.5, -25.0};
	struct atlas_shape_coordinate_s c_l2e_1 = {7.5, 99.0};
	
	
	struct atlas_shape_coordinate_s	result1;
	fail_unless(atlas_shape_lines_intersect(&result1, &c_l1s_1, &c_l1e_1, &c_l2s_1, &c_l2e_1) == 0);
	
//	printf("----2------\n");
//	printf("first line is vertical and they intersect outside of box\n");
	
	// first line is vertical and they intersect outside of "box"
	struct atlas_shape_coordinate_s c_l1s_2 = {3.0, -1.0};
	struct atlas_shape_coordinate_s c_l1e_2 = {3.0, 1.0};
	struct atlas_shape_coordinate_s c_l2s_2 = {-1, -1};
	struct atlas_shape_coordinate_s c_l2e_2 = {1,1};
	
	struct atlas_shape_coordinate_s	result2;
	fail_unless(atlas_shape_lines_intersect(&result2, &c_l1s_2, &c_l1e_2, &c_l2s_2, &c_l2e_2) == 0);
	
//	printf("----3------\n");
//	printf("second line is vertical and they intersect\n");
	
	// second line is vertical and they intersect
	struct atlas_shape_coordinate_s c_l1s_3 = {-5,-5};
	struct atlas_shape_coordinate_s c_l1e_3 = {5,5};
	struct atlas_shape_coordinate_s c_l2s_3 = {1,-20};
	struct atlas_shape_coordinate_s c_l2e_3 = {1,20};
	
	struct atlas_shape_coordinate_s	result3;
	fail_unless(atlas_shape_lines_intersect(&result3, &c_l1s_3, &c_l1e_3, &c_l2s_3, &c_l2e_3) == 1);
	
//	printf("----4------\n");
//	printf("both lines are horizontally parallel but don't meet\n");
	
	// both lines are horizontally parallel but don't meet
	struct atlas_shape_coordinate_s c_l1s_4 = {-5,5};
	struct atlas_shape_coordinate_s c_l1e_4 = {5,5};
	struct atlas_shape_coordinate_s c_l2s_4 = {-2,2};
	struct atlas_shape_coordinate_s c_l2e_4 = {2,2};
	
	struct atlas_shape_coordinate_s	result4;
	fail_unless(atlas_shape_lines_intersect(&result4, &c_l1s_4, &c_l1e_4, &c_l2s_4, &c_l2e_4) == 0);
	
//	printf("----5------\n");
//	printf("lines are identical, but slope points in different direction\n");
	
	// lines are identical, but slope points in different direction
	struct atlas_shape_coordinate_s c_l1s_5 = {-5,-5};
	struct atlas_shape_coordinate_s c_l1e_5 = {5,5};
	struct atlas_shape_coordinate_s c_l2s_5 = {5,5};
	struct atlas_shape_coordinate_s c_l2e_5 = {-5,-5};
	
	struct atlas_shape_coordinate_s	result5;
	fail_unless(atlas_shape_lines_intersect(&result5, &c_l1s_5, &c_l1e_5, &c_l2s_5, &c_l2e_5) == 1);
	
//	printf("----6------\n");
//	printf("lines intersect\n");
	
	// lines intersect
	struct atlas_shape_coordinate_s c_l1s_6 = {-9,-9};
	struct atlas_shape_coordinate_s c_l1e_6 = {10,10};
	struct atlas_shape_coordinate_s c_l2s_6 = {-6,4};
	struct atlas_shape_coordinate_s c_l2e_6 = {2,0};
	
	struct atlas_shape_coordinate_s	result6;
	fail_unless(atlas_shape_lines_intersect(&result6, &c_l1s_6, &c_l1e_6, &c_l2s_6, &c_l2e_6) == 1);
	
//	printf("----7------\n");
//	printf("lines intersect outside of box\n");
	
	// lines intersect outside of "box"
	struct atlas_shape_coordinate_s c_l1s_7 = {-1,-3};
	struct atlas_shape_coordinate_s c_l1e_7 = {2,0};
	struct atlas_shape_coordinate_s c_l2s_7 = {2,5};
	struct atlas_shape_coordinate_s c_l2e_7 = {6,-1};
	
	struct atlas_shape_coordinate_s	result7;
	fail_unless(atlas_shape_lines_intersect(&result7, &c_l1s_7, &c_l1e_7, &c_l2s_7, &c_l2e_7) == 0);
	
    
} END_TEST



#endif // _TEST_SHAPE_IMPL_LINE_INTERSECT_H_

