#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct strbuf {
  int len;     //当前缓冲区（字符串）长度
  int alloc;   //当前缓冲区（字符串）容量
  char *buf;   //缓冲区（字符串）
};

void strbuf_init(struct strbuf *sb, size_t alloc)//初始化 sb 结构体，容量为 alloc。
{
  sb->len=0;
  sb->alloc=alloc;
  sb->buf=(char*)malloc(sizeof(char)*(sb->alloc));
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//将字符串填充到 sb 中，长度为 len, 容量为 alloc。
{
  sb->buf=str;
  sb->alloc=alloc;
  sb->len=len;
}
void strbuf_release(struct strbuf *sb)//释放 sb 结构体的内存。
{
  free(sb);
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)//交换两个 strbuf。
{
  char* arr=(char*)malloc(sizeof(char)*(strlen(a->buf)+1));
  int n=a->len;
  strcpy(arr,a->buf);
  a->len=b->len;
  while(a->alloc>(a->len)){
    a->alloc+=a->alloc;
    a->buf=(char*)realloc(a->buf,sizeof(char)*(a->alloc));
  }
  char*p=a->buf;
  while(*p!='\0'){
    *p='\0';
    p++;
  }
  strcpy(a->buf,b->buf);
  b->len=n;
  while(b->alloc>(b->len)){
    b->alloc+=b->alloc;
    b->buf=(char*)realloc(b->buf,sizeof(char)*(b->alloc));
  }
  p=b->buf;
  while(*p!='\0'){
    *p='\0';
    p++;
  }
  strcpy(b->buf,arr);
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)//将 sb 中的原始内存取出，并获得其长度。
{
  *sz=strlen(sb->buf);
  char*p=sb->buf;
  return p;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)//比较两个 strbuf 的内存是否相同。
{
  if(first->buf==second->buf&&first->alloc==second->alloc){
    return 1;
  }else{
    return -1;
  }
}
void strbuf_reset(struct strbuf *sb)//清空 sb。
{
  free(sb);
}
void strbuf_grow(struct strbuf *sb, size_t extra)//将 sb 的长度扩大 extra。
{
  char*p=sb->buf;
  p=(char*)realloc(p,(sb->alloc+extra)*sizeof(char));
  sb->alloc+=extra;
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)//向 sb 追加长度为 len 的数据 data。
{
  if(sb->len+len<=sb->alloc){
    sb->len+=len;
    strcat(sb->buf,data);
  }else{
    sb->alloc*=2;
    sb->buf=(char*)realloc(sb->buf,sizeof(char)*sb->alloc);
    strcat(sb->buf,data);
    sb->len+=len;
  }
}
void strbuf_addch(struct strbuf *sb, char c)//向 sb 追加一个字符 c。
{
  sb->len++;
  if(sb->len>sb->alloc){
    sb->alloc*=2;
    sb->buf=(char*)realloc(sb->buf,sizeof(char)*sb->alloc);
  }
  strcat(sb->buf,c);
}
void strbuf_addstr(struct strbuf *sb, const char *s)//向 sb 追加一个字符串 s。
{
  sb->len+=strlen(s);
  if(sb->len>sb->alloc){
    sb->alloc*=2;
    sb->buf=(char*)realloc(sb->buf,sizeof(char)*sb->alloc);
  }
  strcat(sb->buf,s);
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//向一个 sb 追加另一个 strbuf的数据。
{
  sb->len+=sb2->len;
  while(sb->alloc>(sb->len+sb2->len)){
    sb->alloc+=sb->alloc;
    sb->buf=(char*)realloc(sb->buf,sizeof(char)*(sb->alloc));
  }
  strcat(sb->buf,sb2->buf);
}
void strbuf_setlen(struct strbuf *sb, size_t len)//设置 sb 的长度 len。
{
  sb->len=len;
}
size_t strbuf_avail(const struct strbuf *sb)//计算 sb 目前仍可以向后追加的字符串长度。
{
  size_t n;
  n=sb->alloc-sb->len;
  return n;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
//向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
{
  sb->len+=len;
  while(sb->alloc>(sb->len+sb->len)){
    sb->alloc+=sb->alloc;
    sb->buf=(char*)realloc(sb->buf,sizeof(char)*(sb->alloc));
  }
  char*p=sb->buf;
  while(p!=pos){
    p++;
  }
  char *arr=(char*)malloc(sizeof(char)*strlen(p));
  for(int i=0;i<strlen(p);i++){
    *(arr+i)=*(p+i);
    *(p+i)='\0';
  }
  strcat(sb->buf,data);
  strcat(sb->buf,arr);
}
void strbuf_rtrim(struct strbuf *sb)//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
{
  int count=0;
  while(sb->buf[count]==' '){
    count++;
  }
  sb->len-=count;
  char*arr=(char*)malloc(sizeof(char)*(strlen(sb->buf)-count));
  while(sb->buf[count]!='\0'){
    int i=0;
    arr[i]=sb->buf[count];
    count++;
    i++;
  }
  char*p=sb->buf;
  while(*p!='\0'){
    *p='\0';
    p++;
  }
  strcpy(sb->buf,arr);
}
void strbuf_ltrim(struct strbuf *sb)//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
{
  int count=sb->len-1;
  while(sb->buf[count]==' '){
    count--;
    sb->len--;
  }
  char*temp=(char*)malloc(sizeof(char)*(count+1));
    strncpy(temp,sb->buf,count+1);
    strcpy(sb->buf,temp);
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
{
  sb->len-=len;
  int length=sb->len-len;
  char* arr=(char*)malloc(sizeof(char)*length);
  char *p=sb->buf;
  int i=0;
  while(p!=pos){
    arr[i]=p[i];
    i++;
    p+=1;
  }
  p+=len;
  while(*p!='\0'){
    arr[i]=p[i];
    i++;
    p++;
  }
  p=sb->buf;
  i=0;
  while(p!='\0'){
    p[i]='\0';
    i++;
  }
  strcpy(sb->buf,arr);
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容读取到 sb 中。
{

}
ssize_t strbuf_read_file(struct strbuf *sb, const char *path, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将路径为 path 的所有文件内容读取到 sb 中。
{

}
int strbuf_getline(struct strbuf *sb, FILE *fp)//将 将文件句柄为 fp 的一行内容读取到 sb 
{

}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
//将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回，max 可以用来限定最大切割数量。
{
  
}
bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
//	target_str : 目标字符串，str : 前缀，strlen : target_str 长度返回，结果相同返回 true 失败 false
{
  
}