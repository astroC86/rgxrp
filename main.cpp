#include <regex>
#include <iostream>
#include "include/arg_hdlr.h"
#include "include/Hndlr_fns.h"
#include "include/utils.h"

using namespace std;

void   init_hnldr_cmds(arg_hdlr &hndlr);
string &apply_args_n_save(arg_hdlr &hndlr, string &buffer, list<pair<swtch *, list<string *>>> &save_drs);

int main(int argc, char** argv) {

    arg_hdlr hndlr = arg_hdlr();
    init_hnldr_cmds(hndlr);
    hndlr.parse(argc,argv);


    string buffer,line_buffer;

    g_line_mode     = hndlr.is_present('l');
    bool regex_prs  = hndlr.is_present('e');

    if(!regex_prs){
        fprintf(stderr,"No regex expression provided ");
        exit(-1);
    }

    list<pair<swtch*,list<string*>>> save_drs = hndlr.get_parsed('o');
    list<pair<swtch*,list<string*>>> fls = hndlr.get_parsed('f');

    if(fls.empty() && save_drs.size() > 1){
        fprintf(stderr,"Unbalanced input to output files.");
        exit(-1);
    }

    if(!fls.empty())
    {
       for(auto & fl : fls)
       {
           buffer  = utils::file_to_str(*fl.second.front());
           buffer = apply_args_n_save(hndlr, buffer, save_drs);

       }
    }else
    {
        buffer =  utils::stdin_to_str();
        buffer = apply_args_n_save(hndlr, buffer, save_drs);

    }

    return 0;
}

string &apply_args_n_save(arg_hdlr &hndlr, string &buffer, list<pair<swtch *, list<string *>>> &save_drs) {
    for(auto & parsed_arg : hndlr.parsed_args){
        if(parsed_arg.first->fn != nullptr){
            parsed_arg.second.insert(parsed_arg.second.end(),&buffer);
            parsed_arg.first->fn(parsed_arg.second);
        }
    }

    if(save_drs.empty())
        printf("%s\n",buffer.c_str());
    else{
        string sfname = *save_drs.front().second.front();
        utils::save_to_file(sfname,buffer);
        save_drs.erase(save_drs.begin());
    }
    return buffer;
}

void   init_hnldr_cmds(arg_hdlr &hndlr){
    hndlr.add_swtch('l',  "Activates line mode");
    hndlr.add_swtch('w', "Applies regex on whole file -  Default");

    hndlr.add_swtch('e', "expression", "Applies regex to the provided input.",Hndlr_fns::apply_expression,1);
    hndlr.add_swtch('h', "help", "Prints commands and their description.", arg_hdlr::pretty_print);

    hndlr.add_swtch('f', "filename", "The target file that the regex is going to be applied on.", nullptr,1);
    hndlr.add_swtch('o', "output", "Save the modified output to file.",nullptr,1);
}