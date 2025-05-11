executable = shooter.exe
include_path = -I../Thirdparty/jsoncpp -I../Thirdparty/SDL2/include \
-I../Thirdparty/SDL2_image/include -I../Thirdparty/SDL2_mixer/include \
-I../Thirdparty/SDL2_ttf/include
lib_path = -L../Thirdparty/SDL2/lib/x64 -L../Thirdparty/SDL2_image/lib/x64 \
-L../Thirdparty/SDL2_mixer/lib/x64 -L../Thirdparty/SDL2_ttf/lib/x64
libs = -lSDL2 -lSDL2_image -lSDL2_mixer -l SDL2_ttf
compile_flags = -Wall
jsoncpp_surce_code = ../Thirdparty/jsoncpp/jsoncpp.cpp



# 这里window进程替换出错 cd / mv, 或许是进程替换不能识别“别名”? why? 为什么del就可以
.PHONY:all
all: $(executable)

# jsoncpp.cpp需要编译，不然只包含了头文件
$(executable): main.cpp $(jsoncpp_surce_code)
	g++ -o $@ $^ -std=c++11 $(include_path) $(lib_path)  $(libs) $(compile_flags)


.PHONY: clean
clean:
	del $(executable)