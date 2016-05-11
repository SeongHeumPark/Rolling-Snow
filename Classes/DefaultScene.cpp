#include "DefaultScene.h"
#include "LogoScene.h"

using namespace cocos2d;

// ������, �Ҹ���
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

	// �ʱ�ȭ
	waitTime = 0;
	
	// ������ ����
	this->schedule( schedule_selector(DefaultScene::waitDefaultScene) );

	return true;
}

// ��� scene ��� �� logo scene �̵�
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
	// ���̵���/�ƿ� : ���� ȭ��
	return CCTransitionFade::create( t, s );
}