#include<iostream>
#include<tmwp>
#include<Request>
using namespace std;
using namespace projector;
void doSomething(Request *request,Response *response)
{
string n=request->get("nm");
string c=request->get("ct");
response->write("<!DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("<title>TM</title>");
response->write("</head>");
response->write("<body>");
response->write("Hello ");
response->write(n);
response->write(" you have been registered");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
void doneSomething(Request *request,Response *response)
{
string n=request->get("kn");
string c=request->get("kt");
response->write("<!DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("<title>TM</title>");
response->write("</head>");
response->write("<body>");
response->write("Hello ");
response->write(n);
response->write(" you have iski maa ka been registered");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}

int main()
{
TMWebProjector server(8080);
server.onRequest("/register",doSomething);
server.onRequest("/registered",doneSomething);
server.start();
return 0;
}