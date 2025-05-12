# 可执行文件名
executable = shooter.exe

# 头文件,库文件目录，所需库文件
include_path = -I../Thirdparty/jsoncpp -I../Thirdparty/SDL2/include \
-I../Thirdparty/SDL2_image/include -I../Thirdparty/SDL2_mixer/include \
-I../Thirdparty/SDL2_ttf/include -I include
lib_path = -L../Thirdparty/SDL2/lib/x64 -L../Thirdparty/SDL2_image/lib/x64 \
-L../Thirdparty/SDL2_mixer/lib/x64 -L../Thirdparty/SDL2_ttf/lib/x64
link_libs = -lSDL2 -lSDL2_image -lSDL2_mixer -l SDL2_ttf

# 编译器选项，C++标准
CXX_STARDARD = -std=c++17
compile_flags = -Wall -g

# json使用源码，所以需要编译
jsoncpp_source_code = ../Thirdparty/jsoncpp/jsoncpp.cpp
source_code = main.cpp resourcesMgr.cpp gameMgr.cpp scene.cpp sceneMain.cpp
# preprocess_code = main.i resourcesMgr.i gameMgr.i scene.i sceneMain.i
# compile_code = main.s resourcesMgr.s gameMgr.s scene.s sceneMain.s
# assembly_code = main.o resourcesMgr.o gameMgr.o scene.o sceneMain.o


# 这里window进程替换出错 cd / mv, 或许是进程替换不能识别“别名”? 为什么del就可以
.PHONY:all
all: $(executable)

# 一步生成
$(executable): $(source_code)
	g++ -o $@ $^ $(include_path) $(lib_path)  $(link_libs) $(CXX_STARDARD) $(compile_flags)


# 分步:预处理/编译/汇编/链接 ESc iso 这里那里出错了?
# $(executable): assembly $(assembly_code)
# 	g++ -o $@ $^ $(lib_path) $(link_libs)

# assembly: compile $(compile_code)
# 	g++ -c $@ $^ 

# compile: preprocess $(preprocess_code)
# 	g++ -S $@ $^ $(CXX_STARDARD) $(compile_flags)

# preprocess: $(source_code)
# 	g++ -E $@ $^ $(include_path) 


.PHONY: clean
clean:
	del ./$(executable)
# del $(preprocess_code)
# del $(compile_code)
# del $(assembly_code)