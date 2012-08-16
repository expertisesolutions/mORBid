/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/poa.hpp>

namespace tecorb { namespace poa {

POAManager_ptr POA::the_POAManager()
{
  return POAManager_ptr();
}

} }