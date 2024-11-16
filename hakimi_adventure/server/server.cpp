#include "../thirdparty/httplib.h"

#include <mutex>
#include <fstream>
#include <iostream>

// �ٽ���Դ
std::mutex g_mutex;		// ȫ�ֻ�����
std::string str_text;	// �����ı�����

int progress_1 = -1;	// ���1����
int progress_2 = -1;	// ���2����


int main(int argc, char* argv[])
{
	// 1.��ȡ�ļ��ı�
	std::fstream file("./text.txt", std::ios::in);
	if (!file.good())
	{
		MessageBox(nullptr, L"�޷����ļ� text.txt.", L"����ʧ��", MB_OK | MB_ICONERROR);
		return -1;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	str_text = str_stream.str();
	file.close();

	// 2.��ʼ�������
	httplib::Server server;
	//std::cout << "This is Server." << std::endl;

	server.Post("/hello", [](const httplib::Request& req, httplib::Response& res)
		{
			std::cout << "Hello from client." << std::endl;
			res.set_content("Hello from server!", "text/plain");
		});

	// ��½,��Ϊ��ҷ���id
	server.Post("/login", [](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			if (progress_1 >= 0 && progress_2 >= 0)
			{
				res.set_content("-1", "text/plain");
				return;
			}

			res.set_content(progress_1 >= 0 ? "2" : "1", "text/plain");
			(progress_1 >= 0) ? progress_2 = 0 : progress_1 = 0;
		});

	// ��ȡ�����ı�
	server.Post("/query_text", [](const httplib::Request& req, httplib::Response& res)
		{
			res.set_content(str_text, "text/plain");
		});

	// ������ҽ���,ͬ����һ���������״̬
	server.Post("/update_1", [](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			progress_1 = std::stoi(req.body);
			res.set_content(std::to_string(progress_2), "text/plain");
		});

	server.Post("/update_2", [](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			progress_2 = std::stoi(req.body);
			res.set_content(std::to_string(progress_1), "text/plain");
		});

	std::cout << "hakimi adventure server has initialized." << std::endl;

	server.listen("0.0.0.0", 25565);
	return 0;
}