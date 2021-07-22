#include <stdio.h>
#include <json-c/json.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

char website[] = "https://api.openweathermap.org/data/2.5/weather?q=";
char api[] = "" ; //Enter weather API
    

struct memory{
        char *response;
        size_t size;
};

void init_string(struct memory *chunk){
    chunk->size = 0;
    chunk->response = malloc(chunk->size + 1);
    if(chunk->response < 0){
        fprintf(stderr,"Error malloc()\n");
        exit(EXIT_FAILURE);
    }
    chunk->response[chunk->size] = '\0';
}

size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
    struct memory *mem = (struct memory *)userp;
    size_t realsize = size * nmemb;
    mem->response = realloc(mem->response,realsize + 1);
    if(mem->response < 0){
        fprintf(stderr,"Error realloc()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(mem->response+mem->size,data,realsize);
    mem->response[realsize] = '\0';
    mem->size = realsize;
    return realsize;
} 




void makeUrl(char *url,size_t size,char *new_url){
    char city[64];
    strcpy(city,new_url); 
    snprintf(url,size,"%s%s%s",website,city,api);
}

void http_response_error(const char *error,char *filename){
    const char http_resp_err[] = "HTTP response code said error";
    if(strcmp(http_resp_err,error) == 0)
    printf("%s\n",error);
}
