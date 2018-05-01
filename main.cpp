#include <string>
#include <iostream>
#include "rimedata.hpp"

void Usage(const char* sz)
{

    std::cout<<sz<<" -q"<<std::endl
             <<"\tQuery the available schema"<<std::endl;
    std::cout<<sz<<" schema"<<std::endl
             <<"\tInstall the specified schema"<<std::endl;
}

int main(int argc,char* argv[])
{
    if(argc == 1) {
        Usage(argv[0]);
        return 1;
    }

    RimeData rime;
    std::string arg = argv[1];

    if(arg.compare("-q") == 0) {
        rime.UpdateRimeSchemaInfo();
        int i=0;
        auto info=rime.GetRimeSchemaInfo();
        if(info.empty()) {
            std::cout<<"No Schema found."<<std::endl;
        } else {
            std::cout<<"The Following Schema is available:"<<std::endl;
            for(auto tr: info) {
                i++;
                std::cout<<"["<<i<<"]"<<tr.first<<"\t"<<tr.second<<std::endl;
            }
        }
    } else {
        if(!rime.SchemaExist(argv[1])) {
            std::cout<<"Schema "<<argv[1]<<" not found."<<std::endl;
            return 2;
        }
        rime.CloneSchema(argv[1]);
    }

    return 0;
}
