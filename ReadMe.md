### 使用说明

1. **配置 MySQL 数据库**：确保有一个 MySQL 数据库实例在运行，并且你可以访问到它。
  
2. **修改连接参数**：在 `main.cpp` 中修改 `host`、`user`、`password` 和 `db` 变量，以匹配你的 MySQL 数据库设置。

3. **构建项目**：使用 CMake 构建项目。在项目根目录下执行以下命令：

   ```sh
   mkdir build
   rm -rf build
   cmake -B build
   cmake --build build -j8
   ```

4. **运行程序**：执行生成的可执行文件：

   ```sh
   ./main
   ```

代码展示了如何使用 C++ 和 MySQL Connector/C++ 进行基本的数据库操作，包括创建表、插入数据、更新数据、删除数据以及多表查询。每个函数和主要步骤都添加了详细的中文注释以帮助理解代码逻辑。
