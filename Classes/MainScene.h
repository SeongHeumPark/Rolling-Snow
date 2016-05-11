#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class MainScene : public CCLayerColor
{
public :
	/* 생성자, 소멸자 */
	MainScene();						// 생성자
	~MainScene();						// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(MainScene);

	/* 메인 scene 구성 */
	CCSprite* background;				// 배경
	CCSprite* gameLogo;					// 게임 캐릭터 로고
	CCSprite* RollingSnow;
	CCMenuItemImage* start;				// start 버튼 폰트
	CCMenuItemImage* store;				// store 버튼 폰트
	CCMenuItemImage* option;				// option 버튼 폰트
	CCMenuItemImage* ranking;			// ranking 버튼 폰트
	CCMenu* menu;						// 메인 scene 메뉴

	void createMainLayer();				// 메인 scene 생성
	void initializeMenu();				// 메뉴 초기화

	void backgroundSound();
	 
	 
	
	/* 메인 scene에서 원하는 scene으로 이동 */
	void sceneTrans( CCObject * );		// 원하는 scene으로 이동
 
	
};

#endif