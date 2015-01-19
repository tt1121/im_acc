/** 文 件 名: im_wideband_speedup.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-27
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-27   张斌创建本文件；
##############################################################################*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "msg.h"
#include "im_wideband_speedup.h"
#include <time.h>

//#include "im_json_parse.h"



#define SOCKET_MODULE		"im_acc"

extern int im_cloud_login(char *url);
extern int im_get_local_ipv4(char *ifname, char *local_ip);
extern int im_handle_msg(char *p_msg, int acc_socket);
extern int im_auto_query_isacc();

const char *jason_key[]=
{
	"header",
	"cmd",
	"ver",
	"seq",
	"device",
	"code",
	"sessionid",
	"sign",
	"data",
	"dev_name",
	"isp",
	"cur_bandwidth",
	"acc_bandwidth",
	"acc_remaintime",
	"is_open",
	"max_bandwidth",
	"is_acc",
	"total_flow",
	"remain_flow",
	"unget_flow",
	"get_flow_id",
	"get_flow_info",
	"peak_flow",
	"is_keepacc",
	"acc_list",
	"use_time",
	"use_flow",
	"open_acc_time",
	"send_inform",
	"send_inform_Id",
	"send_inform_msg",
	"acc_time",
	"acc_url",
	"acc_state",
	"ip",
	"devicetoken",
	"devid",
	"task_id",
	"badge",
	"message",
	"rand",
	"module"
};

S_IM_MSG_HEADER s_im_cmd_header;
S_IM_TERMSG_DATA_SPEEDUP_CMD s_im_speedup;
S_IM_TERMSG_DATA_GETFLOW_CMD s_im_getflow;
S_IM_TERMSG_DATA_KEEPACC_CMD s_im_keepacc;
S_IM_TERMSG_DATA_IOSINFO_CMD s_im_iosinfo[MAX_IOS_DEV];


S_IM_CLOUD_DATA_LOGIN_CMD	    s_im_login_cmd;
S_IM_CLOUD_DATA_LOGIN_RESP	    s_im_login_resp;
S_IM_CLOUD_DATA_ACC_CMD		    s_im_acc;
S_IM_CLOUD_DATA_ISACC_RESP      s_im_isacc_resp;
S_IM_TERMSG_DATA_STARTACC_RESP  s_im_startacc_resp;
S_IM_TERMSG_DATA_FLOW_INFO_RESP s_im_flowinfo_resp;
S_IM_CLOUD_DATA_ACCSPEC_RESP    s_im_accspec;
S_IM_CLOUD_DATA_COUNTFLOW_CMD   s_im_countflow_cmd;
S_IM_CLOUD_DATA_GETAACFLOW_CMD  s_im_getaacflow_cmd;
S_IM_TERMSG_DATA_SPEEDUP_INFO_RESP s_im_speedupinfo_resp;
S_IM_TERMSG_DATA_UNGETFLOW_RESP s_im_ungetflow_resp;
S_IM_TERMSG_DATA_PEAKFLOW_RESP	s_im_peakflow_resp;
S_IM_TERMSG_DATA_ACCLIST_RESP	s_im_acclist_resp;
S_IM_CLOUD_DATA_HEARTBEAT_RESP	s_im_heartbeat_resp;
S_IM_CLOUD_DATA_TASKRESULT_CMD  s_im_taskresult_cmd;


/******************************************************************
function:   查询网络是否改变，改变返回MSG_CODE_SUCESS
params:
return: MSG_CODE_SUCESS, MSG_CODE_FORMAT_ERR
*********************************************************************/
int im_check_isnet_change(void)
{
	int ret = MSG_CODE_SUCESS;
	
	config_read_im_acc(NULL);

	if(!strcmp(uci_save_ip,s_im_login_resp.im_ip))
		ret = MSG_CODE_UNKNOWN;
	printf("uciip=%s,loginip=%s ret=%d\n",uci_save_ip,s_im_login_resp.im_ip,ret);

	return ret;
}

void im_init_session(void)
{
	strcpy(s_im_acc.im_session_id, s_im_login_resp.im_session_id);
	strcpy(s_im_countflow_cmd.im_session_id, s_im_login_resp.im_session_id);
	strcpy(s_im_getaacflow_cmd.im_session_id, s_im_login_resp.im_session_id);
	strcpy(s_im_taskresult_cmd.im_session_id, s_im_login_resp.im_session_id);
}

int im_windband_init(void)
{
	int ret = 0;
	
	im_init_session();

//初始化查询/开启加速结构体
	strcpy(s_im_acc.im_dev_id, s_im_login_cmd.im_dev_id);
	strcpy(s_im_acc.im_ip, s_im_login_cmd.im_ip);
	
	s_im_speedupinfo_resp.im_acc_state = STATE_ACC_CLOSED;

	return ret;
}

char * get_format_time()
{
	static char tstr[64] = {0};
	time_t t;		
	
	t = time(NULL);	
	strcpy(tstr, ctime(&t));	
	tstr[strlen(tstr)-1] = '\0';
	
	return tstr;
}


void im_check_internet_connect(void)
{
	int ret = 0;
	char ipaddr[MAX_IP_LEN] = {0};
	
	//当外网连接成功才往下执行
	do
	{
		ret = im_get_local_ipv4(WAN_IFNAME, ipaddr);
		
		if (ret != MSG_CODE_SUCESS)
		{
			sleep(10);
		}
		
	}while(ret != MSG_CODE_SUCESS);
	
}

void im_start_login_cloud(char *url)
{
	int ret = MSG_CODE_SUCESS;
	
	do
	{
		ret = im_get_local_ipv4(LAN_IFNAME, s_im_login_cmd.im_ip);
		if (ret != MSG_CODE_SUCESS)
		{
			IM_DEBUG("im_get_local_ipv4 lanip fail");
			sleep(20);
			continue;
		}
		
	} while(ret != MSG_CODE_SUCESS);
	
	strcpy(s_im_login_cmd.im_dev_id, "IMOVE00000001");
	ret = im_cloud_login(url);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("login fail!!!!");
		//sleep(10);
	}
}

int im_isacc_init(void)
{
	int ret = MSG_CODE_SUCESS;
	
#if 0	
	ret = im_check_isnet_change();
	if(ret != MSG_CODE_SUCESS) //网络未改变不需要查询
	{
		return ret;
	}
#endif
	im_auto_query_isacc();

	return ret;
}


int main (int argc, char *argv[])
{
	int server_socket = -1;
	int accept_socket = -1;
	int ret = -1;
	
	//char *url = "http://172.16.2.99:8000";
	/*消息格式为:{"header":{"cmd":%d, "ver":%d, "seq":%d, 
	 "device":%d, "appid": %d,"code":%d, "sessionid":"%s", "sign":"%s"}, 
	 "data":[{""}]}
	*/
	
	char *p_msg = NULL;

	im_check_internet_connect();
	im_start_login_cloud(CLOUD_URL);
	im_windband_init();
	im_isacc_init();

	init_alarm_post_config();    
	
	server_socket = IM_DomainServerInit(SOCKET_MODULE);  //初始化本地通信socket
	if (server_socket < 0)
	{
		IM_DEBUG("IM_DomainServerInit error");
		return -1;
	}
	while(1)
    {
    	accept_socket = IM_ServerAcceptClient(server_socket);
		if (accept_socket < 0 )
		{
			if (accept_socket == EINTR)
			{
				continue;
			}
			else
			{
				IM_DEBUG("IM_ServerAcceptClient ERROR");
				return -1;
			}
		}
		
		ret = IM_MsgReceive(accept_socket, &p_msg, NULL);	
		if (ret < 0)
        {
            IM_DEBUG("msg receive failed accept_socket = %d!\n",  accept_socket);
            IM_DomainServerDeinit(accept_socket);
			continue;
        }
	
		printf("p_msg :%s\n",p_msg);
		ret = im_handle_msg(p_msg, accept_socket); //解析消息成能识别的命令
		if (ret != MSG_CODE_SUCESS)
		{
			IM_DEBUG("im_handle_msg fail");
		}
		
		IM_FREE(p_msg);
	    IM_DomainServerDeinit(accept_socket);
	}


	return 0;
}

