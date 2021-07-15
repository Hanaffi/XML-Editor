#include "tree_compression_class.h"
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

	using namespace std;

	node_comp::node_comp()
	{
		left = NULL;
		right = NULL;
		data = 0;
		frequency = 0;
	}
	node_comp::node_comp(unsigned char d,unsigned int f,node_comp *l,node_comp *r)
	{
		left = l;
		right = r;
		data = d;
		frequency = f;
	}
	void node_comp::set_left(node_comp * n)
	{
		left = n;
	}
	void node_comp::set_right(node_comp * n)
	{
		right = n;
	}
	void node_comp::set_data(unsigned char d)
	{
		data = d;
	}
	void node_comp::set_frequency(unsigned int f)
	{
		frequency = f;
	}
	unsigned char node_comp::get_data()const
	{
		return data;
	}
	unsigned int node_comp::get_frquency() const
	{
		return frequency;
	}
	node_comp * node_comp::get_left() const
	{
		return left;
	}
	node_comp * node_comp::get_right() const
	{
		return right;
	}
	void node_comp::add_children(node_comp *r,node_comp  *l)
	{
		right = r;
		left = l;
	}
	bool node_comp::is_leaf()const {
		return (left== NULL && right == NULL) ? 1 : 0;
	}
	//copying node to another
	node_comp::node_comp(const node_comp &m)
	{
		data = m.data;
		frequency = m.frequency;
		add_children(m.get_right(), m.get_left());
	}
	void node_comp::kill_children()
	{
		if(!is_leaf())
		{
			left->kill_children();
			right->kill_children();
		}
		delete left;
		delete right;
		left = NULL;
		right = NULL;
	}



	tree::tree()
	{
		root = NULL;
	}
	tree::tree(node_comp *r)
	{
		root = r;
	}
	tree::node_comp *get_root()
	{
		return root;
	}
	tree::~tree()
	{
		root->get_left()->kill_children();
		root->get_right()->kill_children();
		delete root;
	}


// implementing functions for dealing with min heap to store our tree in array with min freq node at location 0

//Heapify function
void heapify(vector<node_comp*> &V, int i, int length)
{
	int smallest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if (left <= length && V[left]->frequency < V[smallest]->frequency)
		smallest = left;
	if (right<= length && V[right]->frequency < V[smallest]->frequency)
		smallest = right;
	if (smallest != i)
	{
		swap(V[i], V[smallest]);
		heapify(V, smallest, length);
	}
}
//getting the min. element in our heap then heapify again
node_comp* get_min_heap(vector<node_comp*> &v)
{
	if (v.size() >= 1) {
		node_comp* minimum = v[0];
		v[0] = v.back();
		v.pop_back();
		heapify(v, 0, v.size() - 1);
		return minimum;
	}
	return NULL;
}
//inserting element in our heap 
void Insert_Heap(vector<node_comp*> &v, node_comp* e)
{
	v.push_back(e);
	int i = v.size() - 1;
	int parent = (i - 1) / 2;
	while (i > 0 && v[parent]->frequency > v[i]->frequency)
	{
		swap(v[i], v[parent]);
		i = parent;
	}
}
void build_min_heap(vector<node_comp*> &v, int length)
{
	for (int i = (length - 1) / 2; i >= 0; i--)
	{
		heapify(v, i, length);
	}
}