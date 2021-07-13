#include "tree_compression_class"
#include<iostream>
#include<string>


	node_comp::node_comp()
	{
		left = NULL;
		right = NULL;
		data = 0;
		frequency = 0;
	}
	node_comp::node_comp(unsigned char d,unsigned f,node_comp *l,node_comp *r)
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
	void node_comp::set_frequency(unsigned f)
	{
		frequency = f;
	}
	unsigned char node_comp::get_data()const
	{
		return data;
	}
	unsigned node_comp::get_frquency() const
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
