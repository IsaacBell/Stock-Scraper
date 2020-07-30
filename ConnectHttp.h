
#pragma once
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/writer.h"
#include <iostream>
#include <string>
#include <vector>
#include "File.h"
#define CURL_STATICLIB
#include "curl/curl.h"
#ifdef _DEBUG
# pragma comment(lib,"curl/libcurl_a_debug.lib")
#else
# pragma comment(lib,"curl/libcurl_a.lib")
#endif
using namespace rapidjson;
#undef GetObject 
class ConnectHttp
{
public:
  ConnectHttp();
  ~ConnectHttp();
  std::string RequestReadJson(std::string url);
  void CRequestReadJson(std::string url, char *&content);
  void RequestWriteJson(std::string url, std::string path);
  void RequestAddJson(std::string url, std::string path,std::string key);
  std::string name;
  static const std::string path;
  static const std::string URL;
private:
  static bool b_init;
  static size_t Swriter(char*data, size_t size, size_t nmemb, std::string *writerData);
  static size_t Fwriter(void*ptr, size_t size, size_t nmemb, FILE* stream);
  static size_t Cwriter(char * data, size_t size, size_t nmemb, char *&writerData);
};
const std::string ConnectHttp::URL = "https://sandbox.iexapis.com/v1";
const std::string ConnectHttp::path = "Data/";
bool ConnectHttp::b_init = false;
ConnectHttp::ConnectHttp()
{
}
ConnectHttp::~ConnectHttp()
{
  //curl_global_cleanup();
  //ConnectHttp::b_init = true;
}

inline std::string ConnectHttp::RequestReadJson(std::string url)
{
  std::string content;
  CURL *curl = curl_easy_init();
  struct curl_slist *headers = NULL;
  if (curl) {
    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // /* Set headers */
    // headers = curl_slist_append(headers, "cache-control: no-cache");
    // headers = curl_slist_append(headers, "Connection: keep-alive");
    // headers = curl_slist_append(headers, "Host: sandbox.iexapis.com");
    // headers = curl_slist_append(headers, "Accept: */*");
    // headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate");
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    /* Set content */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Swriter);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return content;
}
inline void ConnectHttp::CRequestReadJson(std::string url, char *&content)
{
  CURL *curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Cwriter);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
}
// URL - target web address
// Path - File location to log output to
inline void ConnectHttp::RequestWriteJson(std::string url, std::string path)
{
  FILE *fp;
  CURL *curl = curl_easy_init();
  char errbuf[CURL_ERROR_SIZE];
  if (curl) {
    fp = fopen(path.c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Fwriter);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    /* provide a buffer to store errors in */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    /* set the error buffer as empty before performing a request */
    errbuf[0] = 0;
    CURLcode response = curl_easy_perform(curl);
    /* if the request did not complete correctly, show the error
    information. if no detailed error information was written to errbuf
    show the more generic information from curl_easy_strerror instead.
    */
    if(response != CURLE_OK) {
      size_t len = strlen(errbuf);
      fprintf(stderr, "\nlibcurl: (%d) ", response);
      if(len)
        fprintf(stderr, "%s%s", errbuf,
                ((errbuf[len - 1] != '\n') ? "\n" : ""));
      else
        fprintf(stderr, "%s\n", curl_easy_strerror(response));
    }
    fclose(fp);
    curl_easy_cleanup(curl);
  }
}
inline void ConnectHttp::RequestAddJson(std::string url, std::string path,std::string key)
{
  std::string snewsource = RequestReadJson(url);
  if (snewsource.size() < 5)
    return;
  char *newsource = new char[snewsource.length()+1];
  strcpy(newsource, snewsource.c_str());
  Document newdoc;
  newdoc.Parse(newsource);
  delete[] newsource;
  char* data = NULL;
  if (File::read(path, data)){
    Document old;
    old.Parse(data);
    std::string old_elem = old[old.Size() - 1][old[0][key.c_str()].GetInt()].GetString(); //////////////////////////// correct getstring() 
    for (Value::ConstValueIterator itr = newdoc.Begin(); itr != newdoc.End(); ++itr) {
      std::string name = (*itr)[key.c_str()].GetString();
      if (name.compare(old_elem)==0){
        newdoc.Erase(newdoc.Begin(), itr+1);
        break;
      }
    }
    Document temp(&old.GetAllocator());
    for (Value::ValueIterator itr = newdoc.Begin(); itr != newdoc.End(); ++itr) {
      temp.SetArray();
      std::cout << (*itr)["date"].GetString()<<  std::endl;
      for (Value::MemberIterator itr2 = itr->MemberBegin(); itr->MemberEnd() != itr2; ++itr2) {                            
        temp.PushBack(itr2->value, temp.GetAllocator());
      }
      old.PushBack(temp, old.GetAllocator());
    }
    temp.SetObject();

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    old.Accept(writer);
    newsource=strdup( buffer.GetString() );
    old.Clear();
  }
  else {
    Document translated;
    translated.SetArray();
    Document temp(&translated.GetAllocator());
    temp.SetObject();
    int i = 0;
    for (Value::MemberIterator itr2 = newdoc[0].MemberBegin(); newdoc[0].MemberEnd() != itr2; ++itr2) {
      temp.AddMember(itr2->name,i, temp.GetAllocator());
      i++;
    }
    translated.PushBack(temp, translated.GetAllocator());
    
    for (Value::ValueIterator itr = newdoc.Begin(); itr != newdoc.End(); ++itr) {
      temp.SetArray();
      for (Value::MemberIterator itr2 = itr->MemberBegin(); itr->MemberEnd() != itr2; ++itr2) {
        temp.PushBack(itr2->value, temp.GetAllocator());
      }
      translated.PushBack(temp, translated.GetAllocator());
    }
    temp.SetObject();
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    translated.Accept(writer);
    newsource=strdup( buffer.GetString() );
    translated.SetObject();
  }
  newdoc.Clear();
  File::write(path,newsource);
  delete[] newsource;
  delete[] data; 
}
inline size_t ConnectHttp::Swriter(char * data, size_t size, size_t nmemb, std::string *writerData)
{
  if (writerData == NULL)
    return 0;
  writerData->append(data, size*nmemb);
  return size * nmemb;
}
// Alternative for Cwriter below
// inline size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
//     size_t written;
//     written = fwrite(ptr, size, nmemb, stream);
//     return written;
// }
inline size_t Cwriter(char *ptr, size_t size, size_t nmemb, void *userdata) {
  auto writerData = new char[strlen(ptr)];
  strcpy(writerData, ptr);
  return strlen(writerData);
}
inline size_t ConnectHttp::Fwriter(void *ptr, size_t size, size_t nmemb, FILE * stream)
{
  size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}
inline size_t ConnectHttp::Cwriter(char * data, size_t size, size_t nmemb, char *& writerData)
{
  writerData = new char[strlen(data)];
  strcpy(writerData, data);
  return strlen(writerData);
}
