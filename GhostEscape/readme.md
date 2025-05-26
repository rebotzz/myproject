# 游戏：幽灵逃生

[参考教程](https://cppgamedev.top/courses/modular-ghost-escape)

## 简介

- 1.使用`SDL3`和`glm`
```
    glm编译

    SDL3_mixer编译
```
- 2.新的`代码框架`
![游戏框架设计](showing/03-游戏框架设计1.png)
![游戏框架设计](showing/03-游戏框架设计3.png)
![游戏框架设计](showing/03-游戏框架设计4.png)
![游戏框架设计](showing/03-游戏框架设计5.png)
![游戏框架设计](showing/08-自动挂载功能2.png)

- 3.使用`空间四叉树`优化碰撞检测
```
    又要代码重构了，

	出了好多bug，一天都在找bug，
	不好定位，改了一些地方，但是好像不对。
```

## Debug
```
    1.cmake添加源文件
    file(GLOB_RECURSE my_sources src/*.cpp)

    2.为了方便，我把三方库放在文件路径下，没有添加环境变量，
    所以，在可执行文件路径下需要放置库文件.dll(windows环境)

    3. 野指针/迭代器失效: 
    delete this，如果在类中delete this之后还有执行内容，会野指针。解决：改用标记setCanRemove，下次遍历初delete  
    遍历途中插入新的元素,vector可能扩容释放旧空间。解决：放入临时容器，下次遍历初push    safeAddChild
    场景切换导致资源清理问题， 放入临时变量，下次遍历初change    safeChangeScene
```

## 吐槽
```
总算写完了，
虽然代码采用自动挂载、自动处理的架构方便不少。
但也意味着和我之前习惯的写法有些不同，之前我习惯东西都放一起。
只有handleEvent, update, render
而现在，我需要考虑怎么把东西拆分为主体+组件。
理解和习惯这种模式花费了一些时间。虽然代码量增长不多，但是比上一个SDL_Shooter难写多了。
当我看完一遍教程后，有种感觉忘记的差不多了……(￣_￣|||)
然后呢，自己借着部分印象 + 俺寻思可以这样写 = 最终的成果有点地方可能和原教程差距较大。
接下来，该看看原教程代码和自己的区别。
```