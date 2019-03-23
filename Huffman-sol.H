/**
   @author Alejandro Mujica
*/

string filter_string(const string & s)
{
  string r;

  for (auto c : s)
    {
      if (c == ' ')
	r.push_back('~');
      else if (c == '\n')
	r.push_back('|');
      else
	r.push_back(c);
    }
  
  return r;
}

string unfilter_string(const string & s)
{
  string r;

  for (auto c : s)
    {
      if (c == '~')
	r.push_back(' ');
      else if (c == '|')
	r.push_back('\n');
      else
	r.push_back(c);
    }
  
  return r;
}


vector<string> split_n(const string & s, unsigned long n)
{
  vector<string> result;
  unsigned long start = 0;

  while (start < s.size())
    {
      result.push_back(s.substr(start, n));
      start += n;
    }

  return result;
}

vector<string> split_by_char(const string & s)
{
  return split_n(s, 1);
}

vector<string> split_two_by_two(const string & s)
{
  return split_n(s, 2);
}

vector<string> split_three_by_three(const string & s)
{
  return split_n(s, 3);
}

map<string, unsigned long> build_freq_table(const vector<string> & ss)
{
  map<string, unsigned long> result;
  
  for (const string & s : ss)
    {
      if (result.find(s) == result.end())
	result[s] = 1;
      else
	++result[s];
    }

  return result;
}

Heap init_heap(const map<string, unsigned long> & table)
{
  Heap heap;
  
  for (const auto & p : table)
    heap.push(make_tuple(new HuffmanNode(p.first), p.second));

  return heap;
}

HuffmanNode * build_optimal_tree(Heap & heap)
{
  while (heap.size() > 1)
    {
      HeapInfo l_info = heap.top();
      heap.pop();
      HeapInfo r_info = heap.top();
      heap.pop();

      HuffmanNode * new_node = new HuffmanNode("");
      L(new_node) = get<0>(l_info);
      R(new_node) = get<0>(r_info);
      heap.push(make_tuple(new_node, get<1>(l_info) + get<1>(r_info)));
    }

  return get<0>(heap.top());
}

void build_path_table(HuffmanNode * p, const string & s,
		      map<string, string> & table)
{
  if (p == nullptr)
    return;

  if (is_leaf(p))
    {
      table[K(p)] = s;
      return;
    }

  build_path_table(L(p), s + "0", table);
  build_path_table(R(p), s + "1", table);
}

map<string, string> build_path_table(HuffmanNode * p)
{
  map<string, string> result;
  build_path_table(p, "", result);
  return result;
}

bool to_bit(char c)
{
  return c == '1';
}

tuple<BitSet, HuffmanNode *>
encode(const string & text, vector<string> (*split_fct)(const string &))
{
  auto split_text = split_fct(filter_string(text));
  split_text.push_back(eof);
  auto freq_table = build_freq_table(split_text);
  auto heap = init_heap(freq_table);
  HuffmanNode * root = build_optimal_tree(heap);
  auto path_table = build_path_table(root);

  BitSet bits;

  for (const string & s : split_text)
    {
      const string & cod = path_table[s];

      for (auto c : cod)
	bits.add_bit(to_bit(c));
    }
  
  return make_tuple(bits, root);
}

string decode(const BitSet & bits, HuffmanNode * root)
{
  string text;

  HuffmanNode * it = root;

  for (unsigned long i = 0; i < bits.size(); )
    {
      if (is_leaf(it))
	{
	  if (K(it) == eof)
	    break;
	  
	  text += K(it);
	  it = root;
	  continue;
	}
      
      bool b = bits.get_bit(i++);
      it = (b ? R(it) : L(it));
    }
  
  return unfilter_string(text);
}
