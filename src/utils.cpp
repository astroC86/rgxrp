//
// Created on 3/24/2021.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/utils.h"
using namespace std;

void utils::rm_dbl_spcs(string &s ){
    size_t i_dbl_spc;
    while( (i_dbl_spc = s.find("  ")) != string::npos)
        s.erase(i_dbl_spc, 1);
}
string utils::file_to_str(const string &file_name){
    ifstream file_stream{file_name};

    if (file_stream.fail())
        fprintf(stderr,"Error: Failed to open file." );

    ostringstream str_stream{};
    file_stream >> str_stream.rdbuf();

    if (file_stream.fail() && !file_stream.eof())
        fprintf(stderr,"Error: Failed to read file contents." );

    return str_stream.str();
}
string utils::stdin_to_str(){
    string str; int input;
    while ((input = getchar()) != EOF){
        str+=(char)input;
        if(str.empty()){
            break;
        }
    }
    return str;
}
string utils::replace_nl(string st, const char repl){
    string out;out.reserve(st.size());
    for (const char c: st)
        if(c == '\n')    out+= repl;
        else             out+=c;
    st.clear();
    return out;
}
void utils::save_to_file(string &fname, string &content)
{
    ofstream f(fname);
    f << content;
    f.close();
}
string utils::restore_nl(string st,const char tok){
    string out;out.reserve(st.size());
    for (const char c: st)
        if (c== tok)    out+='\n';
        else            out+=c;
    st.clear();
    return out;
}