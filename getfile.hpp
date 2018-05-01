#ifndef GETFILE_HPP
#define GETFILE_HPP
#include <curl/curl.h>
#include <string>

class GetFile
{
private:
    CURL* curl;
    std::string data;
    static size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        std::string *str = dynamic_cast<std::string*>((std::string*)stream);
        if(0 == size || NULL == ptr) return -1;
        char* pData = (char*)ptr;
        str->append(pData,nmemb*size);
        return nmemb*size;
    }
public:
    GetFile(const char* url)
    {
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void* )&data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Rime Project");
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        return;
    };
    const std::string& GetData()
    {
        return data;
    };
};

#endif // GETFILE_HPP
