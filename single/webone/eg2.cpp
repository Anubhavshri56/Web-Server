#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<string.h>
using namespace std;
int main()
{
FILE *f=fopen("ujjain.jpeg","rb");
char c[1024];
string s;
int e=0,rc;
fseek(f,0,SEEK_END);
int length=ftell(f);
fseek(f,0,SEEK_SET);
while(e<length)
{
rc=length-e;
if(rc>1024) rc=1024;
fread(&c,rc,1,f);
s+=c;
cout<<"CHAR ("<<strlen(c)<<")"<<endl;
cout<<"STRING ("<<s.length()<<")"<<endl;
cout<<rc<<endl;
s.clear();
e+=rc;
}




return 0;
}