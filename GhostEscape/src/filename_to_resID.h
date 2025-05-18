#ifndef _FILENAME_TO_RESID_H_
#define _FILENAME_TO_RESID_H_

#include "resID.h"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, ResID> filename_resID = 
{
	{"1764",	 ResID::Tex_1764},
	{"184_3",	 ResID::Tex_1843},
	{"184_3_2",	 ResID::Tex_18432},
	{"Thunderstrike_w_blur",	 ResID::Tex_ThunderstrikeWBlur},
	{"ghostDead_Sheet",	 ResID::Tex_GhostDeadSheet},
	{"ghostHurt_Sheet",	 ResID::Tex_GhostHurtSheet},
	{"ghost_idle",	 ResID::Tex_GhostIdle},
	{"ghost_move",	 ResID::Tex_GhostMove},
	{"ghost_Sheet",	 ResID::Tex_GhostSheet},
	{"29",	 ResID::Tex_29},
	{"30",	 ResID::Tex_30},
	{"A_Back1",	 ResID::Tex_ABack1},
	{"A_Back2",	 ResID::Tex_ABack2},
	{"A_Back3",	 ResID::Tex_ABack3},
	{"A_Credits1",	 ResID::Tex_ACredits1},
	{"A_Credits2",	 ResID::Tex_ACredits2},
	{"A_Credits3",	 ResID::Tex_ACredits3},
	{"A_Pause1",	 ResID::Tex_APause1},
	{"A_Pause2",	 ResID::Tex_APause2},
	{"A_Pause3",	 ResID::Tex_APause3},
	{"A_Quit1",	 ResID::Tex_AQuit1},
	{"A_Quit2",	 ResID::Tex_AQuit2},
	{"A_Quit3",	 ResID::Tex_AQuit3},
	{"A_Restart1",	 ResID::Tex_ARestart1},
	{"A_Restart2",	 ResID::Tex_ARestart2},
	{"A_Restart3",	 ResID::Tex_ARestart3},
	{"A_Start1",	 ResID::Tex_AStart1},
	{"A_Start2",	 ResID::Tex_AStart2},
	{"A_Start3",	 ResID::Tex_AStart3},
	{"bar_bg",	 ResID::Tex_BarBg},
	{"bar_blue",	 ResID::Tex_BarBlue},
	{"bar_green",	 ResID::Tex_BarGreen},
	{"bar_red",	 ResID::Tex_BarRed},
	{"Blue_Potion",	 ResID::Tex_BluePotion},
	{"circle",	 ResID::Tex_Circle},
	{"Electric_Icon",	 ResID::Tex_ElectricIcon},
	{"Green_Potion",	 ResID::Tex_GreenPotion},
	{"pointer_c_shaded",	 ResID::Tex_PointerCShaded},
	{"Red_Potion",	 ResID::Tex_RedPotion},
	{"Textfield_01",	 ResID::Tex_Textfield01},

	{"OhMyGhost",	  ResID::Mus_OhMyGhost},
	{"Spooky_music",	  ResID::Mus_SpookyMusic},

	{"big_thunder",	  ResID::Sound_BigThunder},
	{"female_scream_02_89290",	  ResID::Sound_FemaleScream0289290},
	{"hit_flesh_02_266309",	  ResID::Sound_HitFlesh02266309},
	{"silly_ghost_sound_242342",	  ResID::Sound_SillyGhostSound242342},
	{"UI_button08",	  ResID::Sound_UIButton08},
	{"UI_button12",	  ResID::Sound_UIButton12},

	{"VonwaonBitmap_16px",	  ResID::Font_VonwaonBitmap16px},
	{"credits",	  ResID::Text_Credits},
};
#endif // _FILENAME_TO_RESID_H_

