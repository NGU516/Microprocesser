// cir_queue.h
#define Q_SIZE 64
extern char q[];
extern int f,r;

void q_init();
int q_insert(char ch);
int q_delete();
