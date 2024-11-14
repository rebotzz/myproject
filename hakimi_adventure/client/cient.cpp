#include "../thirdparty/httplib.h"

#include "player.h"
#include "path.h"

#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>
#include <sstream>
#include <codecvt>

enum class State
{
	Waiting,						// 等待玩家加入
	Ready,							// 准备倒计时
	Racing							// 比赛中
};

State state = State::Waiting;		// 游戏当前状态
int val_countdown = 4;				// 起跑倒计时
int id_player = 0;					// 玩家序号
std::atomic<int> progress_1 = -1;	// 玩家1进度
std::atomic<int> progress_2 = -1;	// 玩家2进度
int num_total_char = 0;				// 总字符个数

// 玩家行进路径
Path path = Path(					
	{
		{842,842},{1322,842},{1322,442},
		{2762,442},{2762,842},{3162,842},
		{3162,1722},{2122,1722},{2122,1562},
		{842,1562},{842,842}
	}
);

int idx_line = 0;							// 文本行索引
int idx_char = 0;							// 当前行文本字符索引
std::string str_text;						// 文本内容
std::vector<std::string> str_line_list;		// 行文本列表


// 素材资源
// 玩家1
Atlas atlas_1P_idle_up;
Atlas atlas_1P_idle_down;
Atlas atlas_1P_idle_left;
Atlas atlas_1P_idle_right;
Atlas atlas_1P_run_up;
Atlas atlas_1P_run_down;
Atlas atlas_1P_run_left;
Atlas atlas_1P_run_right;
// 玩家2
Atlas atlas_2P_idle_up;
Atlas atlas_2P_idle_down;
Atlas atlas_2P_idle_left;
Atlas atlas_2P_idle_right;
Atlas atlas_2P_run_up;
Atlas atlas_2P_run_down;
Atlas atlas_2P_run_left;
Atlas atlas_2P_run_right;
// 界面和背景
IMAGE img_ui_1;
IMAGE img_ui_2;
IMAGE img_ui_3;
IMAGE img_ui_fight;
IMAGE img_ui_textbox;
IMAGE img_background;

std::string str_address;			// 服务器ip和端口
httplib::Client* client = nullptr;	// 客户端对象
int WINDOW_WIDTH = 1280;			// 游戏窗口大小
int WINDOW_HEIGHT = 720;


void load_resources(HWND hwnd);
void login_to_server(HWND hwnd);

int main(int argc, char* argv[])
{
	// 游戏初始化
	HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetWindowText(hwnd, _T("哈基米大冒险"));
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
	BeginBatchDraw();

	load_resources(hwnd);
	login_to_server(hwnd);

	ExMessage msg;
	Camera camera_ui, camera_scene;
	Timer timer_countdown;
	Player player_1(&atlas_1P_idle_left, &atlas_1P_idle_right, &atlas_1P_idle_up, &atlas_1P_idle_down, 
		&atlas_1P_run_left, &atlas_1P_run_right, &atlas_1P_run_up, &atlas_1P_run_down);
	Player player_2(&atlas_2P_idle_left, &atlas_2P_idle_right, &atlas_2P_idle_up, &atlas_2P_idle_down,
		&atlas_2P_run_left, &atlas_2P_run_right, &atlas_2P_run_up, &atlas_2P_run_down);

	camera_ui.set_size({ WINDOW_WIDTH, WINDOW_HEIGHT });
	camera_scene.set_size({ WINDOW_WIDTH, WINDOW_HEIGHT });
	player_1.set_position({ 842,842 });
	player_2.set_position({ 842,842 });

	timer_countdown.set_one_shot(false);
	timer_countdown.set_wait_time(1);
	timer_countdown.set_on_timeout([&]()
		{
			val_countdown--;
			switch (val_countdown)
			{
			case 3: play_audio(_T("ui_3")); break;
			case 2: play_audio(_T("ui_2")); break;
			case 1: play_audio(_T("ui_1")); break;
			case 0: play_audio(_T("ui_fight")); break;
			case -1:
				play_audio(_T("bgm"), true);
				state = State::Racing;
				break;
			default: break;
			}
		});

	using namespace std::chrono;
	nanoseconds frame_duration((int)1e9 / 144);
	steady_clock::time_point last_tick = steady_clock::now();

	// 主循环
	while (true)
	{
		// 处理消息输入
		while (peekmessage(&msg/*, EX_KEY | EX_CHAR*/))
		{
			if (state != State::Racing)
				continue;
			
			if (msg.message == WM_CHAR && idx_line < str_line_list.size())
			{
				switch (rand() % 4)
				{
				case 0: play_audio(_T("click_1"));  break;
				case 1: play_audio(_T("click_2"));  break;
				case 2: play_audio(_T("click_3"));  break;
				case 3: play_audio(_T("click_4"));  break;
				}

				std::string& str_line = str_line_list[idx_line];
				if (msg.ch == str_line[idx_char])
				{
					(id_player == 1) ? progress_1++ : progress_2++;

					idx_char++;
					if (idx_char >= str_line.size())
					{
						idx_char = 0;
						idx_line++;
					}
				}
			}
		}

		// 处理游戏数据更新
		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);

		if (state == State::Waiting)
		{
			if (progress_1 >= 0 && progress_2 >= 0)
				state = State::Ready;
		}
		else
		{
			if (state == State::Ready)
				timer_countdown.on_update(delta.count());

			if ((id_player == 1 && progress_1 >= num_total_char)
				|| (id_player == 2 && progress_2 >= num_total_char))
			{
				stop_audio(_T("bgm"));
				play_audio(id_player == 1 ? _T("1p_win") : _T("2p_win"));
				MessageBox(hwnd, _T("赢了"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
			else if ((id_player == 1 && progress_2 >= num_total_char)
				|| (id_player == 2 && progress_1 >= num_total_char))
			{
				stop_audio(_T("bgm"));
				MessageBox(hwnd, _T("输了"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}

			player_1.set_target(path.get_position_at_progress((float)progress_1 / num_total_char));
			player_2.set_target(path.get_position_at_progress((float)progress_2 / num_total_char));

			player_1.on_update(delta.count());
			player_2.on_update(delta.count());

			camera_scene.look_at((id_player == 1 ? player_1 : player_2).get_position());
		}


		// 渲染画面
		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		if (state == State::Waiting)
		{
			settextcolor(RGB(255, 255, 255));
			outtextxy(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, _T("等待其他玩家加入..."));
		}
		else
		{
			// 绘制背景
			static Rect rect_bg =
			{
				0, 0,
				img_background.getwidth(),
				img_background.getheight()
			};
			putimage_alpha_ex(camera_scene, &img_background, &rect_bg);

			// 绘制玩家
			if (player_1.get_position().y > player_2.get_position().y)
			{
				player_2.on_render(camera_scene);
				player_1.on_render(camera_scene);
			}
			else
			{
				player_1.on_render(camera_scene);
				player_2.on_render(camera_scene);
			}

			// 绘制界面
			if (state == State::Racing)
			{
				static Rect rect_textbox
				{
					0, WINDOW_HEIGHT - img_ui_textbox.getheight(),
					img_ui_textbox.getwidth(),
					img_ui_textbox.getheight()
				};
				putimage_alpha_ex(camera_ui, &img_ui_textbox, &rect_textbox);

				// 字符编码转化string -> wstring
				static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
				std::wstring wstr_line = convert.from_bytes(str_line_list[idx_line]);
				std::wstring wstr_completed = convert.from_bytes(str_line_list[idx_line].substr(0, idx_char));

				settextcolor(RGB(125, 125, 125));
				outtextxy(185 + 2, rect_textbox.y + 65 + 2, wstr_line.c_str());
				settextcolor(RGB(25, 25, 25));
				outtextxy(185, rect_textbox.y + 65, wstr_line.c_str());
				settextcolor(RGB(0, 255, 255));		// (0, 149, 217)
				outtextxy(185, rect_textbox.y + 65, wstr_completed.c_str());
			}

			// 绘制倒计时
			if (state == State::Ready)
			{
				static Rect rect_ui_3 =
				{
					(WINDOW_WIDTH - img_ui_3.getwidth()) / 2,
					(WINDOW_HEIGHT - img_ui_3.getheight()) / 2,
					img_ui_3.getwidth(),
					img_ui_3.getheight()
				};
				static Rect rect_ui_2 =
				{
					(WINDOW_WIDTH - img_ui_2.getwidth()) / 2,
					(WINDOW_HEIGHT - img_ui_2.getheight()) / 2,
					img_ui_2.getwidth(),
					img_ui_2.getheight()
				};
				static Rect rect_ui_1 =
				{
					(WINDOW_WIDTH - img_ui_1.getwidth()) / 2,
					(WINDOW_HEIGHT - img_ui_1.getheight()) / 2,
					img_ui_1.getwidth(),
					img_ui_1.getheight()
				};
				static Rect rect_ui_fight =
				{
					(WINDOW_WIDTH - img_ui_fight.getwidth()) / 2,
					(WINDOW_HEIGHT - img_ui_fight.getheight()) / 2,
					img_ui_fight.getwidth(),
					img_ui_fight.getheight()
				};
				switch (val_countdown)
				{
				case 3: putimage_alpha_ex(camera_ui, &img_ui_3, &rect_ui_3); break;
				case 2: putimage_alpha_ex(camera_ui, &img_ui_2, &rect_ui_2); break;
				case 1: putimage_alpha_ex(camera_ui, &img_ui_1, &rect_ui_1); break;
				case 0: putimage_alpha_ex(camera_ui, &img_ui_fight, &rect_ui_fight); break;
				}
			}

		}

		FlushBatchDraw();


		// 动态延时,维持帧率
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - nanoseconds(steady_clock::now() - last_tick);
		if (sleep_duration.count() > 0)
			std::this_thread::sleep_for(sleep_duration);
	}


	closegraph();
	EndBatchDraw();

	return 0;
}

int test()
{
	httplib::Client client("localhost:25565");
	std::cout << "This is Client." << std::endl;

	httplib::Result result = client.Post("/hello");
	if (!result || result->status != 200)
	{
		std::cout << "client post failed, path: /hello." << std::endl;
		return -1;
	}

	std::cout << result->body << std::endl;
}

void load_resources(HWND hwnd)
{
	// 字体
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, nullptr);

	// 图集和图片
	atlas_1P_idle_up.load(_T("resources/hajimi_idle_back_%d.png"), 4);
	atlas_1P_idle_down.load(_T("resources/hajimi_idle_front_%d.png"), 4);
	atlas_1P_idle_left.load(_T("resources/hajimi_idle_left_%d.png"), 4);
	atlas_1P_idle_right.load(_T("resources/hajimi_idle_right_%d.png"), 4);
	atlas_1P_run_up.load(_T("resources/hajimi_run_back_%d.png"), 4);
	atlas_1P_run_down.load(_T("resources/hajimi_run_front_%d.png"), 4);
	atlas_1P_run_left.load(_T("resources/hajimi_run_left_%d.png"), 4);
	atlas_1P_run_right.load(_T("resources/hajimi_run_right_%d.png"), 4);

	atlas_2P_idle_up.load(_T("resources/manbo_idle_back_%d.png"), 4);
	atlas_2P_idle_down.load(_T("resources/manbo_idle_front_%d.png"), 4);
	atlas_2P_idle_left.load(_T("resources/manbo_idle_left_%d.png"), 4);
	atlas_2P_idle_right.load(_T("resources/manbo_idle_right_%d.png"), 4);
	atlas_2P_run_up.load(_T("resources/manbo_run_back_%d.png"), 4);
	atlas_2P_run_down.load(_T("resources/manbo_run_front_%d.png"), 4);
	atlas_2P_run_left.load(_T("resources/manbo_run_left_%d.png"), 4);
	atlas_2P_run_right.load(_T("resources/manbo_run_right_%d.png"), 4);

	loadimage(&img_ui_1, _T("resources/ui_1.png"));
	loadimage(&img_ui_2, _T("resources/ui_2.png"));
	loadimage(&img_ui_3, _T("resources/ui_3.png"));
	loadimage(&img_ui_fight, _T("resources/ui_fight.png"));
	loadimage(&img_ui_textbox, _T("resources/ui_textbox.png"));
	loadimage(&img_background, _T("resources/background.png"));

	// 音频
	load_audio(_T("resources/bgm.mp3"), _T("bgm"));
	load_audio(_T("resources/1p_win.mp3"), _T("1p_win"));
	load_audio(_T("resources/2p_win.mp3"), _T("2p_win"));
	load_audio(_T("resources/click_1.mp3"), _T("click_1"));
	load_audio(_T("resources/click_2.mp3"), _T("click_2"));
	load_audio(_T("resources/click_3.mp3"), _T("click_3"));
	load_audio(_T("resources/click_4.mp3"), _T("click_4"));
	load_audio(_T("resources/ui_1.mp3"), _T("ui_1"));
	load_audio(_T("resources/ui_2.mp3"), _T("ui_2"));
	load_audio(_T("resources/ui_3.mp3"), _T("ui_3"));
	load_audio(_T("resources/ui_fight.mp3"), _T("ui_fight"));

	// 配置文件
	std::ifstream file("config.cfg");
	if (!file.good())
	{
		MessageBox(hwnd, _T("配置文件config.cfg打开失败"), _T("加载失败"), MB_OK | MB_ICONERROR);
		exit(-1);
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	str_address = str_stream.str();

	file.close();
}

void login_to_server(HWND hwnd)
{
	client = new httplib::Client(str_address);
	client->set_keep_alive(true);

	// 登陆
	httplib::Result result = client->Post("/login");
	if (!result || result->status != 200)
	{
		MessageBox(hwnd, _T("无法连接到服务器"), _T("启动失败"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	id_player = std::stoi(result->body);
	(id_player == 1) ? progress_1 = 0 : progress_2 = 0;

	if (id_player == -1)
	{
		MessageBox(hwnd, _T("比赛已经开始"), _T("拒绝加入"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	// 获取文本
	str_text = client->Post("/query_text")->body;
	std::stringstream str_stream(str_text);
	std::string str_line;
	while (std::getline(str_stream, str_line))
	{
		str_line_list.push_back(str_line);
		num_total_char += (int)str_line.size();
	}

	// 线程:更新同步
	std::thread([&]()
		{
			using namespace std::chrono;
			while (true)
			{
				std::string route = (id_player == 1 ? "/update_1" : "/update_2");
				std::string body = std::to_string((id_player == 1 ? progress_1 : progress_2));
				httplib::Result result = client->Post(route, body, "text/plain");

				if (result && result->status == 200)
				{
					(id_player == 1 ? progress_2 : progress_1) = std::stoi(result->body);
				}

				std::this_thread::sleep_for(nanoseconds((int)1e9 / 10));
			}
		}).detach();
}