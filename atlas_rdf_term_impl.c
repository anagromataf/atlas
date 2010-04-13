/*
 *  atlas_rdf_term_impl.c
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

#include "atlas_rdf_term_impl.h"

#pragma mark -
#pragma mark Create a RDF Term

atlas_rdf_term_t
atlas_rdf_term_create_iri(const char * value,
                          atlas_error_handler err) {
    return 0;
}

atlas_rdf_term_t
atlas_rdf_term_create_blank_node(const char * value,
                                 atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_string(const char * value,
                             const char * lang,
                             atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_typed(const char * value,
                            atlas_rdf_term_t type,
                            atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_boolean(int value,
                              atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_integer(mpz_t value,
                              atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_double(double value,
                             atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_decimal(mpf_t value,
                              atlas_error_handler err) {
    return 0;
}


atlas_rdf_term_t
atlas_rdf_term_create_datetime(time_t value,
                               atlas_error_handler err) {
    return 0;
}


#pragma mark -
#pragma mark Access Type of a RDF Term

atlas_rdf_term_type_t
atlas_rdf_term_type(atlas_rdf_term_t term) {
}

int
atlas_rdf_term_is_type(atlas_rdf_term_t term,
                       atlas_rdf_term_type_t type) {
}

#pragma mark -
#pragma mark Representation of a RDF Term

void
atlas_rdf_term_repr(atlas_rdf_term_t term,
                    void (^handler)(const char * repr)) {
}

#pragma mark -
#pragma mark Access Details of a RDF Literal Term

void
atlas_rdf_term_literal_value(atlas_rdf_term_t term,
                             void (^handler)(const char * value)) {
}

void
atlas_rdf_term_literal_lang(atlas_rdf_term_t term,
                            void (^handler)(const char * lang)) {
}

atlas_rdf_term_t
atlas_rdf_term_literal_type(atlas_rdf_term_t term) {
}

#pragma mark -
#pragma mark Effective Boolean Value

int atlas_rdf_term_ebv(atlas_rdf_term_t term) {
}

#pragma mark -
#pragma mark Operation

int atlas_rdf_term_compare(atlas_rdf_term_t term1,
                           atlas_rdf_term_t term2,
                           atlas_error_handler err) {
}




