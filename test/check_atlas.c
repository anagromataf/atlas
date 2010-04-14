/*
 *  check_atlas.c
 *  atlas
 *
 *  Created by Tobias Kräntzer on 25.03.10.
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

#include <check.h>

#include <lazy.h>
#include <stdlib.h>

#include "test_shape_impl.h"
#include "test_shape_impl_equal.h"
#include "test_shape_impl_line_intersect.h"
#include "test_shape_impl_union.h"
#include "test_shape_impl_geom_equal.h"

#pragma mark -
#pragma mark Atlas Suites

Suite * atlas_suite(void) {
    
    Suite *s = suite_create("Atlas");
    
    TCase *tc_core = tcase_create("Core");
    
    //tcase_add_test(tc_core, ...);
	
	tcase_add_test(tc_core, test_shape_impl);
	tcase_add_test(tc_core, test_shape_impl_equal);
	tcase_add_test(tc_core, test_shape_impl_line_intersect);
	tcase_add_test(tc_core, test_shape_impl_union);
	tcase_add_test(tc_core, test_shape_impl_geom_equal);

    
    suite_add_tcase(s, tc_core);
    
    return s;
}

#pragma mark -
#pragma mark Main Suite

Suite * main_suite(void) {
    Suite *s = suite_create ("Main Suite");
    TCase *tc_core = tcase_create("Core");
    suite_add_tcase(s, tc_core);
    return s;
}

#pragma mark -
#pragma mark Running Tests

int main(int argc, char ** argv) {
    int number_failed;
    
    SRunner *sr = srunner_create(main_suite());
	srunner_set_fork_status(sr, CK_NOFORK);
    
    // add the suites to the main suite
    srunner_add_suite(sr, atlas_suite());
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
