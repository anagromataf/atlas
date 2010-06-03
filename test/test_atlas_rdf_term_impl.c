/*
 *  test_atlas_rdf_term_impl.c
 *  atlas
 *
 *  Created by Tobias Kräntzer on 13.04.10.
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

#include "test_atlas_rdf_term_impl.h"

#include <lazy.h>
#include <stdio.h>
#include <stdlib.h>

#include <atlas.h>

#pragma mark -
#pragma mark Test Create RDF Term

START_TEST (test_create_rdf_term_iri) {
    
    atlas_rdf_term_t term;
    
    // create a valid iri
    term = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == IRI);
        
        // check iri value
        char * value = atlas_rdf_term_iri_value(term);
        fail_unless(strcmp(value, "http://example.com") == 0);
        free(value);
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
        lz_release(term);
    }

	// create invalid iris
    term = atlas_rdf_term_create_iri("+http://example.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri("1http://example.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri("://example.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri(":example.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri("example.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri("http://e<xample.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_iri("http://e xample.com", ^(int err, const char * msg){});
    fail_unless(term == 0);

    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_blank_node) {
    
    atlas_rdf_term_t term;
    
    // create valid blank node
    term = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    fail_if(term == 0);
    if (term) {
        // check type
        fail_unless(atlas_rdf_term_type(term) == BLANK_NODE);
        
        // check blank node value
        char * value = atlas_rdf_term_blank_node_value(term);
        fail_unless(strcmp(value, "foo") == 0);
        free(value);
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
        lz_release(term);
    }

	// create invalid blank nodes
    term = atlas_rdf_term_create_blank_node(";foo", ^(int err, const char * msg){});
	fail_unless(term == 0);

	term = atlas_rdf_term_create_blank_node(".foo", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_blank_node("-foo", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_blank_node("f;oo", ^(int err, const char * msg){});
    fail_unless(term == 0);

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
        fail_unless(strcmp(lang, "") == 0);
        free(lang);
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
        lz_release(term);
    }
    
    // create string literal with language tag
    term = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
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
        fail_unless(strcmp(lang, "de-de") == 0);
        free(lang);
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
        lz_release(term);
    }
    
	// create invalid string literals
	// (use of invalid language tags)
    term = atlas_rdf_term_create_string("Hallo Atlas!", "", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_string("Hallo Atlas!", "-de", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_string("Hallo Atlas!", "--", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_string("Hallo Atlas!", "-", ^(int err, const char * msg){});
    fail_unless(term == 0);

	term = atlas_rdf_term_create_string("Hallo Atlas!", "de_de", ^(int err, const char * msg){});
    fail_unless(term == 0);

    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_term_typed_literal) {
    
    atlas_rdf_term_t term;
    atlas_rdf_term_t type;
    
	// create a typed literal, whereas the type does not correspond
	// to a predefined type, so the literal could not be represented directly as
	// a literal of a predefined type
	//
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
            
            // print repr
            char * repr = atlas_rdf_term_repr(term);
            //printf("%s\n", repr);
            free(repr);
            
            lz_release(term);
        }
        lz_release(type);
    }

	// create a typed literal, that can be represented directly as a
	// literal of this type
	//
	// create the type integer for the term
    type = atlas_rdf_term_create_iri(INTEGER_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("11", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == INTEGER_LITERAL);
            
            // check literal value
			char * value = atlas_rdf_term_literal_value(term);
			fail_unless(strcmp(value, "11") == 0);
			free(value);
						
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
			lz_release(term);
        }
        lz_release(type);
    }
	
	// create the type decimal for the term
    type = atlas_rdf_term_create_iri(DECIMAL_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("11", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == DECIMAL_LITERAL);
            
            // check literal value
			char * value = atlas_rdf_term_literal_value(term);
			fail_unless(strcmp(value, "11") == 0);
			free(value);
			
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
			lz_release(term);
        }
        lz_release(type);
    }
	
	// create the type decimal for the term
    type = atlas_rdf_term_create_iri(DOUBLE_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("4.7", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == DOUBLE_LITERAL);
            
            // check literal value
			char * value = atlas_rdf_term_literal_value(term);
			fail_unless(strcmp(value, "4.700000e+00") == 0);
			free(value);
			
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
			lz_release(term);
        }
        lz_release(type);
    }
	
	// create the type boolean for the term
    type = atlas_rdf_term_create_iri(BOOLEAN_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("1", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == BOOLEAN_LITERAL);
            
            // check literal value
			char * value = atlas_rdf_term_literal_value(term);
			fail_unless(strcmp(value, "true") == 0);
			free(value);
			
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
			lz_release(term);
        }
        lz_release(type);
    }
	
	// create the type datetime for the term
    type = atlas_rdf_term_create_iri(DATETIME_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("0", type, ^(int err, const char * msg){});
        fail_if(term == 0);
        if (term) {
            // check type
            fail_unless(atlas_rdf_term_type(term) == DATETIME_LITERAL);
            
            // check literal value
			char * value = atlas_rdf_term_literal_value(term);
			fail_unless(strcmp(value, "1970-01-01T00:00:00+00:00") == 0);
			free(value);
			
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
			lz_release(term);
        }
        lz_release(type);
    }
	
	// create the type string for the term
    type = atlas_rdf_term_create_iri(STRING_DATATYPE_IRI, ^(int err, const char * msg){});
    fail_if(type == 0);
    if (type) {
        // create typed literal
        term = atlas_rdf_term_create_typed("Hallo Atlas!", type, ^(int err, const char * msg){});
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
			fail_unless(strcmp(lang, "") == 0);
			free(lang);
			
			// print repr
			char * repr = atlas_rdf_term_repr(term);
			//printf("%s\n", repr);
			free(repr);
			
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        
        // print repr
        char * repr = atlas_rdf_term_repr(term);
        //printf("%s\n", repr);
        free(repr);
        
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
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_blank_node) {
    
    atlas_rdf_term_t term1, term2;

    // create blank node
    term1 = atlas_rdf_term_create_blank_node("nodeA", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_blank_node("nodeB", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create blank node
    term1 = atlas_rdf_term_create_blank_node("node", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_blank_node("node", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_str_eq_str) {
    
    atlas_rdf_term_t term1, term2;
    
	// create string literal
    term1 = atlas_rdf_term_create_string("a", "de-de", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_string("a", "de-de", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

	// create string literal
    term1 = atlas_rdf_term_create_string("b", "de-de", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_string("a", "de-de", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

	// create string literal
    term1 = atlas_rdf_term_create_string("a", "da-da", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_string("a", "de-de", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create string literal
    term1 = atlas_rdf_term_create_string("b", "da-da", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_string("a", "de-de", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_typed_literal_eq_typed_literal) {
    
    atlas_rdf_term_t term1, term2, type1, type2;
    
	// create type A
	type1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type1 == 0);

	// create type B
	type2 = atlas_rdf_term_create_iri("http://example.com/example", ^(int err, const char * msg){});
	fail_if(type2 == 0);

	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type1, ^(int err, const char * msg){});
	term2 = atlas_rdf_term_create_typed("foo", type1, ^(int err, const char * msg){});	
	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create typed literal
	term1 = atlas_rdf_term_create_typed("fooA", type1, ^(int err, const char * msg){});
	term2 = atlas_rdf_term_create_typed("fooB", type1, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type1, ^(int err, const char * msg){});
	term2 = atlas_rdf_term_create_typed("foo", type2, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
	
	// create typed literal
	term1 = atlas_rdf_term_create_typed("fooA", type1, ^(int err, const char * msg){});
	term2 = atlas_rdf_term_create_typed("fooB", type2, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
	
	lz_release(type1);
	lz_release(type2);
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_boolean_eq_boolean) {
    
    atlas_rdf_term_t term1, term2;
	
    // create boolean literal
    term1 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create boolean literal
    term1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_datetime_eq_datetime) {
    
    atlas_rdf_term_t term1, term2;
	
    // create datetime literal
    term1 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_datetime(1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create datetime literal
    term1 = atlas_rdf_term_create_datetime(1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_datetime(1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_integer_eq_integer) {
    
    atlas_rdf_term_t term1, term2;
	
	const char * strA = "3141592653589793238462643383279502884";
	const char * strB = "3141592653589793238462643383279502883";
    mpz_t i1;
    mpz_t i2;
    mpz_init_set_str(i1, strA, 10);
    mpz_init_set_str(i2, strB, 10);	
	
    // create datetime literal
    term1 = atlas_rdf_term_create_integer(i1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_integer(i2, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

	// create datetime literal
    term1 = atlas_rdf_term_create_integer(i1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_integer(i1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
	
    lz_wait_for_completion();
	mpz_clear(i1);
    mpz_clear(i2);
	
} END_TEST

START_TEST (test_double_eq_double) {
    
    atlas_rdf_term_t term1, term2;
	
    // create datetime literal
    term1 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_double(9.7, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create datetime literal
    term1 = atlas_rdf_term_create_double(1.0, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_double(1.0, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_decimal_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;
	
	char * strA = "3.14159265358979";
	char * strB = "1.14159265358979";
    mpf_t f1;
    mpf_t f2;
    mpf_init_set_str(f1, strA, 10);
    mpf_init_set_str(f2, strB, 10);
	
    // create datetime literal
    term1 = atlas_rdf_term_create_decimal(f1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_decimal(f2, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	// create datetime literal
    term1 = atlas_rdf_term_create_decimal(f1, ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_decimal(f1, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_if(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f1);
    mpf_clear(f2);
	
} END_TEST

START_TEST (test_iri_eq_blank_node) {
    
    atlas_rdf_term_t term1, term2;
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
    term2 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_string) {
    
    atlas_rdf_term_t term1, term2, term3;
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create string literal without language tag
    term2 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term3 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_typed_literal) {
    
    atlas_rdf_term_t term1, term2, type;

	// create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term2 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
		
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

	lz_release(type);    
    lz_wait_for_completion();
    
} END_TEST
	
START_TEST (test_iri_eq_boolean) {
    
    atlas_rdf_term_t term1, term2, term3;
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create boolean literal (true)
    term2 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_datetime) {
    
    atlas_rdf_term_t term1, term2;
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create datetime literal
    term2 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_iri_eq_integer) {
    
    atlas_rdf_term_t term1, term2;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create integer literal
    term2 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_iri_eq_double) {
    
    atlas_rdf_term_t term1, term2;
    
    // create iri
    term1 = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	// create double literal
    term2 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
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
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_blank_node_eq_string) {
    
    atlas_rdf_term_t term1, term2, term3;
    
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create string literal without language tag
    term2 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term3 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_typed_literal) {
    
    atlas_rdf_term_t term1, term2, type;
	
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term2 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
	
	lz_release(type);    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_boolean) {
    
    atlas_rdf_term_t term1, term2, term3;
    
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create boolean literal (true)
    term2 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_datetime) {
    
    atlas_rdf_term_t term1, term2;
    
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create datetime literal
    term2 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_integer) {
    
    atlas_rdf_term_t term1, term2;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create integer literal
    term2 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_blank_node_eq_double) {
    
    atlas_rdf_term_t term1, term2;
    
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create double literal
    term2 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_blank_node_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
    // create blank node
    term1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
	// create decimal literal
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_string_eq_typed_literal) {
    
    atlas_rdf_term_t term1, term2, term3, type;
	
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});		
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term3 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
	
	lz_release(type);    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_string_eq_boolean) {
    
    atlas_rdf_term_t term1, term2, term3, term4;
    
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
	// create boolean literal (true)
    term3 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term4 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
	fail_if(term4 == 0);
    if (term1 && term2 && term3 && term4) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term4) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term4) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
		lz_release(term4);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_string_eq_datetime) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
	// create datetime literal
    term3 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_string_eq_integer) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
	// create integer literal
    term3 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_string_eq_double) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
	// create double literal
    term3 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_string_eq_decimal) {
    
    atlas_rdf_term_t term1, term2, term3;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
	// create string literal without language tag
    term1 = atlas_rdf_term_create_string("value", 0, ^(int err, const char * msg){});
	// create string literal with language tag
    term2 = atlas_rdf_term_create_string("value", "de-de", ^(int err, const char * msg){});	
	// create decimal literal
    term3 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_typed_literal_eq_boolean) {
    
    atlas_rdf_term_t type, term1, term2, term3;

	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	// create boolean literal (true)
    term2 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }

    lz_release(type);
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_typed_literal_eq_datetime) {
    
    atlas_rdf_term_t type, term1, term2;
    
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	// create datetime literal
    term2 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

    lz_release(type);
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_typed_literal_eq_integer) {
    
    atlas_rdf_term_t type, term1, term2;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	// create integer literal
    term2 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
	lz_release(type);
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_typed_literal_eq_double) {
    
    atlas_rdf_term_t type, term1, term2;
    
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	// create double literal
    term2 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

    lz_release(type);
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_typed_literal_eq_decimal) {
    
    atlas_rdf_term_t type, term1, term2;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
	// create type
	type = atlas_rdf_term_create_iri("http://example.com", ^(int err, const char * msg){});
	fail_if(type == 0);
	// create typed literal
	term1 = atlas_rdf_term_create_typed("foo", type, ^(int err, const char * msg){});
	// create decimal literal
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }

    lz_release(type);
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_boolean_eq_datetime) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	// create boolean literal (true)
    term1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term2 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
	// create datetime literal
    term3 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_boolean_eq_integer) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
	// create boolean literal (true)
    term1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term2 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
	// create integer literal
    term3 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_boolean_eq_double) {
    
    atlas_rdf_term_t term1, term2, term3;
    
	// create boolean literal (true)
    term1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term2 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
	// create double literal
    term3 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_boolean_eq_decimal) {
    
    atlas_rdf_term_t term1, term2, term3;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
	// create boolean literal (true)
    term1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});	
	// create boolean literal (false)
    term2 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});	
	// create decimal literal
    term3 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
	fail_if(term3 == 0);
    if (term1 && term2 && term3) {
        fail_unless(atlas_rdf_term_eq(term1, term3) == 0);
		fail_unless(atlas_rdf_term_eq(term2, term3) == 0);
        lz_release(term1);
        lz_release(term2);
		lz_release(term3);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_datetime_eq_integer) {
    
    atlas_rdf_term_t term1, term2;
    
	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);
	
	// create datetime literal
    term1 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
	// create integer literal
    term2 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_datetime_eq_double) {
    
    atlas_rdf_term_t term1, term2;
    
	// create datetime literal
    term1 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
	// create double literal
    term2 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_datetime_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
	// create datetime literal
    term1 = atlas_rdf_term_create_datetime(0, ^(int err, const char * msg){});	
	// create decimal literal
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
} END_TEST

START_TEST (test_integer_eq_double) {
    
    atlas_rdf_term_t term1, term2;

	const char * str = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, str, 10);

	// create integer literal
    term1 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
	// create double literal
    term2 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
	mpz_clear(i);
    
} END_TEST

START_TEST (test_integer_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;

	const char * strA = "3141592653589793238462643383279502884";
    mpz_t i;
    mpz_init_set_str(i, strA, 10);
	
    char * strB = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, strB, 10);
    
	// create integer literal
    term1 = atlas_rdf_term_create_integer(i, ^(int err, const char * msg){});	
	// create decimal literal
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
	mpz_clear(i);
    
} END_TEST

START_TEST (test_double_eq_decimal) {
    
    atlas_rdf_term_t term1, term2;
	
    char * str = "3.14159265358979";
    mpf_t f;
    mpf_init_set_str(f, str, 10);
    
	// create double literal
    term1 = atlas_rdf_term_create_double(4.7, ^(int err, const char * msg){});	
	// create decimal literal
    term2 = atlas_rdf_term_create_decimal(f, ^(int err, const char * msg){});
    fail_if(term1 == 0);
    fail_if(term2 == 0);
    if (term1 && term2) {
        fail_unless(atlas_rdf_term_eq(term1, term2) == 0);
        lz_release(term1);
        lz_release(term2);
    }
    
    lz_wait_for_completion();
    mpf_clear(f);
    
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
#pragma mark RDF Term Suites

Suite * rdf_term_suite(void) {
    
    Suite *s = suite_create("RDF Term");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
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
    tcase_add_checked_fixture (tc_eq, setup, teardown);
    
    tcase_add_test(tc_eq, test_iri_eq_iri);
	tcase_add_test(tc_eq, test_blank_node_eq_blank_node);
    tcase_add_test(tc_eq, test_str_eq_str);
	tcase_add_test(tc_eq, test_typed_literal_eq_typed_literal);
	tcase_add_test(tc_eq, test_boolean_eq_boolean);
	tcase_add_test(tc_eq, test_datetime_eq_datetime);
	tcase_add_test(tc_eq, test_integer_eq_integer);
	tcase_add_test(tc_eq, test_double_eq_double);
	tcase_add_test(tc_eq, test_decimal_eq_decimal);
	
    tcase_add_test(tc_eq, test_iri_eq_blank_node);
    tcase_add_test(tc_eq, test_iri_eq_string);
    tcase_add_test(tc_eq, test_iri_eq_typed_literal);
    tcase_add_test(tc_eq, test_iri_eq_boolean);
    tcase_add_test(tc_eq, test_iri_eq_datetime);
    tcase_add_test(tc_eq, test_iri_eq_integer);
    tcase_add_test(tc_eq, test_iri_eq_double);
    tcase_add_test(tc_eq, test_iri_eq_decimal);

	tcase_add_test(tc_eq, test_blank_node_eq_string);
	tcase_add_test(tc_eq, test_blank_node_eq_typed_literal);
	tcase_add_test(tc_eq, test_blank_node_eq_boolean);
	tcase_add_test(tc_eq, test_blank_node_eq_datetime);
	tcase_add_test(tc_eq, test_blank_node_eq_integer);
	tcase_add_test(tc_eq, test_blank_node_eq_double);
	tcase_add_test(tc_eq, test_blank_node_eq_decimal);

	tcase_add_test(tc_eq, test_string_eq_typed_literal);
	tcase_add_test(tc_eq, test_string_eq_boolean);
	tcase_add_test(tc_eq, test_string_eq_datetime);
	tcase_add_test(tc_eq, test_string_eq_integer);
	tcase_add_test(tc_eq, test_string_eq_double);
	tcase_add_test(tc_eq, test_string_eq_decimal);

	tcase_add_test(tc_eq, test_typed_literal_eq_boolean);
	tcase_add_test(tc_eq, test_typed_literal_eq_datetime);
	tcase_add_test(tc_eq, test_typed_literal_eq_integer);
	tcase_add_test(tc_eq, test_typed_literal_eq_double);
	tcase_add_test(tc_eq, test_typed_literal_eq_decimal);

	tcase_add_test(tc_eq, test_boolean_eq_datetime);
	tcase_add_test(tc_eq, test_boolean_eq_integer);
	tcase_add_test(tc_eq, test_boolean_eq_double);
	tcase_add_test(tc_eq, test_boolean_eq_decimal);

	tcase_add_test(tc_eq, test_datetime_eq_integer);
	tcase_add_test(tc_eq, test_datetime_eq_double);
	tcase_add_test(tc_eq, test_datetime_eq_decimal);

	tcase_add_test(tc_eq, test_integer_eq_double);
	tcase_add_test(tc_eq, test_integer_eq_decimal);

	tcase_add_test(tc_eq, test_double_eq_decimal);

    suite_add_tcase(s, tc_eq);
    
    return s;
}
