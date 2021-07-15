#include"tree_compression_class.h"
#include<algorithm>
//generating codes for each leaf
void generating_huff_code(node_comp* Root, char leftORright[], int index, vector<long long int> &Huffman_codemap)
{

	if (Root->get_left())
	{
		leftORright[index] = '0';
		generating_huff_code(Root->get_left(), leftORright, index + 1, Huffman_codemap);

	}
	if (Root->get_right())
	{
		leftORright[index] = '1';
		generating_huff_code(Root->get_right(), leftORright, index + 1, Huffman_codemap);
	}
	if (!Root->get_left()&& !Root->get_right())
	{
		for (int i = index; i >= 0; i--)
		{
			if (i != index)
			{
				Huffman_codemap[Root->get_data()] *= 10;
				Huffman_codemap[Root->get_data()] += leftORright[i] - '0';
			}
			else
				Huffman_codemap[Root->get_data()] = 1;
		}
	}
}
//Write tree to file
//each leaf data has 1 behind
void store_tree(ofstream &input, node_comp* Root)
{
	if (!Root->get_left() && !Root->get_right())
	{
		input << '1';
		input << Root->get_data();
	}
	else
	{
		input << '0';
		store_tree(input, Root->get_left());
		store_tree(input, Root->get_right());
	}
}
//buidling the huffman tree
node_comp* Huffman(long long int freq[])
{
	vector<node_comp*>minheap;
	for (int i = 0; i < MAX; i++)
		if (freq[i] != 0)
			minheap.push_back(new node_comp(i, freq[i]));
	build_min_heap(minheap, minheap.size() - 1);
	while (minheap.size() != 1)
	{
		node_comp* first_minheap_e = get_min_heap(minheap);
		node_comp* second_minheap_e = get_min_heap(minheap);

		node_comp* parent = new node_comp(-1, 0, first_minheap_e, second_minheap_e);
		parent->get_frequency() = parent->get_left()->get_frequency() + parent->get_right()->get_frequency();
		Insert_Heap(minheap, parent);
	}
	return(minheap[0]);
}
//save the compressed data in a new file
void Write_compressed(ifstream &input, ofstream &output, vector<long long int > &Huffman_codemap)    
{
	char ch;
	unsigned char bits_8=0;
	long long int counter = 0;
	while (input.get(ch))
	{
		long long int temp = Huffman_codemap[static_cast<unsigned char>(ch)];
		while (temp != 1)
		{
			bits_8 <<= 1;
			if ((temp % 10) != 0)
				bits_8 |= 1;
			temp /= 10;
			counter++;
			if (counter == 8)
			{
				output << bits_8;
				counter = bits_8 = 0;
			}
		}
	}
	while (counter != 8)
	{
		bits_8 <<= 1;
		counter++;
	}
	output << bits_8;
	output.close();
}
