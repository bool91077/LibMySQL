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
