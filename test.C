/** This file contains some examples of how to test your functions.
   
    Write add more test here in order to ensure the correctness of your
    solutions.

    @author Alejandro J. Mujica
*/

# include <cassert>
# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <tuple>
# include <map>
# include <queue>

using namespace std;

# include <defs.H>
# include <helpers.H>
# include <Huffman.H>

using ss_t = vector<string>;
using ft_t = map<string, unsigned long>;
using pt_t = map<string, string>;

/// Converts a string vector in a string.
string to_str(const ss_t & ss)
{
  stringstream out;
  
  out << "[ ";

  for (const string & s : ss)
    out << s << " ";

  out << "]";

  return out.str();
}

/// Converts a map with key K and value V in a string.
template <typename K, typename V>
string to_str(const map<K, V> & t)
{
  stringstream out;
  
  out << "[ ";

  for (const auto & p : t)
    out << '(' << p.first << ", " << p.second << ')';

  out << " ]";

  return out.str();
}

/// Checks for equality between two stl containers
template <class Container, class Eq = equal_to<typename Container::value_type>>
bool eq(const Container & t1, const Container & t2, const Eq & cmp = Eq())
{
  if (t1.size() != t2.size())
    return false;
  
  auto i = t1.begin();
  auto j = t2.begin();

  for ( ; i != t1.end(); ++i, ++j)
    if (not cmp(*i, *j))
      return false;
  
  return true;
}

int main()
{
  string text = "Hello!\nAlgorithms world.";
  
  string filtered_text = filter_string(text);
  assert(filtered_text == "Hello!|Algorithms~world.");
  
  string unfiltered_text = unfilter_string(filtered_text);
  assert(unfiltered_text == text);
  
  ss_t split1 = split_by_char(filtered_text);
  assert(eq(split1,
	    ss_t({"H","e","l","l","o","!","|","A","l","g","o","r","i","t","h",
		  "m","s","~","w","o","r","l","d","."}
	      )
	    )
	 );
  
  vector<string> split2  = split_two_by_two(filtered_text);
  assert(eq(split2,
	    ss_t({"He","ll","o!","|A","lg","or", "it","hm","s~","wo","rl","d."}
	      )
	    )
	 );
  
  vector<string> split3  = split_three_by_three(filtered_text);
  assert(eq(split3, ss_t({"Hel","lo!","|Al","gor","ith","ms~","wor","ld."})));

  ft_t table1 = build_freq_table(split1);
  assert(eq(table1,
	    ft_t({{"!", 1},{".", 1},{"A", 1},{"H", 1},{"d", 1},{"e", 1},{"g", 1},
   	          {"h", 1},{"i", 1},{"l", 4},{"m", 1},{"o", 3},{"r", 2},{"s", 1},
		  {"t", 1},{"w", 1},{"|", 1},{"~", 1}}
	      )
	    )
	 );
  
  ft_t table2 = build_freq_table(split2);
  assert(eq(table2,
	    ft_t({{"He", 1},{"d.", 1},{"hm", 1},{"it", 1},{"lg", 1},{"ll", 1},
	          {"o!", 1},{"or", 1},{"rl", 1},{"s~", 1},{"wo", 1},{"|A", 1}}
	      )
	    )
	 );
  
  ft_t table3 = build_freq_table(split3);
  assert(eq(table3,
	    ft_t({{"Hel", 1},{"gor", 1},{"ith", 1},{"ld.", 1},{"lo!", 1},
                  {"ms~", 1},{"wor", 1},{"|Al", 1}}
	      )
	    )
	 );
  
  
  auto encoded_result = encode(text, split_by_char);
  ofstream out("encoded_text.tmp");
  save_encoded_text(out, get<0>(encoded_result), get<1>(encoded_result));
  out.close();
  destroy_tree(get<1>(encoded_result));

  ifstream in("encoded_text.tmp");
  auto encoded_read = load_enconded_text(in);
  auto decoded = decode(get<0>(encoded_read), get<1>(encoded_read));
  in.close();
  remove("encoded_text.tmp");
  destroy_tree(get<1>(encoded_read));
  
  assert(decoded == text);

  cout << "Everything ok!\n";

  return 0;
}
