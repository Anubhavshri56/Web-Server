#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
 #include<tmwp>
using namespace std;
using namespace projector;
typedef struct _request
{
char *resource;
char *method;
char isClientSideTechnologyResource;
char *mimeType;
}REQUEST;
int extensionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a!=b) return 0;
right++;
left++;
}
return *left==*right;
}
char * getMimeType(char * resource)
{
char *mimeType;
mimeType=NULL;
int len=strlen(resource);
if(len<3) return mimeType;
int lastIndexOfDot;
for(lastIndexOfDot=len-1;lastIndexOfDot>0 && resource[lastIndexOfDot]!='.';lastIndexOfDot--);
if(lastIndexOfDot==0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1,"html"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*25);
strcpy(mimeType,"image/vnd.microsoft.icon");
}
return mimeType;
}
char isClientSideResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';
return 'Y';
}
REQUEST * parseRequest(char *bytes)
{
char method[11],resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;
for(j=0;bytes[i]!=' ';j++,i++) resource[j]=bytes[i];
resource[j]='\0';
printf("Request arrived : %s\n",resource);
REQUEST *request=(REQUEST *)malloc(sizeof(REQUEST));
request->method=(char *)malloc(sizeof(char)*(strlen(method)+1));
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->mimeType=NULL;
request->isClientSideTechnologyResource='Y';
request->resource=NULL;
}
else
{
request->resource=(char *)malloc(sizeof(char)*(strlen(resource)+1));
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMimeType(resource);
}
return request; 
}
TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
this->url=NULL;
this->ptrOnRequest=NULL;
}
TMWebProjector::~TMWebProjector()
{
if(this->url) delete [] this->url;
}
void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)())
{
if(this->url) delete []this->url;
this->url=NULL;
this->ptrOnRequest=NULL;
if(url==NULL || ptrOnRequest==NULL) return;
this->url=new char[strlen(url)+1];
strcpy(this->url,url);
this->ptrOnRequest=ptrOnRequest;
}
void TMWebProjector::start()
{
FILE *f;
WORD ver;
WSADATA wsaData;
ver=MAKEWORD(1,1);
char requestBuffer[8192],responseBuffer[1024],g;
int clientSocketDescriptor,serverSocketDescriptor,len,successCode,bytesExtracted,rc;
struct sockaddr_in clientSocketInformation,serverSocketInformation;
WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Enable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
char message[100];
if(successCode<0)
{
sprintf(message,"Unable to bind socket to %d",this->portNumber);
printf("%s\n",message);
WSACleanup();
return;
}
listen(serverSocketDescriptor,10);
while(1)
{
sprintf(message,"TM server is now ready to accept request on %d",this->portNumber);
printf("%s\n",message);
len=sizeof(clientSocketInformation);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Unable to accept request\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(bytesExtracted<0)
{
//what to be done is yet to be decided
}
else if(bytesExtracted==0)
{
printf("YES\n");
// what to be done is yet to be decided
}
else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
char header[500];
long length;
if(request->isClientSideTechnologyResource=='Y')
{
// Client side resource code
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f!=NULL) printf("Sending index.html\n");
if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL) printf("Sending index.htm\n");
}
if(f==NULL)
{
printf("Sending 404 error page\n");
sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:163\n\n<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Page not found </h3></body></html>",request->mimeType);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
}
else
{
fseek(f,0,SEEK_END);
length=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP/1.1 200 OK\ncontent-type :text/html\ncontent-length : %ld\nkeep-Alive: timeout=5, max=1000\n\n",length);
send(clientSocketDescriptor,header,strlen(header),0);
int e=0;
while(e<length)
{
rc=length-e;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
e+=rc;
}	//infinite loop closed
fclose(f);
closesocket(clientSocketDescriptor);
}	// f==NULL else closed
}	//Resource==NULL closed
else
{
f=fopen(request->resource,"rb");
if(f==NULL)
{
// something needs to be done over here
printf("Sending 404 error page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Resource/%s not found </h3></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:%d\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
}
else
{
printf("Sending resource %s\n",request->resource);
fseek(f,0,SEEK_END);
length=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP/1.1 200 OK\ncontent-type : %s\ncontent-length : %ld\nkeep-Alive: timeout=5, max=1000\n\n",request->mimeType,length);
send(clientSocketDescriptor,header,strlen(header),0);
int e=0;
while(e<length)
{
rc=length-e;
if(rc>1024) rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
e+=rc;
}	//infinite loop closed
fclose(f);
closesocket(clientSocketDescriptor);
}
}	//resource==NULL else closed
}	// isClientSideTechnology closed
else
{
// server side resource code
if(this->url==NULL || this->ptrOnRequest==NULL)
{
printf("Sending 404 error page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Resource/%s not found </h3></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:%d\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
}
else
{
int ii=0;
if(this->url[0]=='/') ii=1;
if(strcmp(this->url+ii,request->resource)==0)
{
this->ptrOnRequest();
printf("Sending processed page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Resource/%s processed </h3></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:%d\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
}
else
{
printf("Sending 404 error page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Resource/%s not found </h3></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:%d\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
}
}
}

}
}	//Server infinite loop ended
//successCode=send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer)+1,0);
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}