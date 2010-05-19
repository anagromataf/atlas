/*
 *  test_atlas_rdf_graph_impl.h
 *  atlas
 *
 *  Created by Tobias Kräntzer on 15.04.10.
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

#include "test_atlas_rdf_graph_impl.h"

#include <lazy.h>
#include <stdio.h>
#include <atlas.h>

#include <dispatch/dispatch.h>

#pragma mark -
#pragma mark Test Create RDF Graph

START_TEST (test_create_rdf_graph_empty) {
    
    // create an empty graph
    atlas_rdf_graph_t graph = atlas_rdf_graph_create(0, 0, ^(int err, const char * msg){});
    fail_if(graph == 0);
    if (graph) {
        fail_unless(atlas_rdf_graph_length(graph) == 0);
        lz_release(graph);
    }
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_create_rdf_graph) {
    
    // create some terms to store in the graph
    atlas_rdf_term_t sub1, pred1, obj1, pred2, obj2;
    sub1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    pred1 = atlas_rdf_term_create_iri("http://example.com/bar", ^(int err, const char * msg){});
    pred2 = atlas_rdf_term_create_iri("http://example.com/baz", ^(int err, const char * msg){});
    obj1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    obj2 = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
    
    // setup the statements
    atlas_rdf_statement_t statements[3];
    statements[0].subject = sub1;
    statements[0].predicate = pred1;
    statements[0].object = obj1;
    statements[1].subject = sub1;
    statements[1].predicate = pred2;
    statements[1].object = obj2;
    statements[2].subject = sub1;
    statements[2].predicate = pred2;
    statements[2].object = obj2;
    
    // create the graph
    atlas_rdf_graph_t graph = atlas_rdf_graph_create(3, statements, ^(int err, const char * msg){});
    fail_if(graph == 0);
    if (graph) {
        fail_unless(atlas_rdf_graph_length(graph) == 2);
        
        // create a semaphore, because the block which is applied to
        // the statements is called concurrent
        dispatch_semaphore_t check_lock = dispatch_semaphore_create(1);
        __block int check[2] = {0, 0};
        
        // check if both terms are in the graph
        atlas_rdf_graph_apply(graph, ^(atlas_rdf_term_t subject,
                                       atlas_rdf_term_t predicate,
                                       atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
            
            // both subjects should be the same
            fail_unless(atlas_rdf_term_eq(sub1, subject));
            
            // apply the block to all statements and count there appearance in the graph
            for (int i=0; i<2; i++) {
                if (atlas_rdf_term_eq(statements[i].subject, subject) &&
                    atlas_rdf_term_eq(statements[i].predicate, predicate) &&
                    atlas_rdf_term_eq(statements[i].object, object)) {
                    dispatch_semaphore_wait(check_lock, DISPATCH_TIME_FOREVER);
                    check[i]++;
                    dispatch_semaphore_signal(check_lock);
                }
            }
        });
        
        // each statement should only be once in the graph
        fail_unless(check[0] == 1);
        fail_unless(check[1] == 1);
        
        dispatch_release(check_lock);
        
        lz_release(graph);
    }
    
    lz_release(sub1);
    lz_release(pred1);
    lz_release(obj1);
    lz_release(pred2);
    lz_release(obj2);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_graph_union) {
    
    // create some terms to store in the graph
    atlas_rdf_term_t sub1, pred1, obj1, pred2, obj2, obj3;
    sub1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    pred1 = atlas_rdf_term_create_iri("http://example.com/bar", ^(int err, const char * msg){});
    pred2 = atlas_rdf_term_create_iri("http://example.com/baz", ^(int err, const char * msg){});
    obj1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    obj2 = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
    obj3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    
    // setup the statements
    atlas_rdf_statement_t statements_g1[2];
    statements_g1[0].subject = sub1;
    statements_g1[0].predicate = pred1;
    statements_g1[0].object = obj1;
    statements_g1[1].subject = sub1;
    statements_g1[1].predicate = pred2;
    statements_g1[1].object = obj2;
    
    atlas_rdf_statement_t statements_g2[2];
    statements_g2[0].subject = sub1;
    statements_g2[0].predicate = pred2;
    statements_g2[0].object = obj2;
    statements_g2[1].subject = sub1;
    statements_g2[1].predicate = pred2;
    statements_g2[1].object = obj3;
    
    atlas_rdf_statement_t statements_union[3];
    statements_union[0].subject = sub1;
    statements_union[0].predicate = pred2;
    statements_union[0].object = obj2;
    statements_union[1].subject = sub1;
    statements_union[1].predicate = pred1;
    statements_union[1].object = obj1;
    statements_union[2].subject = sub1;
    statements_union[2].predicate = pred2;
    statements_union[2].object = obj3;
    
    // create both graphs
    atlas_rdf_graph_t graph1 = atlas_rdf_graph_create(2, statements_g1, ^(int err, const char * msg){});
    atlas_rdf_graph_t graph2 = atlas_rdf_graph_create(2, statements_g2, ^(int err, const char * msg){});
    
    fail_if(graph1 == 0);
    fail_if(graph2 == 0);
    if (graph1 && graph2) {
        
        // print graph1
        //printf("graph1:\n");
        atlas_rdf_graph_apply(graph1, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        // print graph2
        //printf("graph2:\n");
        atlas_rdf_graph_apply(graph2, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        //printf("union (graph1, garph2):\n");
        
        // create the union
        atlas_rdf_graph_t graph_union = atlas_rdf_graph_create_union(graph1, graph2, ^(int err, const char * msg){});
        fail_if(graph_union == 0);
        
        // release the old graphs
        lz_release(graph1);
        lz_release(graph2);
        
        fail_unless(atlas_rdf_graph_length(graph_union) == 3);
        
        // create a semaphore, because the block which is applied to
        // the statements is called concurrent
        dispatch_semaphore_t check_lock = dispatch_semaphore_create(1);
        __block int check[3] = {0, 0, 0};
        
        // check if both terms are in the graph
        atlas_rdf_graph_apply(graph_union, ^(atlas_rdf_term_t subject,
                                             atlas_rdf_term_t predicate,
                                             atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
            
            // apply the block to all statements and count there appearance in the graph
            for (int i=0; i<3; i++) {
                if (atlas_rdf_term_eq(statements_union[i].subject, subject) &&
                    atlas_rdf_term_eq(statements_union[i].predicate, predicate) &&
                    atlas_rdf_term_eq(statements_union[i].object, object)) {
                    dispatch_semaphore_wait(check_lock, DISPATCH_TIME_FOREVER);
                    check[i]++;
                    dispatch_semaphore_signal(check_lock);
                }
            }
        });
        
        // each statement should only be once in the graph
        fail_unless(check[0] == 1);
        fail_unless(check[1] == 1);
        
        dispatch_release(check_lock);
        lz_release(graph_union);
    }
    
    lz_release(sub1);
    lz_release(pred1);
    lz_release(obj1);
    lz_release(pred2);
    lz_release(obj2);
    
    lz_wait_for_completion();
    
} END_TEST


START_TEST (test_create_rdf_graph_intersection) {
    
    // create some terms to store in the graph
    atlas_rdf_term_t sub1, pred1, obj1, pred2, obj2, obj3;
    sub1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    pred1 = atlas_rdf_term_create_iri("http://example.com/bar", ^(int err, const char * msg){});
    pred2 = atlas_rdf_term_create_iri("http://example.com/baz", ^(int err, const char * msg){});
    obj1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    obj2 = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
    obj3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    
    // setup the statements
    atlas_rdf_statement_t statements_g1[2];
    statements_g1[0].subject = sub1;
    statements_g1[0].predicate = pred1;
    statements_g1[0].object = obj1;
    statements_g1[1].subject = sub1;
    statements_g1[1].predicate = pred2;
    statements_g1[1].object = obj2;
    
    atlas_rdf_statement_t statements_g2[2];
    statements_g2[0].subject = sub1;
    statements_g2[0].predicate = pred2;
    statements_g2[0].object = obj2;
    statements_g2[1].subject = sub1;
    statements_g2[1].predicate = pred2;
    statements_g2[1].object = obj3;
    
    atlas_rdf_statement_t statements_intersection[1];
    statements_intersection[0].subject = sub1;
    statements_intersection[0].predicate = pred2;
    statements_intersection[0].object = obj2;
    
    // create both graphs
    atlas_rdf_graph_t graph1 = atlas_rdf_graph_create(2, statements_g1, ^(int err, const char * msg){});
    atlas_rdf_graph_t graph2 = atlas_rdf_graph_create(2, statements_g2, ^(int err, const char * msg){});
    
    fail_if(graph1 == 0);
    fail_if(graph2 == 0);
    if (graph1 && graph2) {
        
        // print graph1
        //printf("graph1:\n");
        atlas_rdf_graph_apply(graph1, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        // print graph2
        //printf("graph2:\n");
        atlas_rdf_graph_apply(graph2, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        //printf("intersection (graph1, garph2):\n");
        
        // create the intersection
        atlas_rdf_graph_t intersection = atlas_rdf_graph_create_intersection(graph1, graph2, ^(int err, const char * msg){});
        fail_if(intersection == 0);
        
        // release the old graphs
        lz_release(graph1);
        lz_release(graph2);
        
        fail_unless(atlas_rdf_graph_length(intersection) == 1);
        
        // create a semaphore, because the block which is applied to
        // the statements is called concurrent
        dispatch_semaphore_t check_lock = dispatch_semaphore_create(1);
        __block int check[1] = {0};
        
        // check if both terms are in the graph
        atlas_rdf_graph_apply(intersection, ^(atlas_rdf_term_t subject,
                                              atlas_rdf_term_t predicate,
                                              atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
            
            // apply the block to all statements and count there appearance in the graph
            for (int i=0; i<1; i++) {
                if (atlas_rdf_term_eq(statements_intersection[i].subject, subject) &&
                    atlas_rdf_term_eq(statements_intersection[i].predicate, predicate) &&
                    atlas_rdf_term_eq(statements_intersection[i].object, object)) {
                    dispatch_semaphore_wait(check_lock, DISPATCH_TIME_FOREVER);
                    check[i]++;
                    dispatch_semaphore_signal(check_lock);
                }
            }
        });
        
        // each statement should only be once in the graph
        fail_unless(check[0] == 1);
        
        dispatch_release(check_lock);
        lz_release(intersection);
    }
    
    lz_release(sub1);
    lz_release(pred1);
    lz_release(obj1);
    lz_release(pred2);
    lz_release(obj2);
    
    lz_wait_for_completion();
    
} END_TEST

START_TEST (test_create_rdf_graph_difference) {
    
    // create some terms to store in the graph
    atlas_rdf_term_t sub1, pred1, obj1, pred2, obj2, obj3;
    sub1 = atlas_rdf_term_create_blank_node("foo", ^(int err, const char * msg){});
    pred1 = atlas_rdf_term_create_iri("http://example.com/bar", ^(int err, const char * msg){});
    pred2 = atlas_rdf_term_create_iri("http://example.com/baz", ^(int err, const char * msg){});
    obj1 = atlas_rdf_term_create_boolean(1, ^(int err, const char * msg){});
    obj2 = atlas_rdf_term_create_string("Hallo Atlas!", "de-de", ^(int err, const char * msg){});
    obj3 = atlas_rdf_term_create_boolean(0, ^(int err, const char * msg){});
    
    // setup the statements
    atlas_rdf_statement_t statements_g1[2];
    statements_g1[0].subject = sub1;
    statements_g1[0].predicate = pred1;
    statements_g1[0].object = obj1;
    statements_g1[1].subject = sub1;
    statements_g1[1].predicate = pred2;
    statements_g1[1].object = obj2;
    
    atlas_rdf_statement_t statements_g2[2];
    statements_g2[0].subject = sub1;
    statements_g2[0].predicate = pred2;
    statements_g2[0].object = obj2;
    statements_g2[1].subject = sub1;
    statements_g2[1].predicate = pred2;
    statements_g2[1].object = obj3;
    
    atlas_rdf_statement_t statements_difference[2];
    statements_difference[0].subject = sub1;
    statements_difference[0].predicate = pred1;
    statements_difference[0].object = obj1;
    statements_difference[1].subject = sub1;
    statements_difference[1].predicate = pred2;
    statements_difference[1].object = obj3;
    
    // create both graphs
    atlas_rdf_graph_t graph1 = atlas_rdf_graph_create(2, statements_g1, ^(int err, const char * msg){});
    atlas_rdf_graph_t graph2 = atlas_rdf_graph_create(2, statements_g2, ^(int err, const char * msg){});
    
    fail_if(graph1 == 0);
    fail_if(graph2 == 0);
    
    if (graph1 && graph2) {
        
        // print graph1
        //printf("graph1:\n");
        atlas_rdf_graph_apply(graph1, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        // print graph2
        //printf("graph2:\n");
        atlas_rdf_graph_apply(graph2, ^(atlas_rdf_term_t subject,
                                        atlas_rdf_term_t predicate,
                                        atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
        });
        
        //printf("difference (graph1, garph2):\n");
        
        // create the difference
        atlas_rdf_graph_t difference = atlas_rdf_graph_create_difference(graph1, graph2, ^(int err, const char * msg){});
        fail_if(difference == 0);
        
        // release the old graphs
        lz_release(graph1);
        lz_release(graph2);
        
        fail_unless(atlas_rdf_graph_length(difference) == 2);
        
        // create a semaphore, because the block which is applied to
        // the statements is called concurrent
        dispatch_semaphore_t check_lock = dispatch_semaphore_create(1);
        __block int check[2] = {0,0};
        
        // check if both terms are in the graph
        atlas_rdf_graph_apply(difference, ^(atlas_rdf_term_t subject,
                                            atlas_rdf_term_t predicate,
                                            atlas_rdf_term_t object){
            
            char * s, * p, * o;
            s = atlas_rdf_term_repr(subject);
            p = atlas_rdf_term_repr(predicate);
            o = atlas_rdf_term_repr(object);
            //printf("%s %s %s.\n", s, p, o);
            free(s);
            free(p);
            free(o);
            
            // apply the block to all statements and count there appearance in the graph
            for (int i=0; i<2; i++) {
                if (atlas_rdf_term_eq(statements_difference[i].subject, subject) &&
                    atlas_rdf_term_eq(statements_difference[i].predicate, predicate) &&
                    atlas_rdf_term_eq(statements_difference[i].object, object)) {
                    dispatch_semaphore_wait(check_lock, DISPATCH_TIME_FOREVER);
                    check[i]++;
                    dispatch_semaphore_signal(check_lock);
                }
            }
        });
        
        // each statement should only be once in the graph
        fail_unless(check[0] == 1);
        fail_unless(check[1] == 1);
        
        dispatch_release(check_lock);
        lz_release(difference);
    }
    
    lz_release(sub1);
    lz_release(pred1);
    lz_release(obj1);
    lz_release(pred2);
    lz_release(obj2);
    
    lz_wait_for_completion();
    
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
#pragma mark RDF Graph Suites

Suite * rdf_graph_suite(void) {
    
    Suite *s = suite_create("RDF Graph");
    
    TCase *tc_create = tcase_create("Create");
    tcase_add_checked_fixture (tc_create, setup, teardown);
    
    tcase_add_test(tc_create, test_create_rdf_graph);
    tcase_add_test(tc_create, test_create_rdf_graph_empty);
    tcase_add_test(tc_create, test_create_rdf_graph_union);
    tcase_add_test(tc_create, test_create_rdf_graph_intersection);
    tcase_add_test(tc_create, test_create_rdf_graph_difference);
    
    suite_add_tcase(s, tc_create);
    
    return s;
}