/*
 *  test_atlas_rdf_term_set_impl.c
 *  atlas
 *
 *  Created by Tobias Kräntzer on 16.04.10.
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

#include "test_atlas_rdf_term_set_impl.h"

#include <lazy.h>
#include <stdio.h>
#include <dispatch/dispatch.h>

#include <atlas.h>

#pragma mark -
#pragma mark Test Create RDF Term Set

START_TEST (test_create_rdf_term_set) {
    
    atlas_rdf_term_t terms[6];
    terms[0] = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    terms[1] = atlas_rdf_term_create_iri("http://example.com/bar", ^(int err, const char * msg){});
    terms[2] = atlas_rdf_term_create_iri("http://example.com/baz", ^(int err, const char * msg){});
    terms[3] = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    terms[4] = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
    terms[5] = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    
    // create set
    atlas_rdf_term_set_t set = atlas_rdf_term_set_create(6, terms, ^(int err, const char * msg){});
    fail_if(set == 0);
    if (set) {
        
        fail_unless(atlas_rdf_term_set_length(set) == 6);
        
        // create a semaphore, because the block which is applied to
        // the statements is called concurrent
        dispatch_semaphore_t check_lock = dispatch_semaphore_create(1);
        __block int check[6] = {0, 0, 0, 0, 0, 0};
        
        atlas_rdf_term_set_apply(set, ^(atlas_rdf_term_t term){
            
            char *t = atlas_rdf_term_repr(term);
            printf("%s\n", t);
            free(t);
            
            // apply the block to all terms and count there appearance in the set
            for (int i=0; i<6; i++) {
                if (atlas_rdf_term_eq(terms[i], term)) {
                    dispatch_semaphore_wait(check_lock, DISPATCH_TIME_FOREVER);
                    check[i]++;
                    dispatch_semaphore_signal(check_lock);
                }
            }
        });
        
        dispatch_release(check_lock);
        lz_release(set);
    }
    
    lz_wait_for_completion();
    
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
#pragma mark RDF Term Set Suites

Suite * rdf_term_set_suite(void) {
    
    Suite *s = suite_create("RDF Set Term");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
    tcase_add_test(tc_create, test_create_rdf_term_set);
    
    suite_add_tcase(s, tc_create);
    
    return s;
}
