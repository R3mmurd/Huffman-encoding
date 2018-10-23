/** This file contains definitions of data types to be used in the homework
    solution.

    @author Alejandro J. Mujica
*/

# ifndef DEFS_H
# define DEFS_H

/// Node to build the Huffman tree.
struct HuffmanNode 
{
  string        value;
  HuffmanNode * l_child;
  HuffmanNode * r_child;

  HuffmanNode(const string & s)
    : value(s), l_child(nullptr), r_child(nullptr)
  {
    // empty
  }
};

/// Info to be stored in the heap.
using HeapInfo = tuple<HuffmanNode *, unsigned long>;

/// Heap comparator. Operator > in order to set the less element to the top.
struct HeapCmp
{
  bool operator () (const HeapInfo & t1, const HeapInfo & t2) const
  {
    return get<1>(t1) > get<1>(t2);
  }
};

/// Type of heap.
using Heap = priority_queue<HeapInfo, vector<HeapInfo>, HeapCmp>;

/// Some random string to represent end of file
static const string eof = "..eof..";

/// Alias for an 8 bits type.
using byte     = uint8_t;

/// Vector to store bytes
using byte_set = vector<byte>;

/// Size (in bits) of a byte.
static constexpr uint8_t BYTE_SIZE = 8;

/// Stores a bits collection.
class BitSet
{
  byte_set      bs;
  unsigned long sz;

  static const uint8_t active_bits[BYTE_SIZE];

  static byte_set::size_type which_byte(unsigned long i)
  {
    return i / BYTE_SIZE;
  }

  static uint8_t which_bit_in_byte(unsigned long i)
  {
    return i % BYTE_SIZE;
  }

public:
  BitSet()
    : bs(), sz(0)
  {
    // empty
  }

  unsigned long size() const
  {
    return sz;
  }

  void add_bit(bool value)
  {
    if (sz % BYTE_SIZE == 0)
      bs.push_back(0);

    uint8_t & last_byte = bs.back();
    auto i = which_bit_in_byte(sz++);
    last_byte |= (value ? active_bits[i] : 0);
  }

  bool get_bit(unsigned long i) const
  {
    if (i >= sz)
      throw overflow_error("no valid position");

    auto p = which_byte(i);
    uint8_t b = bs[p];
    auto q = which_bit_in_byte(i);
    return (b & active_bits[q]) == active_bits[q];
  }

  void save(ofstream & out) const
  {
    out.write(reinterpret_cast<char *>(const_cast<unsigned long *>(&sz)),
	      sizeof(sz));

    for (uint8_t b : bs)
      out.write(reinterpret_cast<char *>(const_cast<uint8_t *>(&b)), sizeof(b));
  }

  void load(ifstream & in)
  {
    in.read(reinterpret_cast<char *>(&sz), sizeof(sz));

    byte_set::size_type vsz = sz / BYTE_SIZE;

    if (sz % BYTE_SIZE)
      ++vsz;

    for (byte_set::size_type i = 0; i < vsz; ++i)
      {
	uint8_t b;
	in.read(reinterpret_cast<char *>(&b), sizeof(b));
	bs.push_back(b);
      }
  }
};

const uint8_t BitSet::active_bits[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

ostream & operator << (ostream & out, const BitSet & bs)
{
  for (unsigned long i = 0; i < bs.size(); ++i)
    out << bs.get_bit(i);
  return out;
}

# endif // DEFS_H
