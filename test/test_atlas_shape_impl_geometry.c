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
#pragma mark test_shape_impl_geometry_point_equal

START_TEST (test_shape_impl_geometry_point_equal) {
	// Checks for point equality
	atlas_shp_coordinate_t c_11 = {3.0, 1.0};
	atlas_shp_coordinate_t c_12 = {3.0, 1.0};
	fail_unless(atlas_shape_points_equal(&c_11, &c_12) == 1);
	
	atlas_shp_coordinate_t c_21 = {(1.0/3.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_22 = {(1.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_points_equal(&c_21, &c_22) == 1);
	
	atlas_shp_coordinate_t c_31 = {(1.0/3.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_32 = {(2.0/3.0), (4.0/3.0)};
	fail_unless(atlas_shape_points_equal(&c_31, &c_32) == 0);
	
	atlas_shp_coordinate_t c_41 = {(1.0/999999.0), (4.0/3.0)};
	atlas_shp_coordinate_t c_42 = {(1.0/999998.0), (4.0/3.0)};
	fail_unless(atlas_shape_points_equal(&c_41, &c_42) == 0);
	
} END_TEST


#pragma mark -
#pragma mark test_shape_impl_geometry_arc_equal

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


#pragma mark -
#pragma mark test_shape_impl_gc_intersection

START_TEST (test_shape_impl_gc_intersection) {
	// first great circle is the equator
	atlas_shp_coordinate_t c_gc_111 = {20.0,  0.0};
	atlas_shp_coordinate_t c_gc_112 = {60.0,  0.0};
	// second great circle is the 10°/-170° longitude
	atlas_shp_coordinate_t c_gc_121 = {10.0, 10.0};
	atlas_shp_coordinate_t c_gc_122 = {10.0, 50.0};
	
	atlas_shp_coordinate_t result11;
	atlas_shp_coordinate_t result12;
	
	int result1 = atlas_shape_gc_intersection(&result11, &result12, &c_gc_111, &c_gc_112, &c_gc_121, &c_gc_122);
	
	fail_unless(result1 == 0);
	
	//printf("GCI (intersect1): lat=%f lon=%f\n", result11.latitude, result11.longitude);
	//printf("GCI (intersect2): lat=%f lon=%f\n", result12.latitude, result12.longitude);
	
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
	
	int result2 = atlas_shape_gc_intersection(&result21, &result22, &c_gc_211, &c_gc_212, &c_gc_221, &c_gc_222);
	
	fail_unless(result2 == 0);
	
	//printf("GCI (intersect1): lat=%f lon=%f\n", result21.latitude, result21.longitude);
	//printf("GCI (intersect2): lat=%f lon=%f\n", result22.latitude, result22.longitude);
	
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
	
	int result3 = atlas_shape_gc_intersection(&result31, &result32, &c_gc_311, &c_gc_312, &c_gc_321, &c_gc_322);
	
	fail_unless(result3 == 0);
	
	//printf("GCI (intersect1): lat=%f lon=%f\n", result31.latitude, result31.longitude);
	//printf("GCI (intersect2): lat=%f lon=%f\n", result32.latitude, result32.longitude);
	
	fail_if( abs_value_check(result31.latitude, 90.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result31.longitude, 0.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result32.latitude, -90.0, 1.0E-10) == 0 );
	fail_if( abs_value_check(result32.longitude, 0.0, 1.0E-10) == 0 );
	
	
	//printf("Equal GC\n");
	atlas_shp_coordinate_t c_gc_411 = {5.0, 89.0};
	atlas_shp_coordinate_t c_gc_412 = {10.0, -89.0};
	atlas_shp_coordinate_t c_gc_421 = {-175.0, -89.0};
	atlas_shp_coordinate_t c_gc_422 = {-170.0, 89.0};
	
	atlas_shp_coordinate_t result41;
	atlas_shp_coordinate_t result42;
	
	int result4 = atlas_shape_gc_intersection(&result41, &result42, &c_gc_411, &c_gc_412, &c_gc_421, &c_gc_422);
	
	fail_unless( result4 == 3 );
	
	
	//printf("GCI (intersect1): lat=%f lon=%f\n", result41.latitude, result41.longitude);
	//printf("GCI (intersect2): lat=%f lon=%f\n", result42.latitude, result42.longitude);
	
	
} END_TEST


#pragma mark -
#pragma mark test_shape_impl_geometry_lon_range_overlap

START_TEST (test_shape_impl_geometry_lon_range_overlap) {	
	/*
	 * Tests for check, if two latitude ranges overlap
	 *
	 * Latitude is always 0.0, since the function tested does not care about
	 * the latitude. However, when testing meridians the latitude is used to
	 * emphazize the meridian test.
	 */
	
	// no overlap
	atlas_shp_coordinate_t c111 = {-30.0 , 0.0};
	atlas_shp_coordinate_t c112 = { 20.0 , 0.0};
	atlas_shp_coordinate_t c121 = { 40.0 , 0.0};
	atlas_shp_coordinate_t c122 = { 50.0 , 0.0};
	fail_unless (0 == atlas_lon_range_overlaps(&c111, &c112, &c121, &c122));
	
	// overlap
	atlas_shp_coordinate_t c211 = {-30.0 , 0.0};
	atlas_shp_coordinate_t c212 = { 20.0 , 0.0};
	atlas_shp_coordinate_t c221 = { 50.0 , 0.0};
	atlas_shp_coordinate_t c222 = {  0.0 , 0.0};
	fail_unless (1 == atlas_lon_range_overlaps(&c211, &c212, &c221, &c222));
	
	// no overlap, using meridian
	atlas_shp_coordinate_t c311 = {-10.0 , -80.0};
	atlas_shp_coordinate_t c312 = {-10.0 ,  80.0};
	atlas_shp_coordinate_t c321 = {  0.0 ,  0.0};
	atlas_shp_coordinate_t c322 = { 30.0 ,  0.0};
	fail_unless (0 == atlas_lon_range_overlaps(&c311, &c312, &c321, &c322));
	
	// overlap, using meridian
	atlas_shp_coordinate_t c411 = { 15.0 , -80.0};
	atlas_shp_coordinate_t c412 = { 15.0 ,  80.0};
	atlas_shp_coordinate_t c421 = {  0.0 ,  0.0};
	atlas_shp_coordinate_t c422 = { 30.0 ,  0.0};
	fail_unless (1 == atlas_lon_range_overlaps(&c411, &c412, &c421, &c422));
	
	// no overlap, second segment across 180 deg meridian
	atlas_shp_coordinate_t c511 = {-150.0 , 0.0};
	atlas_shp_coordinate_t c512 = { -10.0 , 0.0};
	atlas_shp_coordinate_t c521 = { 170.0 , 0.0};
	atlas_shp_coordinate_t c522 = {-170.0 , 0.0};
	fail_unless (0 == atlas_lon_range_overlaps(&c511, &c512, &c521, &c522));
	
	// overlap, second segment across 180 deg meridian
	atlas_shp_coordinate_t c611 = {-150.0 , 0.0};
	atlas_shp_coordinate_t c612 = { -10.0 , 0.0};
	atlas_shp_coordinate_t c621 = { 170.0 , 0.0};
	atlas_shp_coordinate_t c622 = {-140.0 , 0.0};
	fail_unless (1 == atlas_lon_range_overlaps(&c611, &c612, &c621, &c622));
	
	// overlap of two meridians
	atlas_shp_coordinate_t c711 = {20.0 , -80.0};
	atlas_shp_coordinate_t c712 = {20.0 ,  10.0};
	atlas_shp_coordinate_t c721 = {20.0 ,   5.0};
	atlas_shp_coordinate_t c722 = {20.0 ,  80.0};
	fail_unless (1 == atlas_lon_range_overlaps(&c711, &c712, &c721, &c722));
	
} END_TEST


#pragma mark -
#pragma mark test_shape_impl_geometry_initial_course

START_TEST (test_shape_impl_geometry_initial_course) {
	// arbitrary course northern hemisphere
	int i = 1;
	atlas_shp_coordinate_t c11 = {-73.7789, 40.6397};
	atlas_shp_coordinate_t c12 = { 13.2833, 52.5667};
	double hdg11, hdg12;
	atlas_gc_initial_course(&hdg11, &c11, &c12);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg11);
	atlas_gc_initial_course(&hdg12, &c12, &c11);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg12);
	fail_unless(hdg11 > 46 && hdg11 < 47);
	fail_unless(hdg12 > 295 && hdg12 < 296);
	
	// arbitrary course southern hemisphere
	i++;
	atlas_shp_coordinate_t c21 = {-120, -40};
	atlas_shp_coordinate_t c22 = {  15, -60};
	
	double hdg21, hdg22;
	atlas_gc_initial_course(&hdg21, &c21, &c22);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg21);
	atlas_gc_initial_course(&hdg22, &c22, &c21);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg22);
	fail_unless(hdg21 > 158 && hdg21 < 159);
	fail_unless(hdg22 > 214 && hdg22 < 215);
	
	// course along meridian
	i++;
	atlas_shp_coordinate_t c31 = {15, 50};
	atlas_shp_coordinate_t c32 = {15, -20};
	
	double hdg31, hdg32;
	atlas_gc_initial_course(&hdg31, &c31, &c32);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg31);
	atlas_gc_initial_course(&hdg32, &c32, &c31);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg32);
	fail_unless(abs_value_check(hdg31, 180.0, 0.1) == 1);
	fail_unless(abs_value_check(hdg32, 0.0, 0.1) == 1);
	
	// course over pole
	i++;
	atlas_shp_coordinate_t c41 = {-80, -70};
	atlas_shp_coordinate_t c42 = {100, -75};
	
	double hdg41, hdg42;
	atlas_gc_initial_course(&hdg41, &c41, &c42);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg41);
	atlas_gc_initial_course(&hdg42, &c42, &c41);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg42);
	fail_unless(abs_value_check(hdg41, 180.0, 0.1) == 1);
	fail_unless(abs_value_check(hdg42, 180.0, 0.1) == 1);
	
	// course southern hemisphere across dateline
	i++;
	atlas_shp_coordinate_t c51 = { 110, -40};
	atlas_shp_coordinate_t c52 = {-150, -50};
    
	double hdg51, hdg52;
	atlas_gc_initial_course(&hdg51, &c51, &c52);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg51);
	atlas_gc_initial_course(&hdg52, &c52, &c51);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg52);
	fail_unless(hdg51 > 136 && hdg51 < 137);
	fail_unless(hdg52 > 235 && hdg52 < 236);
	
	// point only
	i++;
	atlas_shp_coordinate_t c61 = {0, 0};
	atlas_shp_coordinate_t c62 = {0, 0};
	;
	double hdg61, hdg62;
	atlas_gc_initial_course(&hdg61, &c61, &c62);
	//printf("TEST initial_course %i: course1 = %f \n", i, hdg61);
	atlas_gc_initial_course(&hdg52, &c62, &c61);
	//printf("TEST initial_course %i: course2 = %f \n", i, hdg62);
	
	
} END_TEST


#pragma mark -
#pragma mark test_shape_impl_geometry_lat_range_gc_seg

START_TEST (test_shape_impl_geometry_lat_range_gc_seg) {
	// arbitrary course northern hemisphere
	int i = 1;
	// New York to Berlin
	atlas_shp_coordinate_t c11 = {-73.7789, 40.6397};
	atlas_shp_coordinate_t c12 = { 13.2833, 52.5667};
	double result_min_1, result_max_1;
	
	int latr_1 = atlas_lat_range_gc_seg(&result_min_1, &result_max_1,
										&c11, &c12);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_1);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_1);
	
	fail_unless(40 < result_min_1 < 41);
	fail_unless(56 < result_max_1 < 57);
	
	
	// arbitrary course southern hemisphere
	i++;
	atlas_shp_coordinate_t c21 = { 110, -40};
	atlas_shp_coordinate_t c22 = {-150, -50};
	double result_min_2, result_max_2;
	
	int latr_2 = atlas_lat_range_gc_seg(&result_min_2, &result_max_2,
										&c21, &c22);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_2);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_2);
	
	fail_unless(-57.9 > result_min_2 > -58.1);
	fail_unless(abs_value_check(result_max_2, -40.0, 0.1));
	
	
	// northern hemisphere, one points south, the other one north
	i++;
	atlas_shp_coordinate_t c31 = {-10, 50};
	atlas_shp_coordinate_t c32 = { 30, 30};
	double result_min_3, result_max_3;
	
	int latr_3 = atlas_lat_range_gc_seg(&result_min_3, &result_max_3,
										&c31, &c32);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_3);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_3);
	
	fail_unless(abs_value_check(result_max_3, 50.0, 0.1));
	fail_unless(abs_value_check(result_min_3, 30.0, 0.1));
	
	
	// northern hemisphere, one points south, the other one north
	i++;
	atlas_shp_coordinate_t c41 = {-10, -50};
	atlas_shp_coordinate_t c42 = {-30, -30};
	double result_min_4, result_max_4;
	
	int latr_4 = atlas_lat_range_gc_seg(&result_min_4, &result_max_4,
										&c41, &c42);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_4);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_4);
	
	fail_unless(abs_value_check(result_max_4, -30.0, 0.1));
	fail_unless(abs_value_check(result_min_4, -50.0, 0.1));
	
	// meridian not crossing pole
	i++;
	atlas_shp_coordinate_t c51 = {10,  80};
	atlas_shp_coordinate_t c52 = {10, -70};
	double result_min_5, result_max_5;
	
	int latr_5 = atlas_lat_range_gc_seg(&result_min_5, &result_max_5,
										&c51, &c52);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_5);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_5);
	
	fail_unless(abs_value_check(result_max_5,  80.0, 0.1));
	fail_unless(abs_value_check(result_min_5, -70.0, 0.1));
	
	// meridian crossing north pole
	i++;
	atlas_shp_coordinate_t c61 = {-170, 80};
	atlas_shp_coordinate_t c62 = {  10, 60};
	double result_min_6, result_max_6;
	
	int latr_6 = atlas_lat_range_gc_seg(&result_min_6, &result_max_6,
										&c61, &c62);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_6);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_6);
	
	fail_unless(abs_value_check(result_max_6, 90.0, 0.1));
	fail_unless(abs_value_check(result_min_6, 60.0, 0.1));
	
	// meridian crossing south pole
	i++;
	atlas_shp_coordinate_t c71 = {-110, -80};
	atlas_shp_coordinate_t c72 = {  70, -60};
	double result_min_7, result_max_7;
	
	int latr_7 = atlas_lat_range_gc_seg(&result_min_7, &result_max_7,
										&c71, &c72);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_7);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_7);
	
	fail_unless(abs_value_check(result_max_7, -60.0, 0.1));
	fail_unless(abs_value_check(result_min_7, -90.0, 0.1));
	
	// BES-NOU (Brest, F - Nouméa, NC), both point north, different hemispheres
	i++;
	atlas_shp_coordinate_t c81 = { -4,  48};
	atlas_shp_coordinate_t c82 = {166, -22};
	double result_min_8, result_max_8;
	
	int latr_8 = atlas_lat_range_gc_seg(&result_min_8, &result_max_8,
										&c81, &c82);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_8);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_8);
	
	fail_unless(76.3 < result_max_8 && result_max_8 < 76.4);
	fail_unless(abs_value_check(result_min_8, -22.0, 0.1));
	
	// both antipodal
	i++;
	atlas_shp_coordinate_t c91 = {-80, -10};
	atlas_shp_coordinate_t c92 = {100,  10};
	double result_min_9, result_max_9;
	
	int latr_9 = atlas_lat_range_gc_seg(&result_min_9, &result_max_9,
										&c91, &c92);
	
	//printf("TEST lat_range_gc_seg %i: min-lat = %f \n", i, result_min_9);
	//printf("TEST lat_range_gc_seg %i: max-lat = %f \n", i, result_max_9);
	
	fail_unless(abs_value_check(result_min_9, -10.0, 0.1));
	fail_unless(abs_value_check(result_max_9,  10.0, 0.1));
	
	
	
} END_TEST


#pragma mark -
#pragma mark test_shape_impl_geometry_gc_seg_intersect

START_TEST (test_shape_impl_geometry_gc_seg_intersect) {	
	int i = 1;
	
	// two arbitrary courses on northern hemisphere
	// Seattle to Vienna
	atlas_shp_coordinate_t c111 = {-122.3094, 47.4488};
	atlas_shp_coordinate_t c112 = {  16.5697, 48.1102};
	// New York to Helsinki
	atlas_shp_coordinate_t c121 = {-73.7789, 40.6397};
	atlas_shp_coordinate_t c122 = { 24.9633, 60.3172};
	atlas_shp_coordinate_t result_1;
	int has_intersect_1 
	= atlas_shape_gc_segments_intersection(&result_1, 
										   &c111, &c112, 
										   &c121, &c122);
	//printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
	//	   i, result_1.latitude, result_1.longitude);
	fail_unless(has_intersect_1 == 1);
	
	
	// longitude ranges overlap, latitude ranges do not, segments do not intersect
	i++;
	atlas_shp_coordinate_t c211 = { 140, 10};
	atlas_shp_coordinate_t c212 = {-160, 10};
	atlas_shp_coordinate_t c221 = { 170, 20};
	atlas_shp_coordinate_t c222 = {-150, 20};
	atlas_shp_coordinate_t result_2;
	int has_intersect_2 = 
	atlas_shape_gc_segments_intersection(&result_2, 
										 &c211, &c212, 
										 &c221, &c222);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_2.latitude, result_2.longitude);
	fail_unless(has_intersect_2 == 0);
	
	// latitude ranges overlap, latitude ranges do not, segments do not intersect
	i++;
	atlas_shp_coordinate_t c311 = { 160, -10};
	atlas_shp_coordinate_t c312 = { 170,  20};
	atlas_shp_coordinate_t c321 = {-160,  50};
	atlas_shp_coordinate_t c322 = {-170,   0};
	atlas_shp_coordinate_t result_3;
	int has_intersect_3 
	= atlas_shape_gc_segments_intersection(&result_3, 
										   &c311, &c312, 
										   &c321, &c322);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_3.latitude, result_3.longitude);
	fail_unless(has_intersect_3 == 0);
	
	// one segment is a meridian, the other one is not, they do intersect
	i++;
	atlas_shp_coordinate_t c411 = {-10, -10};
	atlas_shp_coordinate_t c412 = {-10, -60};
	atlas_shp_coordinate_t c421 = {-60, -30};
	atlas_shp_coordinate_t c422 = { 30, -40};
	atlas_shp_coordinate_t result_4;
	int has_intersect_4 
	= atlas_shape_gc_segments_intersection(&result_4, 
										   &c411, &c412, 
										   &c421, &c422);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_4.latitude, result_4.longitude);
	fail_unless(has_intersect_4 == 1);
	
	
	// intersection on north pole
	i++;
	atlas_shp_coordinate_t c511 = {-100, 50};
	atlas_shp_coordinate_t c512 = {  80, 60};
	atlas_shp_coordinate_t c521 = { -80, 30};
	atlas_shp_coordinate_t c522 = { 100, 80};
	atlas_shp_coordinate_t result_5;
	int has_intersect_5 
	= atlas_shape_gc_segments_intersection(&result_5, 
										   &c511, &c512, 
										   &c521, &c522);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_5.latitude, result_5.longitude);
	fail_unless(has_intersect_5 == 1);
	
	
	// BES-NOU (Brest, F - Nouméa, NC), both point north, different hemispheres
	// intersection around 100 E 74 N
	i++;
	atlas_shp_coordinate_t c611 = { -4,  48};
	atlas_shp_coordinate_t c612 = {166, -22};
	atlas_shp_coordinate_t c621 = { -80, 30};
	atlas_shp_coordinate_t c622 = { 100, 60};
	atlas_shp_coordinate_t result_6;
	int has_intersect_6 
	= atlas_shape_gc_segments_intersection(&result_6, 
										   &c611, &c612, 
										   &c621, &c622);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_6.latitude, result_6.longitude);
	fail_unless(has_intersect_6 == 1);
	
	
	
	// second segment is a meridian, the other one is not, they do not intersect
	i++;
	atlas_shp_coordinate_t c711 = { -5,  50};
	atlas_shp_coordinate_t c712 = {170, -20};
	atlas_shp_coordinate_t c721 = { -80, 30};
	atlas_shp_coordinate_t c722 = { 100, 85};
	atlas_shp_coordinate_t result_7;
	int has_intersect_7 
	= atlas_shape_gc_segments_intersection(&result_7, 
										   &c711, &c712, 
										   &c721, &c722);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_7.latitude, result_7.longitude);
	fail_unless(has_intersect_7 == 0);
	
	
	// first segment is a meridian, the other one is not, they do not intersect
	i++;
	atlas_shp_coordinate_t c811 = { -80, 30};
	atlas_shp_coordinate_t c812 = { 100, 85};
	atlas_shp_coordinate_t c821 = { -5,  50};
	atlas_shp_coordinate_t c822 = {170, -20};
	atlas_shp_coordinate_t result_8;
	int has_intersect_8 
	= atlas_shape_gc_segments_intersection(&result_8, 
										   &c811, &c812, 
										   &c821, &c822);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_8.latitude, result_8.longitude);
	fail_unless(has_intersect_8 == 0);
	
	
	// overlapping lat and lon ranges, but no intersection
	i++;
	atlas_shp_coordinate_t c911 = { -5,  50};
	atlas_shp_coordinate_t c912 = {170, -20};
	atlas_shp_coordinate_t c921 = {-110, 30};
	atlas_shp_coordinate_t c922 = {  80, 85};
	atlas_shp_coordinate_t result_9;
	int has_intersect_9 
	= atlas_shape_gc_segments_intersection(&result_9, 
										   &c911, &c912, 
										   &c921, &c922);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_9.latitude, result_9.longitude);
	fail_unless(has_intersect_9 == 0);
	
	
	// both are meridians that overlap
	i++;
	atlas_shp_coordinate_t c1011 = {-100, -50};
	atlas_shp_coordinate_t c1012 = {-100, -20};
	atlas_shp_coordinate_t c1021 = {-100, -30};
	atlas_shp_coordinate_t c1022 = {-100,  85};
	atlas_shp_coordinate_t result_10;
	int has_intersect_10 
	= atlas_shape_gc_segments_intersection(&result_10, 
										   &c1011, &c1012, 
										   &c1021, &c1022);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_10.latitude, result_10.longitude);
	fail_unless(has_intersect_10 == 1);
	
	// both are meridians (one over pole) that overlap
	i++;
	atlas_shp_coordinate_t c1111 = {-100, -50};
	atlas_shp_coordinate_t c1112 = {  80, -20};
	atlas_shp_coordinate_t c1121 = {-100, -60};
	atlas_shp_coordinate_t c1122 = {-100,  10};
	atlas_shp_coordinate_t result_11;
	int has_intersect_11
	= atlas_shape_gc_segments_intersection(&result_11, 
										   &c1111, &c1112, 
										   &c1121, &c1122);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_11.latitude, result_11.longitude);
	fail_unless(has_intersect_11 == 1);
	
	// both are meridians that do not overlap
	i++;
	atlas_shp_coordinate_t c1211 = {-100, -50};
	atlas_shp_coordinate_t c1212 = {-100, -20};
	atlas_shp_coordinate_t c1221 = {-100, -10};
	atlas_shp_coordinate_t c1222 = {-100,  85};
	atlas_shp_coordinate_t result_12;
	int has_intersect_12 
	= atlas_shape_gc_segments_intersection(&result_12, 
										   &c1211, &c1212, 
										   &c1221, &c1222);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_12.latitude, result_12.longitude);
	fail_unless(has_intersect_12 == 0);
	
	
	// Kopenhagen-Bern and Berlin-Paris, intersection around 51,5 N 9,8 E
	i++;
	atlas_shp_coordinate_t c1311 = {12.6, 55.6};
	atlas_shp_coordinate_t c1312 = { 7.4, 46.9};
	atlas_shp_coordinate_t c1321 = {13.4, 52.5};
	atlas_shp_coordinate_t c1322 = { 2.4, 48.8};
	atlas_shp_coordinate_t result_13;
	int has_intersect_13 
	= atlas_shape_gc_segments_intersection(&result_13, 
										   &c1311, &c1312, 
										   &c1321, &c1322);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_13.latitude, result_13.longitude);
	fail_unless(has_intersect_13 == 1);
	
	// intersection around 65,9 N 22,3 E
	i++;
	atlas_shp_coordinate_t c1411 = { 23,  70};
	atlas_shp_coordinate_t c1412 = { 18, -34};
	atlas_shp_coordinate_t c1421 = {140,  35};
	atlas_shp_coordinate_t c1422 = {-10,  40};
	atlas_shp_coordinate_t result_14;
	int has_intersect_14 
	= atlas_shape_gc_segments_intersection(&result_14, 
										   &c1411, &c1412, 
										   &c1421, &c1422);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_14.latitude, result_14.longitude);
	fail_unless(has_intersect_14 == 1);
	
	
	i++;
	atlas_shp_coordinate_t c1511 = {-49, -27};
	atlas_shp_coordinate_t c1512 = { 13,  52};
	atlas_shp_coordinate_t c1521 = {-80,  27};
	atlas_shp_coordinate_t c1522 = { 55,  -5};
	atlas_shp_coordinate_t result_15;
	int has_intersect_15 
	= atlas_shape_gc_segments_intersection(&result_15, 
										   &c1511, &c1512, 
										   &c1521, &c1522);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_15.latitude, result_15.longitude);
	fail_unless(has_intersect_15 == 1);
	
	// two equal segments
	i++;
	atlas_shp_coordinate_t c1611 = {-49, -27};
	atlas_shp_coordinate_t c1612 = { 13,  52};
	atlas_shp_coordinate_t c1621 = {-49, -27};
	atlas_shp_coordinate_t c1622 = { 13,  52};
	atlas_shp_coordinate_t result_16;
	int has_intersect_16 
	= atlas_shape_gc_segments_intersection(&result_16, 
										   &c1611, &c1612, 
										   &c1621, &c1622);
//	printf("TEST gc_seg_intersect %i: lat = %f  lon = %f\n", 
//		   i, result_16.latitude, result_16.longitude);
	fail_unless(has_intersect_16 == 3);
	
} END_TEST


#pragma mark -
#pragma mark Fixtures

static void setup() {
    //printf(">>>\n");
}

static void teardown() {
    //printf("<<<\n");
}


#pragma mark -
#pragma mark Shape Impl Geometry Suites

Suite * shape_impl_geometry_suite(void) {
    
    Suite *s = suite_create("Geometry Operations");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
	tcase_add_test(tc_create, test_shape_impl_geometry_point_equal);
	tcase_add_test(tc_create, test_shape_impl_geometry_arc_equal);
	tcase_add_test(tc_create, test_shape_impl_gc_intersection);
	tcase_add_test(tc_create, test_shape_impl_geometry_lon_range_overlap);
	tcase_add_test(tc_create, test_shape_impl_geometry_initial_course);
	tcase_add_test(tc_create, test_shape_impl_geometry_lat_range_gc_seg);	
	tcase_add_test(tc_create, test_shape_impl_geometry_gc_seg_intersect);
    
    suite_add_tcase(s, tc_create);
    
    return s;
}