/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_COMMON_TYPES
#define MORBID_IDL_COMPILER_COMMON_TYPES

#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/typedef_def.hpp>
#include <morbid/idl_parser/exception_def.hpp>
#include <morbid/idl_parser/struct_def.hpp>

#include <boost/spirit/home/lex.hpp>
#include <boost/spirit/home/lex/lexer_lexertl.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <iterator>
#include <ostream>

namespace morbid { namespace idl_compiler {

typedef boost::spirit::lex::lexertl::token<std::vector<char>::const_iterator> token_type;
typedef boost::spirit::lex::lexertl::actor_lexer<token_type> lexer_type;
        
typedef lexer_type::iterator_type parser_iterator_type;
typedef morbid::idl_parser::interface_def<parser_iterator_type> interface_def_type;
typedef morbid::idl_parser::op_decl<parser_iterator_type> op_decl_type;
typedef morbid::idl_parser::typedef_def<parser_iterator_type> typedef_def_type;
typedef morbid::idl_parser::exception_def<parser_iterator_type> exception_def_type;
typedef morbid::idl_parser::struct_def<parser_iterator_type> struct_def_type;
typedef morbid::idl_parser::struct_member<parser_iterator_type> struct_member_type;
typedef morbid::idl_parser::type_spec<parser_iterator_type> type_spec;
typedef morbid::idl_parser::param_decl<parser_iterator_type> param_decl;
typedef std::ostream_iterator<char> output_iterator_type;

struct module_property_t
{
  typedef boost::vertex_property_tag kind;
};

struct module;

typedef boost::property<module_property_t, boost::shared_ptr<module>
                        /*, boost::property<boost::vertex_color_t, boost::default_color_type>*/ >
  module_property_type;
typedef boost::adjacency_list<boost::setS, boost::listS, boost::bidirectionalS
                              , module_property_type>
  modules_tree_type;
typedef modules_tree_type::vertex_descriptor vertex_descriptor;
typedef modules_tree_type::edge_descriptor edge_descriptor;

} }


#endif
