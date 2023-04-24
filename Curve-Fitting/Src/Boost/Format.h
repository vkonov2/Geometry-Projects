#ifndef FORMAT_H_
#define FORMAT_H_

#include <iostream>
#include <iomanip>

#include "boost/format.hpp"

// 2 custom namespaces, to bring in a few useful names :
namespace MyNS_ForOutput 
{
  using std::cout; using std::cerr;
  using std::string;
  using std::endl; using std::flush;

  using boost::format;
  using boost::io::group;
}
namespace MyNS_Manips {
  using std::setfill;
  using std::setw;
  using std::hex;
  using std::dec;
  using std::showbase;
  using std::left;
  using std::right;
  using std::internal;
}

#endif

