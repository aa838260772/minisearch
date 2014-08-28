#ifndef _PAGELIB_H_
#define _PAGELIB_H_
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

class pagelib
{
    public:
        pagelib(const std::string &dir, 
                const std::string &libname)
            :dir_(dir),
            libname_(libname)
    {}
        void readdirname()
        {
            readfile(dir_);
        }
        void store_to_lib(const std::string&);
    private:
        void readfile(const std::string &);
        void store_to_vector(const std::string&);
        std::string dir_;
        std::string libname_;
        static int i;
        std::vector<std::string> vec;
};

#endif /*PAGELIB_H*/ 
