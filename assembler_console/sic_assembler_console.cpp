/*
includes the following operations
LDA LDX STA
ADD MUL DIV SUB
COMP TIX
JSUB RSUB JET JLT JGT ?
MOV
*/

#include<iostream>
#include<fstream>
#include<map>
#include<string.h>
#include<string>
#include<vector>
#include<sstream>
#include<cstdio>
#include "concol.h"

using namespace std;
using namespace ray;

map<string, string> optab;

struct pseudo
{
    string type;
    string value;
    string address;
};
map<string, pseudo> symtab;
map<string, pseudo>::iterator it;


string ACC;
string X;
string L;
string PC;
char SW[3]; //carry,zero,sign
int a1,cc=0;

string memory[5000];

//vector<string> address;

void OPTAB()
{
	optab["ADD"]="18";
	optab["SUB"]="1C";
	optab["MUL"]="20";
	optab["DIV"]="24";
	optab["LDA"]="00";
	optab["LDX"]="04";
	optab["STA"]="0C";
	optab["STX"]="10";
	optab["COMP"]="28";
	optab["TIX"]="2C";
    optab["RSUB"]="4C";
    optab["JSUB"]="48";
    optab["J"]="3C";
    optab["JET"]="30";
    optab["JGT"]="34";
    optab["JLT"]="38";
    optab["MOV"]="FF";
}

void get_arg(string command, vector<string> &args)
{
	string arg;
	args.clear();

	for(int i=0;i<command.length();i++)
	{
		arg="";
		while(command[i]!=' ' && i<command.length())
		{
			arg+=command[i++];
		}
		args.push_back(arg);
	}
}

//Adding two hex values represented as strings
string add_location(string A,string B, int op)
{
	int m, n;
    istringstream(B)>>hex>>n;
    istringstream(A)>>hex>>m;

    string r;
    stringstream stream;
    if(op==1)
        stream <<hex <<uppercase<<m+n;
    else
        stream<<hex<<uppercase<<m-n;
    stream>>r;

    return r;
}

string multiply_hex(string A,string B, int op)
{
	int m, n;
    istringstream(B)>>hex>>n;
    istringstream(A)>>hex>>m;

    string r;
    stringstream stream;
    if(op==1)
        stream <<hex <<uppercase<< m*n;
    else
        stream<<hex<<uppercase<<m/n;
    stream>>r;

    return r;
}

string get_string(int val)
{
    string r;
    stringstream stream;
    stream <<hex << val;
    stream>>r;
    return r;
}

int get_int(string val)
{
	int n;
    istringstream(val)>>hex>>n;

    return n;
}
void load_memory()
{
    int loop=0;
    string pos;
    cout<<"\nClearing memory...";
    for(int loop=0; loop<5000; loop++)
        memory[loop]="00";

    cout<<"\nLoading memory...";
}
string pass_1()
{
    load_memory();
	fstream in("test_source.txt",ios::in);
		//fstream in("BINARYSEARCH.txt",ios::in);

	//fstream out("OBJECTFILE.txt",ios::out|ios::trunc);
	vector<string> args;

	char command[30];
	string arg,start,location,string_value;
	int int_value,address,line_number=0;

	while(!in.eof())
	{
		in.getline(command,30);
		get_arg(command,args);
                line_number++;
        cout<<"\n"<<args[0];
		if(args.size()==0)
		continue;

		//storing the starting address in the start string
		if(args[0]=="START")
		{
			start=args[2];
			location=start;
			location=add_location(location,"3",1);
		}
		/*assign addresses for all instructions and store them in
           symbol table along with the labels*/
		else
		{
			//location=add_location(start,"3");
			//address.push_back(location);
			if(args[1]=="WORD" || args[1]=="BYTE" || args[1]=="RESW" || args[1]=="RESB")
            {
              pseudo p;
              p.type=args[1];
              p.value=args[2];
              p.address=location;
              symtab[args[0]]=p;

              if(args[1]=="WORD")
              {
                  int sz=args[2].size()-1,i;
                  address=get_int(location);
                  //cout<<"inside word"<<address;
                  memory[address]="";
                        //cout<<"inside word2";

                    for(i=sz;i>=0 && i>sz-2;i--)
                    {
                        memory[address]=(args[2])[i]+memory[address];
                    }
                    if(i!=sz-2)
                        memory[address]='0'+memory[address];
                    cout<<endl<<address<<".."<<memory[address]; //200f=ff
                    location=add_location(location,"1", 1);
                    address=get_int(location);
                    memory[address]="";
                    for(;i>=0 && i>sz-4;i--)
                    {
                        memory[address]=(args[2])[i]+memory[address];
                    }
                    if(i!=sz-4)
                    {
                        memory[address]='0'+memory[address];
                        if(i>=sz-2)
                          memory[address]='0'+memory[address];
                    }
                    cout<<endl<<address<<".."<<memory[address]; //2010=00
                    location=add_location(location,"1", 1);
                    address=get_int(location);
                    memory[address]="";
                    for(;i>=0 && i>sz-6;i--)
                    {
                        memory[address]=(args[2])[i]+memory[address];
                    }
                    if(i!=sz-6)
                    {
                        memory[address]='0'+memory[address];
                        if(i>=sz-4)
                          memory[address]='0'+memory[address];
                    }
                    cout<<endl<<address<<".."<<memory[address]; //2011=00
                    location=add_location(location,"1", 1);
                    //location=add_location(location,"3", 2);
              }
              else if(args[1]=="BYTE")
              {
                    int sz=args[2].size()-1,i;
                    address=get_int(location);
                    memory[address]="";
                    for(i=sz;i>=0 && i>sz-2;i--)
                    {
                        memory[address]=(args[2])[i]+memory[address];
                    }
                    if(i!=sz-2)
                        memory[address]='0'+memory[address];
                    location=add_location(location,"1", 1);


              }
              else if(args[1]=="RESW")
              {
                    int_value=get_int(args[2]);
                    int_value*=3;
                    string_value=get_string(int_value);
                    location=add_location(location,string_value, 1);
                    //if(args[0]=="N")
                       // cout<<"\nlocation of RESW 3 is:"<<location<<".."<<int_value<<".."<<args[2];
              }
              else if(args[1]=="RESB")
              {
                    location=add_location(location,args[2], 1);
              }
            }
            else
            {
                 if(args[0][0]=='L' && args[0][1]=='o'){
                      optab[args[0]]=location;
                      //cout<<"CHECK HERE"<<args[0]<<" "<<optab[args[0]];
                 }
                  if(args[0]=="JSUB" || args[0]=="J" || args[0]=="JLT" || args[0]=="JET" || args[0]=="JGT"){

                      string arg1=get_string(line_number);
                      optab[arg1]=location;
					  optab[location]=arg1;
                      //cout<<"Check here"<<args[1];

                  }
                location=add_location(location,"3", 1);
            }
		}
		cout<<"\t PC is "<<location;
		//out.close();
	}
	cout<<"\nfinal PC is "<<location; //cout<<"\noptab[loop] is "<<optab["Loop"];
	in.close();
	return location;
}

string make_word(string value,int len)
{
	while(value.size()<len)
	{
		value='0'+value;
	}
	return value;
}
void pass_2()
{
    ifstream in("test_source.txt");
    //ifstream in("BINARYSEARCH.txt",ios::in);

	ofstream out("OBJECTFILE.txt",ios::trunc);
	vector<string> args;

	char command[30];
	string arg;
    int line_number=0;
    int i;

	while(!in.eof())
	{
		in.getline(command,30);
		get_arg(command,args);
            line_number++;

        if(args[0]=="END")
        {
        	out<<endl;
        	out<<"E"<<make_word(args[2],6);
        	break;
		}

		if(args.size()==0 || args.size()==1)
		continue;

		if(args[0]=="START")
		{
			out<<"H"<<args[1]<<"\t"<<make_word(args[2],6);
			i=0;
			continue;
		}

		if(i%5==0)
		{
			out<<endl;
			out<<"T"<<"0F";
		}
		i++;

		if(optab.find(args[0])!=optab.end())
		{
			out<<optab[args[0]]<<make_word(symtab[args[1]].address,4);
		}

		if(optab.find(args[1])!=optab.end())
		{
			out<<optab[args[1]]<<make_word(symtab[args[2]].address,4);
		}

	}
	in.close();
	out.close();
}


void ADD(vector<string> &args, int op)
{
    int sz, address;
	string str,str1,s;
	string val="";
    str=(symtab[args[1]]).address;

cout<<"\n"<<get_int(str)<<".."<<memory[get_int(str)];

	for(int i=0;i<=2;i++)
    {
        s=get_string(i);
        str1=add_location(str,s, 1);
        address=get_int(str1);

        val=(memory[address])[1]+val;
        val=(memory[address])[0]+val;
        //cout<<"\nthe loc has "<<memory[str1];

    }cout<<"\nval in add is "<<val;
	ACC=add_location(ACC,val, op);
	sz=ACC.size();
	for(int i=sz;i<6;i++)
        ACC='0'+ACC;
    if(ACC=="000000") //set zero flag
        SW[1]='1';
    else
        SW[1]='0';
}

void MUL(vector<string> &args, int op)
{
    int sz, address;
	string str,str1,s;
	string val="";
    str=(symtab[args[1]]).address;
	for(int i=0;i<=2;i++)
    {
        s=get_string(i);
        str1=add_location(str,s, 1);
        address=get_int(str1);

        val=(memory[address])[1]+val;
        val=(memory[address])[0]+val;

    }//cout<<"\nval is "<<val;
    ACC=multiply_hex(ACC,val, op);
	sz=ACC.size();
	for(int i=sz;i<6;i++)
        ACC='0'+ACC;
        if(ACC=="000000") //set zero flag
            SW[1]='1';
        else
            SW[1]='0';
}

void LDA(vector<string> &args)
{
	int address;
    if((symtab[args[1]]).type=="RESB" || (symtab[args[1]]).type=="BYTE")
    {
        ACC="000000";
       	string str;
        string val="";
        str=(symtab[args[1]]).address;

       if(args.size()==3)
       {
           int val=3*get_int(X);
           string str1=get_string(val);
            str=add_location(str,str1,1);
       }
        address=get_int(str);

        ACC[5]=(memory[address])[1];
        ACC[4]=(memory[address])[0];
    }
	else
    {
        ACC="";
        string str,str1,s;
        str=(symtab[args[1]]).address;
            cout<<"\n..before str is "<<str<<"and "<<args[1];

        if(args.size()==3 || args.size()==4 && args[0][0]!='L')
       {
           int val=3*get_int(X);
           string str1=get_string(val);
            str=add_location(str,str1,1);
            cout<<"\n..here str is "<<str;
       }
       if(args[0][0]=='L' && args[0][1]=='o' && args.size()==3)
       {
           args[0]=args[1];
            args[1]=args[2];
                    str=(symtab[args[1]]).address;
            cout<<"\n..Loop str is "<<str<<"and "<<args[1];


       }
        for(int i=0;i<=2;i++)
        {
        	cout<<"\nInside the loop"<<ACC<<"\n";
            s=get_string(i);
            str1=add_location(str,s, 1);
            address=get_int(str1);
            ACC=(memory[address])[1]+ACC;
            ACC=(memory[address])[0]+ACC;
            cout<<"\nstr1 is"<<str1<<" acc is"<<ACC;

        }
        //cout<<endl<<ACC;
    }
}
void LDX(vector<string> &args)
{
	int address;
    if((symtab[args[1]]).type=="RESB" || (symtab[args[1]]).type=="BYTE")
    {
        X="000000";
       	string str;
        string val="";
        str=(symtab[args[1]]).address;

       // if(args.size()==3)
        	//str=add_location(str,X,1);
                    address=get_int(str);

        X[5]=(memory[address])[1];
        X[4]=(memory[address])[0];
    }
	else
    {
        X="";
        string str,str1,s;
        str=(symtab[args[1]]).address;

        //if(args.size()==3)
        	//str=add_location(str,X,1);

        for(int i=0;i<=2;i++)
        {
            s=get_string(i);
            str1=add_location(str,s, 1);
            address=get_int(str1);
            //cout<<endl<<str1;
            X=(memory[address])[1]+X;
            X=(memory[address])[0]+X;

        }
        //cout<<endl<<ACC;
    }
}
void STA(vector<string> &args)
{
    if(args[0][0]=='L'&& args[0][1]=='o' && args.size()==3) //Loop4 STA POS
    {
        args[0]=args[1];
        args[1]=args[2];
    }
    int address;
    if((symtab[args[1]]).type=="RESB" || (symtab[args[1]]).type=="BYTE")
    {
       	string str;
        str=(symtab[args[1]]).address;
        memory[address]="";
        address=get_int(str);

        memory[address]=ACC[5]+memory[address];
        memory[address]=ACC[4]+memory[address];
    }
    else     //for RESW and WORD
    {
        string str,s;
        str=symtab[args[1]].address;
        address=get_int(str);
        s=get_string(1);
        for(int i=0;i<=2;i++)
        {
            if(i!=0)
            {
                str=add_location(str, s, 1);
            }
            address=get_int(str);
                    memory[address]="";

            memory[address]=ACC[5-(2*i)-0]+memory[address];
            memory[address]=ACC[5-(2*i)-1]+memory[address];
                    //str=symtab[args[1]].address;


        }
    }
}
void STX(vector<string> &args)
{
    if(args[0][0]=='L'&& args[0][1]=='o' && args.size()==3) //Loop4 STA POS
    {
        args[0]=args[1];
        args[1]=args[2];
    }
    int address;
    if((symtab[args[1]]).type=="RESB" || (symtab[args[1]]).type=="BYTE")
    {
       	string str;
        str=(symtab[args[1]]).address;
        memory[address]="";
        address=get_int(str);

        memory[address]=X[5]+memory[address];
        memory[address]=X[4]+memory[address];
    }
    else     //for RESW and WORD
    {
        string str,s;
        str=symtab[args[1]].address;
        address=get_int(str);
        s=get_string(1);
        for(int i=0;i<=2;i++)
        {
            if(i!=0)
            {
                str=add_location(str, s, 1);
            }
            address=get_int(str);
                    memory[address]="";

            memory[address]=X[5-(2*i)-0]+memory[address];
            memory[address]=X[5-(2*i)-1]+memory[address];
                    //str=symtab[args[1]].address;


        }
    }
}

void MOV(vector<string> &args, int op)
{
    if(op==1) // register-to-register or immediate-to-register
    {
        if(args[1]=="A")
        {
            ACC="";
            if(args[2]=="X")
            {
                ACC+=X;
            }
            else if(args[2]=="L")
            {
                ACC+=L;
            }
            else if(args[2]=="PC")
            {
                ACC+=PC;
            }
            else
            {
                ACC="";
                ACC+=symtab[args[2]].value;
            }
        }
        else if(args[1]=="X")
        {
            X="";
            if(args[2]=="A")
            {
                X+=ACC;
            }
            else if(args[2]=="L")
            {
                X+=L;
            }
            else if(args[2]=="PC")
            {
                X+=PC;
            }
            else
            {
                X+=symtab[args[2]].value;
            }
        }
        else if(args[1]=="L")
        {
            L="";
            if(args[2]=="A")
            {
                L+=ACC;
            }
            else if(args[2]=="X")
            {
                L+=X;
            }
            else if(args[2]=="PC")
            {
                L+=PC;
            }
            else
            {
                L+=symtab[args[2]].value;
            }
        }
        else if(args[1]=="PC")
        {
            PC="";
            if(args[2]=="X")
            {
                PC+=X;
            }
            else if(args[2]=="L")
            {
                PC+=L;
            }
            else if(args[2]=="A")
            {
                PC+=ACC;
            }
            else
            {
                PC+=symtab[args[2]].value;
            }
        }
    }
}

int RSUB(vector<string> &args){

         string s=optab[L];
	 //cout<<"\n string is "<<s;
         int k=get_int(s);
         L="000000";
  return k;

}

void JSUB(vector<string> &args,int line_number){


     cout<<"\nJumping to subroutine at location "<<optab[args[1]];
     string arg1=get_string(line_number); //cout<<"\noptab8 and optab 9 are "<<optab["8"]<<" fsdfsd"<<optab["9"];
     //cout<<"..arg1 is "<<arg1;
     L=optab[arg1];

     int length=L.size();
     length=6-length;
     string str_ing="";
     for(int i1=0;i1<length;i1++)
         str_ing+='0';
     L=str_ing+L;
     cout<<"\nLocation stored in L register "<<L;

}

int COMP(vector<string> &args){
	int sz;
	int m,n, address;
	string str,str1,s;
	string val="";
	str=(symtab[args[1]]).address;
	//cout<<endl<<"address(comp)"<<str<<endl;
	//cout<<endl<<"memory:"<<memory[str];
	for(int i=0;i<=2;i++)
    {
        s=get_string(i);
        str1=add_location(str,s, 1);
                address=get_int(str1);

        val=(memory[address])[1]+val;
       // cout<<endl<<"val:"<<val<<endl;
        val=(memory[address])[0]+val;
       // cout<<endl<<"val:"<<val<<endl;

    }cout<<"\nval is(comp) "<<val;
    istringstream(ACC)>>hex>>n;
    istringstream(val)>>hex>>m;
    if(n<m)
    {
        SW[1]='0';
        return(-1);
    }
    if(n>m)
    {
        SW[1]='0';
        return(1);
    }
    //else set zero flag
    SW[1]='1';
    return(0);
}
int TIX(vector<string> &args){
	int sz;
	int m,n, address;
	string str,str1,s;
	string val="";
	str=(symtab[args[1]]).address;
	for(int i=0;i<=2;i++)
    {
        s=get_string(i);
        str1=add_location(str,s,1);
                address=get_int(str1);

        val=(memory[address])[1]+val;
        val=(memory[address])[0]+val;

    }cout<<"\nval is(tix) "<<val;
    X=add_location(X,"000001",1);
    istringstream(X)>>hex>>n;
    istringstream(val)>>hex>>m;
    if(n<m)
    {
        SW[1]='0';
        return(-1);
    }
    if(n>m)
    {
        SW[1]='0';
        return(1);
    }
    //else set zero flag
    SW[1]='1';
    return(0);
}

void show_memory()
{
    int address, value, count=0;
    string key, type;
	string val="";
    for(it=symtab.begin(); it!=symtab.end(); it++)
    {
        //int sz, address;
        type=it->second.type;
        count=0;
        if(type=="RESB")
        {
            key=it->first;
            cout<<"\n"<<purple<<key<<"\t";
                address=get_int(it->second.address);
                value=get_int(it->second.value);
                for(int i=0;i<value;i++)
                {
                    if(count%6==0 && count!=0)
                        cout<<endl;
                    cout<<red<<"["<<add_location(it->second.address, get_string(i), 1)<<"]="<<yellow<<memory[address+i]<<" B\t";
                    count++;
                }
        }
        else if(type=="RESW")
        {
            key=it->first;
        cout<<"\n\n"<<purple<<key<<"\t";
                address=get_int(it->second.address);
                value=get_int(it->second.value);
              for(int i=0;i<value*3;i++)
                {
                    if(count%3==0 && count!=0)
                        cout<<endl<<"\t";
                    cout<<red<<"["<<add_location(it->second.address, get_string(i), 1)<<"]="<<yellow<<memory[address+i]<<" W\t";
                    count++;
                }
        }
        //cout<<"["<<str<<"]="<<val<<"\t";
        //cout<<"\t"<<it->first<<"..."<<it->second.address;
    }
    cout<<endl;
    settextcolor(white);
}

void execute()
{
    char choice='y', step='n';
	fstream in("test_source.txt",ios::in);
	//fstream in("BINARYSEARCH.txt",ios::in);
	vector<string> args;

	char command[30];
	string arg;
    int line_number=0;
    cout<<"\nShow memory?(y/n): ";
        cin>>choice;
    cout<<"\nSimulate step by step?(y/n): ";
        cin>>step;

	while(!in.eof())
	{
	    if(choice=='y')
            show_memory();
		in.getline(command,30);
		get_arg(command,args);
                line_number++;
                if(args[0]=="END")
                    break;
        cout<<blue<<"\nline="<<line_number<<"..."<<command;
        settextcolor(white);
               //cout<<"\nargs[0] is "<<args[0]<<" cc is "<<cc;/////

		if(args.size()==0)
		continue;
		if(args.size()==4) //Loop LDA ARRAY X
		{
			args[0]=args[1];
			args[1]=args[2];
			args[2]=args[3];
		}

		if(args[0]=="ADD" || args[1]=="ADD")
		    ADD(args, 1);
		else if(args[0]=="SUB" || args[1]=="SUB")
                    ADD(args, 2);
		else if(args[0]=="MUL" || args[1]=="MUL")
		    MUL(args, 1);
		else if(args[0]=="DIV" || args[1]=="DIV")
                    MUL(args, 2);
		else if(args[0]=="LDA" || args[1]=="LDA")
		    LDA(args);
        else if(args[0]=="LDX" || args[1]=="LDX")
		    LDX(args);
		else if(args[0]=="STA" || args[1]=="STA")
		    STA(args);
        else if(args[0]=="STX" || args[1]=="STX")
		    STX(args);
        else if(args[0]=="MOV")// pass MOV in CODE as: MOV ACC X or MOV ACC 1F (use no commas thats all)
            MOV(args,1);
		else if(args[0]=="TIX")
		{
			cc=TIX(args);
			cout<<endl<<"decide(tix) in execute is  "<<cc<<endl;
			int sz=X.size();
			for(int i=sz;i<6;i++)
        		X='0'+X;
		}
		else if(args[0]=="COMP" || args[1]=="COMP")
			cc=COMP(args);
        else if(args[0]=="RSUB"){
                      int k=RSUB(args);
                      //cout<<"\n back to"<<k;
                      //k--;
                      while(k){
                        in.getline(command,30);k--;
                      }
        }

        else if(args[0]=="JSUB" || args[0]=="J"|| (args[0]=="JET" && cc==0) || (args[0]=="JGT" && cc==1) || (args[0]=="JLT"&& cc==-1)){
        //cout<<"\ninside j types";

        	  char command3[30];
        	  vector<string> arg3;
        	  int line_num=0;
        	  in.close();

        	  in.open("test_source.txt",ios::in);
        	  while(!in.eof()){
        	  	in.getline(command3,30);
        	  	get_arg(command3,arg3);
        	  	if(arg3[0]==args[0] && arg3[1]==args[1])
        	  	     break;
        	  	line_num++;
			  }

              JSUB(args,line_num+1);

		      in.close();
          	  in.open("test_source.txt",ios::in);

                  char command1[30];
                  vector<string> arg2;
                  int c=0;

                  while(!in.eof()){
                     in.getline(command1,30);
                     get_arg(command1,arg2);

                     if(arg2[0]==args[1]) break;
                         c++;
                  }
                  //cout<<"\nhere here"<<c<<" "<<args[1];//////

		      in.close();
          	  in.open("test_source.txt",ios::in);
                 //c--;
                   // c++;
                  while(c){
                    in.getline(command,30); c--; get_arg(command, arg2); //cout<<"\narg2[0] is "<<arg2[0];
                  }

        }



        if(args[0]=="END"){
                  in.clear();
                  in.seekg(0,ios::end);
        }
    cout<<"\nA="<<ACC<<"\tX="<<X<<"\tL="<<L;
    cout<<"\nSW="<<SW;
        if(step=='y')
        {
        cout<<"\nEnter y for next step: ";
        cin>>step;
        }
	}
	in.close();

}




int main()
{
    int sz,address,signal=1,value;
    string input,input_word;

    concolinit();

	ACC="000000";
	X="000000";
	L="000000";
	PC="000000";
	strcpy(SW,"010");
    load_memory();

	cout<<"\n\nInitial Status:";
    cout<<"\nA="<<ACC;
    cout<<"\nX="<<X;
        cout<<"\nL="<<L;
        cout<<"\nPC="<<PC;
        cout<<"\nSW="<<SW;


	OPTAB();
	PC=pass_1();

	cout<<"-----------USER INPUT---------\n";
	while(signal==1)
    {
        cout<<"Enter the variable name you want to store the value of : ";
        cin>>input;
        it=symtab.find(input);

        if(it!=symtab.end())
        {
            if(symtab[input].type=="RESB")
            {
                cout<<input<<" stores"<<symtab[input].value<<" bytes..Input them(in 2 hexadecimal digits) sequentially\n";
                address=get_int(symtab[input].address);
                value=get_int(symtab[input].value);
                for(int i=0;i<value;i++)
                {
                    cin>>memory[address+i];
                }
            }

            else if(symtab[input].type=="RESW")
            {
                cout<<input<<" stores"<<symtab[input].value<<" words..Input them(in 6 hexadecimal digits) sequentially\n";
                address=get_int(symtab[input].address);
                value=get_int(symtab[input].value);
                for(int i=0;i<value;i++)
                {
                    cin>>input_word;
                    memory[address+3*i]="";
                    memory[address+3*i]+=input_word[4];
                    memory[address+3*i]+=input_word[5];
                    memory[address+3*i+1]="";
                    memory[address+3*i+1]+=input_word[2];
                    memory[address+3*i+1]+=input_word[3];
                    memory[address+3*i+2]="";
                    memory[address+3*i+2]+=input_word[0];
                    memory[address+3*i+2]+=input_word[1];
                }
            }
        }
        else
        {
            cout<<"NO SUCH VARIABLE EXISTS IN THE PROGRAMME\n";
        }
        cout<<"Do you want to input again..Press 1 for yes and press any other key to exit : ";
        cin>>signal;
    }

	pass_2();
	execute();

    sz=PC.size();
	for(int i=sz;i<6;i++)
        PC='0'+PC;



	cout<<"\n\nFinal Status:";
    cout<<"\nA="<<ACC;
    cout<<"\nX="<<X;
        cout<<"\nL="<<L;
        cout<<"\nPC="<<PC;
        cout<<"\nSW="<<SW;
	return 1;
}

