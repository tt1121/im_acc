/** 文 件 名: im_json_parse.h
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-28
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-27   张斌创建本文件；
##############################################################################*/


#ifndef __IM_JSON_PARSE_H__
#define  __IM_JSON_PARSE_H__

//#include "im_wideband_speedup.h"

extern int im_json_parse_terminal(const char *p_msg);
extern int im_json_parse_cloud(const char *p_msg, S_IM_MSG_HEADER *s_im_cmd_header);

extern int im_json_make_cmd_login(S_IM_CLOUD_DATA_LOGIN_CMD *login, char *msg);
extern int im_json_make_cmd_logout(S_IM_CLOUD_DATA_LOGIN_RESP *logout, char *msg);
extern int im_json_make_cmd_heartbeat(S_IM_CLOUD_DATA_LOGIN_RESP *heartbeat, char *msg);
extern int im_json_make_cmd_isacc(S_IM_CLOUD_DATA_ACC_CMD *isaac, char *msg);
extern int im_json_make_cmd_startacc(S_IM_CLOUD_DATA_ACC_CMD *startaac, char *msg);
extern int im_json_make_cmd_stopacc(S_IM_CLOUD_DATA_LOGIN_RESP *stoptaac, char *msg);
extern int im_json_make_cmd_countflow(S_IM_CLOUD_DATA_COUNTFLOW_CMD *countflow, char *msg);
extern int im_json_make_cmd_getaacflow(S_IM_CLOUD_DATA_GETAACFLOW_CMD *TASKID, char *msg);
extern int im_json_make_cmd_flowinfo(S_IM_CLOUD_DATA_LOGIN_RESP *flowinfo, char *msg);
extern int im_json_make_cmd_acclist(S_IM_CLOUD_DATA_LOGIN_RESP *acclist, char *msg);
extern int im_json_make_cmd_accinfo(S_IM_CLOUD_DATA_LOGIN_RESP *accinfo, char *msg);


extern int im_json_make_udp_brocast(const char *p_msg, int errcode, 
											char *data, char *send_msg);

extern int im_json_make_termres_startacc(char *json_data);
extern int im_json_make_termres_speedupinfo(char *json_data);

#endif
