//mov takes 2 byte 
//add takes 2 byte
// lea and lds takes  1 byte 
// xchg takes 2 bytes
#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include <cstdio>
#include <stdlib.h>
#include <sstream>
#include<regex>
using namespace std;
int k=0,lc=0; //global constant lc=location counter
struct pgm //to take input from user
 {
   char line[80];
 } s[100];
 struct symtable 
 {      string var;
		string segment;
		string type;
		string dtype;
		int lc;
		}sym[100];
void printsym() //printing the symbol table
{ cout<<endl<<endl<<string(60,'=')<<endl;
	cout<<"VAR\tsegment\ttype\tlc"<<endl<<string(60,'=')<<endl;
	for(int i=0;i<::k;i++)
	{
		cout<<sym[i].var<<"\t"<<sym[i].segment<<"\t"<<sym[i].type<<"\t"<<sym[i].lc<<endl;
	}
}
bool checkvar(string a) //check the variable in the data segment
{
	string b;
	for(int i=0;i<=::k;i++)
	{ b=sym[i].var;
		if(a.find(b))
		{ 
		return true;}
	}
	return false;
}
void forxchg(string test,string line,string type)
{
	int ax=line.find("ax"); //for register direct
	int bx=line.find("bx");
	int cx=line.find("cx");
	int dx=line.find("dx");
	int al=line.find("al");
	int ah=line.find("ah");
	int bl=line.find("bl");
	int bh=line.find("bh");
	int cl=line.find("cl");
	int ch=line.find("ch");
	int dl=line.find("dl");
	int dh=line.find("dh");
	int si=line.find("si");
	int di=line.find("di");

	if (type=="8")
	{
		if(checkvar(test) || al>=0 || ah>=0 ||bl>=0 || bh>=0 || cl>=0 || ch>=0 ||dl>=0 ||dh>=0)
		{
			::lc+=4;return;
		}
		else
		{
			cout<<"error:size doesnt match";exit(0);
		}
	}
	else
	{
		if(checkvar(test) || ax>0 || bx>0 || cx>0 || dx>0 || si>0 || di >0)
		{
		::lc+=4;return;}
		else
		{
			cout<<"error :size doesnt match";exit(0);
		}
	}
}
void singledeclare(string a)  // check if variable has already been declared or not
{
	string b;
	for(int i=0;i<::k;i++)
	{ b=sym[i].var;
  int pos=a.find(b);
		if(pos>=0)
		{ cout<<b<<pos;
		cout<<"variable already declared";
		exit(0);	
			}
	}
	return;
}
	
bool check(string a,string line,string byte)
{	int ax=a.find("ax"); //for register direct
	int bx=a.find("bx");
	int cx=a.find("cx");
	int dx=a.find("dx");
	int al=a.find("al");
	int ah=a.find("ah");
	int bl=a.find("bl");
	int bh=a.find("bh");
	int cl=a.find("cl");
	int ch=a.find("ch");
	int dl=a.find("dl");
	int dh=a.find("dh");
	int si=a.find("si");
	int di=a.find("di");
	int ax1=a.find("[ax]");  //for register indirect
	int bx1=a.find("[bx]");
	int cx1=a.find("[cx]");
	int dx1=a.find("[dx]");
	int al1=a.find("[al]");
	int ah1=a.find("[ah]");
	int bl1=a.find("[bl]");
	int bh1=a.find("[bh]");
	int cl1=a.find("[cl]");
	int ch1=a.find("[ch]");
	int dl1=a.find("[dl]");
	int dh1=a.find("[dh]");
	int si1=a.find("[si]");
	int di1=a.find("[di]");
	string b;
	for(int i=0;i<=::k;i++)
	{    b=sym[i].var;
		int j=a.find(b);
		if(j>=0)  //for direct operand 
		{   b=sym[i].dtype;
			if(b==byte)
				{
					::lc+=4;
				return true;}
			else if(byte=="dw" && ((b=="db") || (b=="dd") ))
			{
				if(line.find("word ptr">0))
				{	::lc+=4;	
					return true;}
					else
					{
					cout<<"error";exit(0);}
			}
			else if(byte=="db" && ((b=="dw") ||(b=="dd")))
			{
				if(line.find("byte ptr")>0)
				{	::lc+=3;
				return true;}
				else{
				cout<<"error";exit(0);}
			}	
		}	
	}
	//register direct addressing
	if(al>=0 || ah>=0 || ax>=0 || bx>=0 || bl>=0 || bh>=0 || cx>=0 ||cl>=0 ||ch>=0 ||dx>=0 ||dl>=0 || dh>=0 || si>=0 || di>=0)
	{
		if(al>=0 || ah>=0 ||bl>=0 || bh>=0 || cl>=0 || ch>=0 ||dl>=0 ||dh>=0)
		{
			::lc+=3;
			return true;}
		else
		{
			::lc+=4;
			return true;
		}
	}
	//register indirect addressing
	else if( al1>=0 || ah1>=0 || ax1>=0 || bx1>=0 || bl1>=0 || bh1>=0 || cx1>=0 ||cl1>=0 ||ch1>=0 ||dx1>=0 ||dl1>=0 || dh1>=0 || si1>=0 || di1>=0)
	{
		::lc+=4;
	}	
return false;
}
void codesegment(string a)
{
	int ax=a.find("ax");
	int bx=a.find("bx");
	int cx=a.find("cx");
	int dx=a.find("dx");
	int al=a.find("al");
	int ah=a.find("ah");
	int bl=a.find("bl");
	int bh=a.find("bh");
	int cl=a.find("cl");
	int ch=a.find("ch");
	int dl=a.find("dl");
	int dh=a.find("dh");
	int si=a.find("si");
	int di=a.find("di");
	int mov=a.find("mov");
	int lea=a.find("lea");
	int lds=a.find("lds");
	int xchg=a.find("xchg");
	int add=a.find("add");
	regex hex("[0-9]+[h]");
	regex dec("[0-9]+");
	if(mov>=0)
	{
		if(al>mov || ah>mov || ax>mov || bx>mov || bl>mov || bh>mov || cx>mov ||cl>mov ||ch>mov ||dx>mov ||dl>mov || dh>mov || si>mov || di>mov)
		{ 	string test;
			if(al>0 || ah>0 ||bl>0 || bh>0 || cl >0 || ch>0 ||dl>0 ||dh>0)
			{	if(al>0)
				{	if(al+2>=a.length())
					{
						test=a.substr(mov+4,al);
					}
					else
					{test=a.substr(al+3,a.length());}
					int match=test.find("al");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				else if(ah>0)
				{if(ah+2>=a.length())
					{
						test=a.substr(mov+4,ah);
					}
					else
					{test=a.substr(ah+3,a.length());}
					int match=test.find("ah");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
			    else if(bl>0)
				{if(bl+2>=a.length())
					{
						test=a.substr(mov+4,bl);
					}
					else
					{test=a.substr(bl+3,a.length());}
					int match=test.find("bl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(bh>0)
				{if(bh+2>=a.length())
					{
						test=a.substr(mov+4,bh);
					}
					else
					{test=a.substr(bh+3,a.length());}
					int match=test.find("bh");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(cl>0)
					{if(cl+2>=a.length())
					{
						test=a.substr(mov+4,cl);
					}
					else
					{test=a.substr(cl+3,a.length());}
					int match=test.find("cl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(ch>0)
					{if(ch+2>=a.length())
					{
						test=a.substr(mov+4,ch);
					}
					else
					{test=a.substr(ch+3,a.length());}
					int match=test.find("ch");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(dl>0)
					{if(dl+2>=a.length())
					{
						test=a.substr(mov+4,dl);
					}
					else
					{test=a.substr(dl+3,a.length());}
					int match=test.find("dl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				else
					{if(dh+2>=a.length())    //update maar saale isko
					{
						test=a.substr(mov+4,dh);
					}
					else
					{test=a.substr(dh+3,a.length());}
					int match=test.find("dh");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				string type="db";
				
				if(regex_match(test,hex) || regex_match(test,dec))
				{
				 ::lc+=3;
				return;
				}
				else if(check(test,a,type));
				else
				{
					cout<<"error1";
					exit(0);
				}}
				
			else if(ax>0 || bx>0 || cx>0 || dx>0 || si>0 || di >0)
			{   if(ax>0)
				{if(ax+2>=a.length())
					{
						test=a.substr(mov+4,ax);
					}
					else
					{test=a.substr(ax+3,a.length());}
					int match=test.find("ax");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(bx>0)
				{if(bx+2>=a.length())
					{
						test=a.substr(mov+4,bx);
					}
					else
					{test=a.substr(bx+3,a.length());}
					int match=test.find("bx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
			    else if(cx>0)
				{if(cx+2>=a.length())
					{
						test=a.substr(mov+4,cx);
					}
					else
					{test=a.substr(cx+3,a.length());}
					int match=test.find("cx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(dx>0)
				{if(dx+2>=a.length())
					{
						test=a.substr(mov+4,dx);
					}
					else
					{test=a.substr(dx+3,a.length());}
					int match=test.find("dx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(si>0)
				{if(si+2>=a.length())
					{
						test=a.substr(mov+4,si);
					}
					else
					{test=a.substr(si+3,a.length());}
					int match=test.find("si");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(di>0)
				{if(di+2>=a.length())
					{
						test=a.substr(mov+4,di);
					}
					else
					{test=a.substr(di+3,a.length());}
					int match=test.find("di");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				string type="dw";
				if(regex_match(test,hex) || regex_match(test,dec))
				{
				 ::lc+=3;
				return;}
				else if(check(test,a,type));
				else 
				{
				cout<<"error1";
				exit(0);
				}
			}
		}
		else{
			cout<<"error";
			exit(0);
		}
	}
	else if(lea>=0)
	{ string test;
		if(ax>lea ||dx>lea ||bx>lea|| cx>lea || si>lea || di>lea)
		{
			if(ax>0)
				{if(ax+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(ax+3,a.length());}}
				
			else if(bx>0)
				{if(bx+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(bx+3,a.length());}}
				
			else if(cx>0)
				{if(cx+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(cx+3,a.length());}}
				
			else if(dx>0)
				{if(dx+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(dx+3,a.length());}}
				
			else if(si>0)
				{if(si+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(si+3,a.length());}}
				
			else if(di>0)
				{if(di+3>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(di+3,a.length());}}

			if(checkvar(test))
			{
			::lc+=3;
			return;}
			else
			{
			cout<<"error2";
			exit(0);
			}
			
		}    
		else{
		cout<<"error12";exit(0);
		}
		
	}
	else if(lds>=0)
	{
		if(ax>lds ||dx>lds ||bx>lds || cx>lds || si>lds || di>lds)
		{ string test;
			if(ax>0)
				{if(ax+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(ax+3,a.length());}}
				
			else if(bx>0)
				{if(bx+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(bx+3,a.length());}}
				
			else if(cx>0)
				{if(cx+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(cx+3,a.length());}}
				
			else if(dx>0)
				{if(dx+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(dx+3,a.length());}}
				
			else if(si>0)
				{if(si+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(si+3,a.length());}}
				
			else if(di>0)
				{if(di+2>=a.length())
					{
						cout<<"register cannot be destination";
						exit(0);
					}
					else
					{test=a.substr(di+3,a.length());}}

			if(checkvar(test))
			{
			::lc+=3;
			return;}
			else
			{
			cout<<"error";
			exit(0);
			}
			
		}    
		else{
		cout<<"error";exit(0);
		}
		
	}
	else if(xchg>=0)
	{
		if(al>0 || ah>0 ||bl>0 || bh>0 || cl >0 || ch>0 ||dl>0 ||dh>0)
			{	string test;  
        if(al>0)
				{	if(al+2>=a.length())
					{
						test=a.substr(xchg+5,al);
					}
					else
					{test=a.substr(al+3,a.length());}
					int match=test.find("al");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				else if(ah>0)
				{if(ah+2>=a.length())
					{
						test=a.substr(xchg+5,ah);
					}
					else
					{test=a.substr(ah+3,a.length());}
					int match=test.find("ah");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
			    else if(bl>0)
				{if(bl+2>=a.length())
					{
						test=a.substr(xchg+5,bl);
					}
					else
					{test=a.substr(bl+3,a.length());}
					int match=test.find("bl");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(bh>0)
				{if(bh+2>=a.length())
					{
						test=a.substr(xchg+5,bh);
					}
					else
					{test=a.substr(bh+3,a.length());}
					int match=test.find("bh");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(cl>0)
					{if(cl+2>=a.length())
					{
						test=a.substr(xchg+5,cl);
					}
					else
					{test=a.substr(cl+3,a.length());}
					int match=test.find("cl");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(ch>0)
					{if(ch+2>=a.length())
					{
						test=a.substr(xchg+5,ch);
					}
					else
					{test=a.substr(ch+3,a.length());}
					int match=test.find("ch");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(dl>0)
					{if(dl+2>=a.length())
					{
						test=a.substr(xchg+5,dl);
					}
					else
					{test=a.substr(dl+3,a.length());}
					int match=test.find("dl");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				else
					{if(dh+2>=a.length())    //update maar saale isko
					{
						test=a.substr(xchg+5,dh);
					}
					else
					{test=a.substr(dh+3,a.length());}
					int match=test.find("dh");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				forxchg(test,a,"8");
			}
		else if(ax>0 || bx>0 || cx>0 || dx>0 || si>0 || di >0)
			{  string test;
         if(ax>0)
				{if(ax+2>=a.length())
					{
						test=a.substr(xchg+5,ax);
					}
					else
					{test=a.substr(ax+3,a.length());}
					int match=test.find("ax");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(bx>0)
				{if(bx+2>=a.length())
					{
						test=a.substr(xchg+5,bx);
					}
					else
					{test=a.substr(bx+3,a.length());}
					int match=test.find("bx");
					if(match>=0)
					{
					cout<<"error";exit(0);}}
				
			    else if(cx>0)
				{if(cx+2>=a.length())
					{
						test=a.substr(xchg+5,cx);
					}
					else
					{test=a.substr(cx+3,a.length());}
					int match=test.find("cx");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(dx>0)
				{if(dx+2>=a.length())
					{
						test=a.substr(xchg+5,dx);
					}
					else
					{test=a.substr(dx+3,a.length());}
					int match=test.find("dx");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(si>0)
				{if(si+2>=a.length())
					{
						test=a.substr(xchg+5,si);
					}
					else
					{test=a.substr(si+3,a.length());}
					int match=test.find("si");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				
				else if(di>0)
				{if(di+2>=a.length())
					{
						test=a.substr(xchg+5,di);
					}
					else
					{test=a.substr(di+3,a.length());}
					int match=test.find("di");
					if(match>=0)
					{
					cout<<"error";exit(0);}
				}
				forxchg(test,a,"16");
			}
		else
		{
			cout<<"error : one shpuld be a register";exit(0);
		}
	}
	else
	{
		if(al>add || ah>add || ax>add || bx>add || bl>add || bh>add || cx>add ||cl>add ||ch>add ||dx>add ||dl>add || dh>add || si>add || di>add)
		{ string test;
			if(al>0 || ah>0 ||bl>0 || bh>0 || cl >0 || ch>0 ||dl>0 ||dh>0)
			{	if(al>0)
				{	if(al+2>=a.length())
					{
						test=a.substr(add+4,al);
					}
					else
					{test=a.substr(al+3,a.length());}
					int match=test.find("al");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				else if(ah>0)
				{if(ah+2>=a.length())
					{
						test=a.substr(add+4,ah);
					}
					else
					{test=a.substr(ah+3,a.length());}
					int match=test.find("ah");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
			    else if(bl>0)
				{if(bl+2>=a.length())
					{
						test=a.substr(add+4,bl);
					}
					else
					{test=a.substr(bl+3,a.length());}
					int match=test.find("bl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(bh>0)
				{if(bh+2>=a.length())
					{
						test=a.substr(add+4,bh);
					}
					else
					{test=a.substr(bh+3,a.length());}
					int match=test.find("bh");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(cl>0)
					{if(cl+2>=a.length())
					{
						test=a.substr(add+4,cl);
					}
					else
					{test=a.substr(cl+3,a.length());}
					int match=test.find("cl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(ch>0)
					{if(ch+2>=a.length())
					{
						test=a.substr(add+4,ch);
					}
					else
					{test=a.substr(ch+3,a.length());}
					int match=test.find("ch");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(dl>0)
					{if(dl+2>=a.length())
					{
						test=a.substr(add+4,dl);
					}
					else
					{test=a.substr(dl+3,a.length());}
					int match=test.find("dl");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				else
					{if(dh+2>=a.length())
					{
						test=a.substr(add+4,dh);
					}
					else
					{test=a.substr(dh+3,a.length());}
					int match=test.find("dh");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				string type="db";
				if(regex_match(test,hex) || regex_match(test,dec))
				{
				 ::lc+2;   //add instruction takes 2 byte with immediate operand
					return;}
				else if(check(test,a,type));
				else
				{
					cout<<"error"; // if the variable is not declared in the data segment
					exit(0);
				}}
				
			else if(ax>0 || bx>0 || cx>0 || dx>0 || si>0 || di >0)
			{   if(ax>0)
				{if(ax+2>=a.length())
					{
						test=a.substr(add+4,ax);
					}
					else
					{test=a.substr(ax+3,a.length());}
					int match=test.find("ax");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(bx>0)
				{if(bx+2>=a.length())
					{
						test=a.substr(add+4,bx);
					}
					else
					{test=a.substr(bx+3,a.length());}
					int match=test.find("bx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
			    else if(cx>0)
				{if(cx+2>=a.length())
					{
						test=a.substr(add+4,cx);
					}
					else
					{test=a.substr(cx+3,a.length());}
					int match=test.find("cx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(dx>0)
				{if(dx+2>=a.length())
					{
						test=a.substr(add+4,dx);
					}
					else
					{test=a.substr(dx+3,a.length());}
					int match=test.find("dx");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(si>0)
				{if(si+2>=a.length())
					{
						test=a.substr(add+4,si);
					}
					else
					{test=a.substr(si+3,a.length());}
					int match=test.find("si");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				else if(di>0)
				{if(di+2>=a.length())
					{
						test=a.substr(add+4,di);
					}
					else
					{test=a.substr(di+3,a.length());}
					int match=test.find("di");
					if(match>=0)
					{
					cout<<"error: both are same register";exit(0);}
				}
				
				string type="dw";
				if(check(test,a,type));
				else if(regex_match(test,hex) || regex_match(test,dec))
				{
				 ::lc+3;
				return;}
				else 
				{
				cout<<"erroradd";
				exit(0);
				}
			}
		}
		else{
			cout<<"error";
			exit(0);
		}
	}
}
int segment(string p,int lc,int k,int j)
 { 
   int pos0,pos;
		string a=s[j].line,test;
		pos0=a.find("ends");
		pos=a.find("db");
		int q=a.find("?");
		int pos1=a.find("dw");
		int pos2=a.find("dd");
		int pos4=a.find(":");
		int mov=a.find("mov");
		int lea=a.find("lea");
		int lds=a.find("lds");
		int xchg=a.find("xchg");
		int add=a.find("add");
		regex hex("\\s+[0-9]+[h]");
		regex dec("\\s+[0-9]+");
		regex vardec("[a-zA-z]+[0-9]+\\s+");
        regex vardec1("[a-zA-z]+\\s+");
		int dup=a.find("dup");
		int comma=a.find(",");
		if(pos0>0)
		{	pos0=a.find(p);
			if(pos0>=0)
			{
			return j;}
			else{
				cout<<"error";
				exit(0);
			}
		}
		else if(pos>0 || pos1>0 ||pos2>0 || pos4>0)
		{  
			if(pos>=0 )
			{  
				string test=a.substr(pos+2,a.length()),test1=a.substr(0,pos);
				if((regex_match(test,hex) || regex_match(test,dec) || dup>0 || comma>0 || q>0) && (regex_match(test1,vardec)||regex_match(test1,vardec1))){   cout<<"a";
					pos=a.find("db");
					singledeclare(a.substr(0,pos));
					sym[::k].var=a.substr(0,pos);
					int count=1;
					sym[::k].lc=::lc;
					sym[::k].segment=p;
					sym[::k].dtype="db";
					sym[::k].type="var";
					for(int x=pos;x<a.length();x++)
						{
							if(a[pos]==',')
							{
						count++;}
					}
					if(dup>0)
					{	char g[10];
						int f=0;
						dup =dup+3;
						g[f]=a[++dup];
						while(a[++dup]!=')')
						{
							g[++f]=a[dup];}
						if(q>dup || regex_match(a.substr(dup,a.length()),hex) || regex_match(a.substr(dup,a.length()),hex))
						{f=atoi(g);
						count*=f;}
						else{
							cout<<"error";exit(0);
						}
					}
					::lc=::lc+count;				
					return segment(p,::lc,++::k,++j);
				}
			else{
				cout<<"error1";
				exit(0);}
		}
		 else if(pos1>=0)
		 {    	pos=a.find("dw");
				if((regex_match(a.substr(pos+2,a.length()),hex) || regex_match(a.substr(pos+2,a.length()),dec) || dup>0 || comma>0 || q>0 ) && (regex_match(a.substr(0,pos1),vardec) || (regex_match(a.substr(0,pos2),vardec1)))){
					singledeclare(a.substr(0,pos));
					sym[::k].var=a.substr(0,pos);
					int count=2;
					sym[::k].lc=::lc;
					sym[::k].type="var";
					sym[::k].dtype="dw";
					sym[::k].segment=p;
					for(int x=pos;x<a.length();x++)
					{
						if(a[pos]==',')
							{
								count+=2;;}
					}
					pos=a.find("dup");
				    if(pos>=0)
					{
						char g[10];
						int f=0;
						pos=pos+3;
						g[f]=a[++pos];
						cout<<g[f]<<endl;
							while(a[++pos]!=')')
							{
								g[++f]=a[pos];}
						if(q>pos || regex_match(a.substr(pos,a.length()),hex) || regex_match(a.substr(pos,a.length()),hex))
						{f=atoi(g);	
						count*=f;}
						else{
						cout<<"error";exit(0);}
					}
					::lc=::lc+count;
					return segment(p,::lc,++::k,++j);
					}
				else
				{
					cout<<"error";
					}
				
			}
		 else if(pos2>=0 )
		 { 	pos=a.find("dd");	
			if((regex_match(a.substr(pos+2,a.length()),hex) || regex_match(a.substr(pos+2,a.length()),dec) || dup>0 || comma>0 ) && (regex_match(a.substr(0,pos2),vardec) ||(regex_match(a.substr(0,pos2),vardec1))))
				{
					singledeclare(a.substr(0,pos)); 
					sym[::k].var=a.substr(0,pos);
					int count=4;
					sym[::k].lc=::lc;
					sym[::k].dtype="dd";
					sym[::k].type="var";
					sym[::k].segment=p;
					for(int x=pos;x<a.length();x++)
					{
						if(a[pos]==',')
						{
						count+=4;}
					}
					pos=a.find("dup");
					if(pos>=0)
					{ 	
						char g[10];
						int f=0;
						pos=pos+3;
						g[f]=a[++pos];
						cout<<g[f]<<endl;
							while(a[++pos]!=')')
							{
								g[++f]=a[pos];}
						
						if(q>pos || regex_match(a.substr(pos,a.length()),hex) || regex_match(a.substr(pos,a.length()),hex))
						{f=atoi(g);
						count*=f;}
						else{
						cout<<"error";exit(0);}
					}
						
					::lc=::lc+count;
					return segment(p,::lc,++::k,++j);
				}
				else
				{
					cout<<"error";
					exit(0);
					}
		}
		 else if(pos4>=0)
		 {	 pos0=a.find("assume");
		     pos1=a.substr(pos4-2,pos4).find("cs");
		     pos2=a.substr(pos4-2,pos4).find("ds");
			 if(pos0>=0 || pos1>=0 ||  pos2>=0)
			 {  
				 return segment(p,::lc,::k,++j);
			 }
			 else
			 {  
				 sym[::k].var=a.substr(0,pos4);
				 sym[::k].lc=::lc;
				 sym[::k].segment=p;
				 sym[::k].type="label";
				return segment(p,::lc,++::k,++j);
			 }
		 }
		 
		 else
			{
			cout<<"error";
			exit(0);
			}
		}
		else if(mov>=0 || lea>=0 || lds>=0 ||xchg>=0 ||add>=0)
		{
			codesegment(a);
			return segment(p,::lc,::k,++j);
		}
		else
		{
		cout<<"error";
		exit(0);
		}	
 }

int main()
 {
    regex vardec("[a-zA-z]+\\s+");
   int i=0,j=0,pos;
   cout<<"Enter the Program code:";
   do
	{
		gets(s[i].line);
	}
   while(strcmp(s[i++].line,"end start"));

  while(j<i-1)
   {      	 string a=s[j].line;
			 if(a.find("segment")>=0 || a.find("SEGMENT"))
			 {      pos=a.find("segment");
				    sym[::k].var=a.substr(0,pos);
				    sym[::k].segment="itself";
				    sym[::k].type="segment";
					::lc=0;
				    sym[::k].lc=lc;
			    	j=segment(a.substr(0,pos),::lc,++::k,++j);
			    	j++;
       }
			else
			{
			cout<<"error";}
			}
	printsym();
   }
