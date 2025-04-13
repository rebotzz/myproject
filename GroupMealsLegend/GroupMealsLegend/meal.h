#pragma once

enum class Meal
{
	// 餐品
	None,						
	Cola,							// 可乐
	Sprite,							// 雪碧
	BraisedChicken_Hot,				// 黄焖鸡-热
	BraisedChicken_Cold,			// 
	MeatBall_Hot,					// 肉丸-热
	MeatBall_Cold,					// 
	RedCookedPork_Hot,				// 红烧肉-热
	RedCookedPork_Cold,				// 
	BraisedChicken_Box,				// 黄焖鸡（餐品盒子）
	MeatBall_Box,					// 肉丸（餐品盒子）
	RedCookedPork_Box,				// 红烧肉（餐品盒子）
	TakeoutBox,						// 外卖盒

	// 未知调酒原料
	Adelhyde,
	BronsonExt,
	PwdDelta,
	Flanergide,
	Karmotrine,
	Ice,							// 冰块
	Ageing,							// 陈化（特殊处理，当作原料）
	Shaking,						// 剧烈摇晃（特殊处理，当作原料）

	// 饮料
	Unkown,							// ??饮料，与配方不匹配
	CobltVlvt,
	FlffDream,
	MoonBlast,
	SugarRush
};
