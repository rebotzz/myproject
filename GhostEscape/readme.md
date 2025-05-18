# 游戏：幽灵逃生

## 简介

- 1.使用`SDL3`和`glm`
```
    glm编译

    SDL3_mixer编译
```


## Debug
```
    1.cmake添加源文件
    file(GLOB_RECURSE my_sources src/*.cpp)

    2.为了方便，我把三方库放在文件路径下，没有添加环境变量，
    所以，在可执行文件路径下需要放置库文件.dll(windows环境)
```