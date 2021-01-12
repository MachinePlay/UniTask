# DemoServer
=========
[![UnitTest](https://github.com/MachinePlay/DemoServer/actions/workflows/docker-image.yml/badge.svg)](https://github.com/MachinePlay/DemoServer/actions/workflows/docker-image.yml)

项目遵循 谷歌的[`Live at Head`](https://abseil.io/about/philosophy)思想，尽可能地去尝试使用最新版本的依赖和功能，推荐使用最新master主干提交版本。  
Project now follows the Abseil Live at Head philosophy. We recommend using the latest commit in the master branch in your projects.

# 依赖管理
C++的依赖管理一直以来是一个问题，项目使用CMake管理依赖，并且引入现代CMake的`FetchContent`依赖版本管理能力，编译前下载指定版本依赖库，从源码编译依赖库，这样做会带来一些编译期成本，但也能带来版本管理和迭代效率提升：
- 依赖库代码编译成静态库随源代码发布是一种常见的做法，但是在依赖库版本升级和编译工具链升级时会很难操作，可以想象项目迭代后成百上千个依赖都需要手工升级，依次替换静态库。在复杂的系统中是一个非常艰巨的问题。通过指定版本依赖库，从源码编译，可以方便的切换依赖库版本。带来编译时长的增长换可迭代能力，方便后续代码升级。

# 高版本GCC
使用GCC10起步，利用新特性和编译器优化能力。


# 服务目录树
- C++缺少好的依赖管理和组件化工具，我们用CMake做代码管理，DIY
- 各种功能组件应该分开在各自定目录存放，避免都写到一起
- 开源以来可以以静态库的方式存放，也可通过CMake版本依赖，每次随模块从源码编译
- 每次变更应该有单元测试
- 使用Github自带的Action来构建持续集成，有代码提交自动CI，通过容器化的单元测试才可以合入，后续考虑加入更多CI功能
- 服务本身要有容器化启动能力，例如带有docker compose，可以一键构建环境、启动  

- 代码结构  
```
DemoServer                                                  //服务代码库，每个服务一个代码库，独立管理，避免耦合，自主空间大
├── CMakeLists.txt                                          //住要代码组织
├── LICENSE                                                 //协议
├── README.md                                               //说明
├── build                                                   //主程序构建文件夹，提交代码时应该把里面的内容删除
├── conf                                                    //配置文件，仅线下使用，线上通过配置中心拉取
├── database                                                //组件库，例如这个文件夹可以放封装的数据库操作
├── docker-compose-unittest.yml                             //单元测试 容器启动
├── docker-compose.yml                                      //开发、运行时容器
├── frame                                                   //组件库，例如这里可以放核心引擎等
├── proto                                                   //依赖的proto
│   ├── EchoServant.h
│   └── EchoServant.tars
├── protogen                                                //依赖的proto
├── server                                                  //主程序 
│   ├── CMakeLists.txt
│   ├── EchoServantImp.cpp
│   ├── EchoServantImp.h
│   ├── EchoServer.cpp
│   └── EchoServer.h
├── shm_manager                                             //组件库，例如这里可以放共享内存库
├── test                                                    //单元测试的组织，在内层自行组织
│   ├── CMakeLists.txt
│   ├── Makefile
│   ├── build
│   ├── conf
│   ├── testcpp.cpp
│   ├── testcpp.h
│   └── unittest.cpp
├── thirdlib                                                //开源依赖，第三方依赖，一些静态库或者Header-Only的库都可以放这里
│   ├── hiredis
│   ├── mac-protoc
│   ├── mysql
│   ├── protobuf-3.13.0
│   ├── protobuf-cpp-3.13.0.tar.gz
│   └── protoc-3.13.0.0
└── utils                                                   //可以放一些小工具
```
