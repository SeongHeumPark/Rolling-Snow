#include "LogoScene.h"
#include "MainScene.h"
#include <time.h>
#include "SimpleAudioEngine.h"   
#include "OptionScene.h"
extern int soundNum;
using namespace cocos2d;
using namespace CocosDenshion;
// 생성자, 소멸자
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

	// 변수 초기화
	waitTime = 0;

	// 로고 초기화
	this->initializeLogo();

	// 스케줄 시작
	this->schedule( schedule_selector(LogoScene::waitLogoScene) );

	return true;
}

// 로고 scene 생성
void LogoScene::initializeLogo()
{
	logo = CCSprite::create( "Images/logo/logo.jpg" );
	logo->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2) );
	this->addChild( logo );
}

// 로고 scene 대기 후 main scene 이동
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
	//Sound 관련된 Scene :LogoScene(Sound Manager), OptionScene , GameScene , MainScene /* 현재는 메인 씬에만 노래가 나옴 */ 근데 랭킹씬 첫번째 리턴할때 노래가 바뀌는데 왜바뀌는지는 모르겠음
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