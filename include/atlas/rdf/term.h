/*
 *  term.h
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

#ifndef _ATLAS_TYPES_RDF_TERM_H_
#define _ATLAS_TYPES_RDF_TERM_H_

#include <atlas/base.h>

#include <lazy.h>
#include <stdint.h>
#include <atlas/gmp.h>
#include <time.h>

typedef lz_obj atlas_rdf_term_t;

typedef uint16_t atlas_rdf_term_type_t;

#define TERM                0b0000000000000001
#define RESOURCE            0b0000000000000011
#define BLANK_NODE          0b0000000000000111
#define IRI                 0b0000000000001011
#define LITERAL             0b0000000000010001
#define STRING_LITERAL      0b0000000000110001
#define TYPED_LITERAL       0b0000000001010001
#define BOOLEAN_LITERAL     0b0000000011010001
#define DATETIME_LITERAL    0b0000000101010001
#define NUMERIC_LITERAL     0b0000001001010001
#define INTEGER_LITERAL     0b0000011001010001
#define DOUBLE_LITERAL      0b0000101001010001
#define DECIMAL_LITERAL     0b0001001001010001


#pragma mark -
#pragma mark Create a RDF Term

atlas_rdf_term_t
atlas_rdf_term_create_iri(const char * value,
                          atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_blank_node(const char * value,
                                 atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_string(const char * value,
                             const char * lang,
                             atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_typed(const char * value,
                            atlas_rdf_term_t type,
                            atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_boolean(int value,
                              atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_integer(mpz_t value,
                              atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_double(double value,
                             atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_decimal(mpf_t value,
                              atlas_error_handler err);

atlas_rdf_term_t
atlas_rdf_term_create_datetime(time_t value,
                               atlas_error_handler err);

#pragma mark -
#pragma mark Access Type of a RDF Term

atlas_rdf_term_type_t
atlas_rdf_term_type(atlas_rdf_term_t term);

int
atlas_rdf_term_is_type(atlas_rdf_term_t term,
                       atlas_rdf_term_type_t type);

#pragma mark -
#pragma mark Representation of a RDF Term

char *
atlas_rdf_term_repr(atlas_rdf_term_t term);

#pragma mark -
#pragma mark Access Details of a RDF Literal Term

char *
atlas_rdf_term_iri_value(atlas_rdf_term_t term);

char *
atlas_rdf_term_blank_node_value(atlas_rdf_term_t term);

char *
atlas_rdf_term_literal_value(atlas_rdf_term_t term);

char *
atlas_rdf_term_string_lang(atlas_rdf_term_t term);

atlas_rdf_term_t
atlas_rdf_term_typed_type(atlas_rdf_term_t term);

void
atlas_rdf_term_integer_value(atlas_rdf_term_t term,
                             mpz_t result);

double
atlas_rdf_term_double_value(atlas_rdf_term_t term);

void
atlas_rdf_term_decimal_value(atlas_rdf_term_t term,
                             mpf_t result);

time_t
atlas_rdf_term_datetime_value(atlas_rdf_term_t term);

int
atlas_rdf_term_boolean_value(atlas_rdf_term_t term);


#pragma mark -
#pragma mark Effective Boolean Value

int
atlas_rdf_term_ebv(atlas_rdf_term_t term);

#pragma mark -
#pragma mark Operation

int
atlas_rdf_term_eq(atlas_rdf_term_t term1,
                  atlas_rdf_term_t term2);

#endif // _ATLAS_TYPES_RDF_TERM_H_
