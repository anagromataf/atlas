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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#pragma mark -
#pragma mark Data Structure

#define ATLAS_RDF_TERM_HEADER atlas_rdf_term_type_t type;

struct atlas_rdf_term_s {
    ATLAS_RDF_TERM_HEADER;
};

struct atlas_rdf_term_value_s {
    ATLAS_RDF_TERM_HEADER;
    char value[]; 
};

#pragma mark Boolean

struct atlas_rdf_term_boolean_s {
    ATLAS_RDF_TERM_HEADER;
    uint8_t value; 
};

#pragma mark Datetime

struct atlas_rdf_term_datetime_s {
    ATLAS_RDF_TERM_HEADER;
    time_t value;
};

#pragma mark Double

struct atlas_rdf_term_double_s {
    ATLAS_RDF_TERM_HEADER;
    double value;
};

#pragma mark Integer

//typedef struct
//{
//    int _mp_alloc;		/* Number of *limbs* allocated and pointed
//                             to by the _mp_d field.  */
//    int _mp_size;			/* abs(_mp_size) is the number of limbs the
//                             last field points to.  If _mp_size is
//                             negative this is a negative number.  */
//    mp_limb_t *_mp_d;		/* Pointer to the limbs.  */
//} __mpz_struct;

struct atlas_rdf_term_integer_s {
    ATLAS_RDF_TERM_HEADER;
    __mpz_struct value;
};

#pragma mark Decimal

//typedef struct
//{
//    int _mp_prec;			/* Max precision, in number of `mp_limb_t's.
//                             Set by mpf_init and modified by
//                             mpf_set_prec.  The area pointed to by the
//                             _mp_d field contains `prec' + 1 limbs.  */
//    int _mp_size;			/* abs(_mp_size) is the number of limbs the
//                             last field points to.  If _mp_size is
//                             negative this is a negative number.  */
//    mp_exp_t _mp_exp;		/* Exponent, in the base of `mp_limb_t'.  */
//    mp_limb_t *_mp_d;		/* Pointer to the limbs.  */
//} __mpf_struct;

struct atlas_rdf_term_decimal_s {
    ATLAS_RDF_TERM_HEADER;
    __mpf_struct value;
};

#pragma mark -
#pragma mark Create a RDF Term

atlas_rdf_term_t
atlas_rdf_term_create_iri(const char * value,
                          atlas_error_handler err) {
    
    // TODO: Validate input
    
    int length = strlen(value);
    
    int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
    struct atlas_rdf_term_value_s * iri = malloc(size);
    assert(iri != 0);
    
    iri->type = IRI;
    memcpy(iri->value, value, length + 1);
    
    return lz_obj_new(iri, size, ^{
        free(iri);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_blank_node(const char * value,
                                 atlas_error_handler err) {
    
    // TODO: Validate input
    
    int length = strlen(value);
    
    int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
    struct atlas_rdf_term_value_s * bn = malloc(size);
    assert(bn != 0);
    
    bn->type = BLANK_NODE;
    memcpy(bn->value, value, length + 1);
    
    return lz_obj_new(bn, size, ^{
        free(bn);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_string(const char * value,
                             const char * lang,
                             atlas_error_handler err) {
    
    // TODO: Validate lang tag
    
    int value_length = strlen(value);
    int lang_length;
    if (lang != 0) {
        lang_length = strlen(lang);
    } else {
        lang_length = 0;
    }
    
    int size = sizeof(struct atlas_rdf_term_value_s) + value_length + lang_length + 2;
    struct atlas_rdf_term_value_s * str = malloc(size);
    assert(str != 0);
    
    str->type = STRING_LITERAL;
    memcpy(str->value, value, value_length + 1);
    if (lang != 0) {
        memcpy(str->value + value_length + 1, lang, lang_length + 1);
    } else {
        str->value[value_length + 1] = 0;
    }
    
    return lz_obj_new(str, size, ^{
        free(str);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_typed(const char * value,
                            atlas_rdf_term_t type,
                            atlas_error_handler err) {
    if (type) {
        
        int length = strlen(value);
        
        int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
        struct atlas_rdf_term_value_s * tl = malloc(size);
        assert(tl != 0);
        
        tl->type = TYPED_LITERAL;
        memcpy(tl->value, value, length + 1);
        
        return lz_obj_new(tl, size, ^{
            free(tl);
        }, 1, type);
        
    } else {
        return 0;
    }
}


atlas_rdf_term_t
atlas_rdf_term_create_boolean(int value,
                              atlas_error_handler err) {
    
    int size = sizeof(struct atlas_rdf_term_boolean_s);
    struct atlas_rdf_term_boolean_s * bool = malloc(size);
    assert(bool != 0);
    
    bool->type = BOOLEAN_LITERAL;
    bool->value = value;
    
    return lz_obj_new(bool, size, ^{
        free(bool);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_datetime(time_t value,
                               atlas_error_handler err) {
    
    int size = sizeof(struct atlas_rdf_term_datetime_s);
    struct atlas_rdf_term_datetime_s * dt = malloc(size);
    assert(dt != 0);
    
    dt->type = DATETIME_LITERAL;
    dt->value = value;
    
    return lz_obj_new(dt, size, ^{
        free(dt);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_double(double value,
                             atlas_error_handler err) {
    
    int size = sizeof(struct atlas_rdf_term_double_s);
    struct atlas_rdf_term_double_s * dbl = malloc(size);
    assert(dbl != 0);
    
    dbl->type = DOUBLE_LITERAL;
    dbl->value = value;
    
    return lz_obj_new(dbl, size, ^{
        free(dbl);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_integer(mpz_t value,
                              atlas_error_handler err) {
    
    // TODO: Double check the copy of this type
    
    int size = sizeof(struct atlas_rdf_term_integer_s) + sizeof(mp_limb_t) * value->_mp_size;
    struct atlas_rdf_term_integer_s * integer = malloc(size);
    assert(integer != 0);
    
    integer->type = INTEGER_LITERAL;
    integer->value._mp_alloc = value->_mp_size;
    integer->value._mp_size = value->_mp_size;
    integer->value._mp_d = memcpy((char *)(integer) + sizeof(struct atlas_rdf_term_integer_s),
                                  value->_mp_d,
                                  abs(integer->value._mp_size));
    
    return lz_obj_new(integer, size, ^{
        free(integer);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_decimal(mpf_t value,
                              atlas_error_handler err) {
    
    // TODO: Double check the copy of this type
    
    int size = sizeof(struct atlas_rdf_term_decimal_s) + sizeof(mp_limb_t) * value->_mp_size;
    struct atlas_rdf_term_decimal_s * decimal = malloc(size);
    assert(decimal != 0);
    
    decimal->type = DECIMAL_LITERAL;
    decimal->value._mp_prec = value->_mp_prec;
    decimal->value._mp_size = value->_mp_size;
    decimal->value._mp_exp = value->_mp_exp;
    decimal->value._mp_d = memcpy((char *)decimal + sizeof(struct atlas_rdf_term_decimal_s),
                                  value->_mp_d,
                                  abs(decimal->value._mp_size));
    
    return lz_obj_new(decimal, size, ^{
        free(decimal);
    }, 0);
}


#pragma mark -
#pragma mark Access Type of a RDF Term

atlas_rdf_term_type_t
atlas_rdf_term_type(atlas_rdf_term_t term) {
    __block atlas_rdf_term_type_t type;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        type = ((struct atlas_rdf_term_s *)data)->type;
    });
    return type;
}


int
atlas_rdf_term_is_type(atlas_rdf_term_t term,
                       atlas_rdf_term_type_t type) {
    return 0;
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




