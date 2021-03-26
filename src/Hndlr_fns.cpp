#include "../include/Hndlr_fns.h"
#include "../include/utils.h"
#include <string>
#include <regex>
#include <iostream>

using namespace std;

void Hndlr_fns::apply_expression(std::list<std::string *> args) {
    //regex expression is always index 0
    int s = (*args.begin())->find(REPLACE_BGIN) + strlen(REPLACE_BGIN);
    int n = (*args.begin())->find(REPLACE_CLOS)-s;
    string repl =(*args.begin())->substr(s,n);
    string expr = (*args.begin())->substr(s+n+1,(*args.begin())->size());
    regex r (expr);
    args.erase(args.begin());

    if(!g_line_mode)
        for(auto &a: args)
        {
            *a=utils::replace_nl(*a,'`');
            *a =  regex_replace(*a,r,  "");
            *a = utils::restore_nl(*a,'`');
        }
    else{
        //Todo implement line mode
    }
}

