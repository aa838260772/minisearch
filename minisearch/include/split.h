#ifndef _SPLIT_H_
#define _SPLIT_H_
#include <utility>
#include "MixSegment.hpp"
#include <map>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include <set>
#include <algorithm>

typedef struct qelem
{
    std::string word;
    int frequence;
    bool operator<(const qelem &other) const
    {
        return frequence < other.frequence;
    }
}qelem;


class splitpagelib
{
    public:
        splitpagelib(const std::string &libname,
                const std::string &indexname,
                const std::string &stopfilename,
                const std::string &splitfilename
                )
                :libname_(libname),
            indexname_(indexname),
            stopfilename_(stopfilename),
            splitfilename_(splitfilename)
    {}
        void startsplit(const std::string& dic_path, 
                        const std::string& model_path);
        void deletesame();
        void newindexlib(const std::string&,
                         const std::string&);
        void make_reverse_index(const std::string& ,
                                const std::string&);
    private:
        void compute_power();
        bool ifsame(const std::map<std::string, int> &,
                const std::map<std::string, int> &);
        void topten(int, 
                    std::ofstream &, 
                    const std::map<std::string, int> &);
        void weightogether();

        std::string libname_;//从lib文件读内容
        std::string indexname_;//从index读每篇文档相应的offset，读出相应文章
        std::string stopfilename_;//停用词文件
        std::string splitfilename_;//分词后的文件 

        std::set<std::string> set_stop;//读入停用词
        std::map<int, std::string> map_doc;//根据索引把每篇doc读入map
        std::map<int, std::map<std::string, int> > map_lib;
        std::map<std::string, int> words;//将去去重后的所有单词及频数写入map
        std::map<int, std::map<std::string, double> >doc_word_wei;
        std::map<std::string, std::map<int, double> >rev_index;//倒排索引
        std::map<std::string, std::map<int, int> > map_word_fre;
};  

#endif /*SPLIT_H*/ 


