#include "sic.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <unistd.h>

using namespace std;

void Sic::init(){
    ACC="000000";
    X="000000";
    L="000000";
    PC="000000";
    strcpy(SW,"010");
}

void Sic::OPTAB()
{
    optab["ADD"]="18";
    optab["SUB"]="1C";
    optab["MUL"]="20";
    optab["DIV"]="24";
    optab["LDA"]="00";
    optab["LDX"]="04";
    optab["STA"]="0C";
    optab["COMP"]="28";
    optab["TIX"]="2C";
    optab["RSUB"]="10";
    optab["JSUB"]="11";
    optab["MOV"]="FF";
}

void Sic::get_arg(string command, vector<string> &args)
{
    string arg;
    args.clear();
    int x1=command.size();

    for(int i=0;i<x1;i++)
    {
        arg="";
        while(command[i]!=' ' && i<x1)
        {
            arg+=command[i++];
        }
        args.push_back(arg);
    }
}

string Sic::add_location(string A,string B, int op)
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

string Sic::multiply_hex(string A,string B, int op)
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

string Sic::get_string(int val)
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

void Sic::load_memory()
{
    string pos;

    for(int loop=0; loop<5000; loop++)
        memory[loop]="00";
}

string Sic::pass_1()
{
    load_memory();
    fstream in("sourcecode.txt",ios::in);
    fstream file("register.txt",ios::out);
    file<<"";
    file.close();
    file.open("register.txt",ios::app);

    vector<string> args;

    char command[30];
    string arg,start,location,string_value;
    int int_value,address,line_number=0;

    while(!in.eof())
    {
        in.getline(command,30);
        get_arg(command,args);
        line_number++;
        opt[args[0]]=line_number;  //it is mapping between opcode and its line number
        if(args.size()==0)
        continue;

        //storing the starting address in the start string
        if(args[0]=="START")
        {
            start=args[1];
            location=start;
            location=add_location(location,"3",1);
        }
        /*assign addresses for all instructions and store them in
           symbol table along with the labels*/
        else
        {

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

                  memory[address]="";


                    for(i=sz;i>=0 && i>sz-2;i--)
                    {
                        memory[address]=(args[2])[i]+memory[address];
                    }
                    if(i!=sz-2)
                        memory[address]='0'+memory[address];

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

                    location=add_location(location,"1", 1);

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

                 }
                  if(args[0]=="JSUB" || args[0]=="J" || args[0]=="JLT" || args[0]=="JET" || args[0]=="JGT"){

                      string arg1=get_string(line_number);
                      optab[arg1]=location;
                      optab[location]=arg1;


                  }
                location=add_location(location,"3", 1);
            }
        }
        int siz=location.size();
        for(int x7=1;x7<=6-siz;x7++)
            location='0'+location;
        PC=location;
        file <<"\nACC= "<< ACC <<"\nX= "<<X<<"\nL= "<<L<<"\nPC= "<<PC<<"\nSW= "<<SW<<"\n";
        QString ss=QString::fromStdString(location);
        qDebug()<<"In pass 1 pc is"<<ss<<"\n";


    }
    file.close();
    in.close();
    return location;
}
string Sic::make_word(string value,int len)
{
	while(value.size()<len)
	{
		value='0'+value;
	}
	return value;
}
void Sic::pass_2()
{
    ifstream in("sourcecode.txt");
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
        	out<<"E"<<"000000";
        	break;
		}

		if(args.size()==0 || args.size()==1)
		continue;

		if(args[0]=="START")
		{
			out<<"H"<<make_word(args[1],6);
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


void Sic::ADD(vector<string> &args, int op)
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


    }
    QString st1=QString::fromStdString((symtab[args[1]]).value);
    qDebug()<<"val is"<<st1<<"\n";
    ACC=add_location(ACC,val, op);
    sz=ACC.size();
    for(int i=sz;i<6;i++)
        ACC='0'+ACC;
    if(ACC=="000000") //set zero flag
        SW[1]='1';
    else
        SW[1]='0';
    QString st=QString::fromStdString(ACC);
    qDebug()<<"after adding "<<st<<"\n";
}

void Sic::MUL(vector<string> &args, int op)
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

    }
    ACC=multiply_hex(ACC,val, op);
    sz=ACC.size();
    for(int i=sz;i<6;i++)
        ACC='0'+ACC;
    if(ACC=="000000") //set zero flag
         SW[1]='1';
    else
         SW[1]='0';
}

void Sic::LDA(vector<string> &args)
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
    else if((args.size()==3 || args.size()==4) && args[2]=="X"){
          int val=3*get_int(X);
          int str=get_int((symtab[args[1]]).address);
          ACC=(memory[str+val+2]+memory[str+val+1]+memory[str+val]);
          return;
          QString ss=QString::fromStdString(ACC);
          qDebug()<<"After lda array x loading"<<ss<<"\n";
    }
    else
    {
        ACC="";
        string str,str1,s;
        str=(symtab[args[1]]).address;


        if((args.size()==3 || args.size()==4) && args[0][0]!='L')
       {
           int val=3*get_int(X);
           string str1=get_string(val);
            str=add_location(str,str1,1);

       }
       if(args[0][0]=='L' && args[0][1]=='o' && args.size()==3)
       {
           args[0]=args[1];
            args[1]=args[2];
                    str=(symtab[args[1]]).address;
            //cout<<"\n..Loop str is "<<str<<"and "<<args[1];


       }
      /* if(args[2]=='X' && args[3]=='X' && args.size()==4)
       {
           //args[0]=args[1];
            //args[1]=args[2];
                    str=(symtab[args[1]]).address;
            //cout<<"\n..Loop str is "<<str<<"and "<<args[1];


       }*/
        for(int i=0;i<=2;i++)
        {

            s=get_string(i);
            str1=add_location(str,s, 1);
            address=get_int(str1);
            ACC=(memory[address])[1]+ACC;
            ACC=(memory[address])[0]+ACC;


        }
        QString ss=QString::fromStdString(ACC);
        qDebug()<<"After loading"<<ss<<"\n";
    }
}

void Sic::LDX(vector<string> &args)
{
    int address;
    if((symtab[args[1]]).type=="RESB" || (symtab[args[1]]).type=="BYTE")
    {
        X="000000";
        string str;
        string val="";
        str=(symtab[args[1]]).address;


        address=get_int(str);

        X[5]=(memory[address])[1];
        X[4]=(memory[address])[0];
    }
    else
    {
        X="";
        string str,str1,s;
        str=(symtab[args[1]]).address;

        for(int i=0;i<=2;i++)
        {
            s=get_string(i);
            str1=add_location(str,s, 1);
            address=get_int(str1);

            X=(memory[address])[1]+X;
            X=(memory[address])[0]+X;

        }

    }
}

void Sic::STA(vector<string> &args)
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

void Sic::MOV(vector<string> &args, int op)
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

int Sic::RSUB(){

         string s=optab[L];

         int k=get_int(s);
         L="000000";
  return k;

}

void Sic::JSUB(int line_number){

     string arg1=get_string(line_number);

     L=optab[arg1];

     int length=L.size();
     length=6-length;
     string str_ing="";
     for(int i1=0;i1<length;i1++)
         str_ing+='0';
     L=str_ing+L;

}

int Sic::COMP(vector<string> &args){

    int m,n, address;
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


    }
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

int Sic::TIX(vector<string> &args){

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

    }
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

void Sic::show_memory()
{
    int address, value, count=0;
    string key, type;
    string val="",val1;

    fstream file("memory.txt",ios::app);
    //file <<"";
    //file.close();

    //file.open("input.txt",ios::app);

    for(it=symtab.begin(); it!=symtab.end(); it++)
    {
        //int sz, address;
        type=it->second.type;
        count=0;
        if(type=="RESB")
        {
            key=it->first;

                address=get_int(it->second.address);
                value=get_int(it->second.value);
                for(int i=0;i<value;i++)
                {
                    if(count%6==0 && count!=0)
                        cout<<endl;
                    val1=("["+add_location(it->second.address, get_string(i), 1)+"]="+memory[address+i]+" B ");
                    file << val1 << "\n";
                    val1="";
                    count++;
                }
        }
        else if(type=="RESW")
        {
            key=it->first;

                address=get_int(it->second.address);
                value=get_int(it->second.value);
              for(int i=0;i<value*3;i++)
                {
                    if(count%6==0 && count!=0)
                        cout<<endl;
                    val1=("["+add_location(it->second.address, get_string(i), 1)+"]="+memory[address+i]+" W ");
                    file << val1 << "\n";
                    val1="";
                    count++;
                }
        }

    }
    file <<"\n\n";

    file.close();
}

void Sic::execute()
{
    //char choice='y';
    fstream fi("memory.txt",ios::out);
    fi<<"";
    fi.close();

    //fi.open("register.txt",ios::out);
    //fi<<"";
    //fi.close();

    ifstream in("sourcecode.txt",ios::in);

    vector<string> args;

    char command[30];
    string arg;
    int line_number=0;


    while(!in.eof())
    {
        //if(choice=='y')
        show_memory();
        in.getline(command,30);
        get_arg(command,args);
        line_number++;
                if(args[0]=="END")
                    break;

        qDebug()<< opt[args[0]]<<"\n";

        if(args.size()==0)
        continue;
        if(args.size()==4) //Loop LDA ARRAY X
        {
            args[0]=args[1];
            args[1]=args[2];
            args[2]=args[3];
        }
        if(args.size()==3 && args[0][0]=='L' && args[0][1]=='o') //Loop LDA ONE
        {
            args[0]=args[1];
            args[1]=args[2];
            //args[2]=args[3];
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
        else if(args[0]=="MOV")// pass MOV in CODE as: MOV ACC X or MOV ACC 1F (use no commas thats all)
            MOV(args,1);
        else if(args[0]=="TIX")
        {
            cc=TIX(args);
            //qDebug()<<cc<<"\n";
            int sz=X.size();
            for(int i=sz;i<6;i++)
                X='0'+X;
        }
        else if(args[0]=="COMP" || args[1]=="COMP")
            cc=COMP(args);
        else if(args[0]=="RSUB"){
                      int k=RSUB();

                      while(k){
                        in.getline(command,30);k--;
                      }
        }

        else if(args[0]=="JSUB" || args[0]=="J"|| (args[0]=="JET" && cc==0) || (args[0]=="JGT" && cc==1) || (args[0]=="JLT"&& cc==-1)){


              char command3[30];
              vector<string> arg3;
              int line_num=0;
              in.close();

              in.open("sourcecode.txt",ios::in);
              while(!in.eof()){
                in.getline(command3,30);
                get_arg(command3,arg3);
                if(arg3[0]==args[0] && arg3[1]==args[1])
                     break;
                line_num++;
              }

              JSUB(line_num+1);
              //QString st1=QString::fromStdString(optab[get_string(line_num+1)]);
              //qDebug()<<"line number"<<line_num+1<<" "<<st1<<"\n";
              in.close();
              in.open("sourcecode.txt",ios::in);
                  vector<string> arg2;
                  int c=opt[args[1]];      //from pass1()
                  c--;
                  QString st=QString::fromStdString(ACC);
                  qDebug()<<st<<"\n";

                  while(c--){
                    in.getline(command,30); get_arg(command, arg2);
                    QString st=QString::fromStdString(arg2[0]);
                    qDebug()<<st<<"\n";
                    //cout<<"\narg2[0] is "<<arg2[0];
                  }

        }



        if(args[0]=="END"){
                  in.clear();
                  in.seekg(0,ios::end);
        }
        fstream file;
        file.open("register.txt",ios::app);
        file <<"\nACC= "<< ACC <<"\nX= "<<X<<"\nL= "<<L<<"\nPC= "<<PC<<"\nSW= "<<SW<<"\n";
        file.close();

    }
    in.close();
}

void Sic::allocate_memory(){

    fstream file2("input.txt",ios::in);

    vector<string> args;
    char command[30];


    while(!file2.eof()){
        file2.getline(command,30);
        get_arg(command,args);

        if(args.size()==0)
            continue;
        it=symtab.find(args[0]);
        //qDebug()<< args.size()<<"\n";
        if(it!=symtab.end()){

            if(symtab[args[0]].type=="RESB"){
                 int address = get_int(symtab[args[0]].address);
                 int value1= get_int(symtab[args[0]].value);

                 for(int i=0;i<value1;i++){
                     file2.getline(command,30);
                     get_arg(command,args); 
                     memory[address+i]=args[0];
                 }
            }

            if(symtab[args[0]].type=="RESW"){
                 int address = get_int(symtab[args[0]].address);
                 int value1= get_int(symtab[args[0]].value);
                 //qDebug()<< value;
                 for(int i=0;i<value1;i++){
                     file2.getline(command,30);
                     get_arg(command,args);               
                     QString qs=QString::fromStdString(args[0]);
                     qDebug()<< qs<<"\n";
                     memory[address+3*i]="";
                     memory[address+3*i]+=args[0][4];
                     memory[address+3*i]+=args[0][5];
                     memory[address+3*i+1]="";
                     memory[address+3*i+1]+=args[0][2];
                     memory[address+3*i+1]+=args[0][3];
                     memory[address+3*i+2]="";
                     memory[address+3*i+2]+=args[0][0];
                     memory[address+3*i+2]+=args[0][1];
                     qDebug()<<i<<"\n";
                 }

            }


        }

    }
    qDebug()<<"Allocated";
}

void Sic::delay(){
     sleep(1);
}




