//#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// �رտ���̨����

#include "game_system.h"
#undef main

// ��ʹ����֧�����飬Ҳ�����������
int main()
{
	GameSystem::instance()->start();
}
