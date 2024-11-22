// circle_queue.h

#define Q_SIZE 64

extern char q[Q_SIZE];
extern int f, r;

void q_init();
int q_insert(char ch);
int q_delete();
