/** This file contains a little interactive program to encode a text file
    or decode a encoded file.

    exec ./compressor --help in order to get help.

    @author Alejandro J. Mujica
*/

# include <iostream>
# include <fstream>
# include <string>
# include <tuple>
# include <map>
# include <queue>

using namespace std;

# include <tclap/CmdLine.h>
# include <tclap/SwitchArg.h>

using namespace TCLAP;

# include <defs.H>
# include <helpers.H>
# include <draw_tree_utils.H>
# include <Huffman.H>

int main(int argc, char * argv[])
{
  CmdLine cmd("compressor", ' ', "0.0");

  ValueArg<string> action("a", "action", "action to execute (encode or decode)",
			  true, "encode", "action to execute");

  cmd.add(action);

  ValueArg<string> input("i", "input", "name of the input file", true,
			 "", "input file");

  cmd.add(input);
  
  ValueArg<string> output("o", "output", "name of the output file", true, "",
			  "output file");

  cmd.add(output);

  ValueArg<string> function("f", "function",
			    "name of the function to split the text",
			    false, "split_by_char",
			    "function to split text");

  cmd.add(function);

  SwitchArg draw_tree("d", "draw-tree", "draw tree");

  cmd.add(draw_tree);

  cmd.parse(argc, argv);

  ifstream in(input.getValue());

  if (not in)
    {
      cout << "file " << input.getValue() << " does no exist\n";
      return 1;
    }
  
  ofstream out(output.getValue());

  tuple<BitSet, HuffmanNode *> t;
  
  if (action.getValue() == "encode")
    {
      vector<string>(*fct)(const string &);
      
      if (function.getValue() == "split_by_char")
	fct = split_by_char;
      else if (function.getValue() == "split_two_by_two")
	fct = split_two_by_two;
      else if (function.getValue() == "split_three_by_three")
	fct = split_three_by_three;
      else
	{
	  cout << "Function " << function.getValue() << " is not valid\n";
	  in.close();
	  out.close();
	  return 2;
	}
      
      string text = load_text(in);
      t = encode(text, fct);
      save_encoded_text(out, get<0>(t), get<1>(t));
    }
  else if (action.getValue() == "decode")
    {
      t = load_enconded_text(in);
      string text = decode(get<0>(t), get<1>(t));
      save_text(out, text);
    }
  else
    {
      cout << "Action " << action.getValue() << " is not valid\n";
      in.close();
      out.close();
      return 3;
    }

  in.close();
  out.close();

  if (draw_tree.getValue())
    {
      out.open("optimal_tree.tree");
      draw_Huffman_tree(get<1>(t), out);
      out.close();
    }
  
  destroy_tree(get<1>(t));
  
  return 0;
}
