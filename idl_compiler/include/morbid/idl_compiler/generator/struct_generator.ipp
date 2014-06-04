/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 * 
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_STRUCT_GENERATOR_IPP
#define TECORB_IDL_COMPILER_STRUCT_GENERATOR_IPP

#include <morbid/idl_compiler/generator/struct_generator.hpp>

#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <boost/spirit/home/karma/nonterminal.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

#include <string>
#include <ostream>
#include <vector>

namespace morbid { namespace idlc { namespace generator {

template <typename OutputIterator, typename Iterator>
struct_generator<OutputIterator, Iterator>::struct_generator()
    : struct_generator::base_type(start)
{
  using phoenix::at_c;
  using phoenix::find;
  using phoenix::second;
  using karma::eol;
  using karma::_1; using karma::_r1; using karma::_r2;
  using karma::_val; using karma::_a;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  mpl_member_type =
    type_spec
    (
      second(*find(at_c<1>(_r1), at_c<0>(_val)))
    )[_1 = at_c<0>(_val)]
    ;
  mpl_sequence_type =
    karma::string[_1 = "::boost::mpl::vector< "]
    << (mpl_member_type(_r1) % ',')[_1 = at_c<1>(_val)]
    << " >"
    ;
  fusion_at =
    indent << mpl_member_type(_r1)[_1 = _val] << "& _morbid_fusion_at( ::boost::mpl::size_t<"
    << karma::lit(_r2) << "u>) { return " << wave_string[_1 = at_c<1>(_val)] << "; }" << eol
    << indent << mpl_member_type(_r1)[_1 = _val] << " const& _morbid_fusion_at( ::boost::mpl::size_t<"
    << karma::lit(_r2) << "u>) const { return " << wave_string[_1 = at_c<1>(_val)] << "; }" << eol
    ;
  morbid_fusion_model = // karma::eps
        indent << "typedef boost::mpl::size_t<" << karma::lit(phoenix::size(at_c<1>(_val))) << "> _morbid_fusion_size;" << eol
    << indent << "typedef " << mpl_sequence_type(_r1)[_1 = _val] << " _morbid_fusion_types;" << eol
    //      << indent << "typedef ::boost::fusion::fusion_sequence_tag tag;" << eol
    << (*fusion_at(_r1, _a++))[_1 = at_c<1>(_val)]
    ;
  template_parameter =
    ", typename A"
    << karma::lit(_r2)
    << " = "
    << type_spec
        (
        second(*find(at_c<1>(_r1), at_c<0>(_val)))
        )[_1 = at_c<0>(_val)]
    ;
  start =
    eol
    << "template <typename = void"
    << (*template_parameter(_r1, _a++))[_1 = at_c<1>(_val)]
    << "> struct "
    << wave_string[_1 = at_c<0>(_val)]
    << "_struct"
    << '{' << eol
    << indent << "// members" << eol
    << karma::eps[_a = 0]
    << (*(indent << member(_r1, _a++) << ';' << eol))[_1 = at_c<1>(_val)]
    << indent << "// members end" << eol << eol
    << indent << "typedef ::morbid::struct_tag _morbid_type_kind;" << eol
    << morbid_fusion_model(_r1)[_1 = _val]
    << struct_generator_generator(_r1)[_1 = _val]
    << "};" << eol << eol
    << "typedef " << wave_string[_1 = at_c<0>(_val)] << "_struct<> "
    << wave_string[_1 = at_c<0>(_val)] << ";" << eol
    << "inline ::std::ostream& operator<<( ::std::ostream& os, " << wave_string[_1 = at_c<0>(_val)] << " const& obj)" << eol
    << '{' << eol
    << indent << "return os;" << eol
    << '}' << eol
    ;
  indent = karma::space << karma::space;
  member =
    // type_spec
    // (
    //  at_c<1>(_r1)[at_c<0>(_val)]
    //  )[_1 = at_c<0>(_val)]
    "A" << karma::lit(_r2)
    << karma::space << wave_string[_1 = at_c<1>(_val)]
    ;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
