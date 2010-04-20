/*
 *  term.h
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

#ifndef _ATLAS_TYPES_RDF_TERM_H_
#define _ATLAS_TYPES_RDF_TERM_H_

#include <atlas/base.h>

#include <lazy.h>
#include <stdint.h>
#include <atlas/gmp.h>
#include <time.h>

/*  Handle for an Atlas RDF Term
 */
typedef lz_obj atlas_rdf_term_t;

/*  Type of an Atlas RDF Term
 */
typedef uint16_t atlas_rdf_term_type_t;

#define TERM                0b0000000000000001
#define RESOURCE            0b0000000000000011
#define BLANK_NODE          0b0000000000000111
#define IRI                 0b0000000000001011
#define LITERAL             0b0000000000010001
#define STRING_LITERAL      0b0000000000110001
#define TYPED_LITERAL       0b0000000001010001
#define BOOLEAN_LITERAL     0b0000000011010001
#define DATETIME_LITERAL    0b0000000101010001
#define NUMERIC_LITERAL     0b0000001001010001
#define INTEGER_LITERAL     0b0000011001010001
#define DOUBLE_LITERAL      0b0000101001010001
#define DECIMAL_LITERAL     0b0001001001010001


#pragma mark -
#pragma mark Create a RDF Term

/*! Create an IRI from the given string.
 *
 *  This function creates a new RDF Term of type IRI.
 *
 *  \param value The value of the iri.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message. 
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type IRI with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_iri(const char * value,
                          atlas_error_handler err);

/*! Create an Blank Node form the given string.
 *
 *  This function creates a new RDF Term of type BLANK_NODE.
 *  
 *  \param value The lable of the blank node.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type BLANK_NODE with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_blank_node(const char * value,
                                 atlas_error_handler err);

/*! Create a Plain Literal form the given string.
 *
 *  The function atlas_rdf_term_create_string() creates a new
 *  RDF Term of type STRING_LITERAL.
 *
 *  \param value A null terminated c string containing the value
 *               of the stirng literal.
 *
 *  \param lang An optional null terminated c string which determines
 *              the language of the string literal or NULL if the
 *              literal has no language.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type STRING_LITERAL with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_string(const char * value,
                             const char * lang,
                             atlas_error_handler err);

/*! Create a typed literal.
 *
 *  This function creates a RDF Term of type TYPED_LITERAL.
 *
 *  \param value A NULL-terminated c string containing the
 *               the value of the typed literal.
 *
 *  \param type A RDF Term of type IRI describing the type of the
 *              typed literal.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type TYPED_LITERAL, BOOLEAN_LITERAL, INTEGER_LITERAL,
 *          DECIMAL_LITERAL or DATETIME_LITERAL depending on the
 *          type determiend by the parameter iri with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_typed(const char * value,
                            atlas_rdf_term_t type,
                            atlas_error_handler err);

/*! Create a boolean literal.
 *
 *  This function creates a RDF Term of type BOOLEAN_LITERAL.
 *
 *  \param value Value for the boolean literal. The literal is false if
 *               the value is 0, else true.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *  
 *  \return NULL on failure or a RDF Term handle of
 *          type BOOLEAN_LITERAL with a reference count of 1
 */
atlas_rdf_term_t
atlas_rdf_term_create_boolean(int value,
                              atlas_error_handler err);

/*! Create a interger literal.
 *
 *  This function creates a RDF Term of type INTEGER_LITERAL.
 *
 *  \param value Value of the integer literal (for details se the
 *               documentation of gmp). The caller holds the
 *               ownership of this value.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type INTEGER_LITERAL with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_integer(mpz_t value,
                              atlas_error_handler err);

/*! Create a double literal.
 *
 *  This function creates a RDF Term of type DOUBLE_LITERAL.
 *
 *  \param value The value for this literal.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of 
 *          type DOUBLE_LITERAL with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_double(double value,
                             atlas_error_handler err);

/*! Create a decimal literal.
 *
 *  This function creates a RDF Term of type DECIMAL_LITERAL.
 *
 *  \param value Value of the decimal literal (for details se the
 *               documentation of gmp). The caller holds the
 *               ownership of this value.
 *
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type DECIMAL_LITERAL with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_decimal(mpf_t value,
                              atlas_error_handler err);

/*! Create a datetime literal.
 *
 *  This function creates a RDF Term of type DATETIME_LITERAL.
 *  
 *  \param value Value representing the datetime.
 *               (uses the internal time_t).
 *  
 *  \param err An error handler which is called in case
 *             of an error with the error message.
 *
 *  \return NULL on failure or a RDF Term handle of
 *          type DATETIEM_LITERAL with a reference count of 1.
 */
atlas_rdf_term_t
atlas_rdf_term_create_datetime(time_t value,
                               atlas_error_handler err);

#pragma mark -
#pragma mark Access Type of a RDF Term

/*! Type of a RDF Term.
 *
 *  This function returns the type of a RDF Term (e.g., IRI or DATETIEM_LITERAL).
 *
 *  \param term The RDF Term. If the term is NULL, the behavior is undefined.
 *
 *  \return The type of a RDF Term.
 */
atlas_rdf_term_type_t
atlas_rdf_term_type(atlas_rdf_term_t term);

/*! Check if a RDF Term is of a certain type.
 *
 *  This function test it the given term is of the given
 *  type. Either directly or by inheritance.
 *
 *  \param term A RDF Term
 *
 *  \param type A RDF Term type
 *
 *  \return 0 if the term is kind of the given type, else != 0.
 */
int
atlas_rdf_term_is_type(atlas_rdf_term_t term,
                       atlas_rdf_term_type_t type);

#pragma mark -
#pragma mark Representation of a RDF Term

/*! Representation of a RDF Term.
 *
 *  This function returns a NULL terminated c string representing
 *  the term as defined in <http://www.w3.org/TR/rdf-sparql-query/#syntaxTerms>.
 *  
 *  The caller is responseble to call free() on the result.
 */
char *
atlas_rdf_term_repr(atlas_rdf_term_t term);

#pragma mark -
#pragma mark Access Details of a RDF Literal Term

/*! Value of a IRI.
 *
 *  This function returns a NULL terminated c string containing
 *  the value of a RDF Term of type IRI.
 *
 *  The caller is responseble to call free() on the result.
 */
char *
atlas_rdf_term_iri_value(atlas_rdf_term_t term);

/*! Lable of a Blank Node.
 *
 *  This function returns a NULL terminated c string containing the
 *  lable of a RDF Term of type BLANK_NODE.
 *
 *  The caller is responseble to call free() on the result.
 */
char *
atlas_rdf_term_blank_node_value(atlas_rdf_term_t term);

/*! Value of a Literal.
 *
 *  This function returns a NULL terminated c string containing
 *  the value of the typed literal (e.g., if the term is integer literal,
 *  the value vontains the representaion of the number).
 *
 *  The caller is responseble to call free() on the result.
 */
char *
atlas_rdf_term_literal_value(atlas_rdf_term_t term);

/*! Language of a String Literal.
 *
 *  This function returns a NULL terminated c string containing
 *  the language of the typed literal. If the literal does not
 *  have a language, a string of length 0 is returned
 *  (not a null pointer).
 *
 *  The caller is responseble to call free() on the result.
 */
char *
atlas_rdf_term_string_lang(atlas_rdf_term_t term);

/*! Type of a Typed Literal
 *
 *  This function returns a RDF Term of type IRI
 *  decribing the type of a typed literal.
 *
 *  The result is retained. The call must call lz_release() on it.
 */
atlas_rdf_term_t
atlas_rdf_term_typed_type(atlas_rdf_term_t term);

/*! Value of a Integer Literal.
 *
 *  This function returns the value of an integer literal.
 *  The caller has to prepair the variable which should contain the result.
 *
 *  \code
 *  mpz_t x;
 *  mpz_init(x);
 *  atlas_rdf_term_integer_value(term, x);
 *  // ... use the value ...
 *  mpz_clear(x);
 *  \endcode
 */
void
atlas_rdf_term_integer_value(atlas_rdf_term_t term,
                             mpz_t result);

/*! Value of a Double Literal.
 *
 *  This function returns the value of a double literal.
 */
double
atlas_rdf_term_double_value(atlas_rdf_term_t term);


/*! Value of a Decimal Literal.
 *
 *  This function returns the value of an decimal literal.
 *  The caller has to prepair the variable which should contain the result.
 *
 *  \code
 *  mpz_t x;
 *  mpz_init(x);
 *  atlas_rdf_term_integer_value(term, x);
 *  // ... use the value ...
 *  mpz_clear(x);
 *  \endcode
 */
void
atlas_rdf_term_decimal_value(atlas_rdf_term_t term,
                             mpf_t result);

/*! Value of a Datetime Literal.
 *
 *  This function returns the value of a RDF Term of
 *  type DATETIEM_LITERAL as a time_t.
 */
time_t
atlas_rdf_term_datetime_value(atlas_rdf_term_t term);

/*! Value of a Boolean Literal.
 *
 *  This function retunrs the value of a boolean literal.
 *  0 if false, else != 0.
 */
int
atlas_rdf_term_boolean_value(atlas_rdf_term_t term);


#pragma mark -
#pragma mark Effective Boolean Value

/*! EBV of a RDF Term
 *
 *  This function returns the rbv of a RDF Term.
 *
 *  See <http://www.w3.org/TR/rdf-sparql-query/#ebv>.
 */
int
atlas_rdf_term_ebv(atlas_rdf_term_t term);

#pragma mark -
#pragma mark Operation

/*! Compare to RDF Term if they are equal.
 *
 *  This function returns 0 if the terms are not equal, else != 0.
 *
 * See <http://www.w3.org/TR/rdf-sparql-query/#OperatorMapping>.
 */
int
atlas_rdf_term_eq(atlas_rdf_term_t term1,
                  atlas_rdf_term_t term2);

#endif // _ATLAS_TYPES_RDF_TERM_H_
