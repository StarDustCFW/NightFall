/*Copyright (c) 2020 D3fau4

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <switch.h>
#include "net/net.hpp"
#include <cstring>
#include "FS/FS.hpp"
#include <borealis.hpp>

namespace Network
{
    static struct curl_slist *hosts = NULL;

    Net::Net()
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        hosts = curl_slist_append(NULL, "reinx.guide:167.99.228.103");
    }
	
	//Write file in mem to increase download speed Applet mode only has 40 ~ 50 MB but is ok 
	struct MemoryStruct
	{
	  char *memory;
	  size_t size;
	  FILE *fp;
	};

	static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userdata)
	{
	  size_t realsize = size * nmemb;
	  struct MemoryStruct *mem = (struct MemoryStruct *)userdata;

	  char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);

	  if (ptr == NULL)
	  {
		  printf("Failed to realloc mem\n");
		  printf("Writing... %luMb To file\n",mem->size / 1000000 + 1);
		  fwrite(mem->memory, 1, mem->size, mem->fp);
		  free(mem->memory);
		  mem->memory = (char*)malloc(1);
		  mem->size = 0;
		  ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
		  if (ptr == NULL) return 0;
		  return 0;
	  }
	 
	  mem->memory = ptr;
	  memcpy(&(mem->memory[mem->size]), contents, realsize);
	  mem->size += realsize;
	  mem->memory[mem->size] = 0;
	 
	  return realsize;
	}

    size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
    {
        data->append((char *)ptr, size * nmemb);
        return size * nmemb;
    }

    string Net::Request(string method, string url)
    {
        CURLcode res = CURLE_OK;
        CURL *curl = curl_easy_init();
        string response;
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "NightFall/1.0");
            curl_easy_setopt(curl, CURLOPT_RESOLVE, hosts);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return !res ? response : "{}";
    }

    bool Net::Download(string url, string filepath)
    {
		//if exist only for ncas
		if (filepath.find("/temp/") != string::npos){
			if (FS::checkFile(filepath)){
				//brls::Logger::debug("File  "+ filepath+" exist download abort");
				return false;
			}
		}
		std::string out=filepath+".tmp";
        CURLcode res = CURLE_OK;
        CURL *curl = curl_easy_init();

        if (curl)
        {
 			struct MemoryStruct chunk;
            chunk.fp = fopen(out.c_str(), "a");
            chunk.memory = (char*)malloc(1);
            chunk.size = 0;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_RESOLVE, hosts);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "NightFall/1.0");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
            res = curl_easy_perform(curl);
            fwrite(chunk.memory, 1, chunk.size, chunk.fp);// write from mem to file
			curl_easy_cleanup(curl);
			free(chunk.memory);
            fclose(chunk.fp);
        }
        else
        {
            res = CURLE_HTTP_RETURNED_ERROR;
        }
        if (res != CURLE_OK)
            remove(out.c_str());
		else{
			remove(filepath.c_str());
			rename(out.c_str(),filepath.c_str());
			remove(out.c_str());
		}
        return res == CURLE_OK ? false : true;
    }

    bool Net::HasInternet(){
        u32 strg = 0;
        nifmGetInternetConnectionStatus(NULL, &strg, NULL);
        return (strg > 0);
    }
} // namespace Network
