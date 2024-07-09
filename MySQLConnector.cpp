#include "MySQLConnector.h"

// 构造函数，初始化数据库连接
MySQLConnector::MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& db) {
    try {
        // 获取驱动实例
        driver = get_driver_instance();
        // 连接数据库
        conn = driver->connect(host, user, password);
        // 设置使用的数据库
        conn->setSchema(db);
        // 创建 SQL 语句执行器
        stmt = conn->createStatement();
    } catch (sql::SQLException &e) {
        // 捕获并打印 SQL 异常
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        exit(1);
    }
}

// 析构函数，释放资源
MySQLConnector::~MySQLConnector() {
    delete res; // 释放结果集
    delete stmt; // 释放 SQL 语句执行器
    delete conn; // 释放数据库连接
}

// 执行一般的 SQL 查询，返回执行是否成功
bool MySQLConnector::executeQuery(const std::string& query) {
    try {
        // 执行 SQL 查询
        stmt->execute(query);
    } catch (sql::SQLException &e) {
        // 捕获并打印 SQL 异常
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return false;
    }
    return true;
}

// 执行 SELECT 查询，返回查询结果
std::vector<std::vector<std::string>> MySQLConnector::selectQuery(const std::string& query) {
    std::vector<std::vector<std::string>> result; // 存储查询结果

    try {
        // 执行查询，获取结果集
        res = stmt->executeQuery(query);
        int num_fields = res->getMetaData()->getColumnCount(); // 获取结果集中的列数

        // 遍历结果集
        while (res->next()) {
            std::vector<std::string> row;
            for (int i = 1; i <= num_fields; ++i) {
                row.push_back(res->getString(i)); // 获取每一列的值
            }
            result.push_back(row); // 添加到结果向量
        }
    } catch (sql::SQLException &e) {
        // 捕获并打印 SQL 异常
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return result;
}

// 插入数据，返回插入是否成功
bool MySQLConnector::insertData(const std::string& tableName, const std::vector<std::string>& values) {
    std::string query = "INSERT INTO " + tableName + " VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        query += "?"; // 使用占位符
        if (i != values.size() - 1) query += ",";
    }
    query += ")";

    try {
        // 创建预处理语句
        sql::PreparedStatement *prep_stmt = conn->prepareStatement(query);

        // 绑定参数
        for (size_t i = 0; i < values.size(); ++i) {
            prep_stmt->setString(i + 1, values[i]);
        }

        // 执行插入操作
        prep_stmt->executeUpdate();
        delete prep_stmt; // 释放预处理语句

        return true;
    } catch (sql::SQLException &e) {
        // 捕获并打印 SQL 异常
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return false;
    }
}

// 更新数据，返回更新是否成功
bool MySQLConnector::updateData(const std::string& tableName, const std::string& setClause, const std::string& whereClause) {
    std::string query = "UPDATE " + tableName + " SET " + setClause;
    if (!whereClause.empty()) {
        query += " WHERE " + whereClause;
    }

    return executeQuery(query);
}

// 删除数据，返回删除是否成功
bool MySQLConnector::deleteData(const std::string& tableName, const std::string& whereClause) {
    std::string query = "DELETE FROM " + tableName + " WHERE " + whereClause;

    return executeQuery(query);
}
