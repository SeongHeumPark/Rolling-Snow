#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "cocos2d.h"
#include "MainScene.h"
using namespace cocos2d;

class OptionScene : public CCLayerColor
{
public :
	/* ������, �Ҹ��� */
	OptionScene();												// ������
	~OptionScene();												// �Ҹ���

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(OptionScene);

	/* Option Scene ���� */
	CCSprite* background;										// ���
	CCSprite* panel;											// ���� �г�
	CCSprite* sceneName;										// scene �̸�
	CCMenuItemImage* backButton;									// back ��ư font
	CCMenu* back;												// back ��ư �޴�
	CCSprite* sound;											// background sound �̸�
	CCSprite* effect;											// effect sound �̸�
	CCSprite *backgroundSoundOn[2];								// background sound on ��ư
	CCSprite *backgroundSoundOff[2];							// background sound off ��ư
	CCSprite *effectSoundOn[2];									// effect sound on ��ư
	CCSprite *effectSoundOff[2];								// effect sound off ��ư

	void createOptionLayer();									// Option Layer ����
	void createBackgroundSoundButton();							// background sound ��ư ����
	void createEffectSoundButton();								// effect sound ��ư ����
	void returnMenu( CCObject * );								// back ��ư Ŭ�� �� ȿ��

	/* �ɼ� ���� ȿ�� */
	int zOrder[4];												// ��ư ZOrder �޴� ����
	static bool backgroundSoundState;							// background sound�� ���� ǥ��
	static bool effectSoundState;								// effect sound�� ���� ǥ��

	/* ��ġ */
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );
};

#endif