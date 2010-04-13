/*
 *  test_atlas_rdf_term_impl.c
 *  atlas
 *
 *  Created by Tobias Kräntzer on 13.04.10.
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

#include "test_atlas_rdf_term_impl.h"

#include <lazy.h>

#include "atlas_rdf_term_impl.h"

#pragma mark -
#pragma mark Test Create RDF Term

START_TEST (test_create_rdf_term_iri) {
    
    atlas_rdf_term_t term;
	
    // create iri
    term = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == IRI);
    lz_release(term);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_blank_node) {
    
    atlas_rdf_term_t term;
	
    // create blank node
    term = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == BLANK_NODE);
    lz_release(term);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_string) {
    
    atlas_rdf_term_t term;
	
    // create string literal
    term = atlas_rdf_term_create_string("Hallo Atlas!", 0, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == STRING_LITERAL);
    lz_release(term);
    
    // create string literal
    term = atlas_rdf_term_create_string("Hallo Atlas!", "de_de", ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == STRING_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_typed_literal) {
    
    atlas_rdf_term_t term;
    atlas_rdf_term_t type;
    
    // create the type for the term
    type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    
    // create typed literal
    term = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == TYPED_LITERAL);
    lz_release(term);
    lz_release(type);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_boolean) {
    
    atlas_rdf_term_t term;
	
    // create boolean literal
    term = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == BOOLEAN_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_datetime) {
    
    atlas_rdf_term_t term;
    
    // create datetime literal
    term = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == DATETIME_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_integer) {
    
    atlas_rdf_term_t term;
	
    mpz_t i;
    mpz_init_set_str(i, "3141592653589793238462643383279502884", 10);
    
    // create integer literal
    term = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == INTEGER_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
    mpz_clear(i);
} END_TEST


START_TEST (test_create_rdf_term_double) {
    
    atlas_rdf_term_t term;
    
    // create double literal
    term = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == DOUBLE_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
} END_TEST


START_TEST (test_create_rdf_term_decimal) {
    
    atlas_rdf_term_t term;
	
    mpf_t f;
    mpf_init_set_str(f, "3.141592653589793238462643383279502884", 10);
    
    // create decimal literal
    term = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term == 0);
    fail_unless(atlas_rdf_term_type(term) == DOUBLE_LITERAL);
    lz_release(term);
    
    lz_wait_for_completion();
    mpf_clear(f);
} END_TEST

#pragma mark -
#pragma mark Fixtures

void setup() {
}

void teardown() {
}

#pragma mark -
#pragma mark RDF Term Suites

Suite * rdf_term_suite(void) {
    
    Suite *s = suite_create("RDF Term");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    
    tcase_add_test(tc_core, test_create_rdf_term_iri);
    tcase_add_test(tc_core, test_create_rdf_term_blank_node);
    tcase_add_test(tc_core, test_create_rdf_term_string);
    tcase_add_test(tc_core, test_create_rdf_term_typed_literal);
    tcase_add_test(tc_core, test_create_rdf_term_boolean);
    tcase_add_test(tc_core, test_create_rdf_term_datetime);
    tcase_add_test(tc_core, test_create_rdf_term_integer);
    tcase_add_test(tc_core, test_create_rdf_term_double);
    tcase_add_test(tc_core, test_create_rdf_term_decimal);
	
    suite_add_tcase(s, tc_core);
    
    return s;
}
