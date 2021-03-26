#ifndef RGXRP_HNDLR_FNS_H
#define RGXRP_HNDLR_FNS_H
#include <string>
#include <list>
#define REPLACE_BGIN "/R("
#define REPLACE_CLOS ')'

static bool g_line_mode = false;

class Hndlr_fns{
public:
    static void apply_expression(std::list<std::string*> args);
};

#endif //RGXRP_HNDLR_FNS_H
