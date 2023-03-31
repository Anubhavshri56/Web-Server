#include<iostream>
#include<tmwp>
#include<Request>
#include<fstream>
using namespace std;
using namespace projector;
void addStud(Request *request,Response *response)
{
string name=request->get("nm");
string rollNumber=request->get("rl");
ofstream f("Student.data",ios::app);
f<<rollNumber;
f<<" ";
f<<name;
f<<" ";
f.close();
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("student added<br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
void getAllStudents(Request *request,Response *response)
{
ifstream f;
f.open("Student.data");
int e=0;
char c;
string name;
string rollNumber;
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("list of students");
response->write("<table border='1'>");
response->write("<tr>");
response->write("<th>S.no</th>");
response->write("<th>Roll Number</th>");
response->write("<th>Name</th>");
response->write("</tr>");
while(1)
{
e++;
f>>rollNumber;
f>>name;
if(f.eof()) break;
response->write("<tr>");
response->write("<td>");
response->write(to_string(e).c_str());
response->write("</th>");
response->write("<td>");
response->write(rollNumber.c_str());
response->write("</th>");
response->write("<td>");
response->write(name.c_str());
response->write("</th>");
response->write("</tr>");
}
response->write("</table><br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
f.close();
}

void editStud(Request *request,Response *response)
{
string rollNumber=request->get("rl");
ifstream f("Student.data");
bool found=false;
int pos;
string tmpRollNumber,name;
while(1)
{
pos=f.tellg();
f>>tmpRollNumber;
f>>name;
if(f.eof()) break;
if(rollNumber==tmpRollNumber)
{
found=true;
break;
}
}
f.seekg(pos,ios::beg);
if(found)
{
f>>rollNumber;
f>>name;
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("<title>ACBD School</title>");
response->write("</head>");
response->write("<body>");
response->write("Student (Edit Module)");
response->write("<form method='get' action='updateStudent'>");
response->write("Roll Number : ");
response->write(rollNumber);
response->write("<input type='hidden' id='rl' name='rl' value='");
response->write(rollNumber);
response->write("'><br>");
response->write("Name <input type='text' id='nm' name='nm' value='");
response->write(name);
response->write("'><br>");
response->write("<button type='submit'>update</button>");
response->write("</form>");
response->write("<a href=\"index.html\">Home</a><br>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
else
{
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("Invalid roll Number<br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
f.close();
}

void updateStud(Request *request,Response *response)
{
ifstream f;
string rollNumber,name,fRollNumber,fName;
ofstream tmp;
rollNumber=request->get("rl");
name=request->get("nm");
f.open("Student.data");
tmp.open("tmp.tmp",ios::app);
while(1)
{
f>>fRollNumber;
f>>fName;
if(f.eof()) break;
if(fRollNumber!=rollNumber)
{
tmp<<fRollNumber;
tmp<<" ";
tmp<<fName;
tmp<<" ";
}
else
{
tmp<<fRollNumber;
tmp<<" ";
tmp<<name;
tmp<<" ";
}
}
f.close();
tmp.close();
f.open("tmp.tmp");
tmp.open("Student.data",ios::trunc);
while(1)
{
f>>fRollNumber;
f>>fName;
if(f.eof()) break;
tmp<<fRollNumber;
tmp<<" ";
tmp<<fName;
tmp<<" ";
}
f.close();
tmp.close();
tmp.open("tmp.tmp",ios::trunc);
tmp.close();
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("student updated<br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}

void deleteStud(Request *request,Response *response)
{
string rollNumber=request->get("rl");
ifstream f("Student.data");
bool found=false;
int pos;
string tmpRollNumber,name;
while(1)
{
pos=f.tellg();
f>>tmpRollNumber;
f>>name;
if(f.eof()) break;
if(rollNumber==tmpRollNumber)
{
found=true;
break;
}
}
f.seekg(pos,ios::beg);
if(found)
{
f>>rollNumber;
f>>name;
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("<title>ACBD School</title>");
response->write("</head>");
response->write("<body>");
response->write("Student (Delete Module)");
response->write("<form method='get' action='confirmDeleteStudent'>");
response->write("Roll Number : ");
response->write(rollNumber);
response->write("<input type='hidden' id='rl' name='rl' value='");
response->write(rollNumber);
response->write("'><br>");
response->write("Name : ");
response->write(name);
response->write("<input type='hidden' id='nm' name='nm' value='");
response->write(name);
response->write("'><br>");
response->write("<button type='submit'>Confirm Delete</button>");
response->write("</form>");
response->write("<a href=\"index.html\">Home</a><br>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
else
{
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("Invalid roll Number<br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}
f.close();
}


void confirmDelete(Request *request,Response *response)
{
ifstream f;
string rollNumber,name,fRollNumber,fName;
ofstream tmp;
rollNumber=request->get("rl");
name=request->get("nm");
f.open("Student.data");
tmp.open("tmp.tmp",ios::app);
while(1)
{
f>>fRollNumber;
f>>fName;
if(f.eof()) break;
if(fRollNumber!=rollNumber)
{
tmp<<fRollNumber;
tmp<<" ";
tmp<<fName;
tmp<<" ";
}
}
f.close();
tmp.close();
f.open("tmp.tmp");
tmp.open("Student.data",ios::trunc);
while(1)
{
f>>fRollNumber;
f>>fName;
if(f.eof()) break;
tmp<<fRollNumber;
tmp<<" ";
tmp<<fName;
tmp<<" ";
}
f.close();
tmp.close();
tmp.open("tmp.tmp",ios::trunc);
tmp.close();
response->write("<DOCTYPE HTML>");
response->write("<html lang='en'>");
response->write("<head>");
response->write("<meta charset='utf-8'>");
response->write("</head>");
response->write("<body>");
response->write("student deleted<br>");
response->write("<a href=\"index.html\">Home</a>");
response->write("</body>");
response->write("</html>");
response->flush();
response->close();
}


int main()
{
TMWebProjector server(8080);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",editStud);
server.onRequest("/updateStudent",updateStud);
server.onRequest("/deleteStudent",deleteStud);
server.onRequest("/confirmDeleteStudent",confirmDelete);
server.start();
return 0;
}