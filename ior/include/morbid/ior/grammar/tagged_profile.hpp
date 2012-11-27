/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_TAGGED_PROFILE_HPP
#define MORBID_IOR_GRAMMAR_TAGGED_PROFILE_HPP

#include <morbid/giop/grammar.hpp>

#include <iostream>

namespace morbid { namespace ior { namespace grammar {

template <typename Domain, typename Iterator, typename Attr>
struct tagged_profile : giop::grammar<Domain, Iterator, Attr(giop::endian)>
{
  tagged_profile() : tagged_profile::base_type(start)
  {
    start =
      giop::ulong_
      & giop::sequence[giop::octet]
      ;
  }

  giop::rule<Domain, Iterator, Attr(giop::endian)> start;
};

} } }

#endif
