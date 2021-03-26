#ifndef RGXRP_ARG_HDLR_H
#define RGXRP_ARG_HDLR_H

#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <queue>
#include <list>

class swtch {
    public:

        char         flg                                 ;
        std::string  ext_flg                             ;
        std::string  description                         ;
        size_t       n_args                              ;
        void         (*fn)(std::list<std::string*>)      ;


        swtch()                                          ;
        swtch(std::string desc, char flag, int num_args) ;

        virtual ~swtch()                                 ;
};

typedef std::unordered_map<char, swtch*> map;
typedef std::unordered_map<std::string, map> map2d;

class arg_hdlr {
private:
    static map2d groups;

    void prepare(char flag, char **argv, int *pos, int max);
    void prepare(const std::string &ext_flag, char **argv, int *pos, int max);

public:
    std::deque<std::pair<swtch*,std::list<std::string*>>> parsed_args;

    static bool is_swtch(char flg);
    static bool is_swtch(std::string &ext_flg);


    bool is_present(char flg);
    bool is_present(std::string &ext_flg);

    static void pretty_print(std::list<std::string*> args);

    void parse(int n_args,char **argv);

    void add_swtch(char flag, std::string dsp, void (*fn)(std::list<std::string*>) = nullptr,
                   int n_args = 0      , const std::string &grp = "General");

    void add_swtch(char flag, std::string ext_flg    , std::string dsp     ,
                   void (*fn)(std::list<std::string*>) = nullptr, int n_args=0        ,
                   const std::string &grp = "General");

    std::list<std::pair<swtch*,std::list<std::string*>>> get_parsed(char flg);
    std::list<std::pair<swtch*,std::list<std::string*>>> get_parsed(std::string &ext_flg);

    virtual ~arg_hdlr();

};


#endif //RGXRP_ARG_HDLR_H
