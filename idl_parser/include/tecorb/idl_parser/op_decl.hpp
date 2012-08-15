#ifndef TECORB_IDL_PARSER_OP_DECL_HPP
#define TECORB_IDL_PARSER_OP_DECL_HPP

namespace tecorb { namespace idl_parser {

template <typename Iterator>
struct op_decl
{
  typedef typename Iterator::base_iterator_type base_iterator;

  op_decl() {}
  op_decl(boost::iterator_range<base_iterator> type
          , boost::iterator_range<base_iterator> name)
    : type(type), name(name) {}

  boost::iterator_range<base_iterator> type, name;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, op_decl<Iterator> op)
{
  return os << "[op_decl type: " << op.type << " name: " << op.name << "]";
}

} }

#endif