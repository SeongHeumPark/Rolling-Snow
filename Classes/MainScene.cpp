#include "MainScene.h"
#include "LogoScene.h"  

#include "GameScene.h"
#include "StoreScene.h"
#include "OptionScene.h"
#include "RankingScene.h"

 
using namespace cocos2d;
 

// 생성자, 소멸자
MainScene::MainScene()
{
	LogoScene::mainSound();
}
MainScene::~MainScene()
{
}

// CCLayorColor Override
CCScene* MainScene::scene()
{
	MainScene* layer = MainScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool MainScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;  
	
	LogoScene::mainSound();

	// 게임로고 생성
	this->createMainLayer();
			
	// 메뉴 생성
	this->initializeMenu();

	return true;
}

// 메인 scene 구성
void MainScene::createMainLayer()
{
	// 배경
	background = CCSprite::create( "Images/main/background.png" );
	background->setAnchorPoint( ccp(0, 0) );
	background->setPosition( ccp(0, 0) );
	this->addChild( background );

	// 게임 로고
	gameLogo = CCSprite::create( "Images/main/mainChar.png" );
	gameLogo->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2. - 280, (CCDirector::sharedDirector()->getWinSize().height / 2.)) );
	gameLogo->setScale( 0.9f );
	this->addChild( gameLogo );

	RollingSnow = CCSprite::create("Images/main/mainName.png");
	RollingSnow->setPosition( ccp(gameLogo ->getPositionX()+450, gameLogo->getPositionY()+50));
	RollingSnow->setScale(1.1f);
	this->addChild(RollingSnow);
}

void MainScene::initializeMenu()
{  
	start = CCMenuItemImage::create("Images/main/Start_01.png","Images/main/Start_02.png",this,menu_selector(MainScene::sceneTrans) );
	store = CCMenuItemImage::create("Images/main/Store_01.png","Images/main/Store_02.png",this,menu_selector(MainScene::sceneTrans) );
	option = CCMenuItemImage::create("Images/main/Option_01.png","Images/main/Option_02.png",this,menu_selector(MainScene::sceneTrans) );
	ranking = CCMenuItemImage::create("Images/main/Ranking_01.png","Images/main/Ranking_02.png",this,menu_selector(MainScene::sceneTrans) );

	start->setScale( 1.1f );
	store->setScale( 1.1f );
	option->setScale( 1.1f );
	ranking->setScale( 1.1f );

	start->setTag(1);
	store->setTag(2);
	option->setTag(3);
	ranking->setTag(4);

	menu = CCMenu::create( start, store, option, ranking, NULL );
	menu->alignItemsHorizontally();
	menu->alignItemsHorizontallyWithPadding(50);
	menu->setPosition( ccp(gameLogo->getPosition().x +250 , gameLogo->getPosition().y - 400 ) );
	menu->setScale(0.8f);
	gameLogo->addChild( menu );
}

// 메인 scene에서 원하는 scene으로 이동
void MainScene::sceneTrans( CCObject *pSender )
{ 
	LogoScene::mainEffectSound();
	CCScene *pScene;
	switch( ((CCMenuItemImage*)pSender)->getTag() ) {
		case 1 :
			pScene = GameScene::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,pScene)); 
			break;
		case 2 :
			pScene = StoreScene::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,pScene)); 
			break;
		case 3 : pScene = OptionScene::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,pScene)); 
			break;
		case 4 : pScene = RankingScene::scene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5,pScene)); 
			break;
	}
}
// Main Scene 으로 돌아간다. 
 