经过编译但没有连接的文件(比如linux上的.o文件)称为目标文件
ELF文件指连接好的可执行文件

目标文件/ELF文件有3种类型
1. 可重定位文件(relocatable file), 用于与其它目标文件进行连接以构建可执行文件或动态链接库
2. 共享目标文件(shared object file)
3. 可执行文件(executable file)
