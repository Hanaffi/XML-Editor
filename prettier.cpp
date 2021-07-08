#include <bits/stdc++.h>
using namespace std;



class Node{
	public:
	string text;
	string name;
	vector<Node*> children;
	vector<pair<string,string>>attrs; // (name,value)
};


void show_all_node(Node *tmpnode, int presize=0)
{
	
	/******************Get the node name and node attributes*********************/
	cout<< setw(presize)<< "<"<<tmpnode->name << " ";
	for(auto attr:tmpnode->attrs)
	{
		cout<< attr.first << "=\""<< attr.second << "\"" << " ";
	}
	
	cout<<">"<<endl;

	/******************Get node information Text*********************/
	if(tmpnode->text.size())//Be sure to judge otherwise there will be problems. If the text is empty, it will print text= and terminate the process directly
		cout<< setw(presize+4) << tmpnode->text<<" "<<endl;
	for(auto x:tmpnode->children)
	{
		show_all_node(x, presize+4);//Recursively use, print all attributes and text information of child nodes
	}
	
	/******************Node closure information and transfer node *********************/
	cout<< setw(presize) <<"</" <<tmpnode->name<< ">" << endl;

}
int main() {
	
	
	// TEST 
	
	
	
	Node* root = new Node;
	root->name = "Company";
	root->text = "Microsoft";
	root->attrs.push_back(make_pair("headquarters" , "USA"));
	root->attrs.push_back(make_pair("business" , "Software"));
	
	
	Node* c1 = new Node;
	c1->name = "date";
	Node* cc1 = new Node;
	cc1->name = "start";
	cc1->text = "1975";
	Node* cc2 = new Node;
	cc2->name = "end";
	cc2->text = "2028";
	
	Node* c2 = new Node;
	c2->name = "owner";
	c2->text = "Bill Gates";

	root->children.push_back(c1);
	root->children.push_back(c2);
	c1->children.push_back(cc1);
	c1->children.push_back(cc2);
	
	
	
	
	show_all_node(root);
	
	
	return 0;
}