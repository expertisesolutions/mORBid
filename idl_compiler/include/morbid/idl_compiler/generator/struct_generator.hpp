/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_STRUCT_GENERATOR_HPP
#define MORBID_IDL_COMPILER_STRUCT_GENERATOR_HPP

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/generator/struct_generator_generator.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
struct struct_generator : karma::grammar
  <OutputIterator, idl_parser::struct_def(struct_)
   , karma::locals<unsigned int> >
{
  struct_generator();

  generator::type_spec<OutputIterator> type_spec;
  generator::struct_generator_generator<OutputIterator>
    struct_generator_generator;
  karma::rule<OutputIterator, idl_parser::member(struct_, unsigned int)> template_parameter;
  karma::rule<OutputIterator, idl_parser::member(struct_)> mpl_member_type;
  karma::rule<OutputIterator, idl_parser::member(struct_, unsigned int)> fusion_at;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_)> mpl_sequence_type;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_), karma::locals<unsigned int> > morbid_fusion_model;
  karma::rule<OutputIterator, idl_parser::member(struct_, unsigned int)> member;
  karma::rule<OutputIterator, idl_parser::struct_def(struct_)
              , karma::locals<unsigned int> > start;
};

} } }

#endif
