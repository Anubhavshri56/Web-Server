#include<iostream>
#include<tmwp>
using namespace std;


using namespace projector;
void doSomething()
{
cout<<"DoSomething got called"<<endl;
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/kkk",doSomething);
server.start();
return 0;
}