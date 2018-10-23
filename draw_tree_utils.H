/** This file contains a function to prepare a file with a Tree data do be 
    drawn by the program btreepic.
    
    @author Alejandro J. Mujica
*/

# ifndef DRAW_TREE_UTILS_H
# define DRAW_TREE_UTILS_H

void draw_Huffman_tree(HuffmanNode * r, ostream & output)
{
  map<HuffmanNode *, unsigned long> enumeration;
  unsigned long i = 0;

 infix_traverse(r, [&enumeration, &i] (HuffmanNode * p)
		{
		  enumeration[p] = i++;
		});
 
 output << "START-PREFIX ";

  prefix_traverse(r, [&enumeration, &output](HuffmanNode * p)
		  {
		    output << enumeration[p] << " ";
		  });
  
  output << "\nSTART-INFIX ";
  
  infix_traverse(r, [&enumeration, &output](HuffmanNode * p)
		 {
		   output << enumeration[p] << " ";
		 });
  
  output << "\nSTART-KEY ";

  infix_traverse(r, [&output](HuffmanNode * p)
		 {
		   if(is_leaf(p))
		     output << "\"" << K(p) << "\" ";
		   else
		     output << "\"-\" ";
	       });
}

# endif // DRAW_TREE_UTILS_H
