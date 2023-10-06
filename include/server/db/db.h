#ifndef DB_H
#define DB_H
#include<mysql/mysql.h>
#include <string>
using namespace std;
// 数据库操作类
class MySQL
{
public:
// 初始化数据库连接
    MySQL();

    ~MySQL();
    // 连接数据库
    bool connect();
    bool update(string sql);
    MYSQL_RES *query(string sql);
    MYSQL *getConnection();

private:
    MYSQL *_conn;
};

#endif