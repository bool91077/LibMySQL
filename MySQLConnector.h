#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <mysql/jdbc.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <string>
#include <vector>

// MySQLConnector 类用于封装 MySQL 数据库操作
class MySQLConnector {
private:
    sql::Driver *driver; // 驱动实例
    sql::Connection *conn; // 数据库连接
    sql::Statement *stmt; // SQL 语句执行器
    sql::ResultSet *res; // 结果集

public:
    // 构造函数，初始化数据库连接
    MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& db);
    // 析构函数，释放资源
    ~MySQLConnector();

    // 执行一般的 SQL 查询，返回执行是否成功
    bool executeQuery(const std::string& query);
    // 执行 SELECT 查询，返回查询结果
    std::vector<std::vector<std::string>> selectQuery(const std::string& query);
    // 插入数据，返回插入是否成功
    bool insertData(const std::string& tableName, const std::vector<std::string>& values);
    // 更新数据，返回更新是否成功
    bool updateData(const std::string& tableName, const std::string& setClause, const std::string& whereClause = "");
    // 删除数据，返回删除是否成功
    bool deleteData(const std::string& tableName, const std::string& whereClause);
};

#endif // MYSQLCONNECTOR_H
