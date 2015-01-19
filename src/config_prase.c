/******************************************************************************
                                                                                                                      
*Desc:I-MOVE MUL TIMER                                             
                                                                                                          
*Copyright (C) 2014 i-move                         
*Author: yun.zhang <zhangyun@i-move.cn>                                    
                                                                                                      
*This program is free software; you can redistribute it and/or modify       
*it under the terms of the GNU General Public License version 2 as        
*published by the Free Software Foundation.  

*Date: 2014-12-25
*Modify:
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <uci.h>
#include <uci_blob.h>
#include <libubox/blobmsg.h>
#include "im_wideband_speedup.h"

static struct uci_context *uci_ctx;
static struct uci_package *uci_im_acc;
static struct blob_buf b;
char uci_save_ip[MAX_IP_LEN] = {0};

enum {	
	IM_ACC_STATE,	
	IM_IS_ACC,	
	IM_ISP,
	IM_CUR_BANDWIDTH,	
	IM_MAX_BANDWIDTH,
	IM_ACC_BANDWIDTH,
	IM_ACC_REMAINTIME,
	IM_IP,
	__ISACC_MAX	
};

static const struct blobmsg_policy isacc_attr[__ISACC_MAX] = {	
	[IM_ACC_STATE] = { .name = "im_acc_state", .type = BLOBMSG_TYPE_INT32 },	
	[IM_IS_ACC] = { .name = "im_is_acc", .type = BLOBMSG_TYPE_INT32 },	
	[IM_ISP] = { .name = "im_isp", .type = BLOBMSG_TYPE_INT32 },
	[IM_CUR_BANDWIDTH] = { .name = "im_cur_bandwidth", .type = BLOBMSG_TYPE_INT32 },	
	[IM_MAX_BANDWIDTH] = { .name = "im_max_bandwidth", .type = BLOBMSG_TYPE_INT32 },
	[IM_ACC_BANDWIDTH] = { .name = "im_acc_bandwidth", .type = BLOBMSG_TYPE_INT32 },
	[IM_ACC_REMAINTIME] = { .name = "im_acc_remaintime", .type = BLOBMSG_TYPE_INT32 },
	[IM_IP] = { .name = "im_ip", .type = BLOBMSG_TYPE_STRING },
};

const struct uci_blob_param_list isacc_attr_list = {	
	.n_params = __ISACC_MAX,
	.params = isacc_attr,
};



static struct uci_package *config_init_package(const char *config)
{	
	struct uci_context *ctx = uci_ctx;	
	struct uci_package *p = NULL;
	
	if (!ctx) {		
		ctx = uci_alloc_context();		
		uci_ctx = ctx;
	} 
	else {		
		p = uci_lookup_package(ctx, config);		
		if (p)			
			uci_unload(ctx, p);
	}	
	
	if (uci_load(ctx, config, &p))		
		return NULL;	return p;
}


void read_resp_isacc(struct blob_attr *attr,char *cmd)
{	
	struct blob_attr *tb[__ISACC_MAX], *cur;	

	blobmsg_parse(isacc_attr, __ISACC_MAX, tb, blobmsg_data(attr), blobmsg_data_len(attr));
	
	//if ((cur = tb[IM_ACC_STATE]) != NULL) {
	//	s_im_speedupinfo_resp.im_acc_state = blobmsg_get_u32(cur);		
	//}	

	if ((cur = tb[IM_IS_ACC]) != NULL) {
		s_im_speedupinfo_resp.im_is_acc = blobmsg_get_u32(cur);		
	}
	
	if ((cur = tb[IM_ISP]) != NULL) {
		s_im_speedupinfo_resp.im_isp = blobmsg_get_u32(cur);		
	}
	
		
	if ((cur = tb[IM_CUR_BANDWIDTH]) != NULL) {		
		s_im_speedupinfo_resp.im_cur_bandwidth = blobmsg_get_u32(cur);	
	}
	
		
	if ((cur = tb[IM_MAX_BANDWIDTH]) != NULL) {		
		s_im_speedupinfo_resp.im_max_bandwidth = blobmsg_get_u32(cur);	
	}

	if ((cur = tb[IM_ACC_BANDWIDTH]) != NULL) {		
		s_im_speedupinfo_resp.im_acc_bandwidth = blobmsg_get_u32(cur);	
	}
	
	if ((cur = tb[IM_ACC_REMAINTIME]) != NULL) {
		s_im_speedupinfo_resp.im_acc_remaintime = blobmsg_get_u32(cur);		
	}

	if ((cur = tb[IM_IP]) != NULL) {
		strcpy(uci_save_ip,blobmsg_get_string(cur));	
	}
}


void config_parse_im_acc(struct uci_section *s,char *cmd)
{	
	void *isacc_resp;	
	blob_buf_init(&b, 0);	
	isacc_resp = blobmsg_open_array(&b, "resp");	
	uci_to_blob(&b, s, &isacc_attr_list);	
	blobmsg_close_array(&b, isacc_resp);	
	read_resp_isacc(blob_data(b.head),cmd);
}


void config_read_im_acc(char *cmd)
{	
	uci_im_acc = config_init_package("im_acc");	
	struct uci_element *e;	
	uci_foreach_element(&uci_im_acc->sections, e) {		
		struct uci_section *s = uci_to_section(e);		
		if (!strcmp(s->type, "resp")){					
			config_parse_im_acc(s,cmd);		
		}	
	}	
}

void config_write_im_acc(void)
{
	char cmd_acc_state[254];
	char cmd_is_acc[254];
	char cmd_is_isp[254];
	char cmd_cur_bandwidth[254];
	char cmd_max_bandwidth[254];
	char cmd_acc_bandwidth[254];
	char cmd_acc_remaintime[254];
	
	sprintf(cmd_acc_state,"uci set im_acc.isacc.im_acc_state=%d",s_im_speedupinfo_resp.im_acc_state);
	sprintf(cmd_is_acc,"uci set im_acc.isacc.im_is_acc=%d",s_im_speedupinfo_resp.im_is_acc);
	sprintf(cmd_is_isp,"uci set im_acc.isacc.im_isp=%d",s_im_speedupinfo_resp.im_isp);
	sprintf(cmd_cur_bandwidth,"uci set im_acc.isacc.im_cur_bandwidth=%d",s_im_speedupinfo_resp.im_cur_bandwidth);
	sprintf(cmd_max_bandwidth,"uci set im_acc.isacc.im_max_bandwidth=%d",s_im_speedupinfo_resp.im_max_bandwidth);
	sprintf(cmd_acc_bandwidth,"uci set im_acc.isacc.im_acc_bandwidth=%d",s_im_speedupinfo_resp.im_acc_bandwidth);
	sprintf(cmd_acc_remaintime,"uci set im_acc.isacc.im_acc_remaintime=%d",s_im_speedupinfo_resp.im_acc_remaintime);

	system(cmd_acc_state);
	system(cmd_is_acc);
	system(cmd_is_isp);
	system(cmd_cur_bandwidth);
	system(cmd_max_bandwidth);
	system(cmd_acc_remaintime);

	system("uci commit im_acc");
	
}

