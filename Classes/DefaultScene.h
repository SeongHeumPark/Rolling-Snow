#ifndef __DEFAULT_SCENE_H__
#define __DEFAULT_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class DefaultScene : public CCLayerColor
{
public:
	/* ������, �Ҹ��� */
	DefaultScene();													// ������
	~DefaultScene();												// �Ҹ���

	/* CCLayerColor Override */
    virtual bool init();  
    static CCScene* scene();
    CREATE_FUNC(DefaultScene);

	/* ��� scene ��� �� logo scene �̵� */
	int waitTime;													// ��� scene ��� �ð�

	void waitDefaultScene( float );									// ��� scene ��� �ð� ������
	void sceneTrans();												// logo scene���� transition
	CCTransitionScene* createTransition( float, CCScene* );			// scene transition ȿ��
};

#endif