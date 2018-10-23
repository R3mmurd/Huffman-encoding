/**
   Write all the expected info. The curly braces are to represent variables.
   You should remove them when writing your values.
   
   @id {your document id number} 
   @author {your name and your family name}
   @email {your email}
*/

string filter_string(const string &)
{
  return "";
}

string unfilter_string(const string &)
{
  return "";
}


vector<string> split_by_char(const string &)
{
  return vector<string>();
}

vector<string> split_two_by_two(const string &)
{
  return vector<string>();
}

vector<string> split_three_by_three(const string &)
{
  return vector<string>();
}

map<string, unsigned long> build_freq_table(const vector<string> &)
{
  return map<string, unsigned long>();
}

Heap init_heap(const map<string, unsigned long> &)
{
  return Heap();
}

HuffmanNode * build_optimal_tree(Heap &)
{
  return nullptr;
}

map<string, string> build_path_table(HuffmanNode *)
{
  return map<string, string>();
}

tuple<BitSet, HuffmanNode *>
encode(const string &, vector<string> (*split_fct)(const string &))
{
  return make_tuple(BitSet(), nullptr);
}

string decode(const BitSet &, HuffmanNode *)
{
  return "";
}
