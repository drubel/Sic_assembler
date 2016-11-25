#ifndef SIC_H
#define SIC_H

#include<iostream>
#include<fstream>
#include<map>
#include<string.h>
#include<string>
#include<vector>
#include<sstream>
#include<cstdio>
#include <ctime>

using namespace std;

class Sic{

    map<string, string> optab;
    map<string, int> opt;

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

    char SW[3]; //carry,zero,sign
    int a1,cc;

    string memory[5000];

public:
    string PC;
    void init();
    void OPTAB();
    void get_arg(string command, vector<string> &args);
    string add_location(string A,string B, int op);
    string multiply_hex(string A,string B, int op);
    string get_string(int val);
    int get_string(string val);
    void load_memory();
    string pass_1();
    void pass_2();
    void ADD(vector<string> &args, int op);
    void MUL(vector<string> &args, int op);
    void MOV(vector<string> &args, int op);
    void LDA(vector<string> &args);
    void STA(vector<string> &args);
    void LDX(vector<string> &args);
    int RSUB();
    int COMP(vector<string> &args);
    void JSUB(int line_number);
    int TIX(vector<string> &args);
    void show_memory();
    void execute();
    void allocate_memory();
    void delay();
    string make_word(string value,int len);

};


#endif // SIC_H
