#ifndef __RESPONSE__$_
#define __RESPONSE__$_ 123
#include<Response>
#include<windows.h>
#include<string.h>
#include<iostream>
using namespace std;
using namespace projector;
Response::Response()
{
this->clientSocketDescriptor=-1;
}
Response::~Response()
{
//do nothing
}
void Response::setSocket(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
}
int Response::getSocket()
{
return this->clientSocketDescriptor;
}
void Response::write(string response)
{
this->     response+=response;
}
void Response::write(const char *response)
{
this->response+=response;
}
void Response::flush()
{
string header;
header="HTTP/1.1 200 OK\n";
header+="content-type: text/html\n";
header+="content-Length: ";
header+=to_string(this->response.size());
header+="\nConnection: close";
header+="\n\n";
send(clientSocketDescriptor,header.c_str(),header.size()+1,0);
send(clientSocketDescriptor,this->response.c_str(),this->response.size()+1,0);
this->response.clear();
}
void Response::flush(const char *mimeType)
{
string header;
header="HTTP/1.1 200 OK\n";
header+="content-type: ";
header+=mimeType;
header+="\n";
header+="content-Length: ";
header+=to_string(this->response.size());
header+="\nConnection: close";
header+="\n\n";
send(this->clientSocketDescriptor,header.c_str(),header.size()+1,0);
send(this->clientSocketDescriptor,this->response.c_str(),this->response.size()+1,0);
this->response.clear();
}
void Response::flush(const char *responseBuffer,int size)
{
//cout<<"OKAY "<<this->response.size()<<endl;
send(this->clientSocketDescriptor,responseBuffer,size,0);
this->response.clear();
}
void Response::close()
{
closesocket(this->clientSocketDescriptor);
}
void Response::sendError()
{
string errorPage;
errorPage="HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:163\nConnection: close\n\n<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:red'>Page not found </h3></body></html>";
send(this->clientSocketDescriptor,errorPage.c_str(),errorPage.size()+1,0);
this->close();
}
void Response::sendError(const char *resource)
{
string errorHeader,errorResponse;
errorResponse="<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h3 style='color:green'>Resource ";
errorResponse+=resource;
errorResponse+=" not found </h3></body></html>";
errorHeader="HTTP/1.1 200 OK\ncontent-type:text/html\ncontent-length:";
errorHeader+=to_string(errorResponse.size());
errorHeader+="\nConnection: close\n\n";
send(this->clientSocketDescriptor,errorHeader.c_str(),errorHeader.size()+1,0);
send(this->clientSocketDescriptor,errorResponse.c_str(),errorResponse.size()+1,0);
this->close();
}
void Response::writeHeader(const char *mimeType,int contentLength)
{
string header;
header="HTTP/1.1 200 OK\ncontent-type : ";
header+=mimeType;
header+="\ncontent-length : ";
header+=to_string(contentLength);
header+="\nConnection: close";
header+="\n\n";
cout<<header;
send(this->clientSocketDescriptor,header.c_str(),header.size(),0);
}
#endif
