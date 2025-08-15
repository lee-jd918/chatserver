#ifndef USER_HPP
#define USER_HPP
#include <string>
using namespace std;

//user表的ORM类，映射表的字段
class User
{
public:
    User(int id=-1,string name=" ",string password=" ",string state="offline")
    {
        this->id = id;
        this->name = name;
        this->password = password;
        this->state = state;
    }

    void setId(int id){this->id = id;}
    void setName(string name){this->name = name;}
    void setPassword(string password){this->password = password;}
    void setState(string state){this->state = state;}

    int getId(){return this->id;}
    string getName(){return this->name;}
    string getPassword(){return this->password;}
    string getState(){return this->state;}
private:
    int id;
    string name;
    string password;
    string state;

};


#endif