#include "query.h"

using namespace std;
using namespace CppJieba;

void query::read_to_map(
         const string &offsetfile,
        const string &reverseindexfile,
        const string &docfile,
        const string &doc_weifile)
{
  offsetfile_ = offsetfile;
  reverseindexfile_ = reverseindexfile;
  doc_weifile_ = doc_weifile;
  docfile_ = docfile;

    //读倒排索引文件
    ifstream is_revindex(reverseindexfile_.c_str());
    string line;
    while(getline(is_revindex, line) > 0)
    {
        string word;
        word = line;
        getline(is_revindex, line) ;
        istringstream sstream(line);
        int docid;
        double weight;
        while(sstream >> docid >> weight)
        {
            map_reindex[word][docid] = weight;
        }
    }
    is_revindex.close();

    //读偏移索引文件和doc文件，将每篇文档读入map
    ifstream is_offset(offsetfile_.c_str());
    ifstream is_doc(docfile_.c_str());
    string line_off;
    while(getline(is_offset, line_off) > 0)
    {
        istringstream sstream(line_off);
        int docid;
        int offset;
        int size;
        sstream >> docid >> offset >> size;
        is_doc.seekg(offset);
        string every_doc = "";
        while(is_doc.tellg() < (offset + size))
        {
            string line;
            getline(is_doc, line);
            every_doc += line + "\n";
        }
        map_doc[docid] = every_doc;
    }
    is_offset.close();
    is_doc.close();
    //读每篇doc归一化后的top10词文件
    ifstream is_map_wei(doc_weifile_.c_str());
    string line_wei;
    while(getline(is_map_wei, line_wei) > 0)
    {
        int docid;
        istringstream ss(line_wei);
        ss >> docid;
        getline(is_map_wei, line_wei);
        istringstream sstream(line_wei);
        string word;
        double weight;
        while(sstream >>  word >> weight)
        {
            map_wei[docid][word] = weight;
        }
    }
    is_map_wei.close();
}

string query::search(const std::string &word)
{
    priority_queue<simi> result_doc;
    //切词,算出单词权重存储到一个map，相当于一片doc
    MixSegment segment("../dict/jieba.dict.utf8",
            "../dict/hmm_model.utf8");
    vector<string> words;
    map<string, double> map_que;
    segment.cut(word, words);
    double d = 0;
    for (size_t i = 0; i < words.size(); i++) 
    {
        double s = 1 * log( (double)map_reindex.size() / (double)(map_reindex[words[i]].size()+1) );
        cout << words[i] << s << endl;
        map_que[ words[i] ] = s;
        d += s*s;
    }
    for(size_t i = 0; i < words.size(); i++)
    {
        map_que[ words[i] ] /= sqrt(d);
    }
    //找出单词交集的docid 
    map<int, double> com(map_reindex[ words[0] ]);

    for (size_t i = 1; i < words.size(); i++) 
    {
        if(map_reindex[ words[i] ].size() == 0)
            continue;
        for(map<int, double>::iterator it = com.begin(); it != com.end(); ++it)
        {
            if(map_reindex[ words[i] ].count(it->first) == 0)
                com.erase(it->first);
        }
    }
    cout << com.size() << endl;
    for(map<int, double>::iterator it = com.begin(); it != com.end(); ++it)
    {
        similar(map_que, map_wei[it->first], it->first, result_doc);
    }

    simi si;
    string result = "";
    while(!result_doc.empty())
    {
        si = result_doc.top();
        char s[20] = {0};
        sprintf(s, "%d %f\n", si.docid, si.simidegre);
        result += s + map_doc[si.docid];
        result_doc.pop();
    }
    return result;
}

void query::similar(map<string, double> &map_doc1,
                    map<string, double> &map_doc2,
                    int docid,
                    priority_queue<simi> &result_doc)
{
    double s = 0;
    for(map<string, double>::iterator it = map_doc1.begin();
                                    it != map_doc1.end();
                                        ++it)
    {
        if(map_doc2.count(it->first) > 0)
        {
            s += it->second * map_doc2[it->first]; 
        }
    }
    simi si;
    si.docid = docid;
    si.simidegre = s;
    result_doc.push(si);
}

