/*
 *  term_set.h
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

#ifndef _ATLAS_RDF_TERM_SET_H_
#define _ATLAS_RDF_TERM_SET_H_

#include <atlas/base.h>
#include <atlas/rdf/term.h>

#include <lazy.h>

/*! Handle for a RDF Term Set
 */
typedef lz_obj atlas_rdf_term_set_t;

#pragma mark -
#pragma mark Create a RDF Term Set

/*! Create a RDF Term Set
 *
 *  This function creates a set of the given terms.
 *
 *  \param number_of_terms number of term in the set
 *
 *  \param terms An array of terms.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term Set handle
 *          with a reference count of 1.
 */
atlas_rdf_term_set_t
atlas_rdf_term_set_create(int number_of_terms,
                          atlas_rdf_term_t * terms,
                          atlas_error_handler err);

/*! Create the union of two RDF Term Sets
 *
 *  This function creates the union of two sets.
 *
 *  \return NULL on failure or a RDF Term Set handle
 *          with a reference count of 1.
 */
atlas_rdf_term_set_t
atlas_rdf_term_set_create_union(atlas_rdf_term_set_t set1,
                                atlas_rdf_term_set_t set2,
                                atlas_error_handler err);

/*! Create the intersection of two RDF Term Sets
 *
 *  This function creates the intersection of two sets.
 *
 *  \return NULL on failure or a RDF Term Set handle
 *          with a reference count of 1.
 */
atlas_rdf_term_set_t
atlas_rdf_term_set_create_intersection(atlas_rdf_term_set_t set1,
                                       atlas_rdf_term_set_t set2,
                                       atlas_error_handler err);

/*! Create the difference of two RDF Term Sets
 *
 *  This function creates the difference of two sets.
 *
 *  \return NULL on failure or a RDF Term Set handle
 *          with a reference count of 1.
 */
atlas_rdf_term_set_t
atlas_rdf_term_set_create_difference(atlas_rdf_term_set_t set1,
                                     atlas_rdf_term_set_t set2,
                                     atlas_error_handler err);

#pragma mark -
#pragma mark Access Details of a RDF Term Set

/*! Number of RDF Terms in the set.
 */
int
atlas_rdf_term_set_length(atlas_rdf_term_set_t set);

/*! Apply a block to each term in the set.
 *
 *  This function calls the given block for each
 *  term in the set.
 *  
 *  The given block is called concurrent.
 */
void
atlas_rdf_term_set_apply(atlas_rdf_term_set_t set,
                         void(^iterator)(atlas_rdf_term_t term));

#endif // _ATLAS_RDF_TERM_SET_H_

