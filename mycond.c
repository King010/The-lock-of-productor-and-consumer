#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h>

typedef struct _node
{
	int data;
	struct _node* next;
}node,*pnode,**ppnode;

static pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;


static pnode alloc_node(int _d)
{
	pnode _h=(pnode)malloc(sizeof(node));
	if(_h==NULL){
		perror("alloc");
		exit(1);
	}
	_h->data=_d;
	_h->next=NULL;
}


static int is_empty(pnode _h)
{
	assert(_h);
	return _h->next==NULL?1:0;
}

void init_list(ppnode _h)
{
	*_h=alloc_node(0);	
}

void delete_node(pnode _tmp)
{
	if(_tmp)
		free(_tmp);
}

void pop_node(pnode _h,int* _o)
{
	assert(_h);
	assert(_o);
	if(!is_empty(_h)){
	pnode _tmp=_h->next;
	_h->next=_tmp->next;
	*_o =_tmp->data;
	delete_node(_tmp);
	}
}


void destory_list(pnode _h)
{
	assert(_h);
	int data;
	while(!is_empty(_h))
	{
		pop_node(_h,&data);
	}
	delete_node(_h);
		
}


void push_node(pnode _h,int d)
{
	pnode _n=alloc_node(d);
	_n->next=_h->next;
	_h->next=_n;
}

void show_list(pnode _h)
{
	assert(_h);
	pnode p=_h->next;
	while(p!=NULL){
	printf("%d ",p->data);
	p=p->next;
	}
	printf("\n");
}

void *product(void *arg)
{
	pnode p=(pnode)arg;
	for(;;)
	{
		pthread_mutex_lock(&lock);
		int data=rand()%1234;
		//if(!is_empty(p)){
		push_node(p,data);
		printf("productor done ...%d\n",data);
		pthread_mutex_unlock(&lock);
		sleep(1);
		pthread_cond_signal(&cond);
		printf("data is ok,signal consumer.\n");
		
	}
}

void *consume(void *arg)
{
	pnode p=(pnode)arg;
	for(;;)
	{
		pthread_mutex_lock(&lock);
		int data=rand()%1234;
		if(is_empty(p)){
		printf("no data is being consumer...\n");
		pthread_cond_wait(&cond,&lock);
		}
		pop_node(p,&data);
		printf("consumer done...%d\n",data);
		pthread_mutex_unlock(&lock);
	}

}

int main()
{
	pnode head=NULL;
	init_list(&head);
	pthread_t productor;
	pthread_t consumer;
	pthread_create(&consumer,NULL,consume,head);
	pthread_create(&productor,NULL,product,head);

	pthread_join(productor,NULL);
	pthread_join(consumer,NULL);
	destory_list(head);
//	pthread_mutex_destory(&lock);



//	pnode head=NULL;
//	int data=2;
//	init_list(&head);
//	push_node(head,1);
//	push_node(head,2);
//	push_node(head,3);
//	push_node(head,4);
//    pop_node(head,&data);
//    pop_node(head,&data);
//	show_list(head);
	return 0;
}
