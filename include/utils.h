//
// Created by polit on 3/24/2021.
//

#ifndef RGXRP_UTILS_H
#define RGXRP_UTILS_H
#include <string>

class utils {
public:
    static std::string file_to_str(const std::string& file_name);
    static std::string replace_nl( std::string st, const char repl);
    static std::string restore_nl(std::string st,const char tok);
    static void        rm_dbl_spcs(std::string &s );
    static std::string stdin_to_str();
    static void        save_to_file(std::string &fname, std::string &content);
};


#endif //RGXRP_UTILS_H
