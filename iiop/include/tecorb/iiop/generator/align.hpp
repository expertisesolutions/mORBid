/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_ALIGN_HPP
#define TECORB_IIOP_GENERATOR_ALIGN_HPP

#include <boost/spirit/home/karma.hpp>

#include <vector>

namespace tecorb { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

BOOST_SPIRIT_TERMINAL_EX(align)
BOOST_SPIRIT_TERMINAL_EX(alignable)

struct align_generator : karma::primitive_generator<align_generator>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef karma::unused_type type;
  };

  align_generator(unsigned int align)
    : align(align)
  {}

  template <typename OutputIterator, typename Context, typename Delimiter>
  bool generate(OutputIterator& sink, Context&
                , Delimiter const&, karma::unused_type) const
  {
    karma::detail::counting_sink<OutputIterator>* count
      = sink.chain_counting(0);
    sink.chain_counting(count);

    if(!count)
      throw std::runtime_error("Using align without alignable");

    std::size_t dist = count->get_count();
    int div_dif = dist % align;
    int align_dif = div_dif? 4 - div_dif : 0;
    for(;align_dif != 0; --align_dif)
      *sink++ = '\0';
    return true;
  }

  unsigned int align;
};

template <typename Subject>
struct alignable_generator : karma::unary_generator<alignable_generator<Subject> >
{
  typedef Subject subject_type;
  enum
  {
    properties = karma::generator_properties::countingbuffer
    | subject_type::properties::value
  };

  template <typename Context, typename Iterator>
  struct attribute : boost::spirit::traits
    ::attribute_of<subject_type, Context, Iterator>
  {};

  alignable_generator(Subject const& subject)
    : subject(subject) {}

  template <typename OutputIterator, typename Context
            , typename Delimiter, typename Attribute>
  bool generate(OutputIterator& sink, Context& ctx
                , Delimiter const& delimiter, Attribute const& attr) const
  {
    std::cout << "alignable_generator" << std::endl;
    boost::spirit::karma::detail
      ::enable_counting<OutputIterator> counting(sink);
    bool b = subject.generate(sink, ctx, delimiter, attr);
    std::cout << "Finished: " << counting.count() << std::endl;
    return b;
  }

  Subject subject;
};

} } }

namespace boost { namespace spirit {

  ///////////////////////////////////////////////////////////////////////////
  // Enablers
  ///////////////////////////////////////////////////////////////////////////

template <typename Integer>
struct use_terminal
  <karma::domain
   , terminal_ex<tecorb::iiop::generator::tag::align, fusion::vector1<Integer> > >
  : mpl::true_ {};

template <typename T>
struct use_terminal
  <karma::domain
   , terminal_ex<tecorb::iiop::generator::tag::alignable, fusion::vector1<T> > >
  : mpl::true_ {};

template <>
struct use_directive<karma::domain, tecorb::iiop::generator::tag::alignable>
  : mpl::true_ {};

template <>
struct use_lazy_terminal<karma::domain, tecorb::iiop::generator::tag::align, 1>
  : mpl::true_ {};

} } // boost::spirit

namespace boost { namespace spirit { namespace karma {

template <typename Modifiers, typename Integer>
struct make_primitive
<terminal_ex<tecorb::iiop::generator::tag::align, fusion::vector1<Integer> >
   , Modifiers>
{
  typedef tecorb::iiop::generator::align_generator result_type;
  template <typename Terminal>
  result_type operator()(const Terminal& term, unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<tecorb::iiop::generator::tag::alignable, Subject, Modifiers>
{
  typedef tecorb::iiop::generator::alignable_generator<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject
                         , karma::unused_type) const
  {
    return result_type (subject);
  }
};

} } } // boost::spirit::karma

#endif
