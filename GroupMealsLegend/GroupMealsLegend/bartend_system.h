#pragma once
#include "bartend_material.h"
#include "bartend_tool.h"
#include "button.h"
#include "timer.h"

// ����ϵͳͳһ�����������������໥����
class BartendSystem
{
public:
	//enum class Staus
	//{
	//	// ״̬������δ��ʼ�������У������У����
	//};
	enum class Status
	{
		Init, Doing, Done	// ��ʼ״̬->�����У�ҡ�Σ�->������ɣ�ֹͣҡ�Σ�->�ύ
	};

private:
	static BartendSystem* manager;

	// ԭ��
	Adelhyde	 adelhyde;
	BronsonExt	 bronsonext;
	PwdDelta	 pwddelta;
	Flanergide	 flanergide;
	Karmotrine	 karmotrine;
	Ice			 ice;
	Ageing		 ageing;
	// ����ƿ��
	BartendBottle bartendbottle;
	// ���ܰ�ť; ���þ����࣬ע��ص�����
	Button button_redo;
	Button button_modulate;

	bool valid = false;						// �Ƿ�����
	Status status = Status::Init;			// ���ڸ�����������״̬
	Timer timer_button;						// ���°���״̬

private:
	BartendSystem();		
	~BartendSystem() = default;

public:
	static BartendSystem* instance();

	void open();
	void close();
	
	// �����峡������
	void on_update(float delta);

	void reset();
	void modulate();
};