#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <climits>


//Параметры запуска программы
struct execParamsStruct
{
    int urlCount = INT_MAX;
    std::string inPath;
    std::string outPath;
};

struct urlStruct
{
    int count = 1;
    std::string url;

    bool operator==(const std::string& url) const
    {
        return (url == this->url);
    }

    bool operator< (const urlStruct& m) const
    {
        return count < m.count;
    }
};

int main(int argc, char* argv[])
{
    //Заполнение структуры с параметрами программы
    execParamsStruct params;
        for(int i = 1; i < argc; ++i)
        {
            std::string arg(argv[i]);
            if((arg == "-n") && (params.urlCount == INT_MAX))
            {
                params.urlCount = std::stoi(argv[++i]);
                continue;
            }
            if(params.inPath == "")
            {
                params.inPath = arg;
                continue;
            }
            if(params.outPath == "")
            {
                params.outPath = arg;
                continue;
            }
        }
    if (params.inPath == "" || params.outPath == "")
    {
        std::cout << "Exec example: unigine_school [-n NNN] in.txt out.txt" << std::endl;
        return -1;
    }
        std::ifstream inFile(params.inPath, std::ios::in);
    std::ofstream outFile(params.outPath, std::ios::out);
    if (!inFile.is_open())
    {
        std::cout << "Can't open file: " << params.inPath << std::endl;
        return -2;
    }
    if (!outFile.is_open())
    {
        std::cout << "Can't open file: " << params.outPath << std::endl;
        return -3;
    }
    //Словарь с доменами
    std::list<urlStruct> domains;
    //Словарь с путями
    std::list<urlStruct> paths;

    //Счетчик обработанных url
    int count = 0;

    std::string line;
    while(std::getline(inFile, line))
    {
        while(true) {
            std::size_t beginFound = line.find("http://");
            if ((beginFound == -1))
            {
                beginFound = line.find("https://");
            }
            if ((beginFound == -1))
            {
                break;
            }
            std::string url = line.substr(beginFound, line.find("\t") - beginFound);
            line.erase(0, line.find("\t") + 1);
            url = url.substr(url.find("//") + 2);
            std::string domain = url.substr(0, url.find("/"));
            std::string path = url.substr(url.find("/"), url.find("?") - url.find("/"));

            auto i = std::find(domains.begin(), domains.end(), domain);
            if (i != domains.end())
            {
                i->count++;
            } else
            {
                urlStruct newDomain;
                newDomain.url = domain;
                domains.push_back(newDomain);
            }

            i = std::find(paths.begin(), paths.end(), domain);
            if (i != paths.end())
            {
                i->count++;
            } else
            {
                urlStruct newPath;
                newPath.url = path;
                paths.push_back(newPath);
            }
            count++;
        }
    }
    inFile.close();

    outFile << "total urls " << count << ", domains " << domains.size() << ", paths " << paths.size()
            << std::endl << std::endl;
    outFile << "top domains" << std::endl;

    domains.sort();
    if (params.urlCount != INT_MAX)
    {
        domains.resize(params.urlCount);
    }
    for(auto i = domains.begin(); i != domains.end(); ++i)
    {
        outFile << i->count << " " << i->url << std::endl;
    }

    outFile << std::endl << "top paths" << std::endl;

    paths.sort();
    if (params.urlCount != INT_MAX)
    {
        paths.resize(params.urlCount);
    }
    for(auto i = paths.begin(); i != paths.end(); ++i)
    {
        outFile << i->count << " " << i->url << std::endl;
    }

    outFile.close();
    return 0;
}