/*
 *  atlas_rdf_term_impl.c
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

#include "atlas_rdf_term_impl.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

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

struct atlas_rdf_term_integer_s {
    ATLAS_RDF_TERM_HEADER;
    __mpz_struct value;
};

#pragma mark Decimal

struct atlas_rdf_term_decimal_s {
    ATLAS_RDF_TERM_HEADER;
    __mpf_struct value;
};

#pragma mark -
#pragma mark Create a RDF Term

atlas_rdf_term_t
atlas_rdf_term_create_iri(const char * value,
                          atlas_error_handler err) {
    
	// Validate value via a regular expression as a
	// respresentation of the ABNF for IRIs as defined in RFC3987.
    regex_t regex;
	regcomp(&regex,
	   	    "^[a-zA-Z]([a-zA-Z0-9]|\\+|-|\\.)*:((//([^[:cntrl:]\x20<>\"{}|^`\\]*@)?[^[:cntrl:]\x20<>\"{}|^`\\]*(:\\d*)?(/[^[:cntrl:]\x20<>\"{}|^`\\]*)*)|(/([^[:cntrl:]\x20<>\"{}|^`\\]+(/[^[:cntrl:]\x20<>\"{}|^`\\]*)*)?)|([^[:cntrl:]\x20<>\"{}|^`\\]+(/[^[:cntrl:]\x20<>\"{}|^`\\]*)*)|(.{0}))(\\?[^[:cntrl:]\x20<>\"{}|^`\\]*)?(#[^[:cntrl:]\x20<>\"{}|^`\\]*)?$",
			REG_EXTENDED|REG_NOSUB);
	if (regexec(&regex, value, 0, 0, 0) != 0) {
		// TODO: define error constants
		err(0, "Pattern matching not succeeded. Value is not a valid IRI.");
		return 0;
	}
	
    // get the length of the value
    int length = strlen(value);
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
    struct atlas_rdf_term_value_s * iri = malloc(size);
    assert(iri != 0);
    
    // copy the value to the allocated memory
    iri->type = IRI;
    memcpy(iri->value, value, length + 1);
    
    // create a lazy object
    return lz_obj_new(iri, size, ^{
        free(iri);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_blank_node(const char * value,
                                 atlas_error_handler err) {
    
    // Validate value via a regular expression as a
	// representation of the ABNF for blank node labels as defined in 
	// http://www.w3.org/TR/rdf-sparql-query/#rPN_LOCAL
    regex_t regex;
	int a = regcomp(&regex,
	   	    "^([a-zA-Z0-9_\xC0-\xD6\xD8-\xF6\xF8-\xFF])([a-zA-Z0-9_\xC0-\xD6\xD8-\xF6\xF8-\xFF\\-\xB7\\.]*[a-zA-Z0-9_\xC0-\xD6\xD8-\xF6\xF8-\xFF\\-\xB7])?$",
			REG_EXTENDED|REG_NOSUB);
	if (regexec(&regex, value, 0, 0, 0) != 0) {
		// TODO: define error constants
		err(0, "Pattern matching not succeeded. Value is not a valid Blank Node Label.");
		return 0;
	}
	
    // get the length of the value
    int length = strlen(value);
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
    struct atlas_rdf_term_value_s * bn = malloc(size);
    assert(bn != 0);
    
    // copy the value to the allocated memory
    bn->type = BLANK_NODE;
    memcpy(bn->value, value, length + 1);
    
    // create a lazy object
    return lz_obj_new(bn, size, ^{
        free(bn);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_string(const char * value,
                             const char * lang,
                             atlas_error_handler err) {

	// TODO: Validate value via a regular expression as a
	// respresentation of the ABNF for string literals as defined in
	// http://www.w3.org/TR/rdf-sparql-query/#rString.

	if (lang != 0) {
		// Validate lang via a regular expression as a
		// respresentation of the ABNF for language tags as defined in
		// http://www.w3.org/TR/rdf-sparql-query/#rLANGTAG.
		regex_t regex;
		regcomp(&regex,
				"^[a-zA-Z]+(-[a-zA-Z0-9]+)*$",
				REG_EXTENDED|REG_NOSUB);
		if (regexec(&regex, lang, 0, 0, 0) != 0) {
			// TODO: define error constants
			err(0, "Pattern matching not succeeded. Lang is not a valid language tag.");
			return 0;
		}
	}

    // get the length of the value (string and language tag)
    int value_length = strlen(value);
    int lang_length;
    if (lang != 0) {
        lang_length = strlen(lang);
    } else {
        lang_length = 0;
    }
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_value_s) + value_length + lang_length + 2;
    struct atlas_rdf_term_value_s * str = malloc(size);
    assert(str != 0);
    
    // copy the value to the allocated memory
    str->type = STRING_LITERAL;
    memcpy(str->value, value, value_length + 1);
    if (lang != 0) {
        memcpy(str->value + value_length + 1, lang, lang_length + 1);
    } else {
        str->value[value_length + 1] = 0;
    }
    
    // create a lazy object
    return lz_obj_new(str, size, ^{
        free(str);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_typed(const char * value,
                            atlas_rdf_term_t type,
                            atlas_error_handler err) {
    
    // check if the term is a iri 
    if (type && atlas_rdf_term_type(type) == IRI) {
    
		atlas_rdf_term_t term;
		
	    // Check if the type can be represented directly
		if (strstr(atlas_rdf_term_iri_value(type), "integer") != 0) {
			mpz_t i;
			mpz_init_set_str(i, value, 10);
			term = atlas_rdf_term_create_integer(i, err);
			mpz_clear(i);
			if (term) {
				return term;
			}
		}
		if (strstr(atlas_rdf_term_iri_value(type), "decimal") != 0) {
			mpf_t f;
			mpf_init_set_str(f, value, 10);
			term = atlas_rdf_term_create_decimal(f, err);
			mpf_clear(f);
			if (term) {
				return term;
			}
		}
		if (strstr(atlas_rdf_term_iri_value(type), "double") != 0) {
			term = atlas_rdf_term_create_double(strtod(value, 0), err);
			if (term) {
				return term;
			}
		}
		if (strstr(atlas_rdf_term_iri_value(type), "boolean") != 0) {
			term = atlas_rdf_term_create_boolean(atoi(value), err);
			if (term) {
				return term;
			}
		}
		if (strstr(atlas_rdf_term_iri_value(type), "datetime") != 0) {
			term = atlas_rdf_term_create_datetime(atoi(value), err);
			if (term) {
				return term;
			}
		}
		if (strstr(atlas_rdf_term_iri_value(type), "string") != 0) {
			term = atlas_rdf_term_create_string(value, 0, err);
			if (term) {
				return term;
			}
		}
				
        // get the length of the value
        int length = strlen(value);
        
        // calculate the amount of space needed to store this type
        // and allocate memory
        int size = sizeof(struct atlas_rdf_term_value_s) + length + 1;
        struct atlas_rdf_term_value_s * tl = malloc(size);
        assert(tl != 0);
        
        // copy the value to the allocated memory
        tl->type = TYPED_LITERAL;
        memcpy(tl->value, value, length + 1);
        
        // create a lazy object
        return lz_obj_new(tl, size, ^{
            free(tl);
        }, 1, type);
        
    } else {
        // the term is either NULL or not an IRI
        // TODO: Call error handler
        return 0;
    }
}


atlas_rdf_term_t
atlas_rdf_term_create_boolean(int value,
                              atlas_error_handler err) {
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_boolean_s);
    struct atlas_rdf_term_boolean_s * bool = malloc(size);
    assert(bool != 0);
    
    // copy the value to the allocated memory
    bool->type = BOOLEAN_LITERAL;
    bool->value = value;
    
    // create a lazy object
    return lz_obj_new(bool, size, ^{
        free(bool);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_datetime(time_t value,
                               atlas_error_handler err) {
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_datetime_s);
    struct atlas_rdf_term_datetime_s * dt = malloc(size);
    assert(dt != 0);
    
    // copy the value to the allocated memory
    dt->type = DATETIME_LITERAL;
    dt->value = value;
    
    // create a lazy object
    return lz_obj_new(dt, size, ^{
        free(dt);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_double(double value,
                             atlas_error_handler err) {
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_double_s);
    struct atlas_rdf_term_double_s * dbl = malloc(size);
    assert(dbl != 0);
    
    // copy the value to the allocated memory
    dbl->type = DOUBLE_LITERAL;
    dbl->value = value;
    
    // create a lazy object
    return lz_obj_new(dbl, size, ^{
        free(dbl);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_integer(mpz_t value,
                              atlas_error_handler err) {
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_integer_s) + sizeof(mp_limb_t) * abs(value->_mp_size);
    struct atlas_rdf_term_integer_s * integer = malloc(size);
    assert(integer != 0);
    
    // copy the value to the allocated memory
    integer->type = INTEGER_LITERAL;
    integer->value._mp_alloc = value->_mp_size;
    integer->value._mp_size = value->_mp_size;
    integer->value._mp_d = memcpy((char *)(integer) + sizeof(struct atlas_rdf_term_integer_s),
                                  value->_mp_d,
                                  sizeof(mp_limb_t) * abs(integer->value._mp_size));
    
    // create a lazy object
    return lz_obj_new(integer, size, ^{
        free(integer);
    }, 0);
}


atlas_rdf_term_t
atlas_rdf_term_create_decimal(mpf_t value,
                              atlas_error_handler err) {
    
    // calculate the amount of space needed to store this type
    // and allocate memory
    int size = sizeof(struct atlas_rdf_term_decimal_s) + sizeof(mp_limb_t) * (value->_mp_prec + 1);
    struct atlas_rdf_term_decimal_s * decimal = malloc(size);
    assert(decimal != 0);
    
    // copy the value to the allocated memory
    decimal->type = DECIMAL_LITERAL;
    decimal->value._mp_prec = value->_mp_prec;
    decimal->value._mp_size = value->_mp_size;
    decimal->value._mp_exp = value->_mp_exp;
    decimal->value._mp_d = memcpy((char *)decimal + sizeof(struct atlas_rdf_term_decimal_s),
                                  value->_mp_d,
                                  sizeof(mp_limb_t) * (decimal->value._mp_prec + 1));
    
    // create a lazy object
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
    __block int result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        struct atlas_rdf_term_s * t = data;
        result = (t->type & type);
    });
    return result;
}


#pragma mark -
#pragma mark Representation of a RDF Term

char *
atlas_rdf_term_repr(atlas_rdf_term_t term) {
    assert(term != 0);
    
    __block char * result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_s * t = data;
        
        switch (t->type) {
            case IRI:
            {
                struct atlas_rdf_term_value_s *t = data;  
                char * buffer;
                asprintf(&buffer, "<%s>", t->value);
                result = buffer;
                break;
            }
                
            case BLANK_NODE:
            {
                struct atlas_rdf_term_value_s *t = data;  
                char * buffer;
                asprintf(&buffer, "_:%s", t->value);
                result = buffer;
                break;
            }  
                
            case STRING_LITERAL:
            {
                struct atlas_rdf_term_value_s *t = data;
                char * buffer;
                int offset = strlen(t->value) + 1;
                if (strlen(t->value + offset) == 0) {
                    // TODO: Escape ", ' etc.
                    asprintf(&buffer, "\"%s\"", t->value);
                } else {
                    asprintf(&buffer, "\"%s\"@%s", t->value, t->value + offset);
                }
                result = buffer;
                break;
            }
                
            case TYPED_LITERAL:
            {
                struct atlas_rdf_term_value_s *t = data;
                char * buffer;
                char * type = atlas_rdf_term_iri_value(lz_obj_weak_ref(term, 0));
                assert(type);
                asprintf(&buffer, "\"%s\"^^<%s>", t->value, type);
                free(type);
                result = buffer;
                break;
            }
                
            case BOOLEAN_LITERAL:
            {
                struct atlas_rdf_term_boolean_s *t = data;
                char * buffer;
                if (t->value == 0) {
                    asprintf(&buffer, "false");
                } else {
                    asprintf(&buffer, "true");
                }
                result = buffer;
                break;
            }
                
            case DOUBLE_LITERAL:
            {
                struct atlas_rdf_term_double_s *t = data;
                char * buffer;
                asprintf(&buffer, "%e", t->value);
                result = buffer;
                break;
            }
                
            case DATETIME_LITERAL:
            {
                struct atlas_rdf_term_datetime_s *t = data;
                int buff_size = 73;
                char * buffer = malloc(buff_size);
                struct tm time_tmp;
                gmtime_r(&(t->value), &time_tmp);
                int length = strftime(buffer, buff_size, "\"%Y-%m-%dT%H:%M:%S+00:00\"^^<http://www.w3.org/2001/XMLSchema#dateTime>", &time_tmp);
                assert(length != 0);
                result = buffer;
                break;
            }
                
            case DECIMAL_LITERAL:
            {
                struct atlas_rdf_term_decimal_s *t = data;
                char * buffer;
                mpf_t f = { t->value };
                gmp_asprintf(&buffer, "%.Ff", f);
                result = buffer;
                break;
            }
                
            case INTEGER_LITERAL:
            {
                struct atlas_rdf_term_integer_s *t = data;
                char * buffer;
                mpz_t z = { t->value };
                gmp_asprintf(&buffer, "%Zd", z);
                result = buffer;
                break;
            }
                
            default:
                assert(0);
                break;
        }
    });
    return result;
}


#pragma mark -
#pragma mark Access Details of a RDF Literal Term

char *
atlas_rdf_term_iri_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block char * result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_value_s * iri = data;
        assert(iri->type == IRI);
        
        result = strdup(iri->value);
        assert(result);
    });
    return result;
}

char *
atlas_rdf_term_blank_node_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block char * result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_value_s * bn = data;
        assert(bn->type == BLANK_NODE);
        
        result = strdup(bn->value);
        assert(result);
    });
    return result;
}

char *
atlas_rdf_term_literal_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block char * result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_s * literal = data;
        
        switch (literal->type) {
            case STRING_LITERAL:
            {
                struct atlas_rdf_term_value_s * sl = data;
                result = strdup(sl->value);
                break;
            }
                
            case TYPED_LITERAL:
            {
                struct atlas_rdf_term_value_s * sl = data;
                result = strdup(sl->value);
                break;
            }
                
            case BOOLEAN_LITERAL:
            {
                struct atlas_rdf_term_boolean_s * bool = data;
                if (bool->value == 0) {
                    result = strdup("false");
                } else {
                    result = strdup("true");
                }
                break;
            }
            
            case DOUBLE_LITERAL:
            {
                struct atlas_rdf_term_double_s * dl = data;
                char * buffer;
                asprintf(&buffer, "%e", dl->value);
                result = buffer;
                break;
            }
                
            case DATETIME_LITERAL:
            {
                struct atlas_rdf_term_datetime_s * dt = data;
                int buff_size = 26;
                char * buffer = malloc(buff_size);
                struct tm t;
                gmtime_r(&(dt->value), &t);
                int length = strftime(buffer, buff_size, "%Y-%m-%dT%H:%M:%S+00:00", &t);
                assert(length != 0);
                result = buffer;
                break;
            }
                
            case DECIMAL_LITERAL:
            {
                struct atlas_rdf_term_decimal_s *decimal = data;
                char * buffer;
                mpf_t f = { decimal->value };
                gmp_asprintf(&buffer, "%.Ff", f);
                result = buffer;
                break;
            }
            
            case INTEGER_LITERAL:
            {
                struct atlas_rdf_term_integer_s *integer = data;
                char * buffer;
                mpz_t z = { integer->value };
                gmp_asprintf(&buffer, "%Zd", z);
                result = buffer;
                break;
            }
                
            default:
                assert(0);
                break;
        }
        assert(result);
    });
    return result;
}

char *
atlas_rdf_term_string_lang(atlas_rdf_term_t term) {
    assert(term != 0);
    __block char * result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_value_s * str = data;
        assert(str->type == STRING_LITERAL);
        
        int offset = strlen(str->value) + 1;
        result = strdup(str->value + offset);
        assert(result);
    });
    return result;
}

atlas_rdf_term_t
atlas_rdf_term_typed_type(atlas_rdf_term_t term) {
    assert(term != 0);
    __block atlas_rdf_term_t result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_value_s * lit = data;
        assert(lit->type == TYPED_LITERAL);
        
        result = lz_obj_ref(term, 0);
    });
    return result;
}

void
atlas_rdf_term_integer_value(atlas_rdf_term_t term, mpz_t result) {
    assert(term != 0);
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_integer_s * integer = data;
        assert(integer->type == INTEGER_LITERAL);
        
        mpz_t i = { integer->value };
        mpz_set(result, i);
    });
}

double
atlas_rdf_term_double_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block double result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_double_s * d = data;
        assert(d->type == DOUBLE_LITERAL);
        
        result = d->value;
    });
    return result;
}

void
atlas_rdf_term_decimal_value(atlas_rdf_term_t term, mpf_t result) {
    assert(term != 0);
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_decimal_s * decimal = data;
        assert(decimal->type == DECIMAL_LITERAL);
        
        mpf_t f = { decimal->value };
        mpf_set(result, f);
    });
}

time_t
atlas_rdf_term_datetime_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block time_t result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_datetime_s * dt = data;
        assert(dt->type == DATETIME_LITERAL);
        
        result = dt->value;
    });
    return result;
}

int
atlas_rdf_term_boolean_value(atlas_rdf_term_t term) {
    assert(term != 0);
    __block int result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_boolean_s * b = data;
        assert(b->type == BOOLEAN_LITERAL);
        
        result = b->value;
    });
    return result;
}


#pragma mark -
#pragma mark Effective Boolean Value

int atlas_rdf_term_ebv(atlas_rdf_term_t term) {
    assert(term != 0);
    __block int result;
    lz_obj_sync(term, ^(void * data, uint32_t length){
        
        struct atlas_rdf_term_s * literal = data;
        
        switch (literal->type) {
            case IRI:
            {
                struct atlas_rdf_term_value_s * iri = data;  
                break;
            }
                
            case BLANK_NODE:
            {
                struct atlas_rdf_term_value_s * bn = data;  
                break;
            }  
                
            case STRING_LITERAL:
            {
                struct atlas_rdf_term_value_s * sl = data;
                break;
            }
                
            case TYPED_LITERAL:
            {
                struct atlas_rdf_term_value_s * sl = data;
                break;
            }
                
            case BOOLEAN_LITERAL:
            {
                struct atlas_rdf_term_boolean_s * bool = data;
                break;
            }
                
            case DOUBLE_LITERAL:
            {
                struct atlas_rdf_term_double_s * dl = data;
                break;
            }
                
            case DATETIME_LITERAL:
            {
                struct atlas_rdf_term_datetime_s * dt = data;
                break;
            }
                
            case DECIMAL_LITERAL:
            {
                struct atlas_rdf_term_decimal_s *decimal = data;
                break;
            }
                
            case INTEGER_LITERAL:
            {
                struct atlas_rdf_term_integer_s *integer = data;
                break;
            }
                
            default:
                assert(0);
                break;
        }
    });
    return result;
}


#pragma mark -
#pragma mark Operation

int atlas_rdf_term_eq(atlas_rdf_term_t term1,
                      atlas_rdf_term_t term2) {
    if (lz_obj_same(term1, term2)) {
        return 1;
    } else {
        __block int result;
        lz_obj_sync(term1, ^(void * data, uint32_t length){
            struct atlas_rdf_term_s * t1 = data;
            switch (t1->type) {
                case IRI:
                {
                    struct atlas_rdf_term_value_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_value_s * t2 = data;
                        switch (t2->type) {
                            case IRI:
                                result = strcmp(t1->value, t2->value) == 0 ? 1 : 0;
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                case BLANK_NODE:
                {
                    struct atlas_rdf_term_value_s * t1 = data;  
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_value_s * t2 = data;
                        switch (t2->type) {
                            case BLANK_NODE:
                                result = strcmp(t1->value, t2->value) == 0 ? 1 : 0;
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }  
                    
                case STRING_LITERAL:
                {
                    struct atlas_rdf_term_value_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_value_s * t2 = data;
                        switch (t2->type) {
                            case STRING_LITERAL:
                                if (strcmp(t1->value, t2->value) != 0) {
                                    result = 0;
                                } else {
                                    result = strcmp(t1->value + strlen(t1->value) + 1,
                                                    t2->value + strlen(t2->value) + 1) == 0 ? 1 : 0;
                                }
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                case TYPED_LITERAL:
                {
                    struct atlas_rdf_term_value_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_value_s * t2 = data;
                        switch (t2->type) {
                            case TYPED_LITERAL:
                                if (strcmp(t1->value, t2->value) != 0) {
                                    result = 0;
                                } else {
                                    result = atlas_rdf_term_eq(lz_obj_weak_ref(term1, 0),
                                                               lz_obj_weak_ref(term2, 0));
                                }
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                case BOOLEAN_LITERAL:
                {
                    struct atlas_rdf_term_boolean_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_boolean_s * t2 = data;
                        switch (t2->type) {
                            case BOOLEAN_LITERAL:
                                result = (t1->value == t2->value || t1->value != 0 && t2->value != 0) ? 1 : 0;
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                case DOUBLE_LITERAL:
                {
                    struct atlas_rdf_term_double_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_s * t2 = data;
                        switch (t2->type) {
                            case DOUBLE_LITERAL:
                            {
                                struct atlas_rdf_term_double_s * t2 = data;
                                result = t1->value == t2->value;
                                break;
                            }
                            
                            case DECIMAL_LITERAL:
                            {
                                struct atlas_rdf_term_decimal_s * t2 = data;
                                mpf_t f = { t2->value };
                                result = mpf_cmp_d(f, t1->value) == 0 ? 1 : 0;
                                break;
                            }
                            
                            case INTEGER_LITERAL:
                            {
                                struct atlas_rdf_term_integer_s * t2 = data;
                                mpz_t f = { t2->value };
                                result = mpz_cmp_d(f, t1->value) == 0 ? 1 : 0;
                                break;
                            }
                            
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                case DECIMAL_LITERAL:
                {
                    struct atlas_rdf_term_decimal_s *t1 = data;
                    mpf_t f1 = { t1->value };
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_s * t2 = data;
                        switch (t2->type) {
                            case DOUBLE_LITERAL:
                            {
                                struct atlas_rdf_term_double_s * t2 = data;
                                result = mpf_cmp_d(f1, t2->value) == 0 ? 1 : 0;
                                break;                            
                            }
                            
                            case DECIMAL_LITERAL:
                            {
                                struct atlas_rdf_term_decimal_s * t2 = data;
                                mpf_t f2 = { t2->value };
                                result = mpf_cmp(f1, f2) == 0 ? 1 : 0;
                                break;
                            }
                            
                            case INTEGER_LITERAL:
                            {
                                struct atlas_rdf_term_integer_s * t2 = data;
                                mpz_t z = { t2->value };
                                mpf_t f2;
                                mpf_init(f2);
                                mpf_set_z(f2,z);
                                result = mpf_cmp(f1, f2) == 0 ? 1 : 0;
                                mpf_clear(f2);
                                break;
                            }
                            
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                
                case INTEGER_LITERAL:
                {
                    struct atlas_rdf_term_integer_s *t1 = data;
                    mpz_t z1 = { t1->value };
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_s * t2 = data;
                        switch (t2->type) {
                            case DOUBLE_LITERAL:
                            {
                                struct atlas_rdf_term_double_s * t2 = data;
                                result = mpz_cmp_d(z1, t2->value) == 0 ? 1 : 0;
                                break;                            
                            }
                            
                            case DECIMAL_LITERAL:
                            {
                                struct atlas_rdf_term_decimal_s * t2 = data;
                                mpf_t f2 = { t2->value };
                                mpf_t f1;
                                mpf_init(f1);
                                mpf_set_z(f1,z1);
                                result = mpf_cmp(f1, f2) == 0 ? 1 : 0;
                                mpf_clear(f1);
                                break;
                            }
                            
                            case INTEGER_LITERAL:
                            {
                                struct atlas_rdf_term_integer_s * t2 = data;
                                mpz_t z2 = { t2->value };
                                result = mpz_cmp(z1, z2) == 0 ? 1 : 0;
                                break;
                            }
                            
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                
                case DATETIME_LITERAL:
                {
                    struct atlas_rdf_term_datetime_s * t1 = data;
                    lz_obj_sync(term2, ^(void * data, uint32_t length){
                        struct atlas_rdf_term_datetime_s * t2 = data;
                        switch (t2->type) {
                            case DATETIME_LITERAL:
                                result = t1->value == t2->value;
                                break;
                                
                            default:
                                result = 0;
                        }
                    });
                    break;
                }
                    
                    
                default:
                    result = 0;
            }
        });
        return result;        
    }
}

#pragma mark -
#pragma mark Compare Functions




