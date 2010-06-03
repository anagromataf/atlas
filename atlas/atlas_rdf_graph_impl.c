/*
 *  atlas_rdf_graph_impl.c
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
    
    // calculate the amount of space needed for the result graph
    // and allocate memory
    int size = sizeof(__graph) * number_of_statements;
    __graph * graph = malloc(size);
    assert(graph != 0);
    
    // setup a temporary list for all tems, which are used in this graph
    __block int num_refs = 0;
    __block atlas_rdf_term_t * refs = malloc(sizeof(atlas_rdf_term_t) * number_of_statements * 3);
    assert(refs);
    
    // function to find a term in the temporary list and return its position
    // if the term is not in the list, append it
    int(^term_in_refs)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
        // return the position in the reference list if the term exists
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
    // free allocated memory and return 0
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

atlas_rdf_graph_t
atlas_rdf_graph_create_union(atlas_rdf_graph_t graph1,
                             atlas_rdf_graph_t graph2,
                             atlas_error_handler err) {
    assert(graph1 != 0);
    assert(graph2 != 0);
    
    // if both graphs are the same, return the first graph
    if (lz_obj_same(graph1, graph2) != 0) {
        return lz_retain(graph1);
    }
    
    __block atlas_rdf_graph_t result;
    lz_obj_sync(graph1, ^(void * data1, uint32_t length1){
        lz_obj_sync(graph2, ^(void * data2, uint32_t length2){
            
            // calculate the amount of space needed for the result graph
            // and allocate memory (worst case: both graphs are distinct)
            int size = length1 + length2;
            __graph * graph = malloc(size);
            assert(graph != 0);
            
            // number of statements in both graphs
            int num_statements_g1 = length1 / sizeof(__graph);
            int num_statements_g2 = length2 / sizeof(__graph);
            
            // number of statements in the result graph
            __block int num_statements = 0;
            
            // setup a temporary list for all terms
            // which are used in the result graph
            __block int num_refs = 0;
            __block atlas_rdf_term_t * refs = malloc(sizeof(atlas_rdf_term_t) *
                                                     (lz_obj_num_ref(graph1) + lz_obj_num_ref(graph2)));
            assert(refs);
            
            // function to find a term in the temporary list and return its position
            // if the term is not in the list, append it
            int(^term_in_refs)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
                // return the position in the reference list if the term exists
                for (int i=0; i<num_refs; i++) {
                    if (atlas_rdf_term_eq(refs[i], term) != 0) { return i; }
                }
                
                // term not in list
                // add term to the list
                refs[num_refs] = term;
                num_refs++;
                return num_refs - 1;
            };
            
            // function to iterate over both graphs and inserting all 'new'
            // statements in to the result set
            void(^statement_inter)(int num, __graph * data, atlas_rdf_graph_t g) = ^(int num, __graph * data, atlas_rdf_graph_t g){
                for (int loop = 0; loop < num; loop++) {
                    __graph stm = data[loop];
                    
                    // add position of the terms in the reference list to
                    // the statement
                    graph[num_statements].subject = term_in_refs(lz_obj_weak_ref(g, stm.subject));
                    graph[num_statements].predicate = term_in_refs(lz_obj_weak_ref(g, stm.predicate));
                    graph[num_statements].object = term_in_refs(lz_obj_weak_ref(g, stm.object));
                    
                    // avoid putting the same statement into the graph twice
                    int st_in_graph = 0;
                    for (int i=0; i < num_statements; i++) {
                        if (graph[i].subject == graph[num_statements].subject &&
                            graph[i].predicate == graph[num_statements].predicate &&
                            graph[i].object == graph[num_statements].object) {
                            st_in_graph = 1;
                            break;
                        }
                    }
                    
                    // only increment the counter of statements in the result graph,
                    // if the current statement in not already there
                    if (!st_in_graph) {
                        num_statements++;
                    }
                }
            };
            
            // calling the iterator with both graphs
            statement_inter(num_statements_g1, data1, graph1);
            statement_inter(num_statements_g2, data2, graph2);
            
            // resize the graph memory if needed
            if (num_statements_g1 + num_statements_g2 > num_statements) {
                size = sizeof(__graph) * num_statements;
                graph = realloc(graph, sizeof(__graph) * num_statements);
            }
            
            // create a lazy object
            result = lz_obj_new_v(graph, size, ^{
                free(graph);
            }, num_refs, refs);
            
            // free the temporary list holding the references
            free(refs);
        });
    });
    return result;
}

atlas_rdf_graph_t
atlas_rdf_graph_create_intersection(atlas_rdf_graph_t graph1,
                                    atlas_rdf_graph_t graph2,
                                    atlas_error_handler err) {
    assert(graph1 != 0);
    assert(graph2 != 0);
    
    __block atlas_rdf_graph_t result;
    lz_obj_sync(graph1, ^(void * data1, uint32_t length1){
        lz_obj_sync(graph2, ^(void * data2, uint32_t length2){
            
            // calculate the amount of space needed for the result garph
            // and allocate memory (worst case: size of the smallest graph)
            int size = length1 < length2 ? length1 : length2;
            __graph * graph = malloc(size);
            assert(graph != 0);
            
            // number of statements in both graphs
            int num_statements_g1 = length1 / sizeof(__graph);
            int num_statements_g2 = length2 / sizeof(__graph);
            int num_statements_min = size / sizeof(__graph);
            
            // number of statements in the result graph
            __block int num_statements = 0;
            
            // number of references
            int num_ref_g1 = lz_obj_num_ref(graph1);
            int num_ref_g2 = lz_obj_num_ref(graph2);
            int num_ref_min = num_ref_g1 < num_ref_g2 ? num_ref_g1 : num_ref_g1;
            
            // number of references used in the result set
            __block int num_refs = 0;
            
            // setup a temporary list for all terms
            // which are used in the result graph
            __block atlas_rdf_term_t * refs = malloc(sizeof(atlas_rdf_term_t) * num_ref_min);
            assert(refs);
            
            // function to find a term in the temporary list and return its position
            // if the term is not in the list, append it
            int(^term_in_refs)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
                // return the position in the reference list if the term exists
                for (int i=0; i<num_refs; i++) {
                    if (atlas_rdf_term_eq(refs[i], term) != 0) { return i; }
                }
                
                // term not in list
                // add term to the list
                refs[num_refs] = term;
                num_refs++;
                return num_refs - 1;
            };
            
            
            // iterate over the first graph and check for each
            // statement if it is in the second graph
            for (int o_loop = 0; o_loop < num_statements_g1; o_loop++) {
                for (int i_loop = 0; i_loop < num_statements_g2; i_loop++) {
                    
                    __graph stm1 = ((__graph *)data1)[o_loop];
                    __graph stm2 = ((__graph *)data2)[i_loop];
                    
                    atlas_rdf_term_t subject1 = lz_obj_weak_ref(graph1, stm1.subject);
                    atlas_rdf_term_t subject2 = lz_obj_weak_ref(graph2, stm2.subject);
                    if (0 == atlas_rdf_term_eq(subject1, subject2)) {
                        break;
                    } else {
                        graph[num_statements].subject = term_in_refs(subject1);
                    }
                    
                    atlas_rdf_term_t predicate1 = lz_obj_weak_ref(graph1, stm1.predicate);
                    atlas_rdf_term_t predicate2 = lz_obj_weak_ref(graph2, stm2.predicate);
                    if (0 == atlas_rdf_term_eq(predicate1, predicate2)) {
                        break;
                    } else {
                        graph[num_statements].predicate = term_in_refs(predicate1);
                    }
                    
                    atlas_rdf_term_t object1 = lz_obj_weak_ref(graph1, stm1.object);
                    atlas_rdf_term_t object2 = lz_obj_weak_ref(graph2, stm2.object);
                    if (0 == atlas_rdf_term_eq(object1, object2)) {
                        break;
                    } else {
                        graph[num_statements].object = term_in_refs(object1);
                    }
                    
                    // statement is in both graphs, we can incremet the
                    // counter of statements in the result
                    num_statements++;
                }
            }
            
            // resize the graph memory if needed
            if (num_statements_min > num_statements) {
                size = sizeof(__graph) * num_statements;
                graph = realloc(graph, sizeof(__graph) * num_statements);
            }
            
            // create a lazy object
            result = lz_obj_new_v(graph, size, ^{
                free(graph);
            }, num_refs, refs);
            
            // free the temporary list holding the references
            free(refs);
        });
    });
    return result;
}

atlas_rdf_graph_t
atlas_rdf_graph_create_difference(atlas_rdf_graph_t graph1,
                                  atlas_rdf_graph_t graph2,
                                  atlas_error_handler err) {
    assert(graph1 != 0);
    assert(graph2 != 0);
    
    __block atlas_rdf_graph_t result;
    lz_obj_sync(graph1, ^(void * data1, uint32_t length1){
        lz_obj_sync(graph2, ^(void * data2, uint32_t length2){
            
            // calculate the amount of space needed for the result graph
            // and allocate memory (worst case: size of both graphs)
            int size = length1 + length2;
            __graph * graph = malloc(size);
            assert(graph != 0);
            
            // number of statements in both graphs
            int num_statements_g1 = length1 / sizeof(__graph);
            int num_statements_g2 = length2 / sizeof(__graph);
            int num_statements_max = size / sizeof(__graph);
            
            // number of statements in the result graph
            __block int num_statements = 0;
            
            // number of references
            int num_ref_g1 = lz_obj_num_ref(graph1);
            int num_ref_g2 = lz_obj_num_ref(graph2);
            int num_ref_max = num_ref_g1 + num_ref_g2;
            
            // number of references used in the result set
            __block int num_refs = 0;
            
            // setup a temporary list for all terms
            // which are used in the result graph
            __block atlas_rdf_term_t * refs = malloc(sizeof(atlas_rdf_term_t) * num_ref_max);
            assert(refs);
            
            // function to find a term in the temporary list and return its position
            // if the term is not in the list, append it
            int(^term_in_refs)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
                // return the position in the reference list if the term exists
                for (int i=0; i<num_refs; i++) {
                    if (atlas_rdf_term_eq(refs[i], term) != 0) { return i; }
                }
                
                // term not in list
                // add term to the list
                refs[num_refs] = term;
                num_refs++;
                return num_refs - 1;
            };
            
            
            // iterate over the first graph and check for each
            // statement if it is in the second graph
            // if not, put it into the result set
            // and vice versa
            // OPTIMIZE: Find a better solution
            for (int o_loop = 0; o_loop < num_statements_g1; o_loop++) {
                
                int st_in_graph2 = 0;
                
                __graph stm1 = ((__graph *)data1)[o_loop];
                
                atlas_rdf_term_t subject1 = lz_obj_weak_ref(graph1, stm1.subject);
                atlas_rdf_term_t predicate1 = lz_obj_weak_ref(graph1, stm1.predicate);
                atlas_rdf_term_t object1 = lz_obj_weak_ref(graph1, stm1.object);
                
                for (int i_loop = 0; i_loop < num_statements_g2; i_loop++) {
                    
                    __graph stm2 = ((__graph *)data2)[i_loop];
                    
                    atlas_rdf_term_t subject2 = lz_obj_weak_ref(graph2, stm2.subject);
                    atlas_rdf_term_t predicate2 = lz_obj_weak_ref(graph2, stm2.predicate);
                    atlas_rdf_term_t object2 = lz_obj_weak_ref(graph2, stm2.object);
                    
                    if (0 != atlas_rdf_term_eq(subject1, subject2)) {
                        if (0 != atlas_rdf_term_eq(predicate1, predicate2)) {
                            if (0 != atlas_rdf_term_eq(object1, object2)) {
                                st_in_graph2 = 1;
                                break;
                            }
                        }
                    }
                }
                if (st_in_graph2 == 0) {
                    graph[num_statements].object = term_in_refs(object1);
                    graph[num_statements].predicate = term_in_refs(predicate1);
                    graph[num_statements].subject = term_in_refs(subject1);
                    num_statements++;
                }
            }
            
            for (int o_loop = 0; o_loop < num_statements_g2; o_loop++) {
                
                int st_in_graph1 = 0;
                
                __graph stm2 = ((__graph *)data2)[o_loop];
                
                atlas_rdf_term_t subject2 = lz_obj_weak_ref(graph2, stm2.subject);
                atlas_rdf_term_t predicate2 = lz_obj_weak_ref(graph2, stm2.predicate);
                atlas_rdf_term_t object2 = lz_obj_weak_ref(graph2, stm2.object);
                
                for (int i_loop = 0; i_loop < num_statements_g1; i_loop++) {
                    
                    __graph stm1 = ((__graph *)data1)[i_loop];
                    
                    atlas_rdf_term_t subject1 = lz_obj_weak_ref(graph1, stm1.subject);
                    atlas_rdf_term_t predicate1 = lz_obj_weak_ref(graph1, stm1.predicate);
                    atlas_rdf_term_t object1 = lz_obj_weak_ref(graph1, stm1.object);
                    
                    if (0 != atlas_rdf_term_eq(subject1, subject2)) {
                        if (0 != atlas_rdf_term_eq(predicate1, predicate2)) {
                            if (0 != atlas_rdf_term_eq(object1, object2)) {
                                st_in_graph1 = 1;
                                break;
                            }
                        }
                    }
                }
                if (st_in_graph1 == 0) {
                    graph[num_statements].object = term_in_refs(object2);
                    graph[num_statements].predicate = term_in_refs(predicate2);
                    graph[num_statements].subject = term_in_refs(subject2);
                    num_statements++;
                }
            }
            
            
            // resize the graph memory if needed
            if (num_statements_max > num_statements) {
                size = sizeof(__graph) * num_statements;
                graph = realloc(graph, sizeof(__graph) * num_statements);
            }
            
            // create a lazy object
            result = lz_obj_new_v(graph, size, ^{
                free(graph);
            }, num_refs, refs);
            
            // free the temporary list holding the references
            free(refs);
        });
    });
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

#pragma mark -
#pragma mark Graph Predicates

/*! Check if the graph contains a statement.
 */
int
atlas_rdf_graph_contains(atlas_rdf_graph_t graph,
                         atlas_rdf_term_t subject,
                         atlas_rdf_term_t predicate,
                         atlas_rdf_term_t object) {
    assert(graph != 0);
    assert(subject != 0);
    assert(predicate != 0);	
    assert(object != 0);
	
    __block int result = FALSE;
    lz_obj_sync(graph, ^(void * data, uint32_t length){
		__graph * statements = data;
		
		// number of statements in the given graph
		int num_statements = length / sizeof(__graph);
		
		// iteratively check element-wise, whether the
		// given graph contains a statement containing
		// the given subject, predicate and object
		for (int loop = 0; loop < num_statements; loop++) {
			__graph stm = statements[loop];
			
			if (0 != atlas_rdf_term_eq(subject, lz_obj_weak_ref(graph, stm.subject)) &&
				0 != atlas_rdf_term_eq(predicate, lz_obj_weak_ref(graph, stm.predicate)) &&
				0 != atlas_rdf_term_eq(object, lz_obj_weak_ref(graph, stm.object))) {
				result = TRUE;
				break;
			}
		}		
	});
	return result;
}
