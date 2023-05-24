/**
电子诗人 c语言实现
作者： 吕泳寰
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//函数区
int Input(); //输入处理
int Start(int line); //开始生成流程

char *SelectSentence();//选择句式
int AnalyzeSentence(int *nounNum,int *VerbNum,int *AdjNum);//解析句式，传指针获取句子中名词、动词、形容词数量
char *SelectNoun();//选择名词
char *SelectVerb();//选择动词
char *SelectAdj();//选择形容词
char *SpliceSentence();//拼接句子
int PrintfSentence();//打印句子（逐行打印）

int ReadFile(); //读取字典内容，错误返回1
void EndProgram();//释放内存

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

//全局变量区（为什么用全局变量？因为本来就是全局要用的变量，传过来传过去没意思）

int line[4] = {0};//1.名词行数；2.动词行数；3.形容词行数；4.句式行数
noun *noun_head = NULL;
verb *verb_head = NULL;
adjective *adj_head = NULL;
sentence *sentence_head = NULL;

int main() {
    int err = ReadFile();//先读取外存字典内容进入内存
    if (err) {
        return 1;
    }
    int l = Input();
    if (!l) {
        return 1;//程序异常状态退出
    }
    err=Start(l);
    if(err){
        return 1;
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

int Start(int l) {

}
