/**
电子诗人 c语言实现
作者： 吕泳寰
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//结构体区

//1.链表结构体

typedef struct Noun {
    char *data;
    struct Noun *next;
} noun;

typedef struct Verb {
    char *data;
    struct Verb *next;
} verb;

typedef struct Adjective {
    char *data;
    struct Adjective *next;
} adjective;

typedef struct Sentence {
    char *data;
    struct Sentence *next;
} sentence;

typedef struct Poetry{
    char *data;
    struct Poetry *next;
}poetry;

//全局变量区（为什么用全局变量？因为本来就是全局要用的变量，传过来传过去没意思）

int line[4] = {0};//1.名词行数；2.动词行数；3.形容词行数；4.句式行数
noun *noun_head = NULL;
verb *verb_head = NULL;
adjective *adj_head = NULL;
sentence *sentence_head = NULL;

//函数区
int Input(); //输入处理
int Start(); //开始生成流程

char *SelectSentence();//选择句式
char *SelectNoun();//选择名词
char *SelectVerb();//选择动词
char *SelectAdj();//选择形容词
int SpliceSentence(char *sentence,int isFailStore,poetry *tail);//拼接句子并打印
poetry *InitPoetryStore();//初始化store链表
void StoreSentence(char *str,poetry *tail);//储存写的诗句
int WriteFile(poetry *head);//把诗句写入进文件
void FreePoetry(poetry *head);//释放存储诗句的内存

int ReadFile(); //读取字典内容，错误返回1
void EndProgram();//释放内存（然而操作系统好像会自动回收）

int main() {
    int err = ReadFile();//先读取外存字典内容进入内存
    if (err) {
        return 1;
    }
    err=Start();
    if(err){
        return 2;
    }
    EndProgram();
    return 0;
}

int ReadFile() {
    char *fileName[4] = {"noun", "verb", "adjective", "sentence"};//定义字典名称
    //定义队尾指针,方便捕获队尾
    noun *noun_tail = NULL;
    verb *verb_tail = NULL;
    adjective *adj_tail = NULL;
    sentence *sentence_tail = NULL;
    //初始化链表
    noun_head = noun_tail = (noun *) malloc(sizeof(noun));
    verb_head = verb_tail = (verb *) malloc(sizeof(verb));
    adj_head = adj_tail = (adjective *) malloc(sizeof(adjective));
    sentence_head = sentence_tail = (sentence *) malloc(sizeof(sentence));
    noun_tail->next=NULL;
    verb_tail->next=NULL;
    adj_tail->next=NULL;
    sentence_tail->next=NULL;
    for (int i = 0; i < 4; i++) {
        //打开字典
        FILE *file = fopen(fileName[i], "r");
        if (file == NULL) {
            printf("打开字典%s失败！程序退出。\n", fileName[i]);
            return 1;
        }
        //读取内容
        char temp[100];
        while (fgets(temp, sizeof(temp), file) != NULL) {
            //去除每行末尾的换行符
            size_t len = strlen(temp);
            if (len > 0 && temp[len-1] == '\n') {
                temp[len-1] = '\0';
            }
            line[i]++;
            //链表延长，按情况进行
            switch (i) {
                case 0: {
                    noun *noun_node = (noun *) malloc(sizeof(noun));
                    noun_node->data=strdup(temp);//使用strdup函数。分配独立内存并复制数据
                    noun_node->next=NULL;
                    noun_tail->next=noun_node;
                    noun_tail=noun_node;
                    break;
                }
                case 1: {
                    verb *verb_node = (verb *) malloc(sizeof(verb));
                    verb_node->data=strdup(temp);
                    verb_node->next=NULL;
                    verb_tail->next=verb_node;
                    verb_tail=verb_node;
                    break;
                }
                case 2:{
                    adjective *adj_node = (adjective*) malloc(sizeof (adjective));
                    adj_node->data=strdup(temp);
                    adj_node->next=NULL;
                    adj_tail->next=adj_node;
                    adj_tail=adj_node;
                    break;
                }
                case 3:{
                    sentence *sentence_node=(sentence*) malloc(sizeof (sentence));
                    sentence_node->data=strdup(temp);
                    sentence_node->next=NULL;
                    sentence_tail->next=sentence_node;
                    sentence_tail=sentence_node;
                    break;
                }
                default:
                    break;
            }
        }
		fclose(file);//关闭文件
    }
    return 0;
}

void EndProgram(){
    for(noun *i=noun_head;i!=NULL;){
        noun *next=i->next;
        free(i);
        i=next;
    }
    for(verb *i=verb_head;i!=NULL;){
        verb *next=i->next;
        free(i);
        i=next;
    }
    for(adjective *i=adj_head;i!=NULL;){
        adjective *next=i->next;
        free(i);
        i=next;
    }
    for(sentence* i=sentence_head;i!=NULL;){
        sentence *next=i->next;
        free(i);
        i=next;
    }
}

//控制生成进程
int Start() {
    for(;;) {
        int l = Input();
        if (!l) {
            return 1;//程序异常状态退出
        }
        srand(time(NULL));//设置随机数种子
        //初始化存储模块
        int isFailStore=0;
        poetry *head=InitPoetryStore();
        poetry *tail=head;
        if(head==NULL){
            isFailStore=1;
            printf("初始化储存进程失败，储存诗句功能不可用。\n");
        }
		head->data=NULL;
        head->next=NULL;
        printf("电子诗人咏唱中：\n");
        for (int i = 0; i < l; i++) {
            char *sentence = SelectSentence();
            if (sentence == NULL) {
                return 1;
            }
            int err = SpliceSentence(sentence,isFailStore,tail);
            if (err) {
                return 1;
            }
        }
        char flag;
        if(!isFailStore){
            printf("是否储存诗句？是请按Y，否输入除Y的任意键：");
            getchar();
            scanf("%c",&flag);
            if(flag=='Y'){
				WriteFile(head);
            }else{
				FreePoetry(head);
			}
        }
        printf("是否继续生成？是请输入Y，否输入除Y的任意键结束生成：");
        getchar();
        scanf("%c",&flag);
        if(flag!='Y'){
            break;
        }
    }
    return 0;
}

int Input() {
    int l = 0;
    printf("请输入要生成的诗的行数：\n");
    int flag = scanf("%d", &l);
    if (!flag || l <= 0) {
        printf("输入格式不正确，程序退出！\n");
        return 0;
    }
    return l;
}

char *SelectSentence(){
    int random=rand()%line[3];
    int count=0;
    for(sentence *i=sentence_head->next;count<=random&&i!=NULL;i=i->next,count++){
        if(count==random){
            return i->data;
        }
    }
    return NULL;
}

int SpliceSentence(char *sentence,int isFailStore,poetry *tail){
	char* str = (char*)malloc(sizeof(char) * 50);
    while (*sentence!='\0'){
        if(*sentence=='n'){
            char *n=SelectNoun();
            if(n==NULL){
                printf("选择名词错误！\n");
                return 1;
            }
            printf("%s",n);
            strcat(str,n);
        }else if(*sentence=='v'){
            char *v=SelectVerb();
            if(v==NULL){
                printf("选择动词错误！\n");
                return 1;
            }
            printf("%s",v);
            strcat(str,v);
        }else if(*sentence=='a'){
            char *a=SelectAdj();
            if(a==NULL){
                printf("选择形容词错误！\n");
                return 1;
            }
            printf("%s",a);
            strcat(str,a);
        }else{
            printf("%c",*sentence);
            char temp[2] = {*sentence, '\0'};
            strcat(str, temp);
        }
        *sentence++;
    }
    printf("\n");
    if(!isFailStore){
        StoreSentence(str,tail);
    }
    return 0;
}

char *SelectNoun(){
    int random=rand()%line[0];
    int count=0;
    for(noun *i=noun_head->next;count<=random&&i!=NULL;i=i->next,count++){
        if(count==random){
            return i->data;
        }
    }
    return NULL;
}

char *SelectVerb(){
    int random=rand()%line[1];
    int count=0;
    for(verb *i=verb_head->next;count<=random&&i!=NULL;i=i->next,count++){
        if(count==random){
            return i->data;
        }
    }
    return NULL;
}

char *SelectAdj(){
    int random=rand()%line[2];
    int count=0;
    for(adjective *i=adj_head->next;count<=random&&i!=NULL;i=i->next,count++){
        if(count==random){
            return i->data;
        }
    }
    return NULL;
}

poetry *InitPoetryStore(){
    poetry *head=(poetry *)malloc(sizeof (poetry));
    return head;
}

void StoreSentence(char *str,poetry *tail){
    poetry *p=(poetry *) malloc(sizeof (poetry));
    p->data= strdup(str);
    p->next=NULL;
    tail->next=p;
    tail=p;
}

int WriteFile(poetry *head){
	printf("请输入诗的标题：");
	char title[1000];
	scanf("%s",title);
	char name[1000]="collection/";
	strcat(name,title);
	strcat(name,".txt");
	FILE *file=fopen(name,"w");
	if(file==NULL){
		printf("文件创建失败！\n");
		return 1;
	}
	for(poetry *i=head->next;i!=NULL;i=i->next){
		fprintf(file,"%s\r\n",i->data);
	}
	FreePoetry(head);//释放内存
	fclose(file);
	return 0;
}

void FreePoetry(poetry *head){
	for(poetry *i=head;i!=NULL;){
		poetry *temp=i->next;
		free(i->data);
		free(i);
		i=temp;
	}
}
