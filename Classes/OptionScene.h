#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "cocos2d.h"
#include "MainScene.h"
using namespace cocos2d;

class OptionScene : public CCLayerColor
{
public :
	/* 생성자, 소멸자 */
	OptionScene();												// 생성자
	~OptionScene();												// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(OptionScene);

	/* Option Scene 구성 */
	CCSprite* background;										// 배경
	CCSprite* panel;											// 나무 패널
	CCSprite* sceneName;										// scene 이름
	CCMenuItemImage* backButton;									// back 버튼 font
	CCMenu* back;												// back 버튼 메뉴
	CCSprite* sound;											// background sound 이름
	CCSprite* effect;											// effect sound 이름
	CCSprite *backgroundSoundOn[2];								// background sound on 버튼
	CCSprite *backgroundSoundOff[2];							// background sound off 버튼
	CCSprite *effectSoundOn[2];									// effect sound on 버튼
	CCSprite *effectSoundOff[2];								// effect sound off 버튼

	void createOptionLayer();									// Option Layer 생성
	void createBackgroundSoundButton();							// background sound 버튼 생성
	void createEffectSoundButton();								// effect sound 버튼 생성
	void returnMenu( CCObject * );								// back 버튼 클릭 시 효과

	/* 옵션 변경 효과 */
	int zOrder[4];												// 버튼 ZOrder 받는 변수
	static bool backgroundSoundState;							// background sound의 상태 표현
	static bool effectSoundState;								// effect sound의 상태 표현

	/* 터치 */
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );
};

#endif