/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_STRUCT_GENERATOR_GENERATOR_HPP
#define MORBID_IDL_COMPILER_STRUCT_GENERATOR_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>

#include <boost/phoenix.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
struct struct_generator_generator : karma::grammar
  <OutputIterator, idl_parser::struct_def(struct_)>
{
  struct_generator_generator();

  karma::rule<OutputIterator, idl_parser::types::scoped_name(lookuped_type_wrapper)> scoped_name_generator;
  karma::rule<OutputIterator, idl_parser::types::floating_point()> floating_point_generator;
  karma::rule<OutputIterator, idl_parser::types::integer()> integer_generator;
  karma::rule<OutputIterator, idl_parser::types::char_()> char_generator;
  karma::rule<OutputIterator, idl_parser::types::wchar_()> wchar_generator;
  karma::rule<OutputIterator, idl_parser::types::boolean()> boolean_generator;
  karma::rule<OutputIterator, idl_parser::types::octet()> octet_generator;
  karma::rule<OutputIterator, idl_parser::types::any()> any_generator;
  karma::rule<OutputIterator, idl_parser::types::object()> object_generator;
  karma::rule<OutputIterator, idl_parser::types::value_base()> value_base_generator;
  karma::rule<OutputIterator, idl_parser::types::void_()> void_generator;
  karma::rule<OutputIterator, idl_parser::types::sequence(lookuped_type_wrapper)> sequence_generator;

  karma::rule<OutputIterator, idl_parser::member(struct_)> member_generator;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_)> start;
};

} } }

#endif
