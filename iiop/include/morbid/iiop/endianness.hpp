/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_ENDIANNESS_HPP
#define MORBID_IIOP_ENDIANNESS_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/octet.hpp>

#include <morbid/giop/common_terminals.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace fusion = boost::fusion;

namespace generator {

struct endianness_attribute
{
  bool endianness;
};

}

template <typename Attributes>
struct generator_endianness
{
  typedef Attributes attributes_type;
  typedef typename fusion::result_of::find<attributes_type, generator::endianness_attribute>::type index_iterator_type;
  typedef typename fusion::result_of::distance
    <index_iterator_type, typename fusion::result_of::end<attributes_type>::type>::type distance_to_end;
  BOOST_MPL_ASSERT_RELATION(distance_to_end::value, !=, 0);
  typedef typename fusion::result_of::distance
    <typename fusion::result_of::begin<attributes_type>::type, index_iterator_type>::type index_type;


  static bool call(Attributes const& attributes)
  {
    return fusion::at_c<index_type::value>(attributes).endianness;
  }
};

namespace generator {

template <typename Subject>
struct endianness_generator : karma::unary_generator<endianness_generator<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute : spirit::traits::attribute_of<Subject, Context, Iterator>
  {
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
    karma::any_char<octet_encoding, spirit::unused_type> octet_generator;
    return octet_generator.generate(sink, ctx, d, endianness)
      && subject.generate(sink, ctx, d, attr);
  }

  endianness_generator(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject>
struct specific_endianness_generator : endianness_generator<Subject>
{
  typedef endianness_generator<Subject> endianness_type;

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    typedef typename Context::attributes_type attributes_type;
    typedef typename fusion::result_of::as_list
      <typename fusion::result_of::push_back
       <attributes_type, endianness_attribute>::type
       >::type
      endianness_attributes_type;
    typedef spirit::context
      <endianness_attributes_type, typename Context::locals_type> context_type;
    endianness_attribute e = {endianness};
    endianness_attributes_type attributes
      = fusion::as_list(fusion::push_back(ctx.attributes, e));
    context_type context(attributes);
    context.locals = ctx.locals;
    bool r = endianness_type::generate(sink, context, d, attr);
    ctx.locals = context.locals;
    return r;
  }

  specific_endianness_generator(Subject const& subject, bool endianness)
    : endianness_type(subject), endianness(endianness)
  {
  }

  bool endianness;
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::endianness, Subject, Modifiers>
{
  typedef endianness_generator<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<giop::tag::endianness, boost::fusion::vector1<bool> >
                      , Subject, Modifiers>
{
  typedef specific_endianness_generator<Subject> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

}

} }

namespace boost { namespace spirit {

namespace traits {

template <typename Subject>
struct has_semantic_action< ::morbid::iiop::generator::endianness_generator<Subject> >
  : unary_has_semantic_action<Subject> {};

template <typename Subject, typename Attribute, typename Context
          , typename Iterator>
struct handles_container< ::morbid::iiop::generator::endianness_generator<Subject>, Attribute
                          , Context, Iterator>
: unary_handles_container<Subject, Attribute, Context, Iterator> {};

}

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain
                      , terminal_ex<morbid::giop::tag::endianness, boost::fusion::vector1<bool> >
                      , Enable> : mpl::true_ {};

template <>
struct use_lazy_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, 1>
  : mpl::true_ {};

} }

#endif