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
#include <stdio.h>

#include "atlas_rdf_term_impl.h"

#pragma mark -
#pragma mark Test Create RDF Term

START_TEST (test_create_rdf_term_iri) {
    
    atlas_rdf_term_t term;
	
    // create iri
    term = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == IRI);
        
        // check iri value
        char * value = atlas_rdf_term_iri_value(term);
        fail_unless(strcmp(value, "http://example.com") == 0);
        free(value);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_blank_node) {
    
    atlas_rdf_term_t term;
	
    // create blank node
    term = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == BLANK_NODE);
        
        // check blank node value
        char * value = atlas_rdf_term_blank_node_value(term);
        fail_unless(strcmp(value, "foo") == 0);
        free(value);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_string) {
    
    atlas_rdf_term_t term;
	
    // create string literal without language tag
    term = atlas_rdf_term_create_string("Hallo Atlas!", 0, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == STRING_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "Hallo Atlas!") == 0);
        free(value);
        
        // check language tag
        char * lang = atlas_rdf_term_string_lang(term);
        fail_unless(strcmp(value, "") == 0);
        free(lang);
        
        lz_release(term);
    }
    
    // create string literal with language tag
    term = atlas_rdf_term_create_string("Hallo Atlas!", "de_de", ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == STRING_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "Hallo Atlas!") == 0);
        free(value);
        
        // check language tag
        char * lang = atlas_rdf_term_string_lang(term);
        fail_unless(strcmp(value, "de_de") == 0);
        free(lang);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_typed_literal) {
    
    atlas_rdf_term_t term;
    atlas_rdf_term_t type;
    
    // create the type for the term
    type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == TYPED_LITERAL);
            
            // check literal value
            char * value = atlas_rdf_term_literal_value(term);
            fail_unless(strcmp(value, "foo") == 0);
            free(value);
            
            lz_release(term);
        }
        lz_release(type);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_boolean) {
    
    atlas_rdf_term_t term;
	
    // create boolean literal (true)
    term = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == BOOLEAN_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "true") == 0);
        free(value);
        
        lz_release(term);
    }
    
    // create boolean literal (false)
    term = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == BOOLEAN_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "false") == 0);
        free(value);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_datetime) {
    
    atlas_rdf_term_t term;
    
    // create datetime literal
    term = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == DATETIME_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "1970-01-01T00:00:00+00:00") == 0);
        free(value);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_integer) {
    
    atlas_rdf_term_t term;
	
    const char * str = "3141592653589793238462643383279502884";
    
    mpz_t i;
    mpz_init_set_str(i, str, 10);
    
    // create integer literal
    term = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == INTEGER_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, str) == 0);
        free(value);
        
        // check mpz (native) value
        mpz_t x;
        mpz_init(x);
        atlas_rdf_term_integer_value(term, x);
        fail_unless(mpz_cmp(i, x) == 0);
        mpz_clear(x);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    mpz_clear(i);
} END_TEST


START_TEST (test_create_rdf_term_double) {
    
    atlas_rdf_term_t term;
    
    // create double literal
    term = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == DOUBLE_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, "4.700000e+00") == 0);
        free(value);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
} END_TEST


START_TEST (test_create_rdf_term_decimal) {
    
    atlas_rdf_term_t term;
	
    char * str = "3.14159265358979";
    
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
    // create decimal literal
    term = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == DECIMAL_LITERAL);
        
        // check literal value
        char * value = atlas_rdf_term_literal_value(term);
        fail_unless(strcmp(value, str) == 0);
        free(value);
        
        // check mpf (native) value
        mpf_t x;
        mpf_init(x);
        atlas_rdf_term_decimal_value(term, x);
        fail_unless(mpf_cmp(f, x) == 0);
        mpf_clear(x);
        
        lz_release(term);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
} END_TEST

#pragma mark -
#pragma mark Test RDF Term Equality

START_TEST (test_iri_eq_iri) {
    
    atlas_rdf_term_t term1, term2;
	
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_iri("http://example.de", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2));
        lz_release(term1);
        lz_release(term2);
    }
    
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2));
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_blank_node) {
    
    atlas_rdf_term_t term1, term2;
	
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2));
        lz_release(term1);
        lz_release(term2);
    }

    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;
	char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2));
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

// TODO: Write more tests for the function 'atlas_rdf_term_eq()'.

#pragma mark -
#pragma mark Fixtures

static void setup() {
}

static void teardown() {
}

#pragma mark -
#pragma mark RDF Term Suites

Suite * rdf_term_suite(void) {
    
    Suite *s = suite_create("RDF Term");
    
    TCase *tc_create = tcase_create("Create");
    //tcase_add_checked_fixture (tc_create, setup, teardown);
    
    tcase_add_test(tc_create, test_create_rdf_term_iri);
    tcase_add_test(tc_create, test_create_rdf_term_blank_node);
    tcase_add_test(tc_create, test_create_rdf_term_string);
    tcase_add_test(tc_create, test_create_rdf_term_typed_literal);
    tcase_add_test(tc_create, test_create_rdf_term_boolean);
    tcase_add_test(tc_create, test_create_rdf_term_datetime);
    tcase_add_test(tc_create, test_create_rdf_term_integer);
    tcase_add_test(tc_create, test_create_rdf_term_double);
    tcase_add_test(tc_create, test_create_rdf_term_decimal);
    
    suite_add_tcase(s, tc_create);
    
    
    TCase *tc_eq = tcase_create("Equality");
    //tcase_add_checked_fixture (tc_create, setup, teardown);
    
    tcase_add_test(tc_eq, test_iri_eq_iri);
    tcase_add_test(tc_eq, test_iri_eq_blank_node);
    tcase_add_test(tc_eq, test_iri_eq_decimal);
	
    suite_add_tcase(s, tc_eq);
    
    return s;
}
