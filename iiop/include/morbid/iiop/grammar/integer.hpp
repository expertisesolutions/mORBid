/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_INTEGER_HPP
#define MORBID_IIOP_GRAMMAR_INTEGER_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <boost/integer.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct word : qi::grammar<Iterator, boost::uint_t<16u>::least(bool)>
{
  word()
    : word::base_type(start)
  {
    using qi::_r1;
    start %= (qi::eps(_r1) >> qi::little_word)
      | qi::big_word
      ;
    start.name("word");
    qi::debug(start);
  }

  qi::rule<Iterator, boost::uint_t<16u>::least(bool)> start;
};

template <typename Iterator>
struct dword : qi::grammar<Iterator, boost::uint_t<32u>::least(bool)>
{
  dword()
    : dword::base_type(start)
  {
    using qi::_r1;
    start %= (qi::eps(_r1) >> qi::little_dword)
      | qi::big_dword
      ;
    start.name("dword");
    qi::debug(start);
  }

  qi::rule<Iterator, boost::uint_t<32u>::least(bool)> start;
};

template <typename Iterator>
struct qword : qi::grammar<Iterator, boost::uint_t<64u>::least(bool)>
{
  qword()
    : qword::base_type(start)
  {
    using qi::_r1;
    start %= (qi::eps(_r1) >> qi::little_qword)
      | qi::big_qword
      ;
    start.name("dword");
    qi::debug(start);
  }

  qi::rule<Iterator, boost::uint_t<64u>::least(bool)> start;
};

} } }

#endif
