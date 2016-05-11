#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class LogoScene : public CCLayerColor
{
public :
	/* ������, �Ҹ��� */
	LogoScene();														// ������
	~LogoScene();														// �Ҹ���

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(LogoScene);
	
	/* �ΰ� scene ���� */
	CCSprite* logo;														// �ΰ� ��������Ʈ

	void initializeLogo();												// ��������Ʈ ����

	/* �ΰ� scene ��� �� main scene �̵� */
	int waitTime;														// �ΰ� scene ��� �ð�

	void waitLogoScene( float );										// �ΰ� scene ��� �ð� ������
	void sceneTrans();													// main scene���� transition
	CCTransitionScene* createTransition( int, float, CCScene* );		// scene transition ȿ��
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