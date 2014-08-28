#include "split.h"
#include <stdio.h>
using namespace std;
using namespace CppJieba;

void splitpagelib::startsplit(const string &dic_path, 
        const string &model_path)    
{
    MixSegment  segment_(dic_path, model_path);
    //读入停用词到set_stop中去
    ifstream is_stop(stopfilename_.c_str());
    string stopword;
    while(getline(is_stop, stopword) > 0)
    {
        set_stop.insert(stopword);
    }
    //通过index来将lib中文档读出并分词，统计出现最高的10个 放入map中
    vector<string> words;//用来存放切割后的单词
    ifstream is_lib(libname_.c_str());
    ifstream is_index(indexname_.c_str());
    ofstream os_split(splitfilename_.c_str());
    string lib_line, index_line;
    cout << "begin to cut word :" << indexname_ 
         <<" " << libname_ << endl;
    while(getline(is_index, index_line) > 0)
    {
        map<string, int> map_;
        int docid, offset, size;
        istringstream ss(index_line);
        ss >> docid >> offset >> size;
        is_lib.seekg(offset);
        string every_doc = "";
        while(is_lib.tellg() < (offset + size))
        {
            getline(is_lib, lib_line);
            every_doc += lib_line + "\n";
            for(size_t i = 0; i < lib_line.size(); i++)
            {
                if(!(lib_line[i] & (1 << 7)))
                    lib_line[i] = ' ';
            }

            segment_.cut(lib_line.c_str(), words);
            for(size_t i = 0; i < words.size(); i++)
            {
                set<string>::iterator it = set_stop.find(words[i]);
                if(it == set_stop.end() && words[i] != " ")//不去掉的话把空格当单词
                    ++map_[words[i]];
            }
            words.clear();
        }
        os_split << docid << endl;
        topten(docid, os_split, map_);
        os_split << endl;
        map_doc[docid] = every_doc;//将每篇文章读入map_doc
    }
    os_split.close();
    is_lib.close();
    is_index.close();
    cout << "cut word over :write to"  << splitfilename_ << endl;
}

void splitpagelib::topten(int docid,
        ofstream &os,
        const map<string, int> &map_)
{
    priority_queue<qelem> prique_;
    qelem ql;
    for(map<string, int>::const_iterator it = map_.begin(); 
            it != map_.end(); 
            ++it)
    {
        ql.word = it->first;
        ql.frequence = it->second;
        prique_.push(ql);
    }

    int i = 0;
    while(i < 10 && !prique_.empty())
    {
        ql = prique_.top() ;
        map_lib[docid][ql.word] = ql.frequence;
        prique_.pop();
        i++;
        os << ql.word << " " << ql.frequence << " ";
    }
}

void splitpagelib::deletesame()
{    //去重
    cout << "begin to delete the same" << endl;
    int *arr = new int [map_lib.size() + 1];
    for (size_t i = 1; i < map_lib.size() + 1; i++) 
    {
        arr[i] = 1;
    }
    size_t ix1, ix2;
    for(ix1 = 1; ix1 < map_lib.size() + 1; ++ix1)
    {
        if(arr[ix1] == 0)
            continue; 
        for (ix2 = ix1 +1; ix2 < map_lib.size() + 1; ix2++) 
        {
            if(arr[ix2] == 0)
                continue;
            if(ifsame(map_lib[ix1], map_lib[ix2]))
            {
                arr[ix2] = 0;
                map_lib.erase(ix2);
            }
        }        
    }
    delete [] arr;
    cout << "delsanme over" << endl;
}

bool splitpagelib::ifsame(const map<string, int>& map1, 
        const map<string, int>& map2)
{
    int i = 0;
    for(map<string, int>::const_iterator it = map2.begin(); it != map2.end(); ++it)
    {
        if(map1.count(it->first) > 0)
            i++;
    }
    if(i > 6)
        return true;
    else
        return false;
}

void splitpagelib::newindexlib(const string& index,
                        const string& lib)
{
    ofstream os_index(index.c_str());
    ofstream os_lib(lib.c_str());

    map<int, map<string, double> >::iterator it;
    size_t i = 1;
    cout << "begin write to index lib" << index 
         << " " << lib <<  endl;
    for(it = doc_word_wei.begin(); it != doc_word_wei.end(); it++)
    {
        os_index << i << " " << os_lib.tellp() << " " ;
        
        int begin = map_doc[it->first].find("<docid>");
        int end = map_doc[it->first].find("</docid>");
        char s[10] = {0};
        sprintf(s, "%d", i);
        map_doc[it->first].replace(begin + 7, end - begin - 7, s);
        os_lib << map_doc[it->first] << endl;

        os_index << map_doc[it->first].size() << endl;//写index    
        i++;
    }
    cout << "write over" << endl;
    os_index.close();
    os_lib.close();
}

void splitpagelib::compute_power()
{
    map<int, map<string, int> >::iterator it1;    
    map<string, int>::iterator it2 ;
    for(it1 = map_lib.begin(); it1 != map_lib.end(); it1++)
    {
        for(it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
        {
            map_word_fre[it2->first][it1->first] = it2->second;
        }
    }

    for(it1 = map_lib.begin(); it1 != map_lib.end(); it1++)
    {
        for(it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
        {
            double d = (it2->second) * log( (double)(map_word_fre.size()) / (double)(map_word_fre[it2->first].size() +1) );
            doc_word_wei[it1->first][it2->first] = d;
        }
    }
}

void splitpagelib::make_reverse_index(const string &index,
                                        const string &power)
{
    //计算每篇中的权重
    compute_power();
    //对每篇文档的词作归一化
   weightogether();
   //建立倒排索引
   int j = 1;
   for(map<int, map<string, double> >::iterator it = doc_word_wei.begin();
           it != doc_word_wei.end();
           it++)
   {
       for(map<string, double>::iterator it1 = it->second.begin(); 
               it1 != it->second.end(); 
               ++it1)
       {
           rev_index[it1->first][j] = it1->second;  
       }
       j++;
   }
    cout << "write reverse index" << index <<endl;
    ofstream os_index(index.c_str());
    for(map<string, map<int, double> >::iterator it = rev_index.begin(); 
            it != rev_index.end(); 
            ++it)
    {
        os_index << it->first << endl;
        for(map<int, double> ::iterator it1 = it->second.begin(); 
                it1 != it->second.end(); 
                ++it1)
        {
            os_index << it1->first << " " << it1->second << " ";
        }
        os_index << endl;
    }
    os_index.close();
    cout << "write reverse index over" << endl;

    cout << "write doc_word_wei begin" << power << endl;
    ofstream os_power(power.c_str());
    map<int, map<string, double> >::iterator it = doc_word_wei.begin();
    map<string, double>::iterator it1;
    int i = 1;
    for(; it != doc_word_wei.end(); ++it)
    {
        os_power << i << endl;
        for(it1 = it->second.begin(); it1 != it->second.end(); ++it1)
        {
            os_power << it1->first << " " << it1->second << " " ;
        }
        os_power << endl;
        i++; 
    }
    os_power.close();
    cout << "write power end" << endl;
}

void splitpagelib::weightogether()//归一化
{
    map<int, map<string, double> >::iterator it;
    map<string, double>::iterator it1;
    double s;
    for(it = doc_word_wei.begin(); it != doc_word_wei.end(); it++)
    {
        s = 0;
        for(it1 = it->second.begin(); it1 != it->second.end(); it1++)
        {
            s += it1->second * it1->second;
        }

        for(it1 = it->second.begin(); it1 != it->second.end(); it1++)
        {
            it1->second /= sqrt(s);
        }
      }
}
