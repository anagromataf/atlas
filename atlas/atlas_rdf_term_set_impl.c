/*
 *  atlas_rdf_term_set_impl.c
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

#include "atlas_rdf_term_set_impl.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <dispatch/dispatch.h>

#pragma mark -
#pragma mark Create a RDF Term Set

atlas_rdf_term_set_t
atlas_rdf_term_set_create(int number_of_terms,
                          atlas_rdf_term_t * terms,
                          atlas_error_handler err) {
    int num_refs = 0;
    atlas_rdf_term_t refs[number_of_terms];
    
    // remove duplicates
    for (int o=0; o<number_of_terms; o++) {
        int term_in_refs = 0;
        for (int i=0; i<num_refs; i++) {
            if (atlas_rdf_term_eq(terms[o], refs[i])) {
                break;
                term_in_refs = 1;
            }
        }
        if (!term_in_refs) {
            refs[num_refs] = terms[o];
            num_refs++;
        }
    }
    
    return lz_obj_new_v(0, 0, ^{}, num_refs, refs);
}

atlas_rdf_term_set_t
atlas_rdf_term_set_create_union(atlas_rdf_term_set_t set1,
                                atlas_rdf_term_set_t set2,
                                atlas_error_handler err) {
    assert(set1 != 0);
    assert(set2 != 0);
    
    // if both term sets are the same, return the first set
    if (lz_obj_same(set1, set2) != 0) {
        return lz_retain(set1);
    }
    
	// the resulting term set as a union of set1 and set2
    atlas_rdf_term_set_t result;

	// number of terms in both sets
	int num_terms_set1 = atlas_rdf_term_set_length(set1);
	int num_terms_set2 = atlas_rdf_term_set_length(set2);

	// create a new set of terms representing
	// the union of the two given sets set1 and set2
	// and consider the worst case in which set1 and
	// set2 are disjoint
	__block atlas_rdf_term_t * set = malloc(sizeof(atlas_rdf_term_t) * 
											(num_terms_set1 + num_terms_set2));
	assert(set != 0);
	
	// the new set has initially no elements
	__block int num_set = 0;
			
	// function to find a term in the union set and add the term
    // to the set if it is currently not in the set
    void(^iterator)(atlas_rdf_term_t term) = ^(atlas_rdf_term_t term){
		int term_in_set = 0;
				
        for (int i=0; i<num_set; i++) {
			if (atlas_rdf_term_eq(set[i], term) != 0) {
				term_in_set = 1;
				break;
			}
		}
                
		if (!term_in_set) {
			set[num_set++] = term;
		}
	};
            
	// calling the iterator for both sets
	atlas_rdf_term_set_apply_seq(set1, iterator);
	atlas_rdf_term_set_apply_seq(set2, iterator);
			
	// create a lazy object
	result = lz_obj_new_v(0, 0, ^{}, num_set, set);
	
	// free set
	free(set);
	
	return result;
}

atlas_rdf_term_set_t
atlas_rdf_term_set_create_intersection(atlas_rdf_term_set_t set1,
                                       atlas_rdf_term_set_t set2,
                                       atlas_error_handler err) {
    
}

atlas_rdf_term_set_t
atlas_rdf_term_set_create_difference(atlas_rdf_term_set_t set1,
                                     atlas_rdf_term_set_t set2,
                                     atlas_error_handler err) {
    
}

#pragma mark -
#pragma mark Access Details of a RDF Term Set

int
atlas_rdf_term_set_length(atlas_rdf_term_set_t set) {
    return lz_obj_num_ref(set);
}

void
atlas_rdf_term_set_apply(atlas_rdf_term_set_t set,
                         void(^iterator)(atlas_rdf_term_t term)) {
    dispatch_apply(lz_obj_num_ref(set),
                   dispatch_get_global_queue(0, 0),
                   ^(size_t i){
        iterator(lz_obj_weak_ref(set, i));
    });
}

void
atlas_rdf_term_set_apply_seq(atlas_rdf_term_set_t set,
							 void(^iterator)(atlas_rdf_term_t term)) {
    for (int i=0; i<lz_obj_num_ref(set); i++) {
		iterator(lz_obj_weak_ref(set, i));
	}
}

