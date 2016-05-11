#include "LogoScene.h"
#include "MainScene.h"
#include <time.h>
#include "SimpleAudioEngine.h"   
#include "OptionScene.h"
extern int soundNum;
using namespace cocos2d;
using namespace CocosDenshion;
// ������, �Ҹ���
LogoScene::LogoScene()
{	
}
LogoScene::~LogoScene()
{
}

// CCLayerColor Override
CCScene* LogoScene::scene()
{
	LogoScene* layer = LogoScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool LogoScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;

	// ���� �ʱ�ȭ
	waitTime = 0;

	// �ΰ� �ʱ�ȭ
	this->initializeLogo();

	// ������ ����
	this->schedule( schedule_selector(LogoScene::waitLogoScene) );

	return true;
}

// �ΰ� scene ����
void LogoScene::initializeLogo()
{
	logo = CCSprite::create( "Images/logo/logo.jpg" );
	logo->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2) );
	this->addChild( logo );
}

// �ΰ� scene ��� �� main scene �̵�
void LogoScene::waitLogoScene( float f )
{
	++waitTime;
	if( waitTime > 120 ) { 
		this->unschedule( schedule_selector(LogoScene::waitLogoScene) );
		this->sceneTrans();
	}
}
void LogoScene::sceneTrans()
{
	// srand((unsigned)time(NULL));
	CCScene* pScene = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
}

//Sound
void LogoScene::mainSound()
{
	//Sound ���õ� Scene :LogoScene(Sound Manager), OptionScene , GameScene , MainScene /* ����� ���� ������ �뷡�� ���� */ �ٵ� ��ŷ�� ù��° �����Ҷ� �뷡�� �ٲ�µ� �ֹٲ������ �𸣰���
	if( soundNum == 1 )
	{
		offSound();
	}
	else
	{ 
		srand((unsigned int) time(NULL) );

		int rand_sound = rand()%3;

		switch( rand_sound )
		{
			case 0: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/mainScene.mp3"); break;
			case 1: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/mainScene2.mp3"); break;
			case 2: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/mainScene3.mp3"); break; 
		}
	} 
}
void LogoScene::gameSound()
{ 
	if(soundNum == 1 )
	{
		offSound();
	}
	else{
		srand((unsigned int) time(NULL) );

		int rand_sound = rand()%3;

		switch( rand_sound ) {
		case 0: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/gameScene.mp3"); break;
		case 1: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/gameScene2.mp3"); break;
		case 2: SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/gameScene3.mp3"); break; 
		}
	}
} 
void  LogoScene::takeRankSound()
{
	if(soundNum == 1 ) {
		offSound();
	}
	else {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/takeRank.wav");
	}
}
void  LogoScene::notakeRankSound()
{
	if(soundNum == 1 ) {
		offSound();
	}
	else {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/notakeRank.mp3");
	}
}  

void LogoScene::closeBackgroundSound()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);  
}


void LogoScene::mainEffectSound(){

	SimpleAudioEngine::sharedEngine()->preloadEffect("Sounds/buttonSound.wav"); 

	SimpleAudioEngine::sharedEngine()->playEffect("Sounds/buttonSound.wav");
}

void LogoScene::closeEffectSound()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
} 
void LogoScene::onSound()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(10);
}
void LogoScene::offSound()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
}