#include <bits/stdc++.h>
#include <stack>
#include <fstream>

using namespace std;

class Node{
	public:
	string text;
	string name;
	vector<Node*> children;
	vector<pair<string,string>>attrs; // (name,value)
	int level;
};

void xml2json(Node *tmpnode, int presize)
{
	cout << "\n" << setw(presize) << "\"" << tmpnode->name << "\": ";

	// no attr nor text
	if(!(tmpnode->text.size()) && !(tmpnode->attrs.size()))
	cout << "null,";

	// attr exist
	else if(tmpnode->attrs.size())
	{
		cout << "{";
		for(auto attr:tmpnode->attrs)
		{
		cout << "\n" << setw(presize+4) << "\"@" << attr.first << "\": \"" << attr.second << "\",";
		}

		// text exist along with attr
		if(tmpnode->text.size())
		{
			cout << "\n" << setw(presize+11) << "\"#text\": " << tmpnode->text << "\"\n";
		}
	}

	// text only
	else if(tmpnode->text.size())
		cout << "\"" << tmpnode->text << "\"";

	for(auto x:tmpnode->children)
		{
			xml2json(x, presize+3);
		}
	
};

int main ()
{
    // test for xml2json
	// root node with attrs only (2 attrs)
	Node* root = new Node;
	root->name = "synset";
	root->attrs.push_back(make_pair("id", "0000"));
	root->attrs.push_back(make_pair("type", "a"));
	
	//empty element
	Node* c0 = new Node;
	c0->name = "e";

	//child node with text only
	Node* c1 = new Node;
	c1->name = "lex_filenum";
	c1->text = "00";

	//child node with both attrs and text
	//and 2 children
    Node* c2 = new Node;
    c2->name = "word";
	c2->attrs.push_back(make_pair("lex_id", "0"));

    Node* c3 = new Node;
    c3->name = "pointer";
	c3->attrs.push_back(make_pair("refs", "n"));
    c3->text = "Attribute";

	Node* c4 = new Node;
	c4->name = "pointer";
	c4->attrs.push_back(make_pair("refs", "m"));

	Node* c5 = new Node;
	c5->name = "pointer";
	c5->text = "form";

	root->children.push_back(c0);
	root->children.push_back(c1);
	root->children.push_back(c2);
	c2->children.push_back(c3);
	c2->children.push_back(c4);
	c2->children.push_back(c5);

	cout << endl;
	xml2json(root,0);
	cout << endl;

    return 0;

}