
#include <sys/time.h>

#define MAX_TIMER_CNT 10
#define MUL_TIMER_FIR_SEC 5
#define TIMER_UNIT 1
#define MAX_FUNC_ARG_LEN 100
#define INVALID_TIMER_HANDLE (-1)

typedef int timer_handle_t;

typedef struct _timer_manage
{
    struct _timer_info
    {
        int state; /* on or off */
        int interval;
        int elapse; /* 0~interval */
        int (* func_proc) (void *arg, int arg_len);
        char func_arg[MAX_FUNC_ARG_LEN];
        int arg_len;
    }timer_info[MAX_TIMER_CNT];

    void (* old_sigfunc)(int);
    void (* new_sigfunc)(int);
    struct itimerval value, ovalue;
}_timer_manage_t;


typedef  struct _query_server
{
    int (* query_func)(char *arg);
    char func_arg[20];
	int arg_len;
}_query_server_proc;


/* success, return 0; failed, return -1 */
int init_mul_timer(void);
/* success, return 0; failed, return -1 */
int del_mul_timer(void);
/* success, return timer handle(>=0); failed, return -1 */
timer_handle_t set_single_timer(int interval, int (* timer_proc) (void *arg, int arg_len), void *arg, int arg_len);
/* success, return 0; failed, return -1 */
int del_single_timer(timer_handle_t handle);



