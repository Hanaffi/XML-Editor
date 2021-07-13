#ifndef _TREE_COMPRESSION_CLASS_H
#define _TREE_COMPRESSION_CLASS_H
#include <iostream>

class node_comp
{
private:
	unsigned char data;
	unsigned frequency;
	node_comp *left;
	node_comp *right;
public:
	node_comp();
	node_comp(unsigned char d, unsigned f, node_comp *l, node_comp *r);
	void set_left(node_comp * n);
	void set_right(node_comp * n);
	void set_data(unsigned char d);
	void set_frequency(unsigned f);
	unsigned char get_data()const;
	unsigned get_frquency() const;
	node_comp * get_left() const;
	node_comp * get_right() const;
	void add_children(node_comp *r, node_comp  *l);
	bool is_leaf()const;
	node_comp(const node_comp &m);
	void kill_children();
	friend class tree;
};
class tree
{
private:
	node_comp * root;
public:
	tree();
	tree(node_comp *r);
	node_comp *get_root();
	~tree();
};


#endif