/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generator/struct_generator.ipp>
#include <morbid/idl_compiler/iterator.hpp>
#include <morbid/idl_compiler/parse_types.hpp>

template struct
morbid::idlc::generator::struct_generator
  < ::morbid::idlc::output_iterator, morbid::idlc::parser_iterator_type >;
