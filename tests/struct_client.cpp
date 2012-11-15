/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "struct.h"

#include <fstream>

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  CORBA::Object_var obj = orb->string_to_object (ior.c_str());
  struct_interface_var struct_interface_ = struct_interface::_narrow (obj);
  
  some_struct s;
  assert(!CORBA::is_nil(struct_interface_));
  struct_interface_->foo1(s);
  struct_interface_->foo2(s);
  struct_interface_->foo3(s);
  some_struct r = struct_interface_->foo4();

  std::cout << "Finished" << std::endl;
}
