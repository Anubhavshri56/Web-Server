#ifndef __TMWP__$_
#define __TMWP__$_ 123
#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<tmwp>
#include<Request>
#include<Response>
#include<unordered_map>
using namespace std;
using namespace projector;

//-------------------------------------------------------------------------------------------------------------------------EXTENSION EQUALS--------------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------------------------------------GET MIME TYPE----------------------------------------------------------------------------------------------------------------------------
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
// Memory not released
return mimeType;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------IS CLIENT SIDE RESOURCE-----------------------------------------------------------------------------------------------------------------------
char isClientSideResource(char *resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';
return 'Y';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






//----------------------------------------------------------------------------------------------------------------------------PARSE REQUEST----------------------------------------------------------------------------------------------------------------------------
Request * parseRequest(char *bytes)
{
char method[11],resource[1001];
int i,j;
int e,f,count,g,dataCount=0;
char **dataSet=NULL;
char **initials=NULL;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;
int *pc=NULL;

if(strcmp(method,"GET")==0)
{
for(j=0;bytes[i]!=' ' && bytes[i]!='?';j++,i++) resource[j]=bytes[i];            // changed here
resource[j]='\0';
if(bytes[i]=='?')
{
for(e=i+1,count=1;bytes[e]!=' ';e++) if(bytes[e]=='&') count++;
dataCount=count;
pc=new int[dataCount];
dataSet=new char*[dataCount];
initials=new char*[dataCount];
for(e=i+1,f=0;bytes[e]!=' ';e++)
{
if(bytes[e]=='&')
{
pc[f]=e;
f++;
}
}
pc[f]=e;
int howManyToPick;
for(f=i+1,g=0,e=i+1;g<dataCount;g++)
{
while(bytes[e]!='=') e++;
initials[g]=new char[e-f+1];
strncpy(initials[g],bytes+f,e-f);
initials[g][e-f]='\0';
e++;
howManyToPick=pc[g]-e;
dataSet[g]=new char[howManyToPick+1];
strncpy(dataSet[g],bytes+e,howManyToPick);
e=pc[g]+1;
f=pc[g]+1;
dataSet[g][howManyToPick]='\0';
}
}

}// method is of GET type

printf("Request arrived : %s\n",resource);

Request *request=new Request();
request->setMethod(method);
request->setDataCount(dataCount);
request->setDataSet(dataSet);
request->setInitials(initials);
if(resource[0]=='\0')
{
request->setIsClientSideTechnologyResource('Y');
}
else
{
request->setResource(resource);
request->setIsClientSideTechnologyResource(isClientSideResource(resource));
request->setMimeType(getMimeType(resource));
}
// Memory not released
if(pc!=NULL) delete [] pc;
return request;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------CONSTRUCTOR----------------------------------------------------------------------------------------------------------------------------
TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------DESTRUCTOR----------------------------------------------------------------------------------------------------------------------------
TMWebProjector::~TMWebProjector()
{
this->urlMap.clear();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------ON REQUEST----------------------------------------------------------------------------------------------------------------------------
void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(Request *request,Response *response))
{
if(url==NULL || ptrOnRequest==NULL) return;
int e=0,f,len;
len=strlen(url);
string tmpUrl;
if(url[0]=='/') e=1;
for(;e<len;e++) tmpUrl.push_back(url[e]);
this->urlMap[tmpUrl]=ptrOnRequest;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------START----------------------------------------------------------------------------------------------------------------------------
void TMWebProjector::start()
{
FILE *f;
WORD ver;
WSADATA wsaData;
ver=MAKEWORD(1,1);
char requestBuffer[8192],responseBuffer[1024],g;
int serverSocketDescriptor,len,successCode,bytesExtracted,rc;
Response *response=new Response();
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
while(1)//-------------------------------------------------------------------------------------------INFINITE LOOP started-------------------------------------------------------------------------------------------
{
sprintf(message,"TM server is now ready to accept request on %d",this->portNumber);
printf("%s\n",message);
len=sizeof(clientSocketInformation);
response->setSocket(accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len));
if(response->getSocket()<0)
{
printf("Unable to accept request\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(response->getSocket(),requestBuffer,sizeof(requestBuffer),0);
if(bytesExtracted<0)
{
//what to be done is yet to be decided

}//-------------------------------------------------------------------------------------------if(BytesExtracted<0) ended-------------------------------------------------------------------------------------------


else if(bytesExtracted==0)
{
printf("YES\n");
// what to be done is yet to be decided

}//-------------------------------------------------------------------------------------------else if(bytesExtracted==0) ended-------------------------------------------------------------------------------------------



else //-------------------------------------------------------------------------------------------if(BytesExtracted<0)'s last else started-------------------------------------------------------------------------------------------
{
requestBuffer[bytesExtracted]='\0';
Request *request=parseRequest(requestBuffer);
char header[500];
long length;
if(request->getIsClientSideTechnologyResource()=='Y') //-------------------------if(request->isClientSideTechnologyResource=='Y') started-------------------------------------------------------------------------------------------
{
// Client side resource code
if(request->getResource()==NULL) //------------------------------------------------------if(request->resource==NULL) started-------------------------------------------------------------------------------------------
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
response->sendError();
//sprintf(responseBuffer,"HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:163\nConnection: close\n\n<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Page not found </h3></body></html>",request->getMimeType());
//send(response->getSocket(),responseBuffer,strlen(responseBuffer)+1,0);
}
else
{
fseek(f,0,SEEK_END);
length=ftell(f);
fseek(f,0,SEEK_SET);
response->writeHeader("text/html",length);
//sprintf(header,"HTTP/1.1 200 OK\ncontent-type :text/html\ncontent-length : %ld\nConnection: close\n\n",length);
//send(response->getSocket(),header,strlen(header),0);
int e=0;
while(e<length)
{
rc=length-e;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
response->flush(responseBuffer,rc);
//send(response->getSocket(),responseBuffer,rc,0);
e+=rc;
}
fclose(f);
}
response->close();	
}//-------------------------------------------------------------------------------------------if(request->resource==NULL) ended-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------if(request->resource==NULL)'s else started-------------------------------------------------------------------------------------------
else
{
f=fopen(request->getResource(),"rb");
if(f==NULL)
{
// something needs to be done over here
response->sendError(request->getResource());
}
else
{
printf("Sending resource %s\n",request->getResource());
fseek(f,0,SEEK_END);
length=ftell(f);
fseek(f,0,SEEK_SET);
sprintf(header,"HTTP/1.1 200 OK\ncontent-type : %s\ncontent-length : %ld\nConnection: close\n\n",request->getMimeType(),length);
cout<<header;
response->writeHeader(request->getMimeType(),length);
//send(response->getSocket(),header,strlen(header),0);
int e=0;
while(e<length)
{
rc=length-e;
if(rc>1024) rc=1024;
fread(&responseBuffer,rc,1,f);
//send(response->getSocket(),responseBuffer,rc,0);
response->flush(responseBuffer,rc);
e+=rc;
}
fclose(f);
}
response->close();
}//-------------------------------------------------------------------------------------------if(request->resource==NULL)'s else ended-------------------------------------------------------------------------------------------


} //------------------------------------------------------------------------------------------if(request->isClientSideTechnologyResource=='Y') ended-------------------------------------------------------------------------------------------



else //--------------------------------------------------------------------------------------if(request->isClientSideTechnologyResource=='Y')'s else started-------------------------------------------------------------------------------------------
{
// server side resource code
if(this->urlMap.size()==0)
{
printf("Sending 404 error page\n");
char tmp[501];
response->sendError(request->getResource());
}
else
{
unordered_map<string,void (*)(Request *,Response *)>::iterator i;
int e,ii=0;
for(i=this->urlMap.begin();i!=this->urlMap.end();i++) cout<<i->first<<endl;
i=this->urlMap.find(request->getResource());
if(i!=this->urlMap.end())
{
i->second(request,response);
if(request->hasData()) request->release();
printf("Sending processed page\n");
}
else
{
printf("Sending 404 error page\n");
char tmp[501];
response->sendError(request->getResource());
}
}



} //--------------------------------------------------------------------------------------if(request->isClientSideTechnologyResource=='Y')'s else Ended-------------------------------------------------------------------------------------------



}//-------------------------------------------------------------------------------------------if(BytesExtracted<0) last else ended-------------------------------------------------------------------------------------------


// REQUEST *request should be released here
}//-------------------------------------------------------------------------------------------Server infinite loop ended-------------------------------------------------------------------------------------------
//successCode=send(response->getSocket(),responseBuffer,strlen(responseBuffer)+1,0);
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}//-------------------------------------------------------------------------------------------START ends-------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif