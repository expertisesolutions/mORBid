/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_SERIALIZE_OBJECT_HPP
#define MORBID_SERIALIZE_OBJECT_HPP

#include <morbid/type_tag.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/reference_wrapper.hpp>
#include <morbid/iiop/serialize_object.hpp>
#include <morbid/iiop/serialize_reference.hpp>

#include <boost/utility/enable_if.hpp>

namespace morbid { namespace serialization {

template <typename T, typename Enabler = void>
struct serialize_object_aux
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, T a)
  {
    std::cout << "serialize primitive object " << typeid(a).name() << std::endl;
    iiop::serialize_object(iterator, true, a);
  }
};

template <typename T>
struct serialize_object_aux<T, typename boost::enable_if
                            <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                             , void>::type>
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, T a)
  {
    iiop::serialize_object<typename T::template _morbid_generator<OutputIterator> >
      (iterator, true, a);
  }
};

template <typename T>
struct serialize_object_aux<reference_wrapper<T> >
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, reference_wrapper<T> r)
  {
    iiop::serialize_reference(iterator, r.get_ptr()->_structured_ior());
  }
};

template <typename T>
struct serialize_object_aux<boost::shared_ptr<T> >
{
  template <typename OutputIterator>
  static void call(OutputIterator& iterator, boost::shared_ptr<T> r)
  {
    std::cout << "serializing " << typeid(r).name() << std::endl;
    iiop::serialize_reference(iterator, r->_structured_ior());
  }
};

template <typename OutputIterator, typename T>
void serialize_object(OutputIterator& iterator, T a)
{
  std::cout << "serialize_object " << typeid(a).name() << std::endl;
  serialize_object_aux<T>::call(iterator, a);
}

} }

#endif
