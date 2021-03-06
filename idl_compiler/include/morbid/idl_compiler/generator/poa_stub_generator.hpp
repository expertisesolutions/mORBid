/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_POA_STUB_GENERATOR_HPP
#define MORBID_IDL_COMPILER_POA_STUB_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_poa_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(interface_, bool)>
{
  header_poa_stub_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator, std::string()> construct_local_stub_function;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator> dispatch_function;
  karma::rule<OutputIterator, std::string(bool)> class_name;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(interface_, bool)> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_)> operation;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(interface_, bool)> start;
};

template <typename OutputIterator, typename Iterator>
struct cpp_poa_stub_generator : karma::grammar
<OutputIterator, idl_parser::interface_def<Iterator>(interface_, bool)>
{
  cpp_poa_stub_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  idl_compiler::generator::type_spec<OutputIterator, Iterator> type_spec;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    type_spec_select;
  karma::rule<OutputIterator, std::string(bool)> construct_local_stub_function;
  karma::rule<OutputIterator, std::string()> is_a_impl_strcmp;
  karma::rule<OutputIterator, void(unsigned int)> var_indent;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>
              (unsigned int, idl_parser::interface_def<Iterator>)>
    non_user_defined_implementation, is_a_impl;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>
              (idl_parser::interface_def<Iterator>, interface_, bool)
              , karma::locals<unsigned int> > non_user_defined_operations;
  karma::rule<OutputIterator, std::string(bool)> class_name;
  karma::rule<OutputIterator, idl_parser::op_decl<Iterator>
              (unsigned int, std::string, bool, interface_)> dispatching_if;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::interface_def<Iterator>(bool, interface_)>
    dispatch_function;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    synchronous_template_args;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>(interface_, bool)> start;
};

} } }

#endif
