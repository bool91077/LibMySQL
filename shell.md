下面是一个简单的 Shell 脚本示例，包含了 `build.sh`、`rebuild.sh`、`kill.sh`、`start.sh` 和 `restart.sh`，用于管理项目的构建、重建、停止、启动和重启操作。

### build.sh

```bash
#!/bin/bash

# 清理现有的构建目录（如果有）
echo "Cleaning previous build..."
rm -rf build
mkdir build
cd build

# 运行 CMake 构建
echo "Running CMake..."
cmake ..

# 编译并安装（这里使用了 8 个线程）
echo "Building..."
cmake --build . -j8

# 完成构建
echo "Build completed!"
```

### rebuild.sh

```bash
#!/bin/bash

# 清理现有的构建目录（如果有）
echo "Cleaning previous build..."
rm -rf build
mkdir build
cd build

# 运行 CMake 构建
echo "Running CMake..."
cmake ..

# 编译并安装（这里使用了 8 个线程）
echo "Building..."
cmake --build . -j8

# 完成构建
echo "Rebuild completed!"
```

### kill.sh

这个脚本假设你的程序是通过某个进程启动并运行的，需要根据实际情况替换掉 `your_program` 和 `your_process_name`。

```bash
#!/bin/bash

# 查找并杀死正在运行的进程
echo "Killing your_program..."

# 根据进程名杀死进程
pid=$(pgrep -f your_process_name)
if [ -n "$pid" ]; then
    echo "Killing process $pid"
    kill -9 $pid
else
    echo "No process found"
fi
```

### start.sh

这里假设你的程序的可执行文件位于 `build` 目录下，需要根据实际情况替换掉 `your_program`。

```bash
#!/bin/bash

# 启动你的程序
echo "Starting your_program..."

# 进入 build 目录并启动程序
cd build
./your_program &
```

### restart.sh

这个脚本结合了 `kill.sh` 和 `start.sh`，实现了停止程序并重新启动的功能。

```bash
#!/bin/bash

# 停止程序
./kill.sh

# 等待一段时间确保进程已经停止
sleep 1

# 启动程序
./start.sh
```

### 使用方法

1. **将以上五个脚本分别保存为 `build.sh`、`rebuild.sh`、`kill.sh`、`start.sh` 和 `restart.sh` 文件**。

2. **为这些脚本添加执行权限**：

   ```bash
   chmod +x build.sh rebuild.sh kill.sh start.sh restart.sh
   ```

3. **执行这些脚本来执行对应的操作**：

   - `./build.sh`：执行首次构建。
   - `./rebuild.sh`：清理并重新构建。
   - `./kill.sh`：停止正在运行的程序。
   - `./start.sh`：启动程序。
   - `./restart.sh`：停止程序并重新启动。

### 注意事项

- **路径问题：** 确保脚本在正确的项目根目录下运行，以便正确找到相关的文件和程序。
  
- **定制化需求：** 如果你的项目有特殊的需求或者依赖关系，可能需要修改脚本以满足项目的具体要求。

这些脚本可以帮助你简化项目管理过程，减少手动操作的复杂性和错误。