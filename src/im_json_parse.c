/** 文 件 名: im_json_parse.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-27
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-27   张斌创建本文件；
##############################################################################*/
#include "im_wideband_speedup.h"
#include "json-c/json.h"
#include "im_json_parse.h" 
#include <openssl/md5.h>




/*********************************************
function:   获取JSON对象的字符串值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_string(json_object *json_msg, const char *key, char *value)
{
	json_object *my_obj = NULL;
	char *p_msg = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	p_msg  = (char *)json_object_get_string(my_obj);
	strcpy(value, p_msg);

	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}

/*********************************************
function:   获取JSON对象的字符串值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_object_set_string(json_object *json_msg, const char *key, char *value)
{
	json_object *my_obj = NULL;
	char *p_msg = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	json_object_object_add(json_msg, key, json_object_new_string(value));

	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}

/*********************************************
function:   获取JSON对象的整型值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_int(json_object *json_msg, const char *key, int *value)
{
	json_object *my_obj = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	*value = json_object_get_int(my_obj);
	
	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}


/*********************************************
function:   获取JSON对象的整型值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_int64(json_object *json_msg, const char *key, unsigned long *value)
{
	json_object *my_obj = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	*value = json_object_get_int64(my_obj);
	
	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}

/*********************************************
function:   设置JSON对象的整型值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_object_set_int(json_object *json_msg, const char *key, int value)
{
	json_object *my_obj = NULL;
	
	if (!json_msg || !key )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	json_object_object_add(json_msg, key, json_object_new_int(value));
	
	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}
/*********************************************
function:   获取JSON对象的整型值
params:
[IN]json_msg:    json对象
[IN] key     :	  key值
[OUT] value  :        KEY所对应的值
return: -1，0
***********************************************/
int im_json_get_boolean(json_object *json_msg, const char *key, int *value)
{
	json_object *my_obj = NULL;
	
	if (!json_msg || !key || !value)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	my_obj = json_object_object_get(json_msg, key);
	if (my_obj == NULL)
	{
		IM_DEBUG("object_get fail");
		return MSG_CODE_FORMAT_ERR;
	}
	*value = json_object_get_boolean(my_obj);
	
	json_object_put(my_obj);
	return MSG_CODE_SUCESS;
}


/*********************************************
function:   解析JSON消息的头部
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************/

int im_json_parse_header(json_object *json_msg, S_IM_MSG_HEADER *s_im_cmd_header)
{
	json_object *json_header = NULL;
	int ret = MSG_CODE_SUCESS;
	
	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	json_header = json_object_object_get(json_msg, jason_key[JSON_HEADER]);
	if (json_header == NULL)
	{
		IM_DEBUG("json get header object fail");
		return MSG_CODE_FORMAT_ERR;
	}

	//解析消息头
	ret |= im_json_get_int(json_header, jason_key[JSON_CMD], &s_im_cmd_header->im_cmd);
	ret |= im_json_get_int(json_header, jason_key[JSON_VER], &s_im_cmd_header->im_ver);
	ret |= im_json_get_int(json_header, jason_key[JSON_SEQ], &s_im_cmd_header->im_seq);
	ret |= im_json_get_int(json_header, jason_key[JSON_DEVICE], &s_im_cmd_header->im_device);
	ret |= im_json_get_int(json_header, jason_key[JSON_CODE], &s_im_cmd_header->im_code);
	ret |= im_json_get_string(json_header, jason_key[JSON_SESSIONID], s_im_cmd_header->im_sessionid);
	ret |= im_json_get_string(json_header, jason_key[JSON_SIGN], s_im_cmd_header->im_sign);

	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse header get param fail");
		return MSG_CODE_FORMAT_ERR;
	}

	json_object_put(json_header);

	return ret;
}


/**********************************************************
function:   解析JSON消息的data中开启/关闭加速的消息
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/

int im_json_parse_data_speedup(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret = im_json_get_string(dev_obj, jason_key[JSON_DEV_NAME], s_im_speedup.im_dev_name);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/*********************************************
function:   解析JSON对象的data部分的getflow信息
params:
[IN]json_msg:    json消息对象
return: -1，0
***********************************************/

int im_json_parse_data_getflow(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	
	ret |= im_json_get_string(dev_obj, jason_key[JSON_GET_FLOW_ID], s_im_getflow.im_getflow_id);
	ret |= im_json_get_string(dev_obj, jason_key[JSON_DEV_NAME], s_im_getflow.im_dev_name);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}
	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}


/*********************************************************
function:   解析JSON对象的data部分的保持加速信息
params:
[IN]json_msg:    json消息对象
return: -1，0
**********************************************************/

int im_json_parse_data_keepacc(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret = im_json_get_int(dev_obj, jason_key[JSON_IS_KEEPACC], &s_im_keepacc.im_is_keepacc);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}
	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}


/*********************************************************
function:   解析JSON对象的data部分的保持加速信息
params:
[IN]json_msg:    json消息对象
return: -1，0
**********************************************************/

int im_json_parse_data_iosinfo(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;
	int count_dev = 0;
	char devicetoken[MAX_DEVICE_TOKEN] = {0};
	char devid[MAX_SESSIONID] = {0};
	
	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}

	
	ret |= im_json_get_string(dev_obj, jason_key[JSON_DEVICETOKEN], devicetoken);
	ret |= im_json_get_string(dev_obj, jason_key[JSON_DEVID],devid);

	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}
	else
	{
		for(count_dev = 0; count_dev < MAX_IOS_DEV; count_dev++)
		{
			printf("devicetoken[%d][%s]\n", count_dev, s_im_iosinfo[count_dev].devicetoken);
			if(strlen(s_im_iosinfo[count_dev].devicetoken) != 0)
			{
				if(strcmp(devicetoken, s_im_iosinfo[count_dev].devicetoken) == 0)
				{
					break;
				}
			}
		}

		if (count_dev >= MAX_IOS_DEV)
		{
			for(count_dev = 0; count_dev < MAX_IOS_DEV; count_dev++)
			{
				if(strlen(s_im_iosinfo[count_dev].devicetoken) == 0)
				{
					strcpy(s_im_iosinfo[count_dev].devicetoken, devicetoken);
					strcpy(s_im_iosinfo[count_dev].devid, devid);
					break;
				}
			}
		}
	}
	
	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}



/*********************************************
function:   解析JSON消息的DATA
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************/
int im_json_parse_data_terminal(json_object *json_msg, int cmd)
{
	int res = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	switch(cmd)
	{
			case TERM_CMD_START_ACC:
			case TERM_CMD_STOP_ACC:
			res = im_json_parse_data_speedup(json_msg);
			break;
		case TERM_CMD_GETFLOW:
			res = im_json_parse_data_getflow(json_msg);
			break;
		case TERM_CMD_KEEPACC:
			res = im_json_parse_data_keepacc(json_msg);
			break;
		case TERM_CMD_IOSINFO:
			res = im_json_parse_data_iosinfo(json_msg);
			break;
		//以下命令没有DATA部分
		//case TERM_CMD_SPEEDUP_INFO:
		//case TERM_CMD_IS_ACC:
		case TERM_CMD_ACC_STATE_INFO:
		case TERM_CMD_GETFLOW_INFO:
		case TERM_CMD_UNGETTEDFLOW:
		case TERM_CMD_PEAKFLOW:
		case TERM_CMD_GET_ACCLIST:
			break;
		default:
			IM_DEBUG("unkonow cmd");
			break;
	}

	return res;
}


/*********************************************
function:   解析JSON对象
params:
[IN]p_msg:    json字符串
return: -1，0
***********************************************/
int im_json_parse_terminal(const char *p_msg)
{
	json_object *json_msg = NULL;
	int ret = 0;

	if (p_msg == NULL)
	{
		IM_DEBUG("p_msg is null param error");
		return MSG_CODE_UNKNOWN;
	}
	
	json_msg = json_tokener_parse(p_msg);
    if (is_error(json_msg)) 
    {
        IM_DEBUG("json_tokener_parse failed!\n");
		return MSG_CODE_FORMAT_ERR;
    }
		
	
	ret = im_json_parse_header(json_msg, &s_im_cmd_header);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse header fail");
		goto headerfail;
	}

	ret = im_json_parse_data_terminal(json_msg, s_im_cmd_header.im_cmd);
	
#if 0
	printf("im_cmd:[%d][%d][%d][%d][%d][%d]\n[%s][%s][%s][%s][%s]\n",
		s_im_clould_cmd.im_cmd,s_im_clould_cmd.im_ver,s_im_clould_cmd.im_seq,
		s_im_clould_cmd.im_device,s_im_clould_cmd.im_appid,s_im_clould_cmd.im_code,
		s_im_clould_cmd.im_sessionid,s_im_clould_cmd.im_sign,s_im_data_firmware.im_sw_name,
		s_im_data_firmware.im_url,s_im_data_firmware.im_md5);
#endif

headerfail:
	json_object_put(json_msg);
	
	return ret;
}



/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_login_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;
	char cmd_acc_ip[254] = {0};

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret |= im_json_get_string(dev_obj, jason_key[JSON_SESSIONID], s_im_login_resp.im_session_id);
	ret |= im_json_get_int(dev_obj, jason_key[JSON_ACC_TIME], &s_im_login_resp.im_acc_time);
	ret |= im_json_get_string(dev_obj, jason_key[JSON_IP], s_im_login_resp.im_ip);

	//save ip address to uci config 
	sprintf(cmd_acc_ip,"uci set im_acc.isacc.im_ip=%s",s_im_login_resp.im_ip); 
	system(cmd_acc_ip);
	system("uci commit im_acc");

	im_init_session();
	
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}


/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_isacc_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}

	//不判断返回值，表示该值可有可无
	ret |= im_json_get_int(dev_obj, jason_key[JSON_IS_ACC], &s_im_isacc_resp.im_is_acc);
	im_json_get_int(dev_obj, jason_key[JSON_ISP], &s_im_isacc_resp.im_isp);
	im_json_get_int(dev_obj, jason_key[JSON_CUR_BANDWIDTH], &s_im_isacc_resp.im_cur_bandwidth);
	im_json_get_int(dev_obj, jason_key[JSON_MAX_BANDWIDTH], &s_im_isacc_resp.im_max_bandwidth);
	im_json_get_int(dev_obj, jason_key[JSON_ACC_REMAINTIME], &s_im_isacc_resp.im_acc_remaintime);
	printf("im_is_acc=%d im_isp=%d im_cur_bandwidth=%d im_max_bandwidth=%d im_acc_remaintime=%d\n",s_im_isacc_resp.im_is_acc,s_im_isacc_resp.im_isp,s_im_isacc_resp.im_cur_bandwidth,s_im_isacc_resp.im_max_bandwidth,s_im_isacc_resp.im_acc_remaintime);

	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_startacc_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	//不判断返回值，表示该值可有可无
	im_json_get_int(dev_obj, jason_key[JSON_ISP], &s_im_startacc_resp.im_isp);
	im_json_get_int(dev_obj, jason_key[JSON_CUR_BANDWIDTH], &s_im_startacc_resp.im_cur_bandwidth);
	im_json_get_int(dev_obj, jason_key[JSON_MAX_BANDWIDTH], &s_im_startacc_resp.im_max_bandwidth);
	im_json_get_int(dev_obj, jason_key[JSON_ACC_REMAINTIME], &s_im_startacc_resp.im_remaintime);
	im_json_get_int(dev_obj, jason_key[JSON_ACC_BANDWIDTH], &s_im_startacc_resp.im_acc_bandwidth);
	
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}



/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_getflowinfo_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret |= im_json_get_int64(dev_obj, jason_key[JSON_TOTAL_FLOW], &s_im_flowinfo_resp.im_total_flow);
	ret |= im_json_get_int64(dev_obj, jason_key[JSON_REMAIN_FLOW], &s_im_flowinfo_resp.im_remain_flow);
	
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_getacclist_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;
	int i = 0;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}

	for(i = 0; i < json_object_array_length(im_data); i++)
	{
		dev_obj = json_object_array_get_idx(im_data, i);
		
		ret |= im_json_get_int(dev_obj, jason_key[JSON_USE_TIME], &s_im_acclist_resp.acc_list[i].im_use_time);
		ret |= im_json_get_int64(dev_obj, jason_key[JSON_USE_FLOW], &s_im_acclist_resp.acc_list[i].im_use_flow);
		ret |= im_json_get_int64(dev_obj, jason_key[JSON_OPEN_ACC_TIME], &s_im_acclist_resp.acc_list[i].im_open_acc_time);
		if (ret != MSG_CODE_SUCESS)
		{
			IM_DEBUG("json parse data get param error");
		}
	}
#if 0	
	ret |= im_json_get_int64(dev_obj, jason_key[JSON_TOTAL_FLOW], &s_im_flowinfo_resp.im_total_flow);
	ret |= im_json_get_int64(dev_obj, jason_key[JSON_REMAIN_FLOW], &s_im_flowinfo_resp.im_remain_flow);
	ret |= im_json_get_int(dev_obj, jason_key[JSON_ACC_REMAINTIME], &s_im_flowinfo_resp.im_acc_remaintime);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}
#endif
	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_getaccinfo_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret = im_json_get_string(dev_obj, jason_key[JSON_ACC_URL], s_im_accspec.im_acc_url);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_ungetflow_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	/******************************************
	协议未定义解析待实现
	********************************************/
	
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_getflow_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	/******************************************
	协议未定义解析待实现
	********************************************/
	
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}

/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_heartbeat_resp(json_object *json_msg)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	//解析消息data
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json parse data get object fail");
		return MSG_CODE_FORMAT_ERR;
	}
	
	dev_obj = json_object_array_get_idx(im_data, 0);
	if (dev_obj == NULL)
	{
		IM_DEBUG("json parse data array_get_idx fail");
		json_object_put(im_data);
		return MSG_CODE_FORMAT_ERR;
	}
	
	ret = im_json_get_string(dev_obj, jason_key[JSON_IP], s_im_heartbeat_resp.im_ip);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse data get param error");
	}

	json_object_put(dev_obj);
	json_object_put(im_data);

	return ret;
	
}


/**********************************************************
function:   解析JSON消息的data
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************************/
int im_json_parse_data_taskresult_resp(json_object *json_msg, int cmd, int errcode)
{
	json_object *im_data = NULL;
	json_object *dev_obj = NULL;
	int ret = MSG_CODE_UNKNOWN;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return ret;
	}

	if((s_im_speedupinfo_resp.im_acc_state != STATE_ACC_OPENING)
		&&(s_im_speedupinfo_resp.im_acc_state != STATE_ACC_QUERYING))
	{
		IM_DEBUG("im_acc_state:%d error!!!!", s_im_speedupinfo_resp.im_acc_state);
		return ret;
	}

	switch (cmd)
	{
		//errcode  为MSG_CODE_MSGDELAY是解析如下两条
		case CMD_IS_ACC:
		case CMD_START_ACC:
			if (errcode != MSG_CODE_MSGDELAY)
			{	
				IM_DEBUG("cmd:%d, errcode:%d  error\n", cmd, errcode);
				break;
			}
			//解析消息data
			im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
			if (im_data == NULL)
			{
				IM_DEBUG("json parse data get object fail");
				ret =  MSG_CODE_FORMAT_ERR;
				break;
			}
			
			dev_obj = json_object_array_get_idx(im_data, 0);
			if (dev_obj == NULL)
			{
				IM_DEBUG("json parse data array_get_idx fail");
				json_object_put(im_data);
				ret =  MSG_CODE_FORMAT_ERR;
				break;
			}
			
			ret = im_json_get_string(dev_obj, jason_key[JSON_TASKID], s_im_taskresult_cmd.im_taskid);
			printf("taskid = %s\n",s_im_taskresult_cmd.im_taskid);
			if (ret != MSG_CODE_SUCESS)
			{
				IM_DEBUG("json parse data get param error");
			}

			json_object_put(dev_obj);
			json_object_put(im_data);
		break;

		case CMD_GET_TASKRESULT:
			if (errcode != MSG_CODE_SUCESS) //errcode 不是MSG_CODE_SUCESS没有DATA数据
			{	
				ret = MSG_CODE_SUCESS;
				IM_DEBUG("cmd:%d, errcode:%d\n", cmd, errcode);
				break;
			}

			if (s_im_speedupinfo_resp.im_acc_state == STATE_ACC_OPENING)
			{
				ret = im_json_parse_data_startacc_resp(json_msg);
				if (ret == MSG_CODE_SUCESS)
				{
					s_im_speedupinfo_resp.im_isp = s_im_startacc_resp.im_isp;
					s_im_speedupinfo_resp.im_cur_bandwidth = s_im_startacc_resp.im_cur_bandwidth;
					s_im_speedupinfo_resp.im_max_bandwidth = s_im_startacc_resp.im_max_bandwidth;
					s_im_speedupinfo_resp.im_acc_remaintime = s_im_startacc_resp.im_remaintime;
					s_im_speedupinfo_resp.im_acc_bandwidth = s_im_startacc_resp.im_acc_bandwidth;
				}
			}
			else if (s_im_speedupinfo_resp.im_acc_state == STATE_ACC_QUERYING)
			{
				ret = im_json_parse_data_isacc_resp(json_msg);
				if (ret == MSG_CODE_SUCESS)
				{
					s_im_speedupinfo_resp.im_is_acc = s_im_isacc_resp.im_is_acc;
					s_im_speedupinfo_resp.im_isp = s_im_isacc_resp.im_isp;
					s_im_speedupinfo_resp.im_cur_bandwidth = s_im_isacc_resp.im_cur_bandwidth;
					s_im_speedupinfo_resp.im_max_bandwidth = s_im_isacc_resp.im_max_bandwidth;
					s_im_speedupinfo_resp.im_acc_remaintime = s_im_isacc_resp.im_acc_remaintime;
				}
			}

			config_write_im_acc();//init im_acc config
			break;

		default:
				ret = MSG_CODE_UNKNOWN;
				IM_DEBUG("CMD UNKONW!!!!!");
			break;
		
	}
	return ret;
	
}

/*********************************************
function:   解析JSON消息的DATA
params:
[IN]json_msg:    json字符串对象
return: -1，0
***********************************************/
int im_json_parse_data_cloud(json_object *json_msg, int cmd)
{
	int res = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	
	switch(cmd)
	{
		case CMD_DEV_LOGIN:
			res = im_json_parse_data_login_resp(json_msg);
			break;
		case CMD_DEV_LOGOUT:
			break;
		case CMD_HEART_BEAT:
			res = im_json_parse_data_heartbeat_resp(json_msg);
			break;
		case CMD_IS_ACC:
			res = im_json_parse_data_isacc_resp(json_msg);
			break;
		case CMD_START_ACC:
			res = im_json_parse_data_startacc_resp(json_msg);
			break;
		case CMD_GETFLOW_INFO:
			res = im_json_parse_data_getflowinfo_resp(json_msg);
			break;
		case CMD_GET_ACCLIST:
			res = im_json_parse_data_getacclist_resp(json_msg);
			break;
		case CMD_GET_ACCINFO:
			res = im_json_parse_data_getaccinfo_resp(json_msg);
			break;
		case CMD_UNGETFLOW:
			res = im_json_parse_data_ungetflow_resp(json_msg);
			break;
		case CMD_GETFLOW:
			res = im_json_parse_data_getflow_resp(json_msg);
			break;
		case CMD_GET_TASKRESULT:
			res = im_json_parse_data_taskresult_resp(json_msg, CMD_GET_TASKRESULT, MSG_CODE_SUCESS);
			break;
	//以下没有返回DATA
		case CMD_GETACC_FLOW:
		case CMD_STOP_ACC:
		case CMD_COUNT_FLOW:
			break;
		default:
			IM_DEBUG("unkonow cmd");
			break;
	}

	return res;
}


/*********************************************
function:   解析JSON对象
params:
[IN]p_msg:    json字符串
return: -1，0
***********************************************/
int im_json_parse_cloud(const char *p_msg, S_IM_MSG_HEADER *s_im_cmd_header)
{
	json_object *json_msg = NULL;
	int ret = MSG_CODE_SUCESS;
		
	if (p_msg == NULL)
	{
		IM_DEBUG("p_msg is null param error");
		return MSG_CODE_UNKNOWN;
	}

	printf("cloud_msg=%s\n",p_msg);
	json_msg = json_tokener_parse(p_msg);
    if (is_error(json_msg)) 
    {
        IM_DEBUG("json_tokener_parse failed!\n");
		return MSG_CODE_FORMAT_ERR;
    }
		
	
	ret = im_json_parse_header(json_msg, s_im_cmd_header);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json parse header fail");
		goto headerfail;
	}

	IM_DEBUG("errcode:%d", s_im_cmd_header->im_code);
	
	if (s_im_cmd_header->im_code == MSG_CODE_SUCESS)
	{
		ret = im_json_parse_data_cloud(json_msg, s_im_cmd_header->im_cmd);
	}
	else if(s_im_cmd_header->im_code == MSG_CODE_MSGDELAY)
	{
		ret = im_json_parse_data_taskresult_resp(json_msg, s_im_cmd_header->im_cmd, MSG_CODE_MSGDELAY);
	}
	

headerfail:
	json_object_put(json_msg);
	
	return ret;
}


/*********************************************
function:  构造JSON对象的HEADER部分
params:
[IN]json_msg:    json消息对象
[IN]errcode:		错误码
return: -1，0
***********************************************/

im_json_make_header(json_object *json_msg, int errcode)
{
	json_object *json_header = NULL;
	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}
	//获取HEADER对象
	json_header = json_object_object_get(json_msg, jason_key[JSON_HEADER]);
	if (json_header == NULL)
	{
		IM_DEBUG("json get header object fail\n");
		return MSG_CODE_UNKNOWN;
	}
	
	ret |= im_json_object_set_int(json_header, jason_key[JSON_CODE], errcode);
	if (ret < 0)
	{
		IM_DEBUG("json make respone set errcode error");
	}
	
	return ret;
}

/*********************************************
function:  构造JSON对象的data部分
params:
[IN]json_msg:    json消息对象
[IN]data:		返回的data消息
return: -1，0
***********************************************/

int im_json_make_data(json_object *json_msg, json_object *data)
{

	int ret = MSG_CODE_SUCESS;

	if (json_msg == NULL)
	{
		IM_DEBUG("param error");
		return MSG_CODE_UNKNOWN;
	}

	json_object_object_del(json_msg, jason_key[JSON_DATA]);//删除原有的DATA
	json_object_object_add(json_msg, jason_key[JSON_DATA], data);


#if 0
	//获取DATA对象
	im_data = json_object_object_get(json_msg, jason_key[JSON_DATA]);
	if (im_data == NULL)
	{
		IM_DEBUG("json  data getobj fail\n");
		return MSG_CODE_FORMAT_ERR;
	}
	
	if (data == NULL)
	{
		ret |= json_object_array_put_idx(im_data, 0, json_object_new_string(""));
	}
	else
	{
		ret |= json_object_array_put_idx(im_data, 0, json_object_new_string(data));
	}
	if (ret < 0)
	{
		IM_DEBUG("json make respone set data error");
	}
#endif

	return ret;
}

/*********************************************
function:  构造回复JSON消息并发送
params:
[IN]p_msg:	   json字符串
[IN]errcode:	错误码号
[IN]data:		回复的data数组的值
[IN]acc_socket:	回复所用的socket
return: -1，0
***********************************************/

int im_json_make_respone_send(const char *p_msg, int errcode, 
											json_object *data, int acc_socket)
{
	json_object *json_msg = NULL;
	json_object *json_header = NULL;
	json_object *im_data = NULL;
	char *send_msg = NULL;
	int ret = 0;

	if (p_msg == NULL)
	{
		IM_DEBUG("p_msg is null param error");
		return MSG_CODE_UNKNOWN;
	}
	
	json_msg = json_tokener_parse(p_msg);
    if (is_error(json_msg)) 
    {
        IM_DEBUG("json_tokener_parse failed!\n");
		return MSG_CODE_FORMAT_ERR;
    }


	ret = im_json_make_header(json_msg, errcode);
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("json_make_header fail");
		goto fail;
	}

	if (data != NULL)
	{
		ret = im_json_make_data(json_msg, data);
		if (ret != MSG_CODE_SUCESS)
		{
			IM_DEBUG("json_make_data fail");
			goto fail;
		}
	}
	
	send_msg = (char *)json_object_to_json_string(json_msg);
	if(send_msg == NULL)
	{
		IM_DEBUG("to_json_string fail");
		goto fail;
	}


	printf("send_msg = %s\n",send_msg);
	ret = IM_MsgSend(acc_socket, send_msg, strlen(send_msg));
	if (ret != MSG_CODE_SUCESS)
	{
		IM_DEBUG("IM_MsgSend fail");
	}
	else
	{
		IM_DEBUG("IM_MsgSend OK");
	}
	
fail:
	json_object_put(json_msg);
	
	return ret;
}



/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]login:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_brocast_IOS( char *msg,  char *noty_msg,char *sign)
{	
	int ret = MSG_CODE_SUCESS;
	int count_dev = 0;
	json_object *pDataObj;
	json_object *dataarray;
	json_object *obj;
	char json_string[1024] = {0};
	char md5sum [128]= {0};
	time_t tim = time(NULL);

	if (!noty_msg || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}


	obj = json_object_new_object();
	dataarray = json_object_new_array();
	pDataObj = json_object_new_object();

	for(count_dev = 0; count_dev < MAX_IOS_DEV; count_dev++)
	{
		if(s_im_iosinfo[count_dev].devicetoken[0] == 0)
			continue;
		
		json_object_object_add(pDataObj, jason_key[JSON_BADGE], json_object_new_int(1));
		json_object_object_add(pDataObj, jason_key[JSON_MESSAGE], json_object_new_string(noty_msg));
		json_object_object_add(pDataObj, jason_key[JSON_DEVICE], json_object_new_string(s_im_iosinfo[count_dev].devicetoken));
		json_object_object_add(pDataObj, jason_key[JSON_MODELE], json_object_new_string(MODULE));

		json_object_array_add(dataarray, pDataObj);
	}

	json_object_object_add(obj, jason_key[JSON_DATA], dataarray);
	json_object_object_add(obj, jason_key[JSON_RAND], json_object_new_int(tim));

	strcpy(msg , json_object_to_json_string(obj));

	sprintf(json_string,"%s%s",msg,SIGN_KEY);

	printf("json_string = %s\n",json_string);
	im_get_string_md5(json_string,md5sum);

	strcat(md5sum,SIGN_KEY);
	printf("md5sum = %s\n",md5sum);
	im_get_string_md5(md5sum,sign);
	
	json_object_put(pDataObj);
	json_object_put(dataarray);
	json_object_put(obj);

	return ret;
}




/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]login:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_login(S_IM_CLOUD_DATA_LOGIN_CMD *login, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!login || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
    "\"header\": {\"cmd\": %d,\"ver\": %d,"
    "\"seq\": 1, \"device\": 1,"
    "\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
    "\"data\": [{\"dev_id\":\"%s\", \"ip\":\"%s\"}]"
    "}",
	CMD_DEV_LOGIN, CMD_VERSION, CMD_SESSIONID,login->im_dev_id, login->im_ip);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]logout:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_logout(S_IM_CLOUD_DATA_LOGIN_RESP *logout, char *msg)
{
	int ret = MSG_CODE_SUCESS;

	if (!logout || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
    "\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_DEV_LOGOUT, CMD_VERSION, CMD_SESSIONID, logout->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_heartbeat(S_IM_CLOUD_DATA_LOGIN_RESP *heartbeat, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!heartbeat || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
    "\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_HEART_BEAT, CMD_VERSION, CMD_SESSIONID, heartbeat->im_session_id);

	return ret;
}


/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_isacc(S_IM_CLOUD_DATA_ACC_CMD *isaac, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!isaac || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	printf(" 33333333333 =%s\n",isaac->im_session_id);
	sprintf(msg, 
	"{"
    "\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\",\"dev_id\":\"%s\", \"ip\":\"%s\"}]"
"}",
	CMD_IS_ACC, CMD_VERSION, CMD_SESSIONID, 
	isaac->im_session_id, isaac->im_dev_id, isaac->im_ip);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_startacc(S_IM_CLOUD_DATA_ACC_CMD *startaac, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!startaac || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	sprintf(msg, 
	"{"
   "\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\",\"dev_id\":\"%s\", \"ip\":\"%s\"}]"
"}",
	CMD_START_ACC, CMD_VERSION, CMD_SESSIONID, 
	startaac->im_session_id, startaac->im_dev_id, startaac->im_ip);
	return ret;
}


/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_stopacc(S_IM_CLOUD_DATA_LOGIN_RESP *stoptaac, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!stoptaac || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_STOP_ACC, CMD_VERSION, CMD_SESSIONID, stoptaac->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_countflow(S_IM_CLOUD_DATA_COUNTFLOW_CMD *countflow, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!countflow || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\",\"use_flow\":%ld}]"
"}",
	CMD_COUNT_FLOW, CMD_VERSION, CMD_SESSIONID, 
	countflow->im_session_id, countflow->im_use_flow);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_getaacflow(S_IM_CLOUD_DATA_GETAACFLOW_CMD *TASKID, char *msg)
{
	int ret = MSG_CODE_SUCESS;

	if (!TASKID || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\" \"taskid\":\"%s\"}]"
"}",
	CMD_GETACC_FLOW, CMD_VERSION, CMD_SESSIONID, 
	TASKID->im_session_id, TASKID->im_task_id);

	return ret;
}


/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_flowinfo(S_IM_CLOUD_DATA_LOGIN_RESP *flowinfo, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!flowinfo || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_GETFLOW_INFO, CMD_VERSION, CMD_SESSIONID, 
	flowinfo->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_acclist(S_IM_CLOUD_DATA_LOGIN_RESP *acclist, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!acclist || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_GET_ACCLIST, CMD_VERSION, CMD_SESSIONID, 
	acclist->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_getflow_info(S_IM_CLOUD_DATA_LOGIN_RESP *getflow_info, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!getflow_info || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_GETFLOW_INFO, CMD_VERSION, CMD_SESSIONID, 
	getflow_info->im_session_id);

	return ret;
}


/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_ungetflow(S_IM_CLOUD_DATA_LOGIN_RESP *ungetflow, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!ungetflow || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_UNGETFLOW, CMD_VERSION, CMD_SESSIONID, 
	ungetflow->im_session_id);

	return ret;
}


/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_getflow(S_IM_CLOUD_DATA_LOGIN_RESP *getflow, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!getflow || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_GETFLOW, CMD_VERSION, CMD_SESSIONID, 
	getflow->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_accinfo(S_IM_CLOUD_DATA_LOGIN_RESP *accinfo, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!accinfo || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	sprintf(msg, 
	"{"
	"\"header\": {\"cmd\": %d, \"ver\": %d,"
"\"seq\": 1, \"device\": 1,"
"\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},"
"\"data\": [{\"sessionid\":\"%s\"}]"
"}",
	CMD_GET_ACCINFO, CMD_VERSION, CMD_SESSIONID, 
	accinfo->im_session_id);

	return ret;
}

/*********************************************
function:  构造发送到云端的JSON消息
params:
[IN]heartbeat:   DATA 参数
[OUT]msg:		返回的消息
return: -1，0
***********************************************/
int im_json_make_cmd_gettaskresult(S_IM_CLOUD_DATA_TASKRESULT_CMD*accinfo, char *msg)
{
	int ret = MSG_CODE_SUCESS;
	
	if (!accinfo || !msg )
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

		sprintf(msg, 
			"{\"header\": {\"cmd\": %d, \"ver\": %d,\"seq\": 1, \"device\": 1,\"code\": 0, \"sessionid\":\"%s\", \"sign\":\" \"},\"data\": [{\"sessionid\":\"%s\",\"task_id\":\"%s\"}]}",
			CMD_GET_TASKRESULT, CMD_VERSION, CMD_SESSIONID, 
			s_im_acc.im_session_id, accinfo->im_taskid);


	return ret;
}

int im_json_make_termres_startacc(json_object *json_data,json_object *pDataObj)
{	
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	json_object_object_add(pDataObj, jason_key[JSON_ISP], json_object_new_int(s_im_startacc_resp.im_isp));
	json_object_object_add(pDataObj, jason_key[JSON_CUR_BANDWIDTH], json_object_new_int(s_im_startacc_resp.im_cur_bandwidth));
	json_object_object_add(pDataObj, jason_key[JSON_MAX_BANDWIDTH], json_object_new_int(s_im_startacc_resp.im_max_bandwidth));
	json_object_object_add(pDataObj, jason_key[JSON_ACC_REMAINTIME], json_object_new_int(s_im_startacc_resp.im_remaintime));
	json_object_object_add(pDataObj, jason_key[JSON_ACC_BANDWIDTH], json_object_new_int(s_im_startacc_resp.im_acc_bandwidth));

	json_object_array_add(json_data, pDataObj);

}


int im_json_make_termres_speedupinfo(json_object *json_data,json_object *pDataObj)
{	
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	json_object_object_add(pDataObj, jason_key[JSON_ACC_STATE], json_object_new_int(s_im_speedupinfo_resp.im_acc_state));
	json_object_object_add(pDataObj, jason_key[JSON_IS_ACC], json_object_new_int(s_im_speedupinfo_resp.im_is_acc));
	json_object_object_add(pDataObj, jason_key[JSON_ISP], json_object_new_int(s_im_speedupinfo_resp.im_isp));
	json_object_object_add(pDataObj, jason_key[JSON_CUR_BANDWIDTH], json_object_new_int(s_im_speedupinfo_resp.im_cur_bandwidth));
	json_object_object_add(pDataObj, jason_key[JSON_MAX_BANDWIDTH], json_object_new_int(s_im_speedupinfo_resp.im_max_bandwidth));

	if(s_im_speedupinfo_resp.im_acc_state == STATE_ACC_OPENED)
	{
		json_object_object_add(pDataObj, jason_key[JSON_ACC_BANDWIDTH], json_object_new_int(s_im_speedupinfo_resp.im_acc_bandwidth));
		json_object_object_add(pDataObj, jason_key[JSON_ACC_REMAINTIME], json_object_new_int(s_im_speedupinfo_resp.im_acc_remaintime));
	}

	json_object_array_add(json_data, pDataObj);
	
	return MSG_CODE_SUCESS;
}


int im_json_make_termres_flowinfo(json_object *json_data,json_object *pDataObj)
{	
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	json_object_object_add(pDataObj, jason_key[JSON_TOTAL_FLOW], json_object_new_int(s_im_flowinfo_resp.im_total_flow));
	json_object_object_add(pDataObj, jason_key[JSON_REMAIN_FLOW], json_object_new_int(s_im_flowinfo_resp.im_remain_flow));

	s_im_flowinfo_resp.im_acc_remaintime = s_im_speedupinfo_resp.im_acc_remaintime;

	if(s_im_speedupinfo_resp.im_acc_state == STATE_ACC_OPENED)
	{
		
		json_object_object_add(pDataObj, jason_key[JSON_ACC_REMAINTIME], json_object_new_int(s_im_flowinfo_resp.im_acc_remaintime));
	}

	json_object_array_add(json_data, pDataObj);
	
	#if 0
	sprintf(json_data, "\"%s\":%d, \"%s\":%d, \"%s\":%d", 
					jason_key[JSON_TOTAL_FLOW], s_im_flowinfo_resp.im_total_flow,
					jason_key[JSON_REMAIN_FLOW], s_im_flowinfo_resp.im_remain_flow,
					jason_key[JSON_ACC_REMAINTIME], s_im_flowinfo_resp.im_acc_remaintime);
	#endif

	return MSG_CODE_SUCESS;
}

int im_json_make_termres_ungetflow(json_object *json_data,json_object *pDataObj)
{
	
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	json_object_object_add(pDataObj, jason_key[JSON_GET_FLOW_ID], json_object_new_string(s_im_ungetflow_resp.im_unget_flow.im_getflow_id));
	json_object_object_add(pDataObj, jason_key[JSON_GET_FLOW_INFO], json_object_new_string(s_im_ungetflow_resp.im_unget_flow.im_getflow_info));

	json_object_array_add(json_data, pDataObj);
#if 0
	sprintf(json_data, "{\"%s\":\"%s\", \"%s\":\"%s\"}", 
					jason_key[JSON_GET_FLOW_ID], s_im_ungetflow_resp.im_unget_flow.im_getflow_id,
					jason_key[JSON_GET_FLOW_INFO], s_im_ungetflow_resp.im_unget_flow.im_getflow_info);
#endif

	return MSG_CODE_SUCESS;
}

int im_json_make_termres_getflow(json_object *json_data,json_object *pDataObj)
{
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	if (!pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
//	json_object_object_add(json_data, jason_key[JSON_GET_FLOW_ID], json_object_new_string(s_im_getflow.im_getflow_id));
//	json_object_object_add(json_data, jason_key[JSON_DEV_NAME], json_object_new_string(s_im_getflow.im_dev_name));

	return MSG_CODE_SUCESS;
}


int im_json_make_termres_peakflow(json_object *json_data,json_object *pDataObj)
{
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}

	
	json_object_object_add(pDataObj, jason_key[JSON_PEAK_FLOW], json_object_new_int(s_im_peakflow_resp.im_peak_flow));
	json_object_array_add(json_data, pDataObj);

	return MSG_CODE_SUCESS;
}


int im_json_make_termres_acclist(json_object *json_data,json_object *pDataObj)
{
	int i = 0;
		
	if (!json_data || !pDataObj)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	for(i = 0; i < MAX_ACC_HIS; i++)
	{
			if(json_object_new_int(s_im_acclist_resp.acc_list[i].im_use_time) != 0)
			{
				json_object_object_add(pDataObj, jason_key[JSON_USE_TIME], json_object_new_int(s_im_acclist_resp.acc_list[i].im_use_time));
				json_object_object_add(pDataObj, jason_key[JSON_USE_FLOW], json_object_new_int(s_im_acclist_resp.acc_list[i].im_use_flow));
				json_object_object_add(pDataObj, jason_key[JSON_OPEN_ACC_TIME], json_object_new_int(s_im_acclist_resp.acc_list[i].im_open_acc_time));
		
				json_object_array_add(json_data, pDataObj);
			}

			#if 0
			sprintf(data_array, "{\"%s\":%d, \"%s\":%d, \"%s\":%d},", 
					jason_key[JSON_USE_TIME], s_im_acclist_resp.acc_list[record].im_use_time,
					jason_key[JSON_USE_FLOW], s_im_acclist_resp.acc_list[record].im_use_flow,
					jason_key[JSON_OPEN_ACC_TIME], s_im_acclist_resp.acc_list[record].im_open_acc_time);
			strcat(json_data,data_array);
			#endif
	}

	return MSG_CODE_SUCESS;
}


int im_get_string_md5(char *string,char *des)
{
	unsigned char md[16] = {0};
	char tmp[3] = {0};
	int i = 0;
	
	MD5(string,strlen(string),md);

	for(i = 0;i <16; i++){
		sprintf(tmp,"%2.2x",md[i]);
		strcat(des,tmp);
	}
	
	return 0;		
}


