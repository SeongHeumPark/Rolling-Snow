#ifndef __DEFAULT_SCENE_H__
#define __DEFAULT_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class DefaultScene : public CCLayerColor
{
public:
	/* 생성자, 소멸자 */
	DefaultScene();													// 생성자
	~DefaultScene();												// 소멸자

	/* CCLayerColor Override */
    virtual bool init();  
    static CCScene* scene();
    CREATE_FUNC(DefaultScene);

	/* 대기 scene 대기 후 logo scene 이동 */
	int waitTime;													// 대기 scene 대기 시간

	void waitDefaultScene( float );									// 대기 scene 대시 시간 스케쥴
	void sceneTrans();												// logo scene으로 transition
	CCTransitionScene* createTransition( float, CCScene* );			// scene transition 효과
};

#endif