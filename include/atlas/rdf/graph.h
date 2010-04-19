/*
 *  garph.h
 *  atlas
 *
 *  Created by Tobias Kräntzer on 14.04.10.
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

#ifndef _ATLAS_TYPES_RDF_GRAPH_H_
#define _ATLAS_TYPES_RDF_GRAPH_H_

#include <atlas/base.h>
#include <atlas/rdf/term.h>

#include <lazy.h>

typedef lz_obj atlas_rdf_graph_t;

typedef struct atlas_rdf_statement_s {
    atlas_rdf_term_t subject;
    atlas_rdf_term_t predicate;
    atlas_rdf_term_t object;
} atlas_rdf_statement_t;

#pragma mark -
#pragma mark Create a RDF Graph

atlas_rdf_graph_t
atlas_rdf_graph_create(int number_of_statements,
                       atlas_rdf_statement_t * statements,
                       atlas_error_handler err);

atlas_rdf_graph_t
atlas_rdf_graph_create_union(atlas_rdf_graph_t graph1,
                             atlas_rdf_graph_t graph2,
                             atlas_error_handler err);

atlas_rdf_graph_t
atlas_rdf_graph_create_intersection(atlas_rdf_graph_t graph1,
                                    atlas_rdf_graph_t graph2,
                                    atlas_error_handler err);

atlas_rdf_graph_t
atlas_rdf_graph_create_difference(atlas_rdf_graph_t graph1,
                                  atlas_rdf_graph_t graph2,
                                  atlas_error_handler err);

#pragma mark -
#pragma mark Access Details of a RDF Graph

int
atlas_rdf_graph_length(atlas_rdf_graph_t graph);

void
atlas_rdf_graph_apply(atlas_rdf_graph_t graph,
                      void(^iterator)(atlas_rdf_term_t subject,
                                      atlas_rdf_term_t predicate,
                                      atlas_rdf_term_t object));

#pragma mark -
#pragma mark Graph Predicates

int
atlas_rdf_graph_contains(atlas_rdf_graph_t graph,
                         atlas_rdf_term_t subject,
                         atlas_rdf_term_t predicate,
                         atlas_rdf_term_t object);


#endif // _ATLAS_TYPES_RDF_GRAPH_H_



