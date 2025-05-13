# 游戏:SDL2与太空战机

## 简介
- 1.改用`vscode`/`g++`/`make`编译代码,这次不用`visual stuio`
```
    在window下使用vscode,make,g++编译程序,还真是新奇的体验。
    不过make执行指令不能是linux指令,它创建子进程和进程替换也不是用fork.
    好久没有手写makefile了,算是复习。那么调试时也可以复习gdb
    visual stuio平时把这些命令行指令统一成了图形界面，那么vs能用命令行
    例如：msbuild MyProject.sln /p:Configuration=Release

    使用cmake生成中间多了很多步骤，报错也不好找。
    相对而言，使用make能更好的查看报错。还是make更熟悉。
```

- 2.`cmake`使用
```
    cmake在vs和make/g++之上，之后可以配置试试cmake;
    cmake尝试之后，确实不错，编写CmakeLists.txt比makefile简单一些
    使用的话：
    1.在代码目录下创建编写CmakeLists.txt  (注意:新加入.c/.cpp源文件后记得add_executable加上)
    2.创建build文件夹，用于存放中间文件，例如使用visual stuio生成的.sln;g++生成的makefile等
    3.在build目录下运行cmake (参数:指定路径有CmakeLists.txt的路径),生成中间文件(sln或者makefile...) 
    (
        这里可以选择生成工具包(visual stuio 或者 g++; 选项),如果使用vscode这里就很方便了,可以在输出看设定的选项
        [cmake选项] 
        Release/Debug模式: cmake -DCMAKE_BUILD_TYPE=Release
        选择编译器: -DCMAKE_CXX_COMPILER=/usr/local/gcc/bin/g++
        指定生成器: -G "Visual Studio" 或者 -G "MinGW Makefiles"
        一些字段也可以在CmakeLists.txt中设置：
        set (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")
        set (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")

    )
    4.然后在build目录运行cmake --build . 别忘了".",即build路径,通过中间文件(sln...)生成可执行文件
    还待进阶学习...
    比如CMakePresets.json
```

- 3.资源加载
```
    老套路，使用资源管理器，需要注意素材名字统一风格(别出现字母/数字/下划线之外的,这样不好建立ResID变量名),
    powertoys的重命名挺好用，还是递归的。
    使用文件夹区分素材比单纯命名方便多了。对了，不同字体使用代理模式。
    "filesystem递归 + 仿函数不同类型文件分类处理" 方便多了。
```

- 4.装饰者模式
```
    Buff通过装饰模式实现。装饰模式 --> 核心："层层调用"
    version 1.0
    一开始我只打算添加"包装",不解除,知道最后玩家死亡才删除,这是层层析构。
    virtual析构函数的作用终于体现出来了。
    version 2.0
    后来觉得这个太臃肿，而且Buff一般有时间限制。
    所以通过检测是否失效，获取内部装饰的对象指针，delete外层失效包装器。这样就实现了预期效果。
```

## Debug
```
    好久没用gdb了，没有图形界面还需要适应
    有时候改了bug后还运行出错，那估计需要"重新生成",需要"彻底清理"之间的文件。
```