#ifndef RIMEDATA_HPP
#define RIMEDATA_HPP
#include <map>
#include <string>

typedef std::map<std::string, std::string> RimeSchemaInfo;

class RimeData
{
private:
#ifdef OS_WINDOWS
    const char sp='\\';
#else
    const char sp='/';
#endif
    bool has_schema;
    std::string logfile;
    static std::string tmpdir;
    static std::string userdata;
    RimeSchemaInfo schemainfo;
public:
    RimeData();
    std::string GetUserData()
    {
        return userdata;
    }
    std::string GetTmpDir()
    {
        return tmpdir;
    }
    bool SchemaExist(const char* name);
    bool UpdateRimeSchemaInfo();
    const RimeSchemaInfo& GetRimeSchemaInfo()
    {
        return schemainfo;
    }
    bool CloneSchema(const char* schemaname);
};

#endif // RIMEDATA_HPP
