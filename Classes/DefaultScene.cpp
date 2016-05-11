#include "DefaultScene.h"
#include "LogoScene.h"

using namespace cocos2d;

// 생성자, 소멸자
DefaultScene::DefaultScene()
{
}
DefaultScene::~DefaultScene()
{
}

// CCLayorColor Override
CCScene* DefaultScene::scene()
{
    CCScene* scene = CCScene::create();
	DefaultScene *layer = DefaultScene::create();
    scene->addChild(layer);
    return scene;
}
bool DefaultScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(0, 0, 0, 0) ) )
		return false;

	// 초기화
	waitTime = 0;
	
	// 스케줄 시작
	this->schedule( schedule_selector(DefaultScene::waitDefaultScene) );

	return true;
}

// 대기 scene 대기 후 logo scene 이동
void DefaultScene::waitDefaultScene( float f )
{
	++waitTime;
	if( waitTime >= 60 ) {
		this->unschedule( schedule_selector(DefaultScene::waitDefaultScene) );
		this->sceneTrans();
	}
}
void DefaultScene::sceneTrans()
{
	CCScene *pScene = LogoScene::scene();
	CCDirector::sharedDirector()->replaceScene( createTransition( 0.5, pScene ) );
}
CCTransitionScene* DefaultScene::createTransition( float t, CCScene* s )
{
	// 페이드인/아웃 : 검정 화면
	return CCTransitionFade::create( t, s );
}