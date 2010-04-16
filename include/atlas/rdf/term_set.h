/*
 *  term_set.h
 *  atlas
 *
 *  Created by Tobias Kräntzer on 16.04.10.
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

#ifndef _ATLAS_RDF_TERM_SET_H_
#define _ATLAS_RDF_TERM_SET_H_

#include <atlas/base.h>
#include <atlas/rdf/term.h>

#include <lazy.h>

typedef lz_obj atlas_rdf_term_set_t;

#pragma mark -
#pragma mark Create a RDF Term Set

atlas_rdf_term_set_t
atlas_rdf_term_set_create(int number_of_terms,
                          atlas_rdf_term_t * terms,
                          atlas_error_handler err);

atlas_rdf_term_set_t
atlas_rdf_term_set_create_union(atlas_rdf_term_set_t set1,
                                atlas_rdf_term_set_t set2,
                                atlas_error_handler err);

atlas_rdf_term_set_t
atlas_rdf_term_set_create_intersection(atlas_rdf_term_set_t set1,
                                       atlas_rdf_term_set_t set2,
                                       atlas_error_handler err);

atlas_rdf_term_set_t
atlas_rdf_term_set_create_difference(atlas_rdf_term_set_t set1,
                                     atlas_rdf_term_set_t set2,
                                     atlas_error_handler err);

#pragma mark -
#pragma mark Access Details of a RDF Term Set

int
atlas_rdf_term_set_length(atlas_rdf_term_set_t set);

void
atlas_rdf_term_set_apply(atlas_rdf_term_set_t set,
                         void(^iterator)(atlas_rdf_term_t term));

#endif // _ATLAS_RDF_TERM_SET_H_

