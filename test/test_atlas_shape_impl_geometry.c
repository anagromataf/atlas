/*
 *  test_atlas_shape_impl_geometry.h.c
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

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "atlas_shape_impl.h"
#include "atlas_shape_impl_geometry.h"

// TODO: Adapt all tests for use with spheres

START_TEST (test_shape_impl_geometry_line_intersect) {
	
    // case with two vertical lines that don't meet
	atlas_shp_coordinate_t c_l1s_1 = {3.0, -1.0};
	atlas_shp_coordinate_t c_l1e_1 = {3.0, 5.0};
	atlas_shp_coordinate_t c_l2s_1 = {7.5, -25.0};
	atlas_shp_coordinate_t c_l2e_1 = {7.5, 99.0};
	
	
	atlas_shp_coordinate_t	result1;
	fail_unless(atlas_shape_lines_intersect(&result1, &c_l1s_1, &c_l1e_1, &c_l2s_1, &c_l2e_1) == 0);
	
	// first line is vertical and they intersect outside of "box"
	atlas_shp_coordinate_t c_l1s_2 = {3.0, -1.0};
	atlas_shp_coordinate_t c_l1e_2 = {3.0, 1.0};
	atlas_shp_coordinate_t c_l2s_2 = {-1, -1};
	atlas_shp_coordinate_t c_l2e_2 = {1,1};
	
	atlas_shp_coordinate_t	result2;
	fail_unless(atlas_shape_lines_intersect(&result2, &c_l1s_2, &c_l1e_2, &c_l2s_2, &c_l2e_2) == 0);
	
	// second line is vertical and they intersect
	atlas_shp_coordinate_t c_l1s_3 = {-5,-5};
	atlas_shp_coordinate_t c_l1e_3 = {5,5};
	atlas_shp_coordinate_t c_l2s_3 = {1,-20};
	atlas_shp_coordinate_t c_l2e_3 = {1,20};
	
	atlas_shp_coordinate_t	result3;
	fail_unless(atlas_shape_lines_intersect(&result3, &c_l1s_3, &c_l1e_3, &c_l2s_3, &c_l2e_3) == 1);
	
	// both lines are horizontally parallel but don't meet
	atlas_shp_coordinate_t c_l1s_4 = {-5,5};
	atlas_shp_coordinate_t c_l1e_4 = {5,5};
	atlas_shp_coordinate_t c_l2s_4 = {-2,2};
	atlas_shp_coordinate_t c_l2e_4 = {2,2};
	
	atlas_shp_coordinate_t	result4;
	fail_unless(atlas_shape_lines_intersect(&result4, &c_l1s_4, &c_l1e_4, &c_l2s_4, &c_l2e_4) == 0);
	
	// lines are identical, but slope points in different direction
	atlas_shp_coordinate_t c_l1s_5 = {-5,-5};
	atlas_shp_coordinate_t c_l1e_5 = {5,5};
	atlas_shp_coordinate_t c_l2s_5 = {5,5};
	atlas_shp_coordinate_t c_l2e_5 = {-5,-5};
	
	atlas_shp_coordinate_t	result5;
	fail_unless(atlas_shape_lines_intersect(&result5, &c_l1s_5, &c_l1e_5, &c_l2s_5, &c_l2e_5) == 1);
	
	// lines intersect
	atlas_shp_coordinate_t c_l1s_6 = {-9,-9};
	atlas_shp_coordinate_t c_l1e_6 = {10,10};
	atlas_shp_coordinate_t c_l2s_6 = {-6,4};
	atlas_shp_coordinate_t c_l2e_6 = {2,0};
	
	atlas_shp_coordinate_t	result6;
	fail_unless(atlas_shape_lines_intersect(&result6, &c_l1s_6, &c_l1e_6, &c_l2s_6, &c_l2e_6) == 1);
	
	// lines intersect outside of "box"
	atlas_shp_coordinate_t c_l1s_7 = {-1,-3};
	atlas_shp_coordinate_t c_l1e_7 = {2,0};
	atlas_shp_coordinate_t c_l2s_7 = {2,5};
	atlas_shp_coordinate_t c_l2e_7 = {6,-1};
	
	atlas_shp_coordinate_t	result7;
	fail_unless(atlas_shape_lines_intersect(&result7, &c_l1s_7, &c_l1e_7, &c_l2s_7, &c_l2e_7) == 0);
	
    
} END_TEST

START_TEST (test_shape_impl_geometry_point_equal) {
	// Checks for point equality
	atlas_shp_coordinate_t c_11 = {3.0, 1.0};
	atlas_shp_coordinate_t c_12 = {3.0, 1.0};
	fail_unless(atlas_shape_point_equal(&c_11, &c_12) == 1);
	
	atlas_shp_coordinate_t c_21 = {(1.0/3.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_22 = {(1.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_21, &c_22) == 1);
	
	atlas_shp_coordinate_t c_31 = {(1.0/3.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_32 = {(2.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_31, &c_32) == 0);
	
	atlas_shp_coordinate_t c_41 = {(1.0/9999999.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_42 = {(1.0/9999998.0), (4.0/3.0)};
	fail_unless(atlas_shape_point_equal(&c_41, &c_42) == 0);
	
} END_TEST


START_TEST (test_shape_impl_geometry_pol) {	
	
	// Checks for point on line
	atlas_shp_coordinate_t c_pol_11 = {5.0, 5.0};
	atlas_shp_coordinate_t c_pol_12 = {0.0, 0.0};
	atlas_shp_coordinate_t c_pol_13 = {9.0, 9.0};
	fail_unless(atlas_shape_pol(&c_pol_11, &c_pol_12, &c_pol_13) == 1);
	
	atlas_shp_coordinate_t c_pol_21 = {5.00000000000001, 5.0};
	atlas_shp_coordinate_t c_pol_22 = {0.0, 0.0};
	atlas_shp_coordinate_t c_pol_23 = {9.0, 9.0};
	fail_unless(atlas_shape_pol(&c_pol_21, &c_pol_22, &c_pol_23) == 0);
	
} END_TEST

START_TEST (test_shape_impl_geometry_arc_equal) {	
	
	// Checks for arc equality
	atlas_shp_coordinate_t c_arc_111 = {0.0, 0.0};
	atlas_shp_coordinate_t c_arc_112 = {9.0, 9.0};
	atlas_shp_coordinate_t coords_arc_11[] = {c_arc_111, c_arc_112};
	atlas_shp_coordinate_t c_arc_121 = {0.0, 0.0};
	atlas_shp_coordinate_t c_arc_122 = {5.0, 5.0};
	atlas_shp_coordinate_t c_arc_123 = {9.0, 9.0};
	atlas_shp_coordinate_t coords_arc_12[] = {c_arc_121, c_arc_122, c_arc_123};
	
	fail_unless(atlas_shape_arc_equal(coords_arc_11, 2, coords_arc_12, 3) == 1);
	
	
	// Checks for arc equality
	atlas_shp_coordinate_t c_arc_211 = {0.0, 0.0};
	atlas_shp_coordinate_t c_arc_212 = {5.0, 5.0};
	atlas_shp_coordinate_t c_arc_213 = {6.0, 6.0};
	atlas_shp_coordinate_t c_arc_214 = {7.0, 7.0};
	atlas_shp_coordinate_t c_arc_215 = {8.0, 8.0};
	atlas_shp_coordinate_t c_arc_216 = {9.0, 9.0};
	atlas_shp_coordinate_t coords_arc_21[] = {c_arc_211, c_arc_212, c_arc_213, c_arc_214, c_arc_215, c_arc_216};
	atlas_shp_coordinate_t c_arc_221 = {0.0, 0.0};
	atlas_shp_coordinate_t c_arc_222 = {1.0, 1.0};
	atlas_shp_coordinate_t c_arc_223 = {2.0, 2.0};
	atlas_shp_coordinate_t c_arc_224 = {3.5, 3.5};
	atlas_shp_coordinate_t c_arc_225 = {5.0, 5.0};
	atlas_shp_coordinate_t c_arc_226 = {9.0, 9.0};
	atlas_shp_coordinate_t coords_arc_22[] = {c_arc_221, c_arc_222, c_arc_223, c_arc_224, c_arc_225, c_arc_226};
	
	fail_unless(atlas_shape_arc_equal(coords_arc_21, 6, coords_arc_22, 6) == 1);
	
	// modify a coordinate and check again
	coords_arc_22[1].latitude = 1.000001;
	
	fail_unless(atlas_shape_arc_equal(coords_arc_21, 6, coords_arc_22, 6) == 0);
    
} END_TEST


START_TEST (test_shape_impl_geometry_intersect_gc) {
	// first great circle is the equator
	atlas_shp_coordinate_t c_gc_111 = {20.0,  0.0};
	atlas_shp_coordinate_t c_gc_112 = {60.0,  0.0};
	// second great circle is the 10°/-170° longitude
	atlas_shp_coordinate_t c_gc_121 = {10.0, 10.0};
	atlas_shp_coordinate_t c_gc_122 = {10.0, 50.0};
	
	atlas_shp_coordinate_t result11;
	atlas_shp_coordinate_t result12;
	
	int result1 = atlas_shape_lines_intersect_gc(&result11, &result12, &c_gc_111, &c_gc_112, &c_gc_121, &c_gc_122);
	
	fail_unless(result1 == 0);
	
	printf("GCI (intersect1): lat=%f lon=%f\n", result11.latitude, result11.longitude);
	printf("GCI (intersect2): lat=%f lon=%f\n", result12.latitude, result12.longitude);
	
	fail_if( abs_value_check(result11.latitude, 0.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result11.longitude, 10.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result12.latitude, 0.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result12.longitude, -170.0, 1.0E-10) == 0 );
	
	
	
	// KSEA
	atlas_shp_coordinate_t c_gc_211 = {-122.309306, 47.449};
	// LOWW
	atlas_shp_coordinate_t c_gc_212 = {16.569722, 48.110278};
	// KJFK
	atlas_shp_coordinate_t c_gc_221 = {-73.778925, 40.63975};
	// EFHK
	atlas_shp_coordinate_t c_gc_222 = {24.963333, 60.317222};
	
	atlas_shp_coordinate_t result21;
	atlas_shp_coordinate_t result22;
	
	int result2 = atlas_shape_lines_intersect_gc(&result21, &result22, &c_gc_211, &c_gc_212, &c_gc_221, &c_gc_222);
	
	fail_unless(result2 == 0);
	
	printf("GCI (intersect1): lat=%f lon=%f\n", result21.latitude, result21.longitude);
	printf("GCI (intersect2): lat=%f lon=%f\n", result22.latitude, result22.longitude);
	
	fail_if( abs_value_check(result21.latitude, 64.0, 1.0) == 0 );
	fail_if( abs_value_check(result21.longitude, -4.0, 1.0) == 0 );
	fail_if( abs_value_check(result22.latitude, -64.0, 1.0) == 0 );
	fail_if( abs_value_check(result22.longitude, 176.0, 1.0) == 0 );
	
	
	// meridian 1
	atlas_shp_coordinate_t c_gc_311 = {20.0, 90.0};
	atlas_shp_coordinate_t c_gc_312 = {20.0, 20.0};
	// meridian 2
	atlas_shp_coordinate_t c_gc_321 = {-60.0, 70.0};
	atlas_shp_coordinate_t c_gc_322 = {-60.0, 50.0};
	
	atlas_shp_coordinate_t result31;
	atlas_shp_coordinate_t result32;
	
	int result3 = atlas_shape_lines_intersect_gc(&result31, &result32, &c_gc_311, &c_gc_312, &c_gc_321, &c_gc_322);
	
	fail_unless(result3 == 0);
	
	printf("GCI (intersect1): lat=%f lon=%f\n", result31.latitude, result31.longitude);
	printf("GCI (intersect2): lat=%f lon=%f\n", result32.latitude, result32.longitude);
	
	fail_if( abs_value_check(result31.latitude, 90.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result31.longitude, 0.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result32.latitude, -90.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result32.longitude, 0.0, 1.0E-10) == 0 );
	
	atlas_shp_coordinate_t c_gc_411 = {5.0, 89.0};
	atlas_shp_coordinate_t c_gc_412 = {10.0, -89.0};
	atlas_shp_coordinate_t c_gc_421 = {-175.0, -89.0};
	atlas_shp_coordinate_t c_gc_422 = {-170.0, 89.0};
		
	atlas_shp_coordinate_t result41;
	atlas_shp_coordinate_t result42;
	
	int result4 = atlas_shape_lines_intersect_gc(&result41, &result42, &c_gc_411, &c_gc_412, &c_gc_421, &c_gc_422);
	
	fail_unless( result4 == 3 );
	
	printf("GCI (intersect1): lat=%f lon=%f\n", result41.latitude, result41.longitude);
	printf("GCI (intersect2): lat=%f lon=%f\n", result42.latitude, result42.longitude);
	
	
} END_TEST

int 
abs_value_check(double actual, 
				double expected, 
				double precision){
	if (fabs( actual - expected ) > precision){
		return 0;
	} else {
		return 1;
	}
}

#pragma mark -
#pragma mark Fixtures

static void setup() {
    printf(">>>\n");
}

static void teardown() {
    printf("<<<\n");
}


#pragma mark -
#pragma mark Shape Impl Geometry Suites

Suite * shape_impl_geometry_suite(void) {
    
    Suite *s = suite_create("Shape Impl Geometry");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
	
    tcase_add_test(tc_create, test_shape_impl_geometry_line_intersect);
	tcase_add_test(tc_create, test_shape_impl_geometry_point_equal);
	tcase_add_test(tc_create, test_shape_impl_geometry_pol);
	tcase_add_test(tc_create, test_shape_impl_geometry_arc_equal);
	tcase_add_test(tc_create, test_shape_impl_geometry_intersect_gc);
    
    suite_add_tcase(s, tc_create);
    
    return s;
}