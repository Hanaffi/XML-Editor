#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<QFileDialog>
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<algorithm>
#include<iomanip>
#define MAX 256 //8 bits of character
QString file_name,text;
std::string fn_string;

class Node {
public:
    std::string text;//tagValue
    std::string name;//tagName
    std::vector<Node*> children;
    std::vector<std::pair<std::string, std::string>>attrs; // (name,value)
    int level;
    bool node_is_leaf() {
        return (children.size()>0) ? 0 : 1;
    }
    void node_kill_children()
    {
        if(!node_is_leaf())
        {
            for(int i=0;i<children.size();i++)
            children[i]->node_kill_children();

        }
        int size = children.size();
        for(int i=0;i<size;i++)
        delete children[i];
        for(int i=0;i<size;i++)
        children[i] = NULL;
    }
};
Node * r;
class node_comp
{
private:
    unsigned char data;
    unsigned int frequency;
    node_comp *left;
    node_comp *right;
public:
    node_comp()
        {
            left = NULL;
            right = NULL;
            data = 0;
            frequency = 0;
        }
       node_comp(unsigned char d,unsigned int f,node_comp *l,node_comp *r)
        {
            left = l;
            right = r;
            data = d;
            frequency = f;
        }
       node_comp(unsigned char d,unsigned int f)
       {
           data = d;
           frequency = f;
           left = NULL;
           right =NULL;
       }
       node_comp( char d)
           {
               data = d;
               frequency = 0;
               left = NULL;
               right = NULL;
           }
        void set_left(node_comp * n)
        {
            left = n;
        }
        void set_right(node_comp * n)
        {
            right = n;
        }
        void set_data(unsigned char d)
        {
            data = d;
        }
        void set_frequency(unsigned int f)
        {
            frequency = f;
        }
        unsigned char get_data()
        {
            return data;
        }
        unsigned int get_frquency()
        {
            return frequency;
        }
        node_comp * get_left()
        {
            return left;
        }
        node_comp * get_right()
        {
            return right;
        }
        void add_children(node_comp *r,node_comp  *l)
        {
            right = r;
            left = l;
        }
        bool is_leaf() {
            return (left== NULL && right == NULL) ? 1 : 0;
        }
        //copying node to another
        node_comp( node_comp &m)
        {
            data = m.data;
            frequency = m.frequency;
            add_children(m.get_right(), m.get_left());
        }
        void kill_children()
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
};

// implementing functions for dealing with min heap to store our tree in array with min freq node at location 0

//Heapify function
void heapify(std::vector<node_comp*> &V, int i, int length)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left <= length && V[left]->get_frquency() < V[smallest]->get_frquency())
        smallest = left;
    if (right<= length && V[right]->get_frquency() < V[smallest]->get_frquency())
        smallest = right;
    if (smallest != i)
    {
        std::swap(V[i], V[smallest]);
        heapify(V, smallest, length);
    }
}
//getting the min. element in our heap then heapify again
node_comp* get_min_heap(std::vector<node_comp*> &v)
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
void Insert_Heap(std::vector<node_comp*> &v, node_comp* e)
{
    v.push_back(e);
    int i = v.size() - 1;
    int parent = (i - 1) / 2;
    while (i > 0 && v[parent]->get_frquency() > v[i]->get_frquency())
    {
        std::swap(v[i], v[parent]);
        i = parent;
    }
}
void build_min_heap(std::vector<node_comp*> &v, int length)
{
    for (int i = (length - 1) / 2; i >= 0; i--)
    {
        heapify(v, i, length);
    }
}
//generating codes for each leaf of the huffman tree
void generating_huff_code(node_comp* Root, char leftORright[], int index, std::vector<long long int> &Huffman_codemap)
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
//needs an output file to write in
//each leaf data has 1 behind
void store_tree( node_comp* Root)
{
    if (!Root->get_left() && !Root->get_right())
    {
        std::cout << '1';
        std::cout << Root->get_data();
    }
    else
    {
        std::cout << '0';
        store_tree( Root->get_left());
        store_tree(Root->get_right());
    }
}
//buidling the huffman tree
node_comp* Huffman(long long int freq[])
{
    std::vector<node_comp*>minheap;
    for (int i = 0; i < MAX; i++)
        if (freq[i] != 0)
            minheap.push_back(new node_comp(i, freq[i]));
    build_min_heap(minheap, minheap.size() - 1);
    while (minheap.size() != 1)
    {
        node_comp* first_minheap_e = get_min_heap(minheap);
        node_comp* second_minheap_e = get_min_heap(minheap);

        node_comp* parent = new node_comp(-1, 0, first_minheap_e, second_minheap_e);
        parent->set_frequency( parent->get_left()->get_frquency() + parent->get_right()->get_frquency());
        Insert_Heap(minheap, parent);
    }
    return(minheap[0]);
}
//save the compressed data in a new file
//needs an input file to read from and an output file to open to
void Write_compressed(std::vector<long long int > &Huffman_codemap)
{
    char ch;
    unsigned char bits_8=0;
    long long int counter = 0;
    while (std::cin.get(ch))
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
                std::cout << bits_8;
                counter = bits_8 = 0;
            }
        }
    }
    while (counter != 8)
    {
        bits_8 <<= 1;
        counter++;
    }
    std::cout << bits_8;
}
//---------------decompressing functions------------
//Make Tree using the compressed file
node_comp* reconstruct_Huffman_tree()
{
    char data;

    std::cin.get(data);
    if (data == '1')
    {
        std::cin.get(data);
        return (new node_comp(data));
    }
    else
    {
        node_comp* left = reconstruct_Huffman_tree();
        node_comp* right = reconstruct_Huffman_tree();
        return(new node_comp(-1,0, left, right));
    }
}
//Decode each binary symbol according to the tree
void decoding_compressed_file( node_comp * Root,int Total_Freq)
{


    bool eof_flag = false;
    char bits_8;
    node_comp* pointer = Root;
    while (std::cin.get(bits_8))
    {
        int counter = 7;
        while (counter >= 0)
        {
            if (!pointer->get_left() && !pointer->get_right())
            {
                std::cout << pointer->get_data();
                 std::cout<<"so far so good\n";
                Total_Freq--;
                if (!Total_Freq)
                {
                    eof_flag = true;
                    break;
                }
                pointer = Root;
                continue;
            }
            if ((bits_8&(1 << counter)))
            {
                 std::cout<<"so far so good\n";
                pointer = pointer->get_right();
                counter--;
            }
            else
            {
                 std::cout<<"so far so good\n";
                pointer = pointer->get_left();
                counter--;
            }
        }
        if (eof_flag)
            break;
    }
}
std::string take_file(std::string dir) {
    int n = dir.length();
     char char_array[n + 1];
    std::strcpy(char_array, dir.c_str());
    std::freopen(char_array, "r", stdin); // put the file path between " "
    std::string s = "";

    while (!feof(stdin)) { // taking the lines from the file and make it a one line string
        std::string k;
        getline(std::cin, k);
        s += k;
    }
    return s;
}
std::vector<std::pair<int,std::string>>error_place;
std::vector<std::pair<std::string,int>>error_type;
std::string str;
std::string str2;
std::string file;
void detect_errors()
{
    str = take_file(file_name.toLocal8Bit().constData());
    //string str = take_file();
    Node *tmpnode;
    std::stack<Node*>st;//stack to know who will be the current parent tag
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
            std::string name;

            while(c != ' ' && c!='>' && c!='/'){ // parsing the name of the tag
                name+=c;
                c=str[i++];
            }

            tmpnode->name = name;

            while(c!='>'){
                c=str[i++];
            }

            st.push(tmpnode);
            //cout<<st.top()->name<<endl;////////////////////////// debug

            if(str[i-2]=='/')
            {
                //cout<<st.top()->name<<endl;////////////////////////// debug
                st.pop(); // special case of opening and closing tag in the same line
            }

        }

        else if(c=='<'&&str[i]=='/'){ // it is a closing tag
            std::string cmp_name;
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
                    // append closing tag of t1
                    //cout<<"error3"<<endl;
                    i-=2;
                    // from current i append --> ( </st.top->name> to the file)
                    error_place.emplace_back(i,t1->name);
                    i = j;
                    error_type.emplace_back("third",0);

                }
                /*else{
                    // replace cmp_name with t1.name
                    cout<<"error2"<<endl;
                    error_place.emplace_back(i,t1->name);
                    error_type.emplace_back("second",cmp_name.size());

                }*/

                st.push(t2);
                st.push(t1);


                //cout<<st.top()->name<<endl;////////////////////////// debug
                st.pop();
                //cout<<st.top()->name<<endl;////////////////////////// debug
                st.pop();

            }
            else{
                i=j;
                //cout<<st.top()->name<<endl;////////////////////////// debug
                st.pop();
            }
            if(st.empty())break;
        }


        else{ // taking the tag's text

            while(c!='<'){
                c=str[i++];
            }

            if(str[i]!='/'){
                //cout<<"error1"<<endl;
                // el mfrod a3dl 3l file eny adef el top of the stack ll file (</.....>)
                int j = i;
                i-=2;
                // from current i append --> ( </st.top->name> to the file)
                error_place.emplace_back(i,st.top()->name);
                error_type.emplace_back("first",0);
                i = j;
                //cout<<st.top()->name<<endl;////////////////////////// debug
                st.pop();

            }

            i--;

        }
    }


}
std::string solve_errors(){

    str2 = str;
    int x =0;
    for(int i=0;i<error_type.size();i++){

        if(error_type[i].first=="first"){
            std::string t1 = str2.substr(0,error_place[i].first+1);
            t1+="</"+error_place[i].second+">";
            std::string t2 = str2.substr(error_place[i].first+1);
            str2 = t1+t2;
            x += str2.size() - str.size();
        }
        /*else if(error_type[i].first=="second"){
            string t1 = str2.substr(0,error_place[i].first+1);
            t1+=error_place[i].second;
            string t2 = str2.substr(error_place[i].first+1+error_type[i].second);
            str2 = t1+t2;
        }*/
        else{
            std::string t1 = str2.substr(0,error_place[i].first+1+x);
            t1+="</"+error_place[i].second+">";
            std::string t2 = str2.substr(error_place[i].first+1+x);
            str2 = t1+t2;
        }

    }

    file = str2;
    return str2;

}

Node * parsing(std::string dir)
{
    std::string str;
    if(error_place.size()>0){

        str = str2;
    }
    else
        str = take_file(dir);
    Node *tmpnode;
    Node *root = new Node; // the root of the tree,,,,,,,,,,,,,,,,,,,
    std::stack<Node*>st;//stack to know who will be the current parent tag
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
            std::string name;

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
                std::string attr_name;
                while(c!='='){
                    attr_name+=c;
                    c=str[i++];
                }

                std::string attr_val;
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
            std::string txt;
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

void prettify(Node *tmpnode, int presize = 0)
{
    for (int i = 0; i < presize; i++)
        std::cout << "\t";
    std::cout << "<" << tmpnode->name;
    for (auto attr : tmpnode->attrs)
    {
        std::cout << " " << attr.first << "=\"" << attr.second << "\"";
    }

    std::cout << ">\n";

    if (tmpnode->text.size())
    {
        for (int i = 0; i < presize + 1; i++)
            std::cout << "\t";
        std::cout << tmpnode->text << " \n";

    }
    for (auto x : tmpnode->children)
    {
        prettify(x, presize + 1);
    }
    for (int i = 0; i < presize; i++)
        std::cout << "\t";
    std::cout << "</" << tmpnode->name << ">\n";
}
void minify(Node *tmpnode) {
    std::cout << "<" << tmpnode->name;
    for (auto attr : tmpnode->attrs)
    {
        std::cout << " " << attr.first << "=\"" << attr.second << "\"";
    }
    std::cout << ">";
    if (tmpnode->text.size())
    {
        std::cout << tmpnode->text;
    }
    for (auto x : tmpnode->children)
    {
        minify(x);
    }
    std::cout << "</" << tmpnode->name << ">";
}
void xml2json(Node *tmpnode, int presize)
{
    std::cout << "\n" << std::setw(presize) << "\"" << tmpnode->name << "\": ";

    // no attr nor text
    if(!(tmpnode->text.size()) && !(tmpnode->attrs.size())&&!(tmpnode->children.size()))
    std::cout << "null,";

    // attr exist
    else if(tmpnode->attrs.size())
    {
        std::cout << "{";
        for(auto attr:tmpnode->attrs)
        {
        std::cout << "\n" << std::setw(presize+4) << "\"@" << attr.first << "\": \"" << attr.second << "\",";
        }

        // text exist along with attr
        if(tmpnode->text.size())
        {
            std::cout << "\n" << std::setw(presize+11) << "\"#text\": " << tmpnode->text << "\"";
        }
    }

    // text only
    else if(tmpnode->text.size())
        std::cout << "\"" << tmpnode->text << "\"";

    int i = 0;
    for(auto x:tmpnode->children)
    {
        xml2json(x, presize+3);
        if((x->attrs.size()) > 0 || (x->children.size() > 1))
        {
            std::cout << "\n" << std::setw(presize+4) << "}";
        }
    }
};


void xml2json_wrapper(Node *tmpnode, int presize)
{
    xml2json(tmpnode, presize);
    std::cout<<"\n"<<"}";
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    fn_string = "";
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_UPLOAD_clicked()
{

    file_name = QFileDialog::getOpenFileName(this,"open a file","C:\\Users\\mohamed\\Desktop");
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    else
        QMessageBox::information(this,"opened successfully",file_name);

    QTextStream in(&file);
    text = in.readAll();
    ui->plainTextEdit->setPlainText(text);

    file.close();
}



void MainWindow::on_errors_clicked()
{
    detect_errors();
    if(error_type.size()>0)
    {
     QMessageBox::warning(this,"title","ERROR(s) detected");
    }
}


void MainWindow::on_solve_clicked()
{
   std::string file_after_solving = solve_errors();
   std::string m = fn_string;
   int n = m.length();
   char char_array[n + 1];
   strcpy(char_array, m.c_str());
   std::remove(char_array);
   freopen(char_array,"w",stdout);
   std::cout<<file_after_solving;
   QString qstr = QString::fromStdString(file_after_solving);
   ui->plainTextEdit_2->setPlainText(qstr);
}


void MainWindow::on_parse_clicked()
{

    ui->plainTextEdit_2->clear();
    fn_string = file_name.toLocal8Bit().constData();
    std::string m = fn_string;
    std::string out = m;
    for(int i=0;i<m.length();i++)
    {
        if(m[i]=='.'){
            out[i-1] ='t';
            out[i-2]='u';
            out[i-3] = 'o';
        }
    }
    int n = out.length();
     char char_array[n + 1];
    strcpy(char_array, out.c_str());
    freopen(char_array,"w",stdout);
    //parsing operation on the file with directory m
    r = parsing(m);


    //printing on output file here
    prettify(r);
    //end of printing on file
    fclose(stdout);
    //printing the output file here
    QFile file(char_array);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    else
        QMessageBox::information(this,"opened successfully",file_name);

    QTextStream in(&file);
    text = in.readAll();
    ui->plainTextEdit_2->setPlainText(text);
}


void MainWindow::on_compress_clicked()
{
    ui->plainTextEdit_2->clear();
    std::vector<long long int > Huffman_codemap;//Double dimensional vector to store Huffman codes
        Huffman_codemap.resize(MAX);
        long long int Count[MAX] = { 0 }; //Declare and initialize character count array
        fn_string = file_name.toLocal8Bit().constData();
        std::string m = fn_string;
        int n = m.length();
         char char_array[n + 1];
        strcpy(char_array, m.c_str());
        freopen(char_array,"r",stdin);
        char ch;
        while (std::cin.get(ch))//Read file and increase count
            Count[static_cast<unsigned char>(ch)]++;
        std::cin.clear();//Reset File pointers
        std::cin.seekg(0);//Reset File pointers
        node_comp* tree = Huffman(Count);                                                                      //create Huffman tree
        std::string out = m;
        out+=".compressed";
        int out_n = out.length();
         char out_char_array[out_n + 1];
        strcpy(out_char_array, out.c_str());
        freopen(out_char_array,"w",stdout);
        std::cout << tree->get_frquency();                                                                        //Write to file
        std::cout << ',';
        store_tree( tree);
        std::cout << ' ';
        char leftORright[16];//array to store single code
        generating_huff_code(tree, leftORright, 0, Huffman_codemap);//Store Codes in vector
        Write_compressed( Huffman_codemap);//Write to file
        fclose(stdout);//Close file stream
        fclose(stdin);//Close file stream
        QMessageBox::information(this,"compressed successfully",file_name);
        QFile file(out_char_array);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this,"title","file not open");
        }
        else
            QMessageBox::information(this,"opened successfully",file_name);

        QTextStream in(&file);
        text = in.readAll();
        ui->plainTextEdit_2->setPlainText(text);
}


void MainWindow::on_decompress_clicked()
{
    fn_string = file_name.toLocal8Bit().constData();
    std::string m = fn_string;
    std::string out = m;
    out+=".compressed";
    int out_n = out.length();
     char char_array[out_n + 1];
    strcpy(char_array, out.c_str());
    freopen(char_array,"r",stdin);                                          //Open File

        int Total_freq = 0;
        char ch;
        while (std::cin.get(ch))                                                                  //read Original total frequency from file
        {
            if (ch == ',')
                break;
            Total_freq *= 10;
            Total_freq += ch - '0';
        }
        node_comp* Huffman_tree = reconstruct_Huffman_tree();                                         //Remake Huffman tree from file
        //Read Extra space between compressed data and tree from file
        std::cin.get(ch);
        for(int i=0;i<m.length();i++)
        {
            if(m[i]=='.'){
                m[i-1] ='1';
            }
        }
        char out_char_array[m.size() + 1];
       strcpy(out_char_array, m.c_str());
        freopen(out_char_array,"w",stdout);
        decoding_compressed_file( Huffman_tree, Total_freq);
        Huffman_tree->kill_children();
        fclose(stdin);
        fclose(stdout);

}


void MainWindow::on_xmltojson_clicked()
{
    ui->plainTextEdit_2->clear();
    std::string m = fn_string;
    std::string out = m;
    out+=".JSON";
    int out_n = out.length();
     char out_char_array[out_n + 1];
    strcpy(out_char_array, out.c_str());
    freopen(out_char_array,"w",stdout);
    xml2json_wrapper(r,0);
    fclose(stdout);
    QFile file(out_char_array);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    else
        QMessageBox::information(this,"opened successfully",file_name);

    QTextStream in(&file);
    text = in.readAll();
    ui->plainTextEdit_2->setPlainText(text);
}

