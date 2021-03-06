/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_AUXILIARY_HPP
#define MORBID_IIOP_AUXILIARY_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace karma = spirit::karma;

namespace parser {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef qi::eps_parser result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef qi::semantic_predicate result_type;
  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args) ? true : false);
  }
};

template <typename Terminal, typename Actor, int Arity, typename Modifiers>
struct make_primitive<spirit::lazy_terminal<Terminal, Actor, Arity>, Modifiers>
{
  typedef qi::lazy_parser<Actor, Modifiers> result_type;
  result_type operator()(spirit::lazy_terminal<Terminal, Actor, Arity> const& lt
                         , Modifiers const& modifiers) const
  {
    return result_type(lt.actor, modifiers);
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::tag::repeat, Subject, Modifiers>
{
  typedef qi::kleene<Subject> result_type;
  result_type operator()(spirit::unused_type, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector1<T> >, Subject, Modifiers>
{
  typedef qi::exact_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector2<T, T> >, Subject, Modifiers>
{
  typedef qi::finite_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;
  
  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject,
                       iterator_type(
                                     fusion::at_c<0>(term.args)
                                     , fusion::at_c<1>(term.args)
                                     )
                       );
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector2<T, spirit::inf_type> >, Subject, Modifiers>
{
  typedef qi::infinite_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;
  
  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

///////////////////////////////////////////////////////////////////////////
// Parser generator: make_xxx function (objects)
///////////////////////////////////////////////////////////////////////////
template <typename Expr, typename Exposed, typename Transformed
          , typename Modifiers>
struct make_primitive<
  spirit::tag::stateful_tag<Expr, spirit::tag::attr_cast, Exposed, Transformed>, Modifiers>
{
  typedef typename spirit::result_of::compile<parser_domain, Expr>::type
    expr_type;
  typedef qi::attr_cast_parser<Exposed, Transformed, expr_type> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    typedef spirit::tag::stateful_tag<
      expr_type, spirit::tag::attr_cast, Exposed, Transformed> tag_type;
    using spirit::detail::get_stateful_data;
    return result_type(/*giop::compile<parser_domain>*/
                       /*(*/get_stateful_data<tag_type>::call(term))/*)*/;
  }
};

}

namespace generator {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef karma::eps_generator result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};
  
template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef karma::semantic_predicate result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

}

} }

namespace boost { namespace spirit {

template <>
struct use_terminal< ::morbid::iiop::parser_domain, spirit::tag::eps>       // enables eps
  : mpl::true_ {};

template <typename A0>
struct use_terminal
  < ::morbid::iiop::parser_domain
    , terminal_ex<tag::eps, fusion::vector1<A0> > // enables eps(bool-condition)
  > : is_convertible<A0, bool> {};

template <>                                     // enables eps(f)
struct use_lazy_terminal<
  ::morbid::iiop::parser_domain, tag::eps, 1 /*arity*/
  > : mpl::true_ {};

// enables eps
template <>
struct use_terminal< ::morbid::iiop::generator_domain, tag::eps>
  : mpl::true_ {};

// enables eps(bool-condition)
template <typename A0>
struct use_terminal
 < ::morbid::iiop::generator_domain
   , terminal_ex<tag::eps, fusion::vector1<A0> > > 
: is_convertible<A0, bool> {};

// enables lazy eps(f)
template <>
struct use_lazy_terminal< ::morbid::iiop::generator_domain, tag::eps, 1>
  : mpl::true_ {};

template <>
struct use_directive< ::morbid::iiop::parser_domain, tag::repeat>   // enables repeat[p]
  : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector1<T> >    // enables repeat(exact)[p]
 > : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector2<T, T> > // enables repeat(min, max)[p]
 > : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector2<T, inf_type> > // enables repeat(min, inf)[p]
 > : mpl::true_ {};

template <>                                     // enables *lazy* repeat(exact)[p]
struct use_lazy_directive
 < ::morbid::iiop::parser_domain
   , tag::repeat
   , 1 // arity
 > : mpl::true_ {};

template <>                                     // enables *lazy* repeat(min, max)[p]
struct use_lazy_directive                       // and repeat(min, inf)[p]
 < ::morbid::iiop::parser_domain
   , tag::repeat
   , 2 // arity
 > : mpl::true_ {};

// enables attr_cast<>() pseudo parser
template <typename Expr, typename Exposed, typename Transformed>
struct use_terminal< ::morbid::iiop::parser_domain
                    , tag::stateful_tag<Expr, tag::attr_cast, Exposed, Transformed> >
 : mpl::true_ {};

} }

#endif
