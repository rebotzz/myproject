#if 0

void loadAnimation()
{
	for (int i = 0; i < PLAYER_ANIM_NUM; ++i)
	{
		std::wstring path1 = L"resource/img/player_left_" + std::to_wstring(i) + L".png";
		loadimage(img_player_left + i, path1.c_str());

		std::wstring path2 = L"resource/img/player_right_" + std::to_wstring(i) + L".png";
		loadimage(img_player_right + i, path2.c_str());
	}
}

// 使用动画循环播放
static int count = 0;	// 动画帧计数器: 每个几个游戏帧切换下一个动画帧
if (++count % 5 == 0) ++idx_current_anim;	
idx_current_anim %= PLAYER_ANIM_NUM;
//putimage_alpha(player_pos.x, player_pos.y, &img_player_left[idx_current_anim]);








#endif