#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class MainScene : public CCLayerColor
{
public :
	/* ������, �Ҹ��� */
	MainScene();						// ������
	~MainScene();						// �Ҹ���

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(MainScene);

	/* ���� scene ���� */
	CCSprite* background;				// ���
	CCSprite* gameLogo;					// ���� ĳ���� �ΰ�
	CCSprite* RollingSnow;
	CCMenuItemImage* start;				// start ��ư ��Ʈ
	CCMenuItemImage* store;				// store ��ư ��Ʈ
	CCMenuItemImage* option;				// option ��ư ��Ʈ
	CCMenuItemImage* ranking;			// ranking ��ư ��Ʈ
	CCMenu* menu;						// ���� scene �޴�

	void createMainLayer();				// ���� scene ����
	void initializeMenu();				// �޴� �ʱ�ȭ

	void backgroundSound();
	 
	 
	
	/* ���� scene���� ���ϴ� scene���� �̵� */
	void sceneTrans( CCObject * );		// ���ϴ� scene���� �̵�
 
	
};

#endif