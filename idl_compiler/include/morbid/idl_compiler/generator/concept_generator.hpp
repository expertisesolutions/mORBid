/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP
#define MORBID_IDL_COMPILER_CONCEPT_GENERATOR_HPP

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>
#include <morbid/idl_compiler/interface.hpp>

#include <boost/spirit/home/karma.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator, typename Iterator>
struct header_concept_generator : karma::grammar
  <OutputIterator, idl_compiler::interface_def_type(interface_, std::vector<std::string>)
   , karma::locals<std::string> >
{
  header_concept_generator();

  idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
  idl_compiler::generator::type_spec<OutputIterator, Iterator> type_spec;
  idl_compiler::generator::return_<OutputIterator, Iterator> return_;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(interface_)>
    parameter_select;
  karma::rule<OutputIterator> indent;
  karma::rule<OutputIterator, std::string(std::vector<std::string>, std::string)> public_members;
  karma::rule<OutputIterator, std::string()> typedefs;
  karma::rule<OutputIterator
              , idl_parser::interface_def<Iterator>()> common_functions;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_, std::vector<std::string>, std::string)
              , karma::locals<unsigned int> > operation_concept_interface_specialization;
  karma::rule<OutputIterator
              , idl_parser::param_decl<Iterator>(interface_)> arguments;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>(interface_)
              , karma::locals<unsigned int> > operation;
  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>(unsigned int)> args;
  karma::rule<OutputIterator
              , idl_parser::op_decl<Iterator>()> operation_name;
  karma::rule<OutputIterator
              , std::vector<idl_parser::op_decl<Iterator> >()> requirements;
  karma::rule<OutputIterator
              , idl_compiler::interface_def_type(interface_, std::vector<std::string>)
              , karma::locals<std::string> > start;
};

// template <typename OutputIterator, typename Iterator>
// struct cpp_concept_generator : karma::grammar
// <OutputIterator, idl_parser::interface_def<Iterator>(interface_, std::vector<std::string>)>
// {
//   cpp_concept_generator();

//   idl_compiler::generator::parameter<OutputIterator, Iterator> parameter;
//   karma::rule<OutputIterator> indent;
//   karma::rule<OutputIterator, std::string(std::vector<std::string>)> members;
//   karma::rule<OutputIterator, std::string()> construct_remote_stub;
//   karma::rule<OutputIterator
//               , idl_parser::interface_def<Iterator>(interface_, std::vector<std::string>)> start;
// };

} } }

#endif