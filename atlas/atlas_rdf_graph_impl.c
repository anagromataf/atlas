/*
 *  atlas_rdf_graph_impl.c
 *  atlas
 *
 *  Created by Tobias Kräntzer on 14.04.10.
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

#include "atlas_rdf_graph_impl.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <dispatch/dispatch.h>

#pragma mark -
#pragma mark Data Structure

typedef struct {
    uint16_t subject;
    uint16_t predicate;
    uint16_t object;
} __graph;

#pragma mark -
#pragma mark Create a RDF Graph

atlas_rdf_graph_t
atlas_rdf_graph_create(int number_of_statements,
                       atlas_rdf_statement_t * statements,
                       atlas_error_handler err) {
    
    // calculate the amount of space needed to
    // and allocate memory
    int size = sizeof(__graph) * number_of_statements;
    __graph * graph = malloc(size);
    assert(graph != 0);
    
    // setup a temporary list for all therm, which are used in this graph
    __block int num_refs = 0;
    __block atlas_rdf_term_t * refs = malloc(sizeof(atlas_rdf_term_t) * number_of_statements * 3);
    assert(refs);
    
    // create a funtion to find a term in the temporary list and return its position
    // if the term is not in the list, append it
    int(^term_in_refs)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
        // return the position in the reference lis if the term exists
        for (int i=0; i<num_refs; i++) {
            if (atlas_rdf_term_eq(refs[i], term) != 0) { return i; }
        }
        
        // term not in list
        // add term to the list
        refs[num_refs] = term;
        num_refs++;
        return num_refs - 1;
    };
    
    // the number of unique statements in the graph
    int num_graph_st = 0;
    
    int error = 0;
    int loop = 0;
    
    for (int loop = 0; loop < number_of_statements; loop++) {
        atlas_rdf_statement_t stm = statements[loop];
        
        // check subject
        if (!atlas_rdf_term_is_type(stm.subject, RESOURCE)) {
            error = 1;
            char * repr = atlas_rdf_term_repr(stm.subject);
            char * buff;
            asprintf(&buff, "Subject in statement %d is not a resource: %s", loop, repr);
            err(1, buff);
            free(repr);
            free(buff);
            break;
        }
        
        // check predicate
        if (!atlas_rdf_term_is_type(stm.predicate, IRI)) {
            error = 1;
            char * repr = atlas_rdf_term_repr(stm.predicate);
            char * buff;
            asprintf(&buff, "Predicate in statement %d is not an iri: %s", loop, repr);
            err(1, buff);
            free(repr);
            free(buff);
            break;
        }
        
        // add position of the terms in the reference list in the graph
        graph[num_graph_st].subject = term_in_refs(stm.subject);
        graph[num_graph_st].predicate = term_in_refs(stm.predicate);
        graph[num_graph_st].object = term_in_refs(stm.object);

        // avoid putting the same statement into the graph twice
        int st_in_graph = 0;
        for (int i=0; i < num_graph_st; i++) {
            if (graph[i].subject == graph[num_graph_st].subject &&
                graph[i].predicate == graph[num_graph_st].predicate &&
                graph[i].object == graph[num_graph_st].object) {
                st_in_graph++;
                break;
            }
        }
        if (!st_in_graph) {
            num_graph_st++;
        }
    }
    
    // resize the graph memory if needed
    if (number_of_statements > num_graph_st) {
        size = sizeof(__graph) * num_graph_st;
        graph = realloc(graph, sizeof(__graph) * num_graph_st);
    }
    
    // an error occurred while setting up the graph
    // free allocated memory and  return 0
    // the error handler has already been called
    if (error) {
        free(refs);
        free(graph);
        return 0;
    }
    
    // create a lazy object
    lz_obj result = lz_obj_new_v(graph, size, ^{
        free(graph);
    }, num_refs, refs);
    
    // free the temporary list holding the references
    free(refs);
    
    // return the result
    return result; 
}

#pragma mark -
#pragma mark Access Details of a RDF Graph

int
atlas_rdf_graph_length(atlas_rdf_graph_t graph) {
    __block int result;
    lz_obj_sync(graph, ^(void * data, uint32_t length){
        result = length / sizeof(__graph);
    });
    return result;
}

void
atlas_rdf_graph_apply(atlas_rdf_graph_t graph,
                      void(^iterator)(atlas_rdf_term_t subject,
                                      atlas_rdf_term_t predicate,
                                      atlas_rdf_term_t object)) {
    lz_obj_sync(graph, ^(void * data, uint32_t length){
        __graph * g = data;
        int num = length / sizeof(__graph);
        
        dispatch_apply(num, dispatch_get_global_queue(0, 0), ^(size_t i){
            iterator(lz_obj_weak_ref(graph, g[i].subject),
                     lz_obj_weak_ref(graph, g[i].predicate),
                     lz_obj_weak_ref(graph, g[i].object));
        });
    });   
}
