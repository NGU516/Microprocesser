// linked_list.h

struct node {
	int 		 data;
	struct node *link;
};

extern struct node *Head;

struct node *get_node();
void free_node(struct node *np);

void insert_node_head(struct node *np);
void insert_node_tail(struct node *np);
void insert_node_ascn(struct node *cp);

void tour_list();
void free_list();
