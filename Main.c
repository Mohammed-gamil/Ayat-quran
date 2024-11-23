#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <C:/Curl/include/curl/curl.h>
#include <windows.h>
#include <stddef.h>
#include "cJSON.h"
#include <wchar.h>
#include <locale.h>
#include "resource.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};
 

typedef enum Format {
    DATA_SIZE,
    DATA,
    BYTES,
} Format;

// Function to convert UTF-8 to wide character string
wchar_t* utf8_to_wchar(const char* utf8_str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
    wchar_t* wstr = (wchar_t*)malloc(size_needed * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wstr, size_needed);
    return wstr;
}

char* ayah_url() {
    char* url = (char*)malloc(100 * sizeof(char));
    srand((unsigned int)time(NULL)); // Initialize random number generator
    int ayah_number = rand() % 6236 + 1; // Generate random number between 1 and 6236
    printf("Ayah number: %d\n", ayah_number);
    sprintf(url, "https://api.alquran.cloud/v1/ayah/%d", ayah_number);
    return url;
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

    struct MemoryStruct *memn = (struct MemoryStruct *)userp;

  return realsize;
}
 
int main(void)
{
  //make the code run every 1 minute
  while(1){

  setlocale(LC_ALL, "");
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = (char *)malloc(1);  /* grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */
  curl_handle = curl_easy_init();
  // get random number between 1 and 6236
  
  char* ayah_url();
 
  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, ayah_url());
 
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  /* disable SSL verification */
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

  /* get it! */
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    FILE *file = fopen("response.json", "w");
    if (file) {
        fwrite(chunk.memory, sizeof(char), chunk.size, file);
        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file for writing\n");
    }
    // printf("%s\n",chunk.memory);
    cJSON *ayah = NULL ;
    cJSON *data = NULL ;
    cJSON *json = cJSON_ParseWithLength(chunk.memory, chunk.size);
    data = cJSON_GetObjectItem(json, "data");
    ayah = cJSON_GetObjectItem(data, "text");
    // ayah = ayah->next ;
    char *ayah_r = cJSON_Print(ayah); ; 

     file = fopen("ayah.json", "w");
    if (file) {
        fwrite(ayah->valuestring, sizeof(char), strlen(ayah->valuestring) , file);
        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file for writing\n");
    }
    wchar_t *w_ayah = utf8_to_wchar(ayah->valuestring);
    MessageBoxW(NULL, w_ayah, L"Quran Ayah", MB_OK);
    free(w_ayah);
  }
 
  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);
 
  free(chunk.memory);
 
  /* we are done with libcurl, so clean it up */
  curl_global_cleanup();
  Sleep(60000);
  }
  return 0;
}
