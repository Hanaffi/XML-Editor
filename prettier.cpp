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



vector<pair<int,string>>error_place;
vector<pair<string,int>>error_type;
string str ;
string str2;

string file;

string take_file(){
    freopen("","r",stdin); // put the file path between " "

    string s="";

    while (!feof(stdin)){ // taking the lines from the file and make it a one line string
        string k;
        getline(cin,k);
        s+=k;
    }

    return s;
}

void solve_errors(){

    str2 = str;
    int x =0;
    for(int i=0;i<error_type.size();i++){

        if(error_type[i].first=="first"){
            string t1 = str2.substr(0,error_place[i].first+1);
            t1+="</"+error_place[i].second+">";
            string t2 = str2.substr(error_place[i].first+1);
            str2 = t1+t2;
            x += str2.size() - str.size();
        }

        else{
            string t1 = str2.substr(0,error_place[i].first+1+x);
            t1+="</"+error_place[i].second+">";
            string t2 = str2.substr(error_place[i].first+1+x);
            str2 = t1+t2;
        }

    }

    file = str2;
    str = str2;

}

void detect_errors()
{

    str = take_file();
    Node *tmpnode;
    stack<Node*>st;//stack to know who will be the current parent tag
    int i=0;

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

            while(c!='>'){
                c=str[i++];
            }

            st.push(tmpnode);

            if(str[i-2]=='/')
            {
                st.pop(); // special case of opening and closing tag in the same line
            }

        }

        else if(c=='<'&&str[i]=='/'){ // it is a closing tag
            string cmp_name;
            int j = i+1;
            c=str[j++];
            while(c!='>'){
                cmp_name+=c;
                c=str[j++];
            }
            if(st.top()->name!=cmp_name){ // error
                Node* t1;
                Node * t2;
                t1 = st.top();
                st.pop();
                t2 = st.top();
                st.pop();

                if(cmp_name==t2->name){
                    i-=2;
                    error_place.emplace_back(i,t1->name);
                    i = j;
                    error_type.emplace_back("third",0);

                }

                st.push(t2);
                st.push(t1);

                st.pop();
                st.pop();

            }
            else{
                i=j;
                st.pop();
            }
            if(st.empty())break;
        }


        else{ // taking the tag's text

            while(c!='<'){
                c=str[i++];
            }

            if(str[i]!='/'){
                int j = i;
                i-=2;
                error_place.emplace_back(i,st.top()->name);
                error_type.emplace_back("first",0);
                i = j;
                st.pop();

            }

            i--;

        }
    }




}

Node * parsing()
{
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
    detect_errors();
    solve_errors();
    Node* r = parsing();
    prettify(r,0);
    minify(r);

    return 0;
}
