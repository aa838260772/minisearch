#ifndef _QUERY_H_
#define _QUERY_H_

#include <map>
#include "MixSegment.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>

typedef struct simi
{
    int docid;
    double simidegre;
    bool operator<( const simi &right)const
    {
        return simidegre < right.simidegre;
    }
}simi;

class query
{
    public:
        query(){}
     void read_to_map(const std::string&,
           const std::string&,
           const std::string&,
           const std::string&);
     std::string search(const std::string&);
    private:
     static void similar(std::map<std::string, double>&,
                         std::map<std::string, double>&,
                         int,
                         std::priority_queue<simi>&);
     
      std::string offsetfile_;
      std::string reverseindexfile_;
      std::string docfile_;
      std::string doc_weifile_;
     
      std::map<std::string, std::map<int, double> > map_reindex;//存储倒排索引
      std::map<int, std::map<std::string, double> > map_wei;
      std::map<int, std::string> map_doc;
};

#endif /*QUERY_H*/ 
