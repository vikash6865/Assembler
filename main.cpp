#include<iostream>
#include<fstream>
#include<regex>
using namespace std;

struct symbol_table{
    string var;
    string offset;
    string segment;
    string type;
}seg[100];

void statement(string str){
    if(str[0]==';'){
        cout<<"Commment"<<endl;
    }
    else if(pos=(str.find("segment")>=0)){
        seg_no=0;
        symbol_table seg[seg_no];
        seg.var= str.substr(0,pos);
        seg.offset="OOH";
        seg.segment=" ";
        seg.type="segment";
        // seg_no++;
        string seg_end=seg.var+" ENDS";
        return ;
    }
    else if(pos=(str.find(seg_end)>=0)){
        
    }
}

int main(){
    ifstream source;
    string stmnt;
    int loc=0;
    source.open("program.txt");
    if(source.is_open()){
        while(getline(source,stmnt)){
            statement(stmnt);
    }
    }
    source.close();
}