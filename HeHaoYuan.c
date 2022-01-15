/*

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
struct strbuf {
    int len;
    int alloc;
    char *buf;
};

void strbuf_init(struct strbuf *sb, size_t alloc);
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);
void strbuf_release(struct strbuf *sb);
void strbuf_swap(struct strbuf *a, struct strbuf *b);
char *strbuf_detach(struct strbuf *sb, size_t *sz);
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
void strbuf_reset(struct strbuf *sb);

void strbuf_grow(struct strbuf *sb, size_t extra);
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
void strbuf_addch(struct strbuf *sb, char c);
void strbuf_addstr(struct strbuf *sb, const char *s);
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
void strbuf_setlen(struct strbuf *sb, size_t len);
size_t strbuf_avail(const struct strbuf *sb);
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);

void strbuf_rtrim(struct strbuf *sb);
void strbuf_ltrim(struct strbuf *sb);
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
int strbuf_getline(struct strbuf *sb, FILE *fp);

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max);
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen);
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len);
*/
#include"strbuf.h"

//2A
void strbuf_init(struct strbuf *sb, size_t alloc)//初始化 sb 结构体，容量为 alloc。
{
  sb->len=0;
  sb->alloc=alloc;
  sb->buf=(char*)malloc(sizeof(char)*(alloc));

	memset(sb->buf, '\0', alloc);
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//将字符串填充到 sb 中，长度为 len, 容量为 alloc。
{
  strbuf_init(sb,alloc);

  sb->buf=(char*)str;
  sb->len=len;

}
void strbuf_release(struct strbuf *sb)//释放 sb 结构体的内存。
{
  free(sb->buf);
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)//交换两个 strbuf。
{

  char*p=a->buf;
  a->buf=b->buf;
  b->buf=p;

  int temp=a->len;
  a->len=b->len;
  b->len=temp;

  temp=a->alloc;
  a->alloc=b->alloc;
  b->alloc=temp;
  
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)//将 sb 中的原始内存取出，并获得其容量。
{
  *sz=(unsigned int)sb->alloc;
  return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)//比较两个 strbuf 的内存是否相同。
{
  if(strncmp(first->buf,second->buf,first->len>second->len?second->len:first->len)||first->len!=second->len){
    return 1;
  }else{
    return 0;
  }
}
void strbuf_reset(struct strbuf *sb)//清空 sb。
{
  memset(sb->buf, '\0', sb->alloc);
  sb->len=0;
}


//2B
void strbuf_grow(struct strbuf *sb, size_t extra)//将 sb 的长度扩大 extra。
{
  if(sb->alloc==0){
        sb->alloc=extra;
    }else{
        sb->alloc=sb->len+extra;
    }
  sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
  
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)//向 sb 追加长度为 len 的数据 data。
{
  if(sb->alloc==0){
        sb->alloc=len+1;
    }else{
        while(sb->alloc<sb->len+len){
            sb->alloc*=2;
        }
    }
  sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
  strncat(sb->buf,(char*)data,len);
  sb->len+=len;
  sb->buf[sb->len]='\0';
}
void strbuf_addch(struct strbuf *sb, int c)//向 sb 追加一个字符 c。
{
  if(sb->alloc==0){
        sb->alloc=2;
    }else{
        while(sb->alloc<sb->len+2){
            sb->alloc*=2;
        }
    }
  sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
  sb->len++;
  sb->buf[sb->len-1]=c;
  sb->buf[sb->len]='\0';
}
void strbuf_addstr(struct strbuf *sb, const char *s)//向 sb 追加一个字符串 s。
{
  /*if(sb->alloc==0){
        sb->alloc=strlen(s)+1;
    }else{
        while(sb->alloc<sb->len+strlen(s)+1){
            sb->alloc*=2;
        }
    }
  sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
  strncat(sb->buf,s,strlen(s));
  */
    if(sb->alloc<=(sb->len+strlen(s)+1)) {
        if(sb->alloc==0){
        sb->alloc=strlen(s)+1;
    }else{
        sb->alloc=sb->len+strlen(s)+1;
    }
  sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
    }
    memcpy(sb->buf+sb->len,s,strlen(s));
    sb->len+=strlen(s);
    sb->buf[sb->len]='\0';
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//向一个 sb 追加另一个 strbuf的数据。
{
  /*
  int count=0;
  if(!sb->alloc){
        sb->alloc=sb2->len+1;
        sb->buf=(char*)malloc((sb2->len+1)*sizeof(char));
    }else{
        while(sb->alloc<sb->len+sb2->len+1){
            sb->alloc*=2;
            count++;
        }
    }
  if(count){
    sb->buf=(char*)realloc(sb->buf,sb->alloc*sizeof(char));
  }
  memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
  sb->len+=sb2->len;
  sb->buf[sb->len]='\0';
  */
  strbuf_addstr(sb,sb2->buf);
  
}
void strbuf_setlen(struct strbuf *sb, size_t len)//设置 sb 的长度 len。
{
  sb->len=(int)len;
  sb->buf[sb->len]='\0';
}
size_t strbuf_avail(const struct strbuf *sb)//计算 sb 目前仍可以向后追加的字符串长度。
{
  size_t n;
  n=sb->alloc-sb->len-1;
  return n;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
//向 sb 内存,坐标为 pos 位置插入长度为 len 的数据 data。
{
  if(sb->alloc==0){
    sb->alloc=len+1;
  }
  while(sb->alloc<sb->len+len){
    sb->alloc+=sb->alloc;
  }
  sb->buf=(char*)realloc(sb->buf,sizeof(char)*(sb->alloc));

  memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
  memmove(sb->buf+pos,(char*)data,len);
  sb->buf[sb->len+len]='\0';
  sb->len+=len;
  
}


//2C
void strbuf_ltrim(struct strbuf *sb)//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
{
  int count=0;
  while(sb->buf[count]==' '||sb->buf[count]=='\t'){
    count++;
  }
  memmove(sb->buf,sb->buf+count,strlen(sb->buf+count));
  sb->len=strlen(sb->buf+count);
  sb->buf[sb->len]='\0';
  /*sb->len-=count;
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
  free(arr);
  */
}
void strbuf_rtrim(struct strbuf *sb)//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
{
  int count=0;
  while(sb->buf[sb->len-1-count]==' '||sb->buf[sb->len-1-count]=='\t'){
    count++;
  }
  sb->buf[sb->len-count]='\0';
  sb->len-=count;
  /*
  char*temp=(char*)malloc(sizeof(char)*(count+1));
    strncpy(temp,sb->buf,count+1);
    strcpy(sb->buf,temp);
    free(temp);
    */
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
{
  memmove(sb->buf+pos,sb->buf+pos+len,len);
  sb->len-=len;
  sb->buf[sb->len]='\0';
  /*
  sb->len-=len;
  int length=sb->len-len;
  char* arr=(char*)malloc(sizeof(char)*length);
  char *p=sb->buf;
  int i=0;
  while(p!=sb->buf+pos){
    arr[i]=p[i];
    i++;
    p++;
  }
  p+=len;
  while(*p!='\0'){
    arr[i]=p[i];
    i++;
    p++;
  }
  p=sb->buf;
  i=0;
  while(*p!='\0'){
    p[i]='\0';
    i++;
  }
  strcpy(sb->buf,arr);
  free(arr);
  */
}


//2D
int strbuf_getline(struct strbuf *sb, FILE *fp)//将 将文件句柄为 fp 的一行内容读取到 sb 
{
  char a;
  while((a=fgetc(fp))!=EOF){
      if(a=='\n'||feof(fp)!=0){
        break;
      }
      if(sb->alloc==0){
      sb->alloc=2;
      }else{
        sb->alloc=sb->len+2;
      }

      sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
      sb->buf[sb->len]=a;
      sb->len++; 
    }
  sb->buf[sb->len]='\0';
  
  return sb->len;
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容读取到 sb 中。
{

  FILE*fp;
  fp=fdopen(fd,"r");
    
    char ch;
     while ((ch = fgetc(fp)) != EOF )
    {
        while(sb->alloc <= sb->len+1)
        {
            sb->buf = (char *)realloc(sb->buf,sb->len+(hint ? hint : 8192) );
            sb->alloc += (hint ? hint : 8192);
        }
      if(sb->alloc==0){
      sb->alloc=2;
      }else{
          while(sb->alloc<sb->len+2){
          sb->alloc*=2;
        }
      }
    sb->buf=(char*)realloc(sb->buf,(sb->alloc)*sizeof(char));
    sb->len++;
    sb->buf[sb->len-1]=ch;
    sb->buf[sb->len]='\0';
    }
    return sb->len;

}


//challenge
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
//将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回，max 可以用来限定最大切割数量。
//返回 struct strbuf 的指针数组，数组的最后元素为 NULL
{
  return NULL;
}
bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
//实现判断一个 strbuf 是否以指定字符串开头的功能（C 系字符串函数的另一个痛点）。
//target_str : 目标字符串，str : 前缀字符串，strlen : target_str 长度 ，前缀相同返回 true 失败返回 false
{

  if(strnlen<strlen(str)||target_str==NULL){
    return false;
  }

  int count;
  for(count=0;count<strlen(str);count++){
    if(target_str[count]==str[count]){
      count++;
    }else{
      break;
    }
  }
  if(count==strlen(str)){
    return true;
  }else{
    return false;
  }
}
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
//target_str : 目标字符串，begin : 开始下标，end 结束下标。len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，[begin, end)区间。
{
  if(target_buf==NULL||end-begin<=0){
    return NULL;
  }else{
    char* arr;
    arr=(char*)malloc(sizeof(char)*(end-begin+1));
    memcpy(arr,target_buf+begin,end-begin);
    arr[end-begin]='\0';
    return arr;
  }
}