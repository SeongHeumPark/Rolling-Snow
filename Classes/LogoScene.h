#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class LogoScene : public CCLayerColor
{
public :
	/* 생성자, 소멸자 */
	LogoScene();														// 생성자
	~LogoScene();														// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(LogoScene);
	
	/* 로고 scene 구성 */
	CCSprite* logo;														// 로고 스프라이트

	void initializeLogo();												// 스프라이트 생성

	/* 로고 scene 대기 후 main scene 이동 */
	int waitTime;														// 로고 scene 대기 시간

	void waitLogoScene( float );										// 로고 scene 대기 시간 스케쥴
	void sceneTrans();													// main scene으로 transition
	CCTransitionScene* createTransition( int, float, CCScene* );		// scene transition 효과
	void static mainSound();
	void static gameSound();
	void static takeRankSound();
	void static notakeRankSound();
	void static closeBackgroundSound();

	void static mainEffectSound();
	void static closeEffectSound();

	void static onSound();
	void static offSound();
};

#endif