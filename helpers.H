/** This file contains some functions to help you to build your homework
    solution.

    @author Alejandro J. Mujica
*/

# ifndef HELPERS_H
# define HELPERS_H

/// Gets a reference to the string stored in a Huffman node.
string & K(HuffmanNode * p)
{
  return p->value;
}

/// Gets a reference to the l_child pointer stored in a Huffman node.
HuffmanNode *& L(HuffmanNode *& p)
{
  return p->l_child;
}

/// Gets a reference to the r_child pointer stored in a Huffman node.
HuffmanNode *& R(HuffmanNode *& p)
{
  return p->r_child;
}


/// Free the allocated memory by a Huffman tree.
void destroy_tree(HuffmanNode *& r)
{
  if (r == nullptr)
    return;

  destroy_tree(L(r));
  destroy_tree(R(r));
  delete r;
  r = nullptr;
}

/// Checks whether a HuffmanNode is a leaf.
bool is_leaf(HuffmanNode * p)
{
  return L(p) == nullptr and R(p) == nullptr;
}

/** A recursive algorithm to traverse a Huffman tree in prefix by executing
    the op function on each node.
*/
template <class Op>
void prefix_traverse(HuffmanNode * r, Op & op)
{
  if (r == nullptr)
    return;

  op(r);
  prefix_traverse(L(r), op);
  prefix_traverse(R(r), op);
}

/// Wrapper to prefix traverse.
template <class Op>
void prefix_traverse(HuffmanNode * r, Op && op = Op())
{
  return prefix_traverse<Op>(r, op);
}

/** A recursive algorithm to traverse a Huffman tree in infix by executing
    the op function on each node.
*/
template <class Op>
void infix_traverse(HuffmanNode * r, Op & op)
{
  if (r == nullptr)
    return;

  infix_traverse(L(r), op);
  op(r);
  infix_traverse(R(r), op);
}

/// Wrapper to infix traverse.
template <class Op>
void infix_traverse(HuffmanNode * r, Op && op = Op())
{
  return infix_traverse<Op>(r, op);
}

/** A recursive algorithm to traverse a Huffman tree in sufix by executing
    the op function on each node.
*/
template <class Op>
void sufix_traverse(HuffmanNode * r, Op & op)
{
  if (r == nullptr)
    return;

  sufix_traverse(L(r), op);
  sufix_traverse(R(r), op);
  op(r);
}

/// Wrapper to sufix traverse.
template <class Op>
void sufix_traverse(HuffmanNode * r, Op && op = Op())
{
  return sufix_traverse<Op>(r, op);
}

/// A recursive algorithms to build the code of a Huffman tree in a BitSet.
void tree_to_bits(HuffmanNode * r, BitSet & bs)
{
  if (r == nullptr)
    {
      bs.add_bit(0);
      return;
    }

  bs.add_bit(1);
  tree_to_bits(L(r), bs);
  tree_to_bits(R(r), bs);
}

/// Wrapper to tree_to_bits.
BitSet tree_to_bits(HuffmanNode * r)
{
  BitSet bs;
  tree_to_bits(r, bs);
  return bs;
}

/// A recursive algorithm to build a Huffman tree from its binary code.
HuffmanNode * bits_to_tree(const BitSet & bs, unsigned long & i)
{
  if (bs.get_bit(i++) == 0)
    return nullptr;

  HuffmanNode * r = new HuffmanNode("");

  L(r) = bits_to_tree(bs, i);
  R(r) = bits_to_tree(bs, i);

  return r;
}

/// Wrapper to bits_to_tree.
HuffmanNode * bits_to_tree(const BitSet & bs)
{
  unsigned long i = 0;
  return bits_to_tree(bs, i);
}

/// Loads a text from a file and stores it in a string.
string load_text(istream & in)
{
  string ret_val;
  char c;

  while (in.get(c))
    ret_val.push_back(c);

  return ret_val;
}

/// Writes a text in a file.
void save_text(ostream & out, const string & text)
{
  out << text;
}

/// Saves the encoded text in a binary format file.
void save_encoded_text(ofstream & out, const BitSet & bs, HuffmanNode * r)
{
  BitSet bst = tree_to_bits(r);
  bst.save(out);
  prefix_traverse(r, [&out] (HuffmanNode * p)
		  {
		    if (not is_leaf(p))
		      return;
		    
		    uint8_t sz = K(p).size() + 1;
		    out.write(reinterpret_cast<char *>(&sz), sizeof(sz));
		    out.write(K(p).c_str(), sz);
		  });
  bs.save(out);
}

/// Loads the encoded text from a binary format file.
tuple<BitSet, HuffmanNode *> load_enconded_text(ifstream & in)
{
  BitSet bst;
  bst.load(in);
  
  HuffmanNode * r = bits_to_tree(bst);

  prefix_traverse(r, [&in] (HuffmanNode * p)
		  {
		    if (not is_leaf(p))
		      {
			K(p) = "";
			return;
		      }
		    
		    uint8_t sz;
		    in.read(reinterpret_cast<char *>(&sz), sizeof(sz));
		    char s[sz];
		    in.read(s, sz);
		    K(p) = s;
		  });
  
  BitSet bs;
  bs.load(in);

  return make_tuple(bs, r);
}


# endif // HELPERS_H

