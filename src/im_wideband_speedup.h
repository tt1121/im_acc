/** 文 件 名: im_update.h
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-27
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-27   张斌创建本文件；
##############################################################################*/
#ifndef __IM_WINDBAND_SPEEDUP_H__
#define  __IM_WINDBAND_SPEEDUP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAX_SESSIONID   33		//im_sessionid数组的长度
#define MAX_SIGN		33		//im_sign 数组的长度
#define MAX_STRING		33
#define MAX_URL_LEN		1024
#define MAX_NAME_LEN	MAX_STRING+8  //文件名叫扩展名
#define MAX_ACC_HIS		20		//最大加速历史保存个数
#define MAX_UDP_MSG		512		//广播消息的最大字节数
#define MAX_IP_LEN		16		//IP地址的最大字符长度

#define CMD_SESSIONID	"imove_home"
#define CLOUD_URL		"http://172.16.2.79:80"

#define IM_DEBUG_FLAG 1

#if IM_DEBUG_FLAG
#define IM_DEBUG(format, ...) \
    do { \
        fprintf(stderr, "[%s@%s,%d] " format "\n", \
             __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)  
#else
#define IM_DEBUG(format, ...)
#endif

//JSON命令KEY值索引
enum JSON_IM_CMD
{
	JSON_HEADER,
	JSON_CMD,
	JSON_VER,
	JSON_SEQ,
	JSON_DEVICE,
	JSON_CODE,
	JSON_SESSIONID,
	JSON_SIGN,
	JSON_DATA,
	JSON_DEV_NAME,
	JSON_ISP,
	JSON_CUR_BANDWIDTH,
	JSON_ACC_BANDWIDTH,
	JSON_ACC_REMAINTIME,
	JSON_IS_OPEN,
	JSON_MAX_BANDWIDTH,
	JSON_IS_ACC,
	JSON_TOTAL_FLOW,
	JSON_REMAIN_FLOW,
	JSON_GET_FLOW,
	JSON_GET_FLOW_ID,
	JSON_GET_FLOW_INFO,
	JSON_PEAK_FLOW,
	JSON_IS_KEEPACC,
	JSON_ACC_LIST,
	JSON_USE_TIME,
	JSON_USE_FLOW,
	JSON_OPEN_ACC_TIME,
	JSON_SEND_INFORM,
	JSON_SEND_INFORM_ID,
	JSON_SEND_INFORM_MSG,
	JSON_ACC_TIME,
	JSON_ACC_URL
};


//返回消息码类型
enum MSG_CODE  
{
	MSG_CODE_SUCESS,
	MSG_CODE_UNKNOWN,					//未知错误
	MSG_CODE_TIMEOUT,					//超时
	MSG_CODE_FORMAT_ERR,				//格式错误
	MSG_CODE_NO_ATTR,					//缺少属性
	MSG_CODE_NO_TRAFFIC,				//没有可领取的流量
	MSG_CODE_TRAFFIC_ACCEPTED,			//流量包已被领走
	MSG_CODE_ACCELERATED=10,			//该设备已被加速过
	MSG_CODE_ACCELERATING,				//该设备在加速中
	MSG_CODE_SPEEDUP_FAIL,				//加速失败，该地区不支持
	MSG_CODE_SPEEDUP_MAX				//加速失败，已达最大速度
};

enum CLOUD_MSG_CMD  
{
	CMD_DEV_LOGIN = 500,				//设备登录请求
	CMD_DEV_LOGOUT,						//设备登出
	CMD_HEART_BEAT,						//设备心跳
	CMD_IS_ACC,							//检测是否能加速
	CMD_START_ACC,						//开启加速
	CMD_STOP_ACC,						//停止加速
	CMD_COUNT_FLOW,						//加速流量统计
	CMD_GETACC_FLOW,					//获取加速流量接口
	CMD_GETFLOW_INFO,					//获取流量情况
	CMD_GET_ACCLIST,					//获取加速历史
	CMD_GET_ACCINFO						//获取加速详情510
};

enum TERM_MSG_CMD  
{
	TERM_CMD_START_ACC = 600,				//开启加速
	TERM_CMD_STOP_ACC,						//停止加速
	TERM_CMD_SPEEDUP_INFO,				//查询加速服务开启情况
	TERM_CMD_IS_ACC,							//检测是否能加速
	TERM_CMD_GETFLOW_INFO,				//查询流量信息
	TERM_CMD_UNGETTEDFLOW,				//查询未领取流量
	TERM_CMD_GETFLOW,					//领取流量
	TERM_CMD_PEAKFLOW,					//查询流量峰值
	TERM_CMD_KEEPACC,					//保持长时间加速
	TERM_CMD_GET_ACCLIST,					//获取加速历史
};

//加速历史结构数组
typedef struct
{
	int 		im_use_time;		//使用时间戳
unsigned long 	im_use_flow;		//使用的流量
unsigned long	im_open_acc_time;	//开启带宽加速时间

}S_IM_DATA_ACC_HISTROY;  

//查询未领取的流量
typedef struct
{
	char 	im_getflow_id[MAX_STRING];
	char	im_getflow_info[MAX_STRING];
}S_IM_TERMSG_DATA_GETFLOW;	

//发送通知数组
typedef struct
{
	char 	im_sendinform_id[MAX_STRING];
	char	im_sendinform_msg[MAX_UDP_MSG];
}S_IM_TERMSG_DATA_SENDINFORM;	


//交互命令结构体
typedef struct 
{
	int 	im_cmd;
	int 	im_ver;
	int 	im_seq;
	int 	im_device;
	int 	im_code;
	char 	im_sessionid[MAX_SESSIONID];
	char 	im_sign[MAX_SIGN];
}S_IM_MSG_HEADER;

/************************************
从终端接收到的消息
*************************************/
//开启加速或停止加速
typedef struct
{
	char 	im_dev_name[MAX_NAME_LEN];
}S_IM_TERMSG_DATA_SPEEDUP_CMD;  


//开启加速返回值结构
typedef struct
{
	int     im_isp;
	char 	im_cur_bandwidth[MAX_STRING];	//当前带宽
	char	im_bandwidth[MAX_STRING];		//加速后带宽
	int 	im_remaintime;					//加速剩余时间
}S_IM_TERMSG_DATA_STARTACC_RESP;  		


//查询加速服务开启情况返回结构
typedef struct
{
	int		im_is_open;						//是否开启
	int     im_isp;							//ISP提供商1电信，2联通
	char 	im_cur_bandwidth[MAX_STRING];	//当前带宽
	char	im_bandwidth[MAX_STRING];		//加速后带宽
	int 	im_remaintime;					//加速剩余时间
}S_IM_TERMSG_DATA_SPEEDUP_INFO_RESP;  		


//查询是否能加速返回结构
typedef struct
{
	int		im_is_acc;							//是否支持加速
	int     im_isp;								//ISP提供商1电信，2联通
	char 	im_cur_bandwidth[MAX_STRING];		//当前带宽
	char	im_max_bandwidth[MAX_STRING];		//加速后带宽
	int		im_acc_remaintime;					//加速剩余时间
}S_IM_TERMSG_DATA_ISACC_RESP;  		

//查询流量信息返回结构
typedef struct
{
unsigned long     	im_total_flow;			//总流量
unsigned long		im_remain_flow;			//剩余流量
	int				im_acc_remaintime;		//加速剩余时间
}S_IM_TERMSG_DATA_FLOW_INFO_RESP;  		

//查询未领取流量返回结构
typedef struct
{
	S_IM_TERMSG_DATA_GETFLOW     im_get_flow;			//未领取流量
}S_IM_TERMSG_DATA_UNGETTEDFLOW_RESP;  		


//领取流量请求结构
typedef struct
{
	char 	im_getflow_id[MAX_STRING];			//领取流量的ID
	char 	im_dev_name[MAX_NAME_LEN];			//客户端设备标识
}S_IM_TERMSG_DATA_GETFLOW_CMD;  		

//领取流量返回结构
typedef struct
{
	char 	im_dev_name[MAX_NAME_LEN];			//客户端设备标识
}S_IM_TERMSG_DATA_GETFLOW_RESP;  		


//查询流量峰值返回结构
typedef struct
{
	unsigned long		im_peak_flow;			//流量峰值
}S_IM_TERMSG_DATA_PEAKFLOW_RESP;  		


//保持长时间加速请求结构
typedef struct
{
	int		im_is_keepacc;			//是否保持长时间加速
}S_IM_TERMSG_DATA_KEEPACC_CMD;  		



//获取加速历史接口返回结构
typedef struct
{
	S_IM_DATA_ACC_HISTROY		acc_list[MAX_ACC_HIS];			//流量峰值
}S_IM_TERMSG_DATA_ACCLIST_RESP;  		




/************************************
发送到云端的消息
*************************************/
//设备登录请求结构
typedef struct
{
	char im_dev_id[MAX_STRING];			//设备ID
	char im_ip[MAX_IP_LEN];				//设备的IP
}S_IM_CLOUD_DATA_LOGIN_CMD; 		

//设备登录返回结构
typedef struct
{
	char im_session_id[MAX_STRING];		//会话ID
	int im_acc_time;					//已加速的时间
}S_IM_CLOUD_DATA_LOGIN_RESP; 		

//设备登出请求结构
typedef struct
{
	//此结构不使用，使用登录保存的sessionid 
	char im_session_id[MAX_STRING];			//会话ID
}S_IM_CLOUD_DATA_LOGOUT_CMD; 		

//心跳请求结构
typedef struct
{
	//此结构不使用，使用登录保存的sessionid
	char im_session_id[MAX_STRING];			//会话ID
}S_IM_CLOUD_DATA_HEARTBEAT_CMD; 		

//查询是否能加速/开启加速请求结构
typedef struct
{
	char im_session_id[MAX_STRING];			//会话ID
	char im_dev_id[MAX_STRING];			//设备ID
	char im_ip[MAX_STRING];				//设备的IP
}S_IM_CLOUD_DATA_ACC_CMD; 		

//加速流量统计
typedef struct
{
	char im_session_id[MAX_STRING];			//会话ID
unsigned long im_use_flow;					//此次加速已使用的流罧KB	
}S_IM_CLOUD_DATA_COUNTFLOW_CMD; 		


//获取加速流量接口
typedef struct
{
	char im_session_id[MAX_STRING];			//会话ID
	char im_task_id[MAX_STRING];			//此次加速已使用的流量	
}S_IM_CLOUD_DATA_GETAACFLOW_CMD; 		

//获取加速详情返回结构
typedef struct
{
	char im_acc_url[MAX_STRING];			//加速说明
}S_IM_CLOUD_DATA_ACCSPEC_RESP; 		






extern const char *jason_key[];
extern S_IM_TERMSG_DATA_SPEEDUP_CMD s_im_speedup;
extern S_IM_MSG_HEADER s_im_cmd_header;
extern S_IM_TERMSG_DATA_GETFLOW_CMD s_im_getflow;
extern S_IM_TERMSG_DATA_KEEPACC_CMD s_im_keepacc;

extern S_IM_CLOUD_DATA_LOGIN_CMD	 s_im_login_cmd;
extern S_IM_CLOUD_DATA_LOGIN_RESP	s_im_login_resp;
extern S_IM_CLOUD_DATA_ACC_CMD		 s_im_acc;
extern S_IM_TERMSG_DATA_ISACC_RESP  s_im_isacc_resp;
extern S_IM_TERMSG_DATA_STARTACC_RESP s_im_startacc_resp;
extern S_IM_TERMSG_DATA_FLOW_INFO_RESP s_im_flowinfo_resp;
extern S_IM_CLOUD_DATA_ACCSPEC_RESP s_im_accspec;
extern S_IM_CLOUD_DATA_COUNTFLOW_CMD s_im_countflow_cmd;
extern S_IM_CLOUD_DATA_GETAACFLOW_CMD s_im_getaacflow_cmd;
extern S_IM_TERMSG_DATA_SPEEDUP_INFO_RESP s_im_speedupinfo_resp;
#endif
