#ifndef SNMP_ALARM_H
#define SNMP_ALARM_H

#define RETSIGTYPE void

#define TIMER_UNIT_UDP_BROUDCAST 500000
#define MAX_FUNC_ARG_LEN 2048


typedef int (SNMPAlarmCallback) (void *clientarg);

/*
* alarm flags 
*/
#define SA_REPEAT 0x01          /* keep repeating every X seconds */
typedef int timer_handle_t;

struct snmp_alarm {
    struct timeval t;
    unsigned int flags;
    unsigned int clientreg;
    struct timeval t_last;
    struct timeval t_next;
    void *clientarg;
    SNMPAlarmCallback *thecallback;
    struct snmp_alarm *next;
};

typedef  struct _udpbrocast_timer
{
	int (* udpbrocast_func)(void *arg);
	char func_arg[MAX_FUNC_ARG_LEN];
}__udpbrocast_timer_proc;



typedef  struct _query_server
{
    int (* query_func)(void *arg);
    char func_arg[MAX_FUNC_ARG_LEN];
}_query_server_proc;



/*
* the ones you should need 
*/
void snmp_alarm_unregister(unsigned int clientreg);
void snmp_alarm_unregister_all(void);
unsigned int snmp_alarm_register(unsigned int when,
                                 unsigned int flags,
                                 SNMPAlarmCallback * thecallback,
                                 void *clientarg);

unsigned int snmp_alarm_register_hr(struct timeval t,
                                    unsigned int flags,
                                    SNMPAlarmCallback * cb, void *cd);

/*
* the ones you shouldn't 
*/
int init_alarm_post_config();
void sa_update_entry(struct snmp_alarm *alrm);
struct snmp_alarm *sa_find_next(void);
void run_alarms(void);
RETSIGTYPE alarm_handler(int a);
void set_an_alarm(void);
int get_next_alarm_delay_time(struct timeval *delta);

#endif

