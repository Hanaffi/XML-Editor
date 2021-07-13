#include <bits/stdc++.h>
#include <fstream>

using namespace std;

class Node{
	public:
	string text;
	string name;
	vector<Node*> children;
	vector<pair<string,string>>attrs; // (name,value)
};

void xml2json(Node *tmpnode, int presize)
{
    cout << "\n" << setw(presize) << "\"" << tmpnode->name << "\": ";
	
	if(tmpnode->attrs.size() || tmpnode->text.size())
	{
		if(tmpnode->attrs.size())
		{
			cout << "{";
			for(auto attr:tmpnode->attrs)
			{
			cout << "\n" << setw(presize+4) << "\"@" << attr.first << "\": \"" << attr.second << "\",";
			}
		}

		if(tmpnode->text.size() && tmpnode->attrs.size())
		{
			cout << "\n" << setw(presize+11) << "\"#text\": " << tmpnode->text << "\"\n";
			if(!(tmpnode->children.size()))
			cout << "},";
		}

		else if(tmpnode->text.size())
		{
			cout << "\"" << tmpnode->text << "\"";
		}

		for(auto x:tmpnode->children)
		{
			xml2json(x, presize+4);
		}
	}

	else cout << "null,";
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
    c2->text = "able";

    Node* c3 = new Node;
    c3->name = "pointer";
	c3->attrs.push_back(make_pair("refs", "n"));
    c3->text = "Attribute";

	Node* c4 = new Node;
	c4->name = "pointer";
	c4->attrs.push_back(make_pair("source", "1"));
	c4->attrs.push_back(make_pair("target", "1"));
	c4->text = "form";

	root->children.push_back(c0);
	root->children.push_back(c1);
	root->children.push_back(c2);
	c2->children.push_back(c3);
	c2->children.push_back(c4);

	cout << endl;
	xml2json(root,0);
	cout << endl;

    return 0;

}