// uart_q.h

#define E0T 0x04	// ^D: End of Text

void q_init();
int qi_insert(char ch);
int qi_delete();
int qo_insert(char ch);
int qo_delete();
