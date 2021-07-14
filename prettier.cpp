#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    string text;//tagValue
    string name;//tagName
    vector<Node*> children;
    vector<pair<string,string>>attrs; // (name,value)
    int level;
};




string take_file(){
    freopen("C:\\Users\\DELL\\CLionProjects\\testingggggg\\test2.txt","r",stdin); // put the file path between " "

    string s="";

    while (!feof(stdin)){ // taking the lines from the file and make it a one line string
        string k;
        getline(cin,k);
        s+=k;
    }

    return s;
}

Node * parsing()
{
    string str = take_file();
    Node *tmpnode;
    Node *root = new Node; // the root of the tree,,,,,,,,,,,,,,,,,,, 
    stack<Node*>st;//stack to know who will be the current parent tag
    int i=0;
    int lvlnumb = 0;

    while (true){ //iterating on all the characters of the file
        char c;
        tmpnode = new Node;
        c=str[i++];

        if(c=='<' && str[i]=='?'){ //it may be the XML prolog
            while(c!='>')c=str[i++];
        }

        else if(c=='<' && str[i]=='!'){ //it is a comment
            while(c!='>')c=str[i++];
        }

        else if(c == '<' && str[i]!='/'){ // it's an opening tag
            c=str[i++];
            string name;

            while(c != ' ' && c!='>' && c!='/'){ // parsing the name of the tag
                name+=c;
                c=str[i++];
            }

            tmpnode->name = name;

            while(c!='>'){ // taking the tag's attributes
                if(c=='/'){
                    c=str[i++];
                    break;
                }
                c=str[i++];
                if(c==' ')c=str[i++];
                string attr_name;
                while(c!='='){
                    attr_name+=c;
                    c=str[i++];
                }

                string attr_val;
                c=str[i++];
                c=str[i++];
                while(c!='"')
                {
                    attr_val+=c;
                    c=str[i++];
                }
                tmpnode->attrs.emplace_back(attr_name,attr_val);
                c=str[i++];

            }

            if(st.empty()){ // the root will be the first parent to enter the stack
                root = tmpnode;
                st.push(root);
                st.top()->level = 0;
            }
            else{
                st.top()->children.push_back(tmpnode);
                tmpnode->level = ++lvlnumb;
                st.push(tmpnode);
            }

            if(str[i-2]=='/')
            {
                lvlnumb--;
                st.pop(); // special case of opening and closing tag in the same line
            }


        }

        else if(c=='<'&&str[i]=='/'){ // it is a closing tag
            lvlnumb--;
            st.pop();
            while(c!='>')c=str[i++];
            if(st.empty())break;
        }


        else{ // taking the tag's text
            string txt;
            while(c!='<'){
                txt+=c;
                c=str[i++];
            }
            i--;
            st.top()->text=txt;
        }
    }


    return root;


}

void prettify(Node *tmpnode, int presize=0)
{
    for(int i=0;i<presize;i++)
        cout<<"\t";
    cout<< "<"<<tmpnode->name;
    for(auto attr:tmpnode->attrs)
    {
        cout<<" "<< attr.first << "=\""<< attr.second << "\"" ;
    }

    cout<<">"<<endl;

    if(tmpnode->text.size())
    {
        for(int i=0;i<presize+1;i++)
            cout<<"\t";
        cout<< tmpnode->text<<" "<<endl;

    }
    for(auto x:tmpnode->children)
    {
        prettify(x, presize+1);
    }
    for(int i=0;i<presize;i++)
        cout<<"\t";
    cout<<"</" <<tmpnode->name<< ">" << endl;
}

void minify (Node *tmpnode){
    cout<< "<"<<tmpnode->name;
    for(auto attr:tmpnode->attrs)
    {
        cout<<" "<< attr.first << "=\""<< attr.second << "\"" ;
    }
    cout<<">";
    if(tmpnode->text.size())
    {
        cout<< tmpnode->text;
    }
    for(auto x:tmpnode->children)
    {
        minify(x);
    }
    cout<<"</" <<tmpnode->name<< ">";
}


int main() {
    Node* r = parsing();
    prettify(r,0);
    minify(r);
    return 0;
}
