/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_IPP
#define TECORB_IDL_COMPILER_PROXY_REFERENCE_GENERATOR_IPP

#include <morbid/idl_compiler/generator/proxy_reference_generator.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace std {

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, std::pair<T, U> p)
{
  return os << "[pair first: " << p.first << " second: " << p.second << ']';
}

}

namespace morbid { namespace idlc { namespace generator {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

template <typename OutputIterator>
proxy_reference_generator<OutputIterator>::proxy_reference_generator()
  : proxy_reference_generator::base_type(start)
{
  namespace phoenix = boost::phoenix;
  using karma::_1;
  using karma::_val;
  using karma::_a; using karma::_r1;
  using karma::eol;

  using phoenix::at_c;
  using phoenix::find;
  using phoenix::second;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  start = eol
    << "template <typename T>" << eol
    << "struct "
    << wave_string[_1 = at_c<0>(_val)]
    << "_concept::proxy_reference" << eol
    << "{" << eol
    << indent << "proxy_reference(boost::reference_wrapper<T> ref) : _ptr(&boost::unwrap_ref(ref)) {}" << eol
    << indent << "// Start of operations defined in IDL" << eol
    << (*(operation(_r1) << eol))[_1 = at_c<1>(_val)]
    << indent << "// End of operations defined in IDL" << eol
    << indent << "T* _ptr;" << eol
    << "};" << eol << eol
    ;
  operation =
    indent
    << return_
    (
     second(*find(at_c<1>(_r1), at_c<0>(_val))) // interface_.lookups[type_spec]
     , std::string("T")
    )
    [_1 = at_c<0>(_val)]
    << karma::space
    << wave_string[_1 = at_c<1>(_val)]
    << "("
    << -((parameter_select(_r1) << " arg" << karma::lit(++_a)) % ", ")[_1 = at_c<2>(_val)]
    << ")" << eol
    << indent << "{" << eol
    << indent << indent << "return _ptr->" << wave_string[_1 = at_c<1>(_val)]
    << "("
    << karma::eps[_a = 0]
    << -(synchronous_args(++_a) % (eol << indent << indent << indent << indent << ", "))[_1 = at_c<2>(_val)]
    << ");"
    << eol
    << indent << "}" << eol
    ;
  return_ = "typename ::morbid::lazy_eval< " << return_traits(_r1)[_1 = _val] << ", " << karma::lit(spirit::_r2) << ">::type";
  synchronous_args %= "arg" << karma::lit(_r1);
  parameter_select %= "typename ::morbid::lazy_eval< "
    << parameter(second(*find(at_c<1>(_r1), at_c<1>(_val)))) << ", T>::type";
  indent = karma::space << karma::space;

  start.name("proxy_reference_generator");
  operation.name("operation");

  karma::debug(start);
  karma::debug(operation);

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif
