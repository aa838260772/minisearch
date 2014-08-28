#include "split.h"
using namespace std;

int main(int argc, const char *argv[])
{
    string s1 = "../dict/jieba.dict.utf8" ;
    string s2 = "../dict/hmm_model.utf8" ;
    splitpagelib sp("../data/pagelib.txt",
            "../data/index.txt",
            "../data/mystoplist.lib",
            "../data/split.txt");
    sp.startsplit(s1, s2);//分词
    sp.deletesame();//去重
    sp.make_reverse_index("../data/reverse_index.txt",
                         "../data/doc_wei.txt");//构建倒排索引
    sp.newindexlib("../data/index2.txt",
            "../data/pagelib2.txt");//构建新的网络库和文档偏移索引
    return 0;
}
