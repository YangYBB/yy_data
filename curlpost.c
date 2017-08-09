#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define POSTDATA "{\"username\":\"ace\", \"passwd\":\"123123\", \"type\":\"10\"}"

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
	FILE *fp = (FILE *)userdata;
	
	int data_len = size * nmemb;
	int fwrite_len = 0;

	fwrite_len = fwrite(ptr,size,nmemb,fp);
	printf("读到的长度:%d\n",data_len);
	printf("写入的长度:%d\n",fwrite_len);
	
	return fwrite_len;
}

int main(void){
	//将读到的内容写到文件中
	FILE *fp = NULL;
	
	//创建CURL句柄
	CURL *curl = NULL;
	
	CURLcode res;

	curl = curl_easy_init();
	if(curl == NULL){
		return -1;
	}

	fp = fopen("httpreturn.txt","w");
	
	//组织通讯url post形式，回掉处理回传的数据，将内容直接写到文件，传个文件指针
	curl_easy_setopt(curl,CURLOPT_URL,"http://0.0.0.0:8888/");
	curl_easy_setopt(curl,CURLOPT_POST,1);
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,POSTDATA);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_callback);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
	
	//执行
	res = curl_easy_perform(curl);
	if(res != CURLE_OK){
		printf("perform error res = %d\n",res);
	}

	curl_easy_cleanup(curl);

	fclose(fp);

	return 0;
}
