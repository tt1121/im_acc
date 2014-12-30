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
#include <signal.h>
#include <time.h> 
#include "mmi.h"



static struct _timer_manage timer_manage;

static struct _query_server query_server;
	
static void signal_func(int signo);

int query_timer_interval[5] = {20,30,40,50,60};

/* success, return 0; failed, return -1 */
int init_mul_timer(void)
{
	int ret;
	
	memset(&timer_manage, 0, sizeof(struct _timer_manage));
	if( (timer_manage.old_sigfunc = signal(SIGALRM, signal_func)) == SIG_ERR)
	{
		return (-1);
	}
	timer_manage.new_sigfunc = signal_func;
	
	timer_manage.value.it_value.tv_sec = MUL_TIMER_FIR_SEC;
	timer_manage.value.it_value.tv_usec = 0;
	timer_manage.value.it_interval.tv_sec = TIMER_UNIT;
	timer_manage.value.it_interval.tv_usec = 0;
	ret = setitimer(ITIMER_REAL, &timer_manage.value, &timer_manage.ovalue); 
	
	return (ret);
}
	
	
/* success, return 0; failed, return -1 */
int del_mul_timer(void)
{
	int ret;
	
	if( (signal(SIGALRM, timer_manage.old_sigfunc)) == SIG_ERR)
	{
		return (-1);


	}

	ret = setitimer(ITIMER_REAL, &timer_manage.ovalue, &timer_manage.value);
	if(ret < 0)
	{
		return (-1);
	} 
	memset(&timer_manage, 0, sizeof(struct _timer_manage));
	
	return(0);
}


/* success, return timer handle(>=0); failed, return -1 */
timer_handle_t set_single_timer(int interval, int (* func_proc) (void *arg, int arg_len), void *arg, int arg_len)
{
	int i;

	if(func_proc == NULL || interval <= 0)
	{
		return (-1);
	} 
	
	for(i = 0; i < MAX_TIMER_CNT; i++)
	{
		if(timer_manage.timer_info[i].state == 1)
		{
			continue;
		}
		
		memset(&timer_manage.timer_info[i], 0, sizeof(timer_manage.timer_info[i]));
		timer_manage.timer_info[i].func_proc = func_proc;
		if(arg != NULL)
		{
			if(arg_len > MAX_FUNC_ARG_LEN)
			{
				return (-1);
			}
			memcpy(timer_manage.timer_info[i].func_arg, arg, arg_len);
			timer_manage.timer_info[i].arg_len = arg_len;
		}
		timer_manage.timer_info[i].interval = interval;
		timer_manage.timer_info[i].elapse = 0;
		timer_manage.timer_info[i].state = 1;
		break;
	}
	
	if(i >= MAX_TIMER_CNT)
	{
		return (-1);
	}
	return (i);
}


/* success, return 0; failed, return -1 */
int del_single_timer(timer_handle_t handle)
{
	if(handle < 0 || handle >= MAX_TIMER_CNT)
	{
		return (-1);
	}
	
	memset(&timer_manage.timer_info[handle], 0, sizeof(timer_manage.timer_info[handle]));
	
	return (0);
}



static void signal_func(int signo)
{
	int i;

	for(i = 0; i < MAX_TIMER_CNT; i++)
	{
		if(timer_manage.timer_info[i].state == 0)
		{
			continue;
		}
		timer_manage.timer_info[i].elapse++;
		if(timer_manage.timer_info[i].elapse == timer_manage.timer_info[i].interval)
		{
			timer_manage.timer_info[i].elapse = 0;
			timer_manage.timer_info[i].func_proc(timer_manage.timer_info[i].func_arg, timer_manage.timer_info[i].arg_len);
		}
	}
}


static void get_format_time(char *tstr)
{
	time_t t;
	
	t = time(NULL);
	strcpy(tstr, ctime(&t));
	tstr[strlen(tstr)-1] = '/0';
	
	return;
}


timer_handle_t handle[10], timer_count = 0,handle_t[3];


/*int func1(void *arg, int len)
{
	char tstr[200];
	static int i, ret;
	
	get_format_time(tstr);
	printf("hello %s: func1 is here.\n", tstr);
	if(i >= 20)
	{
		get_format_time(tstr);
		ret = del_single_timer(handle[0]);
		printf("func1: %s del_single_timer::ret=%d\n", tstr, ret);
	}
	i++;
	timer_count++;
	
	
	return (1);
}*/


int test(void * command,int len)
{
	int j = 0;
	char tstr[200];
	get_format_time(tstr);
	printf("%s command%d = %s\n",tstr,j++,command);
}



int mdy_query_server_timer(void *arg,int arg_len)
{
	char tstr[200];
	static int interval = 0;
	
	query_server.query_func(query_server.func_arg);
	
	if (interval < 60)
	{
		del_single_timer(handle_t[0]);
		interval = query_timer_interval[timer_count++];
		handle_t[0] = set_single_timer(interval, mdy_query_server_timer,NULL, 0);
	}
}


int query_server_timer(int (* func_proc) (void *arg, int arg_len), void *arg, int arg_len)
{
	int count = 0;
	query_server.query_func = func_proc;
	memcpy(query_server.func_arg, arg, arg_len);
	query_server.arg_len = arg_len;
	
	handle_t[0] = set_single_timer(10, mdy_query_server_timer,NULL, 0);

	return (1);
}


#if 0
int main(void)
{
	char arg[50] = "hello,yunyun";
	char tstr[200];
	int ret;
	
	init_mul_timer();
	//handle[0] = set_single_timer(10, func1, NULL, 0);
	query_server_timer(test,arg,20);
	
	while(1)
	{
		if(timer_count >= 20)
		{
			get_format_time(tstr);
			ret = del_mul_timer();
			printf("main: %s del_mul_timer, ret=%d\n", tstr, ret);
			break;
		}
		
	}
	
	return 0;
}

#endif

