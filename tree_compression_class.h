#ifndef _TREE_COMPRESSION_CLASS_H
#define _TREE_COMPRESSION_CLASS_H
#include <iostream>


class node_comp
{
private:
	unsigned char data;
	unsigned int frequency;
	node_comp *left;
	node_comp *right;
public:
	node_comp();
	node_comp(unsigned char d, unsigned int f, node_comp *l, node_comp *r);
	void set_left(node_comp * n);
	void set_right(node_comp * n);
	void set_data(unsigned char d);
	void set_frequency(unsigned int f);
	unsigned char get_data()const;
	unsigned int get_frquency() const;
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
void heapify(vector<node_comp*> &V, int i, int length);
node_comp* get_min_heap(vector<node_comp*> &v);
void Insert_Heap(vector<node_comp*> &v, node_comp* e);
void build_min_heap(vector<node_comp*> &v, int length);

#endif