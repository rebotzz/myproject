#pragma comment (linker,"/subsystem:windows /entry:mainCRTStartup")		// �رտ���̨����

#include "../thirdparty/httplib.h"

#include "player.h"
#include "path.h"

#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>
#include <sstream>
#include <codecvt>

// v1.1 �����˵�,�˻���ս,�Ѷ�ѡ��,��������
// ��Ϸ����
enum class Scene
{
	Menu,
	Game
};
// ��Ϸ�˵�
enum class Menu
{
	PlayerVsPlayer,
	PlayerVsComputer,
	Invalid
};
// ��Ϸ�Ѷ�
enum class Difficulty
{
	D1,
	D2,
	D3,
	Invalid
};
Scene scene = Scene::Menu;							// ����
Menu menu_choice = Menu::PlayerVsPlayer;			// ��Ϸģʽ
Difficulty difficulty = Difficulty::D1;				// �˻��Ѷ�
int hits_num = 0;									// ��ǰ������
int pos_offset_hits_x = 0, pos_offset_hits_y = 0;	// hitsλ��ƫ��


// ��Ϸ״̬
enum class State
{
	Waiting,						// �ȴ���Ҽ���
	Ready,							// ׼������ʱ
	Racing,							// ������
	None
};

State state = State::None;			// ��Ϸ��ǰ״̬
int val_countdown = 4;				// ���ܵ���ʱ
int id_player = 0;					// ������
std::atomic<int> progress_1 = -1;	// ���1����
std::atomic<int> progress_2 = -1;	// ���2����
int num_total_char = 0;				// ���ַ�����

// ����н�·��
Path path = Path(
	{
		{842,842},{1322,842},{1322,442},
		{2762,442},{2762,842},{3162,842},
		{3162,1722},{2122,1722},{2122,1562},
		{842,1562},{842,842}
	}
);

int idx_line = 0;							// �ı�������
int idx_char = 0;							// ��ǰ���ı��ַ�����
std::string str_text;						// �ı�����
std::vector<std::string> str_line_list;		// ���ı��б�


// �ز���Դ
// ���1
Atlas atlas_1P_idle_up;
Atlas atlas_1P_idle_down;
Atlas atlas_1P_idle_left;
Atlas atlas_1P_idle_right;
Atlas atlas_1P_run_up;
Atlas atlas_1P_run_down;
Atlas atlas_1P_run_left;
Atlas atlas_1P_run_right;
// ���2
Atlas atlas_2P_idle_up;
Atlas atlas_2P_idle_down;
Atlas atlas_2P_idle_left;
Atlas atlas_2P_idle_right;
Atlas atlas_2P_run_up;
Atlas atlas_2P_run_down;
Atlas atlas_2P_run_left;
Atlas atlas_2P_run_right;
// ����ͱ���
IMAGE img_ui_1;
IMAGE img_ui_2;
IMAGE img_ui_3;
IMAGE img_ui_fight;
IMAGE img_ui_textbox;
IMAGE img_background;

std::string str_address;			// ������ip�Ͷ˿�
httplib::Client* client = nullptr;	// �ͻ��˶���
int WINDOW_WIDTH = 1280;			// ��Ϸ���ڴ�С
int WINDOW_HEIGHT = 720;


void load_resources(HWND hwnd);
void login_to_server(HWND hwnd);

int main(int argc, char* argv[])
{
	// ��Ϸ��ʼ��
	HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetWindowText(hwnd, _T("�����״�ð��"));
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);
	BeginBatchDraw();

	load_resources(hwnd);
	//login_to_server(hwnd);

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

	Timer timer_computer_P2;
	timer_computer_P2.set_one_shot(false);
	timer_computer_P2.set_wait_time(0.1f);
	timer_computer_P2.set_on_timeout([&]
		{
			switch (difficulty)
			{
			case Difficulty::D1: if (rand() % 100 <= 20) progress_2++; break;
			case Difficulty::D2: if (rand() % 100 <= 30) progress_2++; break;
			case Difficulty::D3: if (rand() % 100 <= 40) progress_2++; break;
			}
		});

	using namespace std::chrono;
	nanoseconds frame_duration((int)1e9 / 144);
	steady_clock::time_point last_tick = steady_clock::now();

	// ��ѭ��
	while (true)
	{
		// ������Ϣ����
		while (peekmessage(&msg/*, EX_KEY | EX_CHAR*/))
		{
			switch (scene)
			{
			case Scene::Menu:
				if (msg.message == WM_KEYDOWN)
				{
					switch (msg.vkcode)
					{
					case VK_UP:
						menu_choice = (Menu)(((int)menu_choice - 1 + (int)Menu::Invalid) % ((int)Menu::Invalid));
						break;
					case VK_DOWN:
						menu_choice = (Menu)(((int)menu_choice + 1) % ((int)Menu::Invalid));
						break;
					case VK_LEFT:
						difficulty = (Difficulty)(((int)difficulty - 1 + (int)Difficulty::Invalid) % ((int)Difficulty::Invalid));
						break;
					case VK_RIGHT:
						difficulty = (Difficulty)(((int)difficulty + 1) % ((int)Difficulty::Invalid));
						break;
					case VK_RETURN:
						state = State::Waiting;
						scene = Scene::Game;
						login_to_server(hwnd);

						if (menu_choice == Menu::PlayerVsComputer)
						{
							progress_2 = 0;
						}
						break;
					default:
						break;
					}
				}
				break;

			case Scene::Game:
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
						// ������¼
						hits_num++;
						pos_offset_hits_x = rand() % 20;
						pos_offset_hits_y = rand() % 20;
					}
					else
					{
						hits_num = 0;
					}
				}
				break;
			default:
				break;
			}
		}

		// ������Ϸ���ݸ���
		steady_clock::time_point frame_start = steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);

		if (state == State::Racing && menu_choice == Menu::PlayerVsComputer)
			timer_computer_P2.on_update(delta.count());

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
				MessageBox(hwnd, _T("��,Ӯ��"), _T("��Ϸ����"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
			else if ((id_player == 1 && progress_2 >= num_total_char)
				|| (id_player == 2 && progress_1 >= num_total_char))
			{
				stop_audio(_T("bgm"));
				MessageBox(hwnd, _T("��,����"), _T("��Ϸ����"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}

			player_1.set_target(path.get_position_at_progress((float)progress_1 / num_total_char));
			player_2.set_target(path.get_position_at_progress((float)progress_2 / num_total_char));

			player_1.on_update(delta.count());
			player_2.on_update(delta.count());

			camera_scene.look_at((id_player == 1 ? player_1 : player_2).get_position());
		}


		// ��Ⱦ����
		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		switch (scene)
		{
		case Scene::Menu:
			// ������Ϸ�˵�����
			settextcolor(RGB(255, 255, 255));
			settextstyle(45, 0, _T("IPix"));
			outtextxy((WINDOW_WIDTH - textwidth(_T("<<�����״�ð��>>"))) / 2, 200, _T("<<�����״�ð��>>"));
			settextstyle(28, 0, _T("IPix"));
			outtextxy((WINDOW_WIDTH - textwidth(_T("1.��� VS ���"))) / 2, 400, _T("1.��� VS ���"));
			outtextxy((WINDOW_WIDTH - textwidth(_T("2.��� VS ����"))) / 2, 450, _T("2.��� VS ����"));
			outtextxy((WINDOW_WIDTH - textwidth(_T("[�����] �� ��ѡ��VSģʽ, �� ��ѡ���˻��Ѷ�"))) / 2, 600, _T("[�����] �� ��ѡ��VSģʽ, �� ��ѡ���˻��Ѷ�"));
			outtextxy((WINDOW_WIDTH - textwidth(_T("[�س���]��ʼ��Ϸ"))) / 2, 660, _T("[�س���]��ʼ��Ϸ"));

			settextcolor(RGB(0, 255, 255));
			static int choice_offset_x = (WINDOW_WIDTH + textwidth(_T("1.��� VS ���"))) / 2 + 20;
			switch (menu_choice)
			{
			case Menu::PlayerVsPlayer:
				outtextxy(choice_offset_x, 400, _T(":>"));
				break;
			case Menu::PlayerVsComputer:
				outtextxy(choice_offset_x, 450, _T(":>"));
				break;
			}

			settextcolor(RGB(160, 160, 160));
			outtextxy((WINDOW_WIDTH - textwidth(_T("�˻��Ѷ�"))) / 2, 490, _T("�˻��Ѷ�"));
			static int diff_offset_x = (WINDOW_WIDTH + textwidth(_T("�˻��Ѷ�"))) / 2 + 20;
			switch (difficulty)
			{
			case Difficulty::D1:
				settextcolor(RGB(102, 255, 102));
				outtextxy(diff_offset_x, 490, _T("D1"));
				break;
			case Difficulty::D2:
				settextcolor(RGB(255, 178, 102));
				outtextxy(diff_offset_x, 490, _T("D2"));
				break;
			case Difficulty::D3:
				settextcolor(RGB(255, 102, 102));
				outtextxy(diff_offset_x, 490, _T("D3"));
				break;
			}

			break;
		case Scene::Game:

			if (state == State::Waiting)
			{
				settextcolor(RGB(255, 255, 255));
				outtextxy(WINDOW_WIDTH / 2 - textwidth(_T("�ȴ�������Ҽ���...")) / 2, WINDOW_HEIGHT / 2, _T("�ȴ�������Ҽ���..."));
			}
			else
			{
				// ���Ʊ���
				static Rect rect_bg =
				{
					0, 0,
					img_background.getwidth(),
					img_background.getheight()
				};
				putimage_alpha_ex(camera_scene, &img_background, &rect_bg);

				// �������
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

				// ���ƽ���
				if (state == State::Racing)
				{
					static Rect rect_textbox
					{
						0, WINDOW_HEIGHT - img_ui_textbox.getheight(),
						img_ui_textbox.getwidth(),
						img_ui_textbox.getheight()
					};
					putimage_alpha_ex(camera_ui, &img_ui_textbox, &rect_textbox);

					// �ַ�����ת��string -> wstring
					static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
					std::wstring wstr_line = convert.from_bytes(str_line_list[idx_line]);
					std::wstring wstr_completed = convert.from_bytes(str_line_list[idx_line].substr(0, idx_char));

					settextstyle(28, 0, _T("IPix"));
					settextcolor(RGB(125, 125, 125));
					outtextxy(180 + 2, rect_textbox.y + 65 + 2, wstr_line.c_str());
					settextcolor(RGB(25, 25, 25));
					outtextxy(180, rect_textbox.y + 65, wstr_line.c_str());
					settextcolor(RGB(0, 255, 255));		// (0, 149, 217)
					outtextxy(180, rect_textbox.y + 65, wstr_completed.c_str());
				}

				// ���Ƶ���ʱ
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

				// ������������
				settextstyle(45, 0, _T("IPix"));
				if (hits_num <= 5) settextcolor(RGB(128, 128, 128));
				else if (hits_num <= 15) settextcolor(RGB(153, 255, 255));
				else if (hits_num <= 25) settextcolor(RGB(102, 178, 255));
				else if (hits_num <= 40) settextcolor(RGB(255, 178, 102));
				else settextcolor(RGB(255, 0, 127));
				std::wstring wstr_hits = std::to_wstring(hits_num) + _T(" Hits");
				outtextxy(WINDOW_WIDTH - 220 + pos_offset_hits_x, 70 + pos_offset_hits_y, wstr_hits.c_str());
			}

			break;
		}

		FlushBatchDraw();


		// ��̬��ʱ,ά��֡��
		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - nanoseconds(steady_clock::now() - last_tick);
		if (sleep_duration.count() > 0)
			std::this_thread::sleep_for(sleep_duration);
	}


	closegraph();
	EndBatchDraw();

	return 0;
}

void load_resources(HWND hwnd)
{
	// ����
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, nullptr);

	// ͼ����ͼƬ
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

	// ��Ƶ
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

	// �����ļ�
	std::ifstream file("config.cfg");
	if (!file.good())
	{
		MessageBox(hwnd, _T("�����ļ�config.cfg��ʧ��"), _T("����ʧ��"), MB_OK | MB_ICONERROR);
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

	// ��½
	httplib::Result result = client->Post("/login");
	if (!result || result->status != 200)
	{
		MessageBox(hwnd, _T("�޷����ӵ�������"), _T("����ʧ��"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	id_player = std::stoi(result->body);
	(id_player == 1) ? progress_1 = 0 : progress_2 = 0;

	if (id_player == -1)
	{
		MessageBox(hwnd, _T("�����Ѿ���ʼ"), _T("�ܾ�����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	// ��ȡ�ı�
	str_text = client->Post("/query_text")->body;
	std::stringstream str_stream(str_text);
	std::string str_line;
	while (std::getline(str_stream, str_line))
	{
		str_line_list.push_back(str_line);
		num_total_char += (int)str_line.size();
	}

	// �߳�:����ͬ��
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
					if (menu_choice == Menu::PlayerVsPlayer)
						(id_player == 1 ? progress_2 : progress_1) = std::stoi(result->body);
				}

				std::this_thread::sleep_for(nanoseconds((int)1e9 / 10));
			}
		}).detach();
}