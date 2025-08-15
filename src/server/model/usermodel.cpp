#include "usermodel.hpp"
#include "db/db.h"
//User表的增加方法
bool UserModel::insert(User &user)
{
    //组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            //获取插入成功用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
        
    }
    return false;      
}

User UserModel::query(int id)
{
    //组长sql语句
    char sql[1024]={0};
    sprintf(sql,"select * from user where id = %d",id);
    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES* res=mysql.query(sql);
        if(res!=nullptr)
        {
            MYSQL_ROW row=mysql_fetch_row(res);
            if(row!=nullptr)
            {
                User user;
                user.setId(atoi(row[0])); // id
                user.setName(row[1]);      // name
                user.setPassword(row[2]);  // password
                user.setState(row[3]);     // state
                mysql_free_result(res);    // 释放结果集
                return user;               // 返回查询到的用户信息
            }

        }
    }
    return User(); // 返回空用户对象
}

bool UserModel::updateState(User &user)
{
    //组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d",user.getState().c_str(), user.getId());

    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
        
    }
    return false;      
}

void UserModel::resetState()
{
    //组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = 'offline' where state = 'online'");

    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);

    }
  
}