#include "pagelib.h"
using namespace std;
int pagelib::i = 1;

void pagelib::readfile(const string &s)
{
    DIR *dir;
    struct dirent * mydir;
    struct stat mystat;
    char str[256];
    dir=opendir(s.c_str());
    if(!dir)
    {
        cout << s << endl;
        exit(-1);
    }

    while((mydir=readdir(dir))!=NULL)
    {
        sprintf(str, "%s/%s", s.c_str(), mydir->d_name);
        stat(str, &mystat);
        if(!strcmp(mydir->d_name, ".") || !strcmp(mydir->d_name, ".."))
            continue;
        if( mystat.st_mode & 0040000 ) 
        {
            string s1 = str;
            readfile(s1);
        }
        else
        {
            cout << str << endl;
            store_to_vector(str);
        }
    }
}

void pagelib::store_to_vector(const string &str)
{
    string content, line;
    ifstream is(str.c_str());

    string title;
    getline(is, title);
    {
        for (size_t i = 0; i < title.size(); i++) {
            if(title[i] == '\r')
                title[i] = ' ';
        }
    }

    while(getline(is, line) > 0)
    {
        for (size_t i = 0; i < line.size(); i++) {
            if(line[i] == '\r')
                line[i] = '\n';
        }
        content += line;
    }
    char s1[5];
    sprintf(s1, "%d", i++);
    string doc = string("<doc>\n  <docid>") + s1 + "</docid>\n   <url>" + string(str) 
        + "</url>\n   <title>" + title + "</title>\n     <content>\n" 
        + content + "     </content>\n</doc>\n\n";
    vec.push_back(doc);
    is.close();
}

void pagelib::store_to_lib(const string& index)
{
    ofstream os_lib(libname_.c_str());
    ofstream os_index(index.c_str());
    vector<string>::iterator it = vec.begin();
    int j = 1;
    os_lib << "<pagelib>\n" << endl;
    for(; it != vec.end(); it++)
    {
        os_index << j++ << " " << os_lib.tellp() ;
        os_lib << *it;
        os_index << " " << (*it).size() << endl;
    }
    os_lib << "</pagelib>\n" << endl;
    os_index.close();
    os_lib.close();
}

