#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#include <sys/time.h>
#include <time.h>

#include "timer.h"

#define SNMP_MALLOC_STRUCT(s)   (struct s *) calloc(1, sizeof(struct s))

static struct snmp_alarm *thealarms = NULL;
static int start_alarms = 0;
static unsigned int regnum = 1;
static struct _udpbrocast_timer udpbrocast_timer;
timer_handle_t  handle_t[3],timer_count = 0;
static struct _query_server query_server;

int query_timer_interval[6] = {20,40,60,80,100,120};

int init_alarm_post_config()
{
    start_alarms = 1;
    set_an_alarm();
    return 0;
}

void sa_update_entry(struct snmp_alarm *a)
{
    if(a->t_last.tv_sec == 0 && a->t_last.tv_usec == 0) {
        struct timeval t_now;

        /*
         * Never been called yet, call time `t' from now.  
         */
        gettimeofday(&t_now, NULL);

        a->t_last.tv_sec = t_now.tv_sec;
        a->t_last.tv_usec = t_now.tv_usec;

        a->t_next.tv_sec = t_now.tv_sec + a->t.tv_sec;
        a->t_next.tv_usec = t_now.tv_usec + a->t.tv_usec;

        while(a->t_next.tv_usec >= 1000000) {
            a->t_next.tv_usec -= 1000000;
            a->t_next.tv_sec += 1;
        }
    }
    else if(a->t_next.tv_sec == 0 && a->t_next.tv_usec == 0) {
        /*
         * We've been called but not reset for the next call.  
         */
        if(a->flags & SA_REPEAT) {
            if(a->t.tv_sec == 0 && a->t.tv_usec == 0) {
                printf("update_entry: illegal interval specified\n");
                snmp_alarm_unregister(a->clientreg);
                return;
            }

            a->t_next.tv_sec = a->t_last.tv_sec + a->t.tv_sec;
            a->t_next.tv_usec = a->t_last.tv_usec + a->t.tv_usec;

            while(a->t_next.tv_usec >= 1000000) {
                a->t_next.tv_usec -= 1000000;
                a->t_next.tv_sec += 1;
            }
        }
        else {
            /*
             * Single time call, remove it.  
             */
            snmp_alarm_unregister(a->clientreg);
        }
    }
}


void snmp_alarm_unregister(unsigned int clientreg)
{
    struct snmp_alarm *sa_ptr, **prevNext = &thealarms;

    for(sa_ptr = thealarms;
        sa_ptr != NULL && sa_ptr->clientreg != clientreg;
        sa_ptr = sa_ptr->next) {
        prevNext = &(sa_ptr->next);
    }

    if(sa_ptr != NULL) {
        *prevNext = sa_ptr->next;
        printf("unregistered alarm %d\n", sa_ptr->clientreg);
        /*
         * Note:  do not free the clientarg, its the clients responsibility 
         */
        free(sa_ptr);
    }
    else {
        printf("no alarm %d to unregister\n", clientreg);
    }
}


void snmp_alarm_unregister_all(void)
{
    struct snmp_alarm *sa_ptr, *sa_tmp;

    for(sa_ptr = thealarms; sa_ptr != NULL; sa_ptr = sa_tmp) {
        sa_tmp = sa_ptr->next;
        free(sa_ptr);
    }
    printf("ALL alarms unregistered\n");
    thealarms = NULL;
}

struct snmp_alarm *sa_find_next(void)
{
    struct snmp_alarm *a, *lowest = NULL;

    for(a = thealarms; a != NULL; a = a->next) {
        if(lowest == NULL) {
            lowest = a;
        }
        else if(a->t_next.tv_sec == lowest->t_next.tv_sec) {
            if(a->t_next.tv_usec < lowest->t_next.tv_usec) {
                lowest = a;
            }
        }
        else if(a->t_next.tv_sec < lowest->t_next.tv_sec) {
            lowest = a;
        }
    }
    return lowest;
}

struct snmp_alarm *sa_find_specific(unsigned int clientreg)
{
    struct snmp_alarm *sa_ptr;

    for(sa_ptr = thealarms; sa_ptr != NULL; sa_ptr = sa_ptr->next) {
        if(sa_ptr->clientreg == clientreg) {
            return sa_ptr;
        }
    }
    return NULL;
}

void run_alarms(void)
{
    int done = 0;
    struct snmp_alarm *a = NULL;
    unsigned int clientreg;
    struct timeval t_now;

    /*
     * Loop through everything we have repeatedly looking for the next thing to
     * call until all events are finally in the future again.  
     */

    while(!done) {
        if((a = sa_find_next()) == NULL) {
            return;
        }

        gettimeofday(&t_now, NULL);

        if((a->t_next.tv_sec < t_now.tv_sec) ||
           ((a->t_next.tv_sec == t_now.tv_sec) &&
            (a->t_next.tv_usec < t_now.tv_usec))) {
            clientreg = a->clientreg;
            printf("run alarm %d\n", clientreg);
            (*(a->thecallback)) (a->clientarg);
            printf("alarm %d completed\n", clientreg);

            if((a = sa_find_specific(clientreg)) != NULL) {
                a->t_last.tv_sec = t_now.tv_sec;
                a->t_last.tv_usec = t_now.tv_usec;
                a->t_next.tv_sec = 0;
                a->t_next.tv_usec = 0;
                sa_update_entry(a);
            }
            else {
                printf("alarm %d deleted itself\n", clientreg);
            }
        }
        else {
            done = 1;
        }
    }
}



RETSIGTYPE alarm_handler(int a)
{
    run_alarms();
    set_an_alarm();
}



int get_next_alarm_delay_time(struct timeval *delta)
{
    struct snmp_alarm *sa_ptr;
    struct timeval t_diff, t_now;

    sa_ptr = sa_find_next();

    if(sa_ptr) {
        gettimeofday(&t_now, 0);

        if((t_now.tv_sec > sa_ptr->t_next.tv_sec) ||
           ((t_now.tv_sec == sa_ptr->t_next.tv_sec) &&
            (t_now.tv_usec > sa_ptr->t_next.tv_usec))) {
            /*
             * Time has already passed.  Return the smallest possible amount of
             * time.  
             */
            delta->tv_sec = 0;
            delta->tv_usec = 1;
            return sa_ptr->clientreg;
        }
        else {
            /*
             * Time is still in the future.  
             */
            t_diff.tv_sec = sa_ptr->t_next.tv_sec - t_now.tv_sec;
            t_diff.tv_usec = sa_ptr->t_next.tv_usec - t_now.tv_usec;

            while(t_diff.tv_usec < 0) {
                t_diff.tv_sec -= 1;
                t_diff.tv_usec += 1000000;
            }

            delta->tv_sec = t_diff.tv_sec;
            delta->tv_usec = t_diff.tv_usec;
            return sa_ptr->clientreg;
        }
    }

    /*
     * Nothing Left.  
     */
    return 0;
}


void set_an_alarm(void)
{
    struct timeval delta;
    int nextalarm = get_next_alarm_delay_time(&delta);

    /*
     * We don't use signals if they asked us nicely not to.  It's expected
     * they'll check the next alarm time and do their own calling of
     * run_alarms().  
     */

    if(nextalarm) {
        struct itimerval it;

        it.it_value.tv_sec = delta.tv_sec;
        it.it_value.tv_usec = delta.tv_usec;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_usec = 0;

        signal(SIGALRM, alarm_handler);
        setitimer(ITIMER_REAL, &it, NULL);
        printf("schedule alarm %d in %d.%03d seconds\n",
               nextalarm, (int) delta.tv_sec,
               (int) (delta.tv_usec / 1000));

    }
    else {
        printf("no alarms found to schedule\n");
    }
}


unsigned int
snmp_alarm_register(unsigned int when, unsigned int flags,
                    SNMPAlarmCallback * thecallback, void *clientarg)
{
    struct snmp_alarm **sa_pptr;

    if(thealarms != NULL) {
        for(sa_pptr = &thealarms; (*sa_pptr) != NULL;
            sa_pptr = &((*sa_pptr)->next)) ;
    }
    else {
        sa_pptr = &thealarms;
    }

    *sa_pptr = SNMP_MALLOC_STRUCT(snmp_alarm);
    if(*sa_pptr == NULL)
        return 0;

    if(0 == when) {
        (*sa_pptr)->t.tv_sec = 0;
        (*sa_pptr)->t.tv_usec = TIMER_UNIT_UDP_BROUDCAST;
    }
    else {
        (*sa_pptr)->t.tv_sec = when;
        (*sa_pptr)->t.tv_usec = 0;
    }
    (*sa_pptr)->flags = flags;
    (*sa_pptr)->clientarg = clientarg;
    (*sa_pptr)->thecallback = thecallback;
    (*sa_pptr)->clientreg = regnum++;
    (*sa_pptr)->next = NULL;
    sa_update_entry(*sa_pptr);

    if(start_alarms){
        set_an_alarm();
    }

    return (*sa_pptr)->clientreg;
}



unsigned int
snmp_alarm_register_hr(struct timeval t, unsigned int flags,
                       SNMPAlarmCallback * cb, void *cd)
{
    struct snmp_alarm **s = NULL;

    for(s = &(thealarms); *s != NULL; s = &((*s)->next)) ;

    *s = SNMP_MALLOC_STRUCT(snmp_alarm);
    if(*s == NULL) {
        return 0;
    }

    (*s)->t.tv_sec = t.tv_sec;
    (*s)->t.tv_usec = t.tv_usec;
    (*s)->flags = flags;
    (*s)->clientarg = cd;
    (*s)->thecallback = cb;
    (*s)->clientreg = regnum++;
    (*s)->next = NULL;

    sa_update_entry(*s);

    printf("registered alarm %d, t = %d.%03d, flags=0x%02x\n",
           (*s)->clientreg, (int) (*s)->t.tv_sec,
           (int) ((*s)->t.tv_usec / 1000), (*s)->flags);

    if(start_alarms) {
        set_an_alarm();
    }

    return (*s)->clientreg;
}



int  im_udpbrocast_timer_proc(void *arg)
{
	static int udp_time = 0;
	
	if( udp_time < 4)
	{
		udpbrocast_timer.udpbrocast_func(udpbrocast_timer.func_arg);
		udp_time++;
	}
	else
	{
		snmp_alarm_unregister(handle_t[0]);	
		udp_time = 0;
	}

	return 0;
}


void im_udpbrocast_timer_start(SNMPAlarmCallback * thecallback, void *clientarg)
{
	udpbrocast_timer.udpbrocast_func = thecallback;
	strcpy(udpbrocast_timer.func_arg, clientarg);
	
	handle_t[0] = snmp_alarm_register(0, SA_REPEAT,im_udpbrocast_timer_proc,NULL);
}


int mdy_query_server_timer(void *arg)
{
	static int interval = 0;
	
	if (interval < 120)
	{
		snmp_alarm_unregister(handle_t[1]);
		interval = query_timer_interval[timer_count++];
		handle_t[1] = snmp_alarm_register(interval, SA_REPEAT,mdy_query_server_timer,NULL);
	}
	query_server.query_func(query_server.func_arg);
	
	return 0;
}


int query_server_timer(SNMPAlarmCallback * thecallback, void *arg)
{
	query_server.query_func = thecallback;
	strcpy(query_server.func_arg, arg);

	handle_t[1] = snmp_alarm_register(10,SA_REPEAT, mdy_query_server_timer,NULL);
	return 0;
}

void im_query_server_timer_stop(void)
{
	snmp_alarm_unregister(handle_t[1]);
}


