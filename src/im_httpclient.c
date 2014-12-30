/** 文 件 名: im_json_parse.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-28
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-28   张斌创建本文件；
##############################################################################*/

#include "im_wideband_speedup.h"
#include "curl/curl.h"



int im_curl_init(CURL **curlhandle)
{
	CURLcode res;

	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	res = curl_global_init(CURL_GLOBAL_ALL);
	if (res != CURLE_OK)
	{
		IM_DEBUG("curl_global_init fail:%s", curl_easy_strerror(res));	
		return -1;
	}
	
	*curlhandle = curl_easy_init(); 
	if (*curlhandle == NULL)
	{
		IM_DEBUG("curl_easy_init error\n");
		return -1;
	}

	return res;
}

int im_curl_clean(CURL *curlhandle)
{
	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();
}

int im_curl_perform(CURL *curlhandle)
{
	CURLcode res;
	long retcode = 0;

	if (curlhandle == NULL)
	{
		IM_DEBUG("PARAM ERROR");
		return -1;
	}
	
	res = curl_easy_perform(curlhandle);
	if (res != CURLE_OK)
	{
		  IM_DEBUG("%s\n", curl_easy_strerror(res));
		  return -1;
	}

	res = curl_easy_getinfo(curlhandle, CURLINFO_RESPONSE_CODE , &retcode);
	if ((res == CURLE_OK)&& (retcode == 200))	
	{
		IM_DEBUG("http ok retcode:%d", retcode);
	}
	else 
	{
		IM_DEBUG("fail %s  retcode:%d\n", curl_easy_strerror(res), retcode);
		return -1;
	}

	return 0;
	
}

size_t im_get_respone(void *ptr, size_t size, size_t nmemb, void *stream)  
{
	char *httpresp = (char *)stream;
	strcat(httpresp, ptr);
	//printf("%s\n", ptr);
	//int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	//return written;
	return strlen(ptr);
}

int im_curl_send_msg(CURL *curlhandle, char *url, char *postdata, char *httpresp, int timeout)
{
	struct curl_slist *headers = NULL;
	int ret = 0;
	
	if (!curlhandle || !url || !postdata)
	{
		IM_DEBUG("Bad param!!!");
		return -1;
	}
	
	headers = curl_slist_append(headers, "User-Agent: imove");
	headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");

	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
	curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, im_get_respone);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, httpresp);
	
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	//curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postdata); 

	ret = im_curl_perform(curlhandle);

	return ret;
}


int im_http_sendmsg(char *url, char *postdata, char *httpresp, int timeout)
{
	CURL *curlhandle = NULL;
	int res = 0; 
	
	if (!url || !postdata )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	res = im_curl_init(&curlhandle);
	if (res < 0)
	{
		IM_DEBUG("im_curl_init fail\n");
		return -1;
	}

	res = im_curl_send_msg(curlhandle, url, postdata, httpresp, timeout);
	if (res < 0)
	{
		IM_DEBUG("im_curl_send_msg fail\n");
		return -1;
	}

	res = im_curl_clean(curlhandle);
	if (res < 0)
	{
		IM_DEBUG("im_curl_clean fail\n");
		return -1;
	}

	return res;
}
