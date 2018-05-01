#include <boost/filesystem/operations.hpp>
#include <cstdlib>
#include <fstream>
#include <git2/clone.h>
#include <git2/global.h>
#include <json/reader.h>
#include <regex>
#include "getfile.hpp"
#include "rimedata.hpp"

std::string RimeData::tmpdir="";
std::string RimeData::userdata="";
const char* RepoURL="https://api.github.com/users/rime/repos";
const char* PreRepoURL="https://api.github.com/repos/rime/rime-";
const char* PreGitURl="https://github.com/rime/rime-";
const char* AppGitURl=".git";

RimeData::RimeData()
{
    using boost::filesystem::directory_entry;
    using boost::filesystem::directory_iterator;

    if(tmpdir.size()!=0) return;

#ifdef OS_WINDOWS
    tmpdir=getenv("TMP");
#else
    tmpdir=getenv("TMPDIR");
#endif

    boost::filesystem::path p(tmpdir);
    for (const directory_entry& x : directory_iterator(p)) {
        std::regex expr("^rime\\.\\w+.*INFO.*");
        if(std::regex_match(x.path().filename().c_str(), expr)) {
            logfile=x.path().c_str();
            break;
        }
    }

    std::ifstream i(logfile);
    while(std::getline(i, userdata)) {
        const char* ins="installation.yaml";
        auto idx=userdata.find(ins);
        if(idx!=-1) {
            userdata.erase(idx,userdata.size()-idx);
            auto ilen=userdata.find(sp);
#ifdef OS_WINDOWS
            ilen-=2;
#endif
            userdata.erase(0, ilen);
            break;
        }
    }
    i.close();

    return;

}

bool RimeData::SchemaExist(const char* name)
{
    std::string url=PreRepoURL;
    url+=name;
    GetFile f(url.c_str());

    auto i=f.GetData().find("\"message\":\"Not Found\"");
    if(i==1)return false;
    return true;
}

bool RimeData::UpdateRimeSchemaInfo()
{
    has_schema=false;
    GetFile f(RepoURL);
    const std::string s=f.GetData();
    if(s.empty()) return false;

    Json::CharReaderBuilder b;
    Json::CharReader* r = b.newCharReader();
    Json::Value root;
    b["collectComments"] = false;
    JSONCPP_STRING errs;

    if (!r->parse(s.data(), s.data()+s.size(), &root, &errs)) return false;

    for (int i=0; i<root.size(); i++) {
        std::string name = root[i]["name"].toStyledString();
        auto p = name.find("rime-");
        if(p==1) {
            std::string desc=root[i]["description"].toStyledString();
            schemainfo.insert({name.substr(6, name.size()-8),
                               desc.substr(1, desc.size()-3)
                              });
        }
    }

    return true;
}

bool RimeData::CloneSchema(const char* schemaname)
{
    using boost::filesystem::directory_entry;
    using boost::filesystem::directory_iterator;
    using boost::filesystem::copy_option;

    git_repository* repo = NULL;
    std::string url=PreGitURl;
    url=url+schemaname+AppGitURl;
    std::string gitdir;
    gitdir=tmpdir+PROJECT_NAME;

    if(!boost::filesystem::exists(gitdir)) {
        boost::filesystem::create_directories(gitdir);
    }

    git_libgit2_init();
    git_repository_init(&repo, gitdir.c_str(),false);
    gitdir=gitdir+sp+schemaname;
    boost::filesystem::create_directories(gitdir);
    git_clone(&repo, url.c_str(), gitdir.c_str(),NULL);
    git_repository_free(repo);
    git_libgit2_shutdown();

    boost::filesystem::path p(gitdir);
    std::string dst;
    for (const directory_entry& x : directory_iterator(p)) {
        std::regex expr("^.*\\.yaml");
        if(std::regex_match(x.path().filename().c_str(), expr)) {
            dst=userdata+x.path().filename().c_str();
            boost::filesystem::copy_file(x.path().c_str(),
                                         dst.c_str(),
                                         copy_option::overwrite_if_exists);
        }
    }

    boost::filesystem::remove_all(gitdir);
    return true;
}
