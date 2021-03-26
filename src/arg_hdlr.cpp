#include <iostream>
#include <utility>
#include <cstring>
#include "../include/arg_hdlr.h"

using namespace std;

map2d arg_hdlr::groups;

///
/// \param flag
/// \param dsp
/// \param fn
/// \param n_args
/// \param grp
void
arg_hdlr::add_swtch(char flag, std::string dsp, void (*fn)(std::list<std::string*>),
                    int n_args                , const std::string &grp   )
{
    auto dsc  = new swtch(std::move(dsp), flag, n_args);
    if(fn!= nullptr){
        dsc->fn = fn;
    }

    groups[grp][flag] = dsc;
}

///
/// \param flag
/// \param ext_flg
/// \param dsp
/// \param fn
/// \param n_args
/// \param grp
void
arg_hdlr::add_swtch(char   flag      , string ext_flg                   ,
                    string dsp       , void (*fn)(std::list<std::string*>),
                    int n_args       , const string &grp   )
{

    auto dsc = new swtch(std::move(dsp), flag, n_args);
    dsc->ext_flg = std::move(ext_flg);
    if (fn != nullptr) {
        dsc->fn = fn;
    }

    groups[grp][flag] = dsc;
}

///
arg_hdlr::~arg_hdlr()
{
    for(auto &g: groups){
        for(auto &h:g.second)
            delete h.second;
        g.second.erase(
                    g.second.begin(),
                    g.second.end()
                );
    }
    groups.erase(
            groups.begin(),
            groups.end()
            );
}

///
/// \param args
void
arg_hdlr::pretty_print(list<string*> args)
{
    for(auto &g: groups){
        printf("%s:\n",g.first.c_str());
        for(auto &h: g.second){
            printf("\t-%c%8.15s     %s\n",
                   h.first,
                   !h.second->ext_flg.empty() ?
                                               (string(",--")+ h.second->ext_flg).c_str()
                                              :"",
                   h.second->description.c_str());
        }
    }
}

///
/// \param flag
/// \param argv
/// \param pos
/// \param max
void
arg_hdlr::prepare(char flag, char **argv, int *pos, int max)
{
    for( auto &g: arg_hdlr::groups){
        auto res = g.second.find(flag);
        if(res!=g.second.end() && *pos +1< max){
            list<string*> args;*pos+=1;

            for(int i = 0; i < g.second[flag]->n_args;i++,*pos+=1)
                args.push_back(new string(argv[*pos]));
            parsed_args.push_back(make_pair(g.second[flag],args));
            *pos-=1;
            return;
        }else{
            fprintf(stderr,"Invalid flag or no argument provided for %c flag",flag);
            exit(-1);
        }
    }
}

///
/// \param ext_flag
/// \param argv
/// \param pos
/// \param max
void
arg_hdlr::prepare(const string &ext_flag, char **argv, int *pos, int max)
{
    list<string*> args;
    int arg_pos = ext_flag.find('=');
    if(arg_pos == string::npos){
        fprintf(stderr,"No argument provided for %s", ext_flag.c_str());
        exit(-1);
    }
    args.push_back(new string(ext_flag.substr(arg_pos+1,ext_flag.size())));
    string ext_flag_ = ext_flag.substr(0,arg_pos);

    for( auto &g: arg_hdlr::groups){
        for(auto &h: g.second){
            if (!h.second->ext_flg.empty()  &&
                h.second->ext_flg==ext_flag_){
                parsed_args.push_back(make_pair(h.second,args));
                return;
            }
        }
    }
}

///
/// \param n_args
/// \param argv
void
arg_hdlr::parse(int n_args, char **argv)
{

    for(int i = 1; i < n_args; i++)
        if(argv[i][0]=='-' && argv[i][1]!='-')
            prepare(argv[i][1], argv, &i, n_args);
        else if ( argv[i][0]=='-'&& argv[i][1]=='-')
            prepare(string(argv[i], 2, strlen(argv[0]) - 1), argv, &i, n_args);
        else
            fprintf(stderr, "Invalid argument %s %d", argv[i],i);

}

///
/// \param ext_flg
/// \return
bool
arg_hdlr::is_swtch(string &ext_flg)
{
    for( auto &g: arg_hdlr::groups){
        for(auto &h: g.second)
        {
            if (!h.second->ext_flg.empty()           &&
                h.second->ext_flg==ext_flg)
                return true;
        }
    }
    return false;
}

///
/// \param flg
/// \return
bool
arg_hdlr::is_swtch(char flg)
{
    for( auto &g: arg_hdlr::groups){
        auto res = g.second.find(flg);
        if(res!=g.second.end() && g.second[flg]->fn != nullptr)
            return true;
    }
    return false;
}

///
/// \param flg
/// \return
bool
arg_hdlr::is_present(char flg) {
    queue<pair<swtch*,list<string*>>> q (parsed_args);
    bool res = false;

    while(!q.empty()){
        if(q.front().first->flg == flg)
            res = true;
        q.pop();
    }
    return res;
}

///
/// \param ext_flg
/// \return
bool
arg_hdlr::is_present(string &ext_flg) {
    queue<pair<swtch*,list<string*>>> q (parsed_args);
    bool res = false;

    while(!q.empty()){
        if(q.front().first->ext_flg == ext_flg)
            res = true;
        q.pop();
    }
    return res;
}

///
/// \param flg
/// \return
list<pair<swtch *, list<string *>>>
arg_hdlr::get_parsed(char flg) {
    list<pair<swtch *, list<string *>>> res;
    for (auto it = parsed_args.begin(); it != parsed_args.end(); ) {
        if(it->first->flg == flg){
            res.insert(res.end(),*it);
            it = parsed_args.erase(it);
        }else
            ++it;
    }

    return res;
}

///
/// \param ext_flg
/// \return
list<pair<swtch *, list<string *>>>
arg_hdlr::get_parsed(string &ext_flg) {
    list<pair<swtch *, list<string *>>> res;
    for (auto it = parsed_args.begin(); it != parsed_args.end(); ) {
        if(it->first->ext_flg == ext_flg){
            res.insert(res.end(),*it);
            it = parsed_args.erase(it);
        } else
            ++it;
    }
    return res;
}

///
/// \param desc
/// \param flag
/// \param num_args
swtch::swtch(string desc, char flag, int num_args)
{
    description = std::move(desc);
    flg = flag;
    fn = nullptr;
    n_args = num_args;
}

/// Empty Constructor for the swtch class
swtch::swtch()
{
    n_args = 0;
    fn = nullptr;
}

/// Destructor for the swtch class
swtch::~swtch()
{
    description.clear();
    if (!ext_flg.empty())
        ext_flg.clear();
}
