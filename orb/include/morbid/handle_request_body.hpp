/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_HANDLE_REQUEST_BODY_HPP
#define MORBID_HANDLE_REQUEST_BODY_HPP

#include <morbid/any.hpp>
#include <morbid/detail/max_args.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/arguments_traits.hpp>
#include <morbid/transforms.hpp>
#include <morbid/giop/grammars/arguments.hpp>
#include <morbid/giop/grammars/message_1_0.hpp>
#include <morbid/giop/grammars/reply_1_0.hpp>
#include <morbid/iiop/all.hpp>

#include <boost/fusion/include/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/copy_if.hpp>

#include <boost/type_traits/is_fundamental.hpp>

namespace morbid {

namespace fusion = boost::fusion;

template <typename T>
struct create_argument_transform;

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::in_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::in_tag> tagged;
  BOOST_MPL_ASSERT((mpl::not_<boost::is_const<typename tagged::type> >));
  typedef typename boost::add_reference<typename tagged::type>::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::out_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::out_tag> tagged;
  typedef typename tagged::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::inout_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::inout_tag> tagged;
  BOOST_MPL_ASSERT((mpl::not_<boost::is_const<typename tagged::type> >));
  typedef typename boost::add_reference<typename tagged::type>::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
  typedef const char* type;
  typedef type result_type;

  result_type operator()(std::string const& str) const
  {
    return str.c_str();
  }
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::out_tag> >
  : create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::inout_tag> >
  : create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
};

template <typename T>
struct reply_argument_transform;

template <typename T>
struct reply_argument_transform<type_tag::value_type_tag<T, type_tag::out_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::in_tag> tagged;
  typedef typename boost::add_reference<typename tagged::type>::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <typename T>
struct reply_argument_transform<type_tag::value_type_tag<T, type_tag::inout_tag> >
  : reply_argument_transform<type_tag::value_type_tag<T, type_tag::out_tag> >
{
};

template <>
struct reply_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::out_tag> >
{
  typedef std::string type;
  typedef type result_type;

  result_type operator()(const char* str) const
  {
    return str;
  }
};

template <>
struct reply_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::inout_tag> >
  : reply_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::out_tag> >
{
};

template <typename T>
struct tag
{
};

template <typename ParseArguments, typename Condition, typename ResultLambda>
struct initialize_arguments
{
  template <typename R>
  struct result;

  template <typename This, int I, typename S, typename T>
  struct result<This(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>
  {
    typedef typename mpl::apply1<typename mpl::lambda<Condition>::type, T>::type condition;
    typedef typename mpl::apply1<typename mpl::lambda<ResultLambda>::type, T>::type transform_result;

    typedef typename mpl::if_
    <condition
     , std::pair<mpl::int_<I>
                 , fusion::cons
                 <transform_result, S> >
     , std::pair<mpl::int_<I+1>
                 , fusion::cons
                 <transform_result, S> >
     >::type type;
  };

  initialize_arguments(ParseArguments& parse_arguments)
    : parse_arguments(parse_arguments)
  {}

  template <int I, typename S, typename T>
  typename result<initialize_arguments<ParseArguments, Condition, ResultLambda>
                  (std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S>const& s, tag<T>const&, mpl::true_) const
  {
    BOOST_MPL_ASSERT((mpl::not_<boost::is_fundamental<T> >));
    typedef typename mpl::apply1<typename mpl::lambda<ResultLambda>::type, T>::type type;
    return std::pair<mpl::int_<I>, fusion::cons<type, S> >
      (mpl::int_<I>(), fusion::cons<type, S>(type(), s.second));
  }

  template <int I, typename S, typename T>
  typename result<initialize_arguments<ParseArguments, Condition, ResultLambda>
                  (std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S>const& s, tag<T>const&, mpl::false_) const
  {
    BOOST_MPL_ASSERT((mpl::not_<boost::is_fundamental<T> >));
    typedef typename mpl::apply1<typename mpl::lambda<ResultLambda>::type, T>::type type;
    typedef create_argument_transform<T> transform;
    typedef fusion::result_of::at_c<ParseArguments, I> arg_type;
    // BOOST_MPL_ASSERT((mpl::not_<boost::is_const<typename boost::remove_reference<arg_type>::type> >));
    // BOOST_MPL_ASSERT((mpl::not_<boost::is_const<typename boost::remove_reference<typename transform::result_type>::type> >));
    BOOST_MPL_ASSERT((boost::is_same<typename boost::result_of<transform(arg_type)>::type, type>));
    return std::pair<mpl::int_<I+1>, fusion::cons<type, S> >
      (mpl::int_<I+1>(), fusion::cons<type, S>
       (transform()(fusion::at_c<I>(parse_arguments)), s.second));
  }

  template <int I, typename S, typename T>
  typename result<initialize_arguments<ParseArguments, Condition, ResultLambda>
                  (std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  operator()(std::pair<mpl::int_<I>, S>const& s, tag<T>const& t) const
  {
    BOOST_MPL_ASSERT((mpl::not_<boost::is_fundamental<T> >));
    typedef typename mpl::apply1<typename mpl::lambda<Condition>::type, T>::type condition;
    return call(s, t, condition());
  }

  ParseArguments& parse_arguments;
};

template <typename ParseArguments>
struct reply_arguments_generator
{
  template <typename R>
  struct result;

  template <typename This, int I, typename S, typename T>
  struct result<This(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>
  {
    template <typename U>
    struct create_pair
    {
      typedef std::pair<mpl::int_<I+1>, fusion::cons<typename reply_argument_transform<U>::type, S> > type;
    };

    typedef type_tag::is_not_in_type_tag<T> is_not_in;
    typedef typename mpl::eval_if
    <is_not_in
     ,  create_pair<T>
      , mpl::identity<std::pair<mpl::int_<I>, S> >
    >::type
     type;
  };

  reply_arguments_generator(ParseArguments& parse_arguments)
    : parse_arguments(parse_arguments)
  {}

  template <int I, typename S, typename T>
  typename result<reply_arguments_generator<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S> const& s, tag<T>const&, type_tag::in_tag) const
  {
    return s;
  }

  template <int I, typename S, typename T, typename Tag>
  typename result<reply_arguments_generator<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S> const& s, tag<T>const&, Tag) const
  {
    typedef reply_argument_transform<T> transform;
    typedef typename transform::type type;
    return std::pair<mpl::int_<I+1>, fusion::cons<type, S> >
      (mpl::int_<I+1>(), fusion::cons<type, S>(transform()(fusion::at_c<I>(parse_arguments)), s.second));
  }

  template <int I, typename S, typename T>
  typename result<reply_arguments_generator<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  operator()(std::pair<mpl::int_<I>, S> const& s, tag<T>const& t) const
  {
    return call(s, t, typename T::tag());
  }

  ParseArguments& parse_arguments;
};

template <typename NotInParams, typename ReplyArguments>
void make_request_reply(reply& r, ReplyArguments& reply_arguments)
{
  std::cout << "Generating reply with reply arguments " << typeid(ReplyArguments).name() << std::endl;
  typedef ReplyArguments reply_argument_types;

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef std::vector<fusion::vector2<unsigned int, std::vector<char> > > service_context_list;
  typedef fusion::vector4<service_context_list
                          , unsigned int, unsigned int
                          , typename reply_argument_types::second_type>
    reply_attribute_type;
  typedef fusion::vector1<fusion::vector1<reply_attribute_type> >
    message_reply_attribute_type;

  typedef giop::grammars::arguments<iiop::generator_domain
                                    , output_iterator_type, NotInParams
                                    , typename reply_argument_types::second_type>
    arguments_grammar;
  typedef giop::grammars::reply_1_0<iiop::generator_domain
                                    , output_iterator_type, reply_attribute_type>
    reply_grammar;
  typedef giop::grammars::message_1_0<iiop::generator_domain
                                      , output_iterator_type, message_reply_attribute_type
                                      , 1u /* reply */>
    message_reply_grammar;
  
  morbid::arguments_traits arguments_traits;
  arguments_grammar arguments_grammar_(arguments_traits);
  reply_grammar reply_grammar_(arguments_grammar_);
  message_reply_grammar message_grammar_(reply_grammar_);

  service_context_list l;
  message_reply_attribute_type message_attribute
    (fusion::make_vector
     (reply_attribute_type
      (l
       , r.request_id
       , 0u /* NO_EXCEPTION */
       , reply_arguments.second)));
  output_iterator_type iterator(r.reply_body);
  namespace karma = boost::spirit::karma;
  if(karma::generate(iterator, giop::compile<iiop::generator_domain>
                     (message_grammar_(giop::native_endian))
                     , message_attribute))
  {
    std::cout << "reply generated" << std::endl;
  }
  else
  {
    std::cout << "Failed generating reply" << std::endl;
    throw morbid::MARSHALL();
  }
}

template <typename R, typename SeqParam, typename F, typename T, typename Args>
void handle_request_reply(F f, T* self, reply& r, Args& args, mpl::identity<void>)
{
  fusion::single_view<T*> self_view(self);
  fusion::joint_view<fusion::single_view<T*> const, Args> new_args(self_view, args);
  f(new_args);

  typedef Args argument_types;

  typedef typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type is_not_in_lambda;
  typedef typename mpl::copy_if<SeqParam, is_not_in_lambda>::type not_in_params;

  typedef typename mpl::transform<SeqParam, create_argument_transform<mpl::_1> >::type mpl_argument_types;
  typedef typename mpl::transform<SeqParam, tag<mpl::_1> >::type mpl_identity_argument_types;
  typedef typename fusion::result_of::as_vector<mpl_identity_argument_types>::type identity_argument_types;
  identity_argument_types const identity_arguments;

  typedef typename boost::remove_reference<
    typename fusion::result_of::fold<identity_argument_types const, std::pair<mpl::int_<0>, fusion::nil>
                                     , reply_arguments_generator<argument_types> >::type
    >::type
    reply_argument_types;

  std::cout << "reply_argument_types " << typeid(reply_argument_types).name() << std::endl;

  reply_argument_types reply_arguments = fusion::fold(identity_arguments, std::pair<mpl::int_<0>, fusion::nil>()
                                                      , reply_arguments_generator<argument_types>(args));
  make_request_reply<not_in_params>(r, reply_arguments);
}

template <typename R, typename SeqParam, typename F, typename T, typename Args>
void handle_request_reply(F f, T* self, reply& r, Args& args, mpl::identity<R>)
{
  fusion::single_view<T*> self_view(self);
  fusion::joint_view<fusion::single_view<T*> const, Args> new_args(self_view, args);
  typename return_traits<R>::type result = f(new_args);

  typedef Args argument_types;

  typedef type_tag::value_type_tag<R, type_tag::out_tag> result_tag;
  typedef typename mpl::push_front<SeqParam, result_tag>::type sequence_params;

  typedef typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type is_not_in_lambda;
  typedef typename mpl::copy_if<sequence_params, is_not_in_lambda>::type not_in_params;

  typedef typename mpl::transform<sequence_params, create_argument_transform<mpl::_1> >::type mpl_argument_types;
  typedef typename mpl::transform<sequence_params, tag<mpl::_1> >::type mpl_identity_argument_types;
  typedef typename fusion::result_of::as_vector<mpl_identity_argument_types>::type identity_argument_types;
  identity_argument_types const identity_arguments;

  typedef fusion::cons<typename return_traits<R>::type, argument_types> args_with_result_type;

  typedef typename boost::remove_reference<
    typename fusion::result_of::fold<identity_argument_types const, std::pair<mpl::int_<0>, fusion::nil>
                                     , reply_arguments_generator<args_with_result_type> >::type
    >::type
    reply_argument_types;

  std::cout << "reply_argument_types " << typeid(reply_argument_types).name() << std::endl;

  args_with_result_type args_with_result(result, args);
  reply_argument_types reply_arguments = fusion::fold(identity_arguments, std::pair<mpl::int_<0>, fusion::nil>()
                                                      , reply_arguments_generator
                                                      <args_with_result_type>
                                                      (args_with_result));
  make_request_reply<not_in_params>(r, reply_arguments);
}

template <typename R, typename SeqParam, typename T, typename F>
void handle_request_body(T* self, F f, std::size_t align_offset
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r)
{
  std::cout << "handle_request_body " << typeid(f).name() << " align_offset " << align_offset << std::endl;
  typedef typename mpl::lambda<type_tag::is_not_out_type_tag<mpl::_1> >::type
    is_not_out_lambda;
  typedef typename mpl::copy_if<SeqParam, is_not_out_lambda>::type not_out_params;

  typedef typename mpl::transform
    <not_out_params, transforms::from_unmanaged_to_managed<mpl::_1> >::type
    mpl_parse_argument_types;

  typedef typename fusion::result_of::as_vector<mpl_parse_argument_types>::type
    parse_argument_types;

  parse_argument_types parse_arguments;

  morbid::arguments_traits arguments_traits;
  typedef giop::grammars::arguments<iiop::parser_domain
                                    , const char*, not_out_params
                                    , parse_argument_types>
    arguments_grammar;
  arguments_grammar arguments_grammar_(arguments_traits);

  namespace qi = boost::spirit::qi;
  if(qi::parse(rq_first, rq_last
               , giop::compile<iiop::parser_domain>
               (iiop::aligned(align_offset)[arguments_grammar_(giop::endian(little_endian))])
               , parse_arguments))
  {
    std::cout << "Parsed arguments correctly " << typeid(parse_arguments).name() << std::endl;
    std::cout << "Arguments parsed " << parse_arguments << std::endl;
    std::cout << "handle_request_body not_out_params " << typeid(parse_argument_types).name() << std::endl;

    typedef typename mpl::transform<SeqParam, create_argument_transform<mpl::_1> >::type mpl_argument_types;
    typedef typename mpl::transform<SeqParam, tag<mpl::_1> >::type mpl_identity_argument_types;

    typedef typename fusion::result_of::as_vector<mpl_identity_argument_types>::type identity_argument_types;
    identity_argument_types const identity_arguments;

    typedef typename boost::remove_reference<
      typename fusion::result_of::fold
      <identity_argument_types const
       , std::pair<mpl::int_<0>, fusion::nil>
       , initialize_arguments
       <parse_argument_types
        , type_tag::is_out_type_tag<mpl::_>
        , 
        mpl::if_
        <
          mpl::and_
          <type_tag::is_not_out_type_tag<mpl::_1>
           , mpl::not_<boost::is_same
                       <mpl::_1, type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
                       >
           >
          , boost::add_reference<type_tag::value_type<mpl::_1> >
          , boost::remove_reference<type_tag::value_type<mpl::_1> >
        >
       > >::type
      >::type
      argument_types;

    std::cout << "handle_request_body not_out_params " << typeid(identity_argument_types).name() << std::endl;
    std::cout << "handle_request_body not_out_params " << typeid(argument_types).name() << std::endl;

    std::pair<mpl::int_<0>, fusion::nil> const pair;
    argument_types arguments = fusion::fold
      (identity_arguments, pair, initialize_arguments
       <parse_argument_types
       // Default construct out types
       , type_tag::is_out_type_tag<mpl::_>
       // Add reference to not out types (to avoid copying)
       , 
        mpl::if_
        <
          mpl::and_
          <type_tag::is_not_out_type_tag<mpl::_1>
           , mpl::not_<boost::is_same
                       <mpl::_1, type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
                       >
           >
          , boost::add_reference<type_tag::value_type<mpl::_1> >
          , boost::remove_reference<type_tag::value_type<mpl::_1> >
        >
       >
       (parse_arguments));
    // Calling function with arguments
    fusion::fused<F> fused(f);
    // Create reply
    handle_request_reply<R, SeqParam>(fused, self, r, arguments.second
                                      , mpl::identity<R>());
  }
  else
  {
    throw morbid::MARSHALL();
  }
}

}

#endif
