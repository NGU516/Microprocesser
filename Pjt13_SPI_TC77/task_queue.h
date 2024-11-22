// Task_queue.h
#define TC77_WAIT_HI 1
#define TC77_WAIT_LO 2

void task_init();	// Task Queue
int task_insert();
int task_delete();
void task_tc1047(char *arg);
void task_tc77(char *arg);
void task_cmd(char *arg);
