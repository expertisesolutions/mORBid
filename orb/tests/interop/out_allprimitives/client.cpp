/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef USING_TAO
#include "out_allprimitives.h"
#include <CORBA.h>
#else
#include "out_allprimitivesC.h"
#include <tao/corba.h>
#endif

#include <iostream>
#include <fstream>
#include <cassert>

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
  out_allprimitives_var out_allprimitives_ = out_allprimitives::_narrow (obj);
  
  assert(!CORBA::is_nil(out_allprimitives_));
  CORBA::Boolean b = false;
  out_allprimitives_->foo1(b);
  assert(b == true);
  CORBA::Char c = 'a';
  out_allprimitives_->foo2(c);
  assert(c == 'c');
  CORBA::Double d = 1.0;
  out_allprimitives_->foo3(d);
  assert(d == 2.0);
  CORBA::Float f = 1.0f;
  out_allprimitives_->foo4(f);
  assert(f == 2.0f);
  CORBA::Long l = 1;
  out_allprimitives_->foo5(l);
  assert(l == 2);
  CORBA::Octet octet = 'a';
  out_allprimitives_->foo6(octet);
  assert(octet == 'c');
  CORBA::Short s = 1; 
  out_allprimitives_->foo7(s);
  assert(s == 2);
  // CORBA::String_var str = "qwe";
  // out_allprimitives_->foo8(str);
  // assert(!std::strcmp(str, "abc"));
  // allprimitives_->foo9(L'q');
  // allprimitives_->foo10(L"qwe");
  // CORBA::Any_ptr any(new CORBA::Any);
  // allprimitives_->foo11(any);

  std::cout << "Finished" << std::endl;
}
