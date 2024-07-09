好的，下面是添加了更详细中文注释的完整项目代码：

### MySQLConnector.h

```cpp
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
```

### MySQLConnector.cpp

```cpp
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
```

### main.cpp

```cpp
#include "MySQLConnector.h"
#include <iostream>
#include <vector>

int main() {
    // 连接到 MySQL 数据库
    std::string host = "tcp://127.0.0.1:3306"; // 数据库主机
    std::string user = "root"; // 数据库用户
    std::string password = "your_password"; // 替换为你的 MySQL 密码
    std::string db = "test"; // 替换为你的数据库名称

    // 创建数据库连接器实例
    MySQLConnector connector(host, user, password, db);

    // 如果不存在，创建学生表
    std::string createStudentsTableQuery = "CREATE TABLE IF NOT EXISTS students ("
                                           "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," // 自动递增的主键
                                           "name VARCHAR(100) NOT NULL" // 学生姓名，非空
                                           ")";
    if (connector.executeQuery(createStudentsTableQuery)) {
        std::cout << "Table 'students' created or already exists.\n";
    } else {
        std::cerr << "Failed to create table 'students'.\n";
        return 1;
    }

    // 如果不存在，创建课程表
    std::string createCoursesTableQuery = "CREATE TABLE IF NOT EXISTS courses ("
                                          "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY," // 自动递增的主键
                                          "name VARCHAR(100) NOT NULL," // 课程名称，非空
                                          "student_id INT NOT NULL," // 学生 ID，非空
                                          "FOREIGN KEY (student_id) REFERENCES students(id)" // 外键，引用 students 表中的 id
                                          ")";
    if (connector.executeQuery(createCoursesTableQuery)) {
        std::cout << "Table 'courses' created or already exists.\n";
    } else {
        std::cerr << "Failed to create table 'courses'.\n";
        return 1;
    }

    // 示例：向学生表插入数据
    std::vector<std::string> insertStudentValues = {"John"}; // 插入的学生姓名
    if (connector.insertData("students", insertStudentValues)) {
        std::cout << "Data inserted into 'students' table successfully.\n";
    } else {
        std::cerr << "Failed to insert data into 'students' table.\n";
        return 1;
    }

    // 示例：向课程表插入数据
    std::vector<std::string> insertCourseValues = {"Math", "1"}; // 1 是学生表中的 student_id
    if (connector.insertData("courses", insertCourseValues)) {
        std::cout << "Data inserted into 'courses' table successfully.\n";
    } else {
        std::cerr << "Failed to insert data into 'courses' table.\n";
        return 1;
    }

    // 示例：多表查询，获取学生及其所选课程
    std::string multiTableQuery = "SELECT students.name AS student_name, courses.name AS course_name "
                                  "FROM students "
                                  "JOIN courses ON students.id = courses.student_id";
    std::vector<std::vector<std::string>> result = connector.selectQuery(multiTableQuery);



    // 输出查询结果
    std::cout << "Multi-table Query result:\n";
    for (const auto& row : result) {
        for (const auto& col : row) {
            std::cout << col << "\t"; // 输出每一列的值
        }
        std::cout << "\n";
    }

    return 0;
}
```

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.0)
project(MySQLConnectorExample)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 MySQL Connector/C++ 库
find_package(MySQLConnectorCpp REQUIRED)

# 包含 MySQL 头文件目录
include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIRS})

# 添加可执行文件
add_executable(main main.cpp MySQLConnector.cpp MySQLConnector.h)

# 链接 MySQL Connector/C++ 库
target_link_libraries(main ${MYSQLCONNECTORCPP_LIBRARIES})
```

### 使用说明

1. **配置 MySQL 数据库**：确保有一个 MySQL 数据库实例在运行，并且你可以访问到它。
  
2. **修改连接参数**：在 `main.cpp` 中修改 `host`、`user`、`password` 和 `db` 变量，以匹配你的 MySQL 数据库设置。

3. **构建项目**：使用 CMake 构建项目。在项目根目录下执行以下命令：

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **运行程序**：执行生成的可执行文件：

   ```sh
   ./main
   ```

以上代码展示了如何使用 C++ 和 MySQL Connector/C++ 进行基本的数据库操作，包括创建表、插入数据、更新数据、删除数据以及多表查询。每个函数和主要步骤都添加了详细的中文注释以帮助理解代码逻辑。
