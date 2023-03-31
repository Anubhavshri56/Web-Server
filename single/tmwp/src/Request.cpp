#ifndef __REQUEST__$_
#define __REQUEST__$_ 123
#include<RequestInterface>
#include<Request>
#include<iostream>
#include<string.h>
using namespace std;
using namespace projector;
Request::Request()
{
this->resource=NULL;
this->method=NULL;
this->mimeType=NULL;
this->isClientSideTechnologyResource=' ';
this->dataCount=0;
this->dataSet=NULL;
this->initials=NULL;
}
Request::~Request()
{
//do nothing
}
void Request::setResource(char *resource)
{
this->resource=(char *)malloc(sizeof(char)*(strlen(resource)+1));
strcpy(this->resource,resource);
}
char *Request::getResource()
{
return this->resource;
}
void Request::setMethod(char *method)
{
this->method=(char *)malloc(sizeof(char)*(strlen(method)+1));
strcpy(this->method,method);
}
char *Request::getMethod()
{
return this->method;
}
void Request::setIsClientSideTechnologyResource(char isClientSideTechnologyResource)
{
this->isClientSideTechnologyResource=isClientSideTechnologyResource;
}
char Request::getIsClientSideTechnologyResource()
{
return isClientSideTechnologyResource;
}
void Request::setDataCount(int dataCount)
{
this->dataCount=dataCount;
}
int Request::getDataCount()
{
return this->dataCount;
}
void Request::setMimeType(char *mimeType)
{
this->mimeType=mimeType;
}
char *Request::getMimeType()
{
return this->mimeType;
}
void Request::setDataSet(char *dataSet[])
{
if(this->dataSet!=NULL)
{
for(int e=0;e<this->dataCount;e++) delete [] this->dataSet[e];
delete []this->dataSet;
}
this->dataSet=dataSet;
}
void Request::setInitials(char *initials[])
{
this->initials=initials;
}
char * Request::get(const char *title)
{
int e,len;
char *tmpDataSet;
for(e=0;e<this->dataCount;e++) if(strcmp(title,this->initials[e])==0) break;
len=strlen(this->dataSet[e])+1;
tmpDataSet=new char[len];
strcpy(tmpDataSet,this->dataSet[e]);
tmpDataSet[len]='\0';
return tmpDataSet;
}
void Request::release()
{
for(int e=0;e<this->getDataCount();e++) delete [] this->dataSet[e];
delete []this->dataSet;
this->dataSet=NULL;
}
bool Request::hasData()
{
return this->dataSet!=NULL;
}

#endif