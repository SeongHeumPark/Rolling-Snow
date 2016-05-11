#include "OptionScene.h"
#include "LogoScene.h"

using namespace cocos2d;

// static 변수 재선언
bool OptionScene::backgroundSoundState;
bool OptionScene::effectSoundState;
extern  int soundNum = 0;

// 생성자, 소멸자
OptionScene::OptionScene()
{  
}
OptionScene::~OptionScene()
{
}

// CCLayerColor Override
CCScene* OptionScene::scene()
{
	OptionScene* layer = OptionScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool OptionScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;

	soundNum =1; 
	
	// 옵션 정보도 파일 입출력으로 현재 상태를 받아와야 한다. 초기값은 다 1(On)로 준다.
	// 게임 시작과 동시에 옵션 정보 파일을 읽어들여 배경음, 효과음을 설정한다.( on/off 설정에 따라 눌린 이미지를 설정한다. )
	// 옵션으로 들어와 설정을 바꾸고 back을 했을 때 옵션 정보 파일에 설정 값을 다시 저장한다.
	this->createOptionLayer();
	this->createEffectSoundButton();
	this->createBackgroundSoundButton();

	return true;
}		

// Option Scene 구성
void OptionScene::createOptionLayer()
{
	// back ground
	background = CCSprite::create( "Images/option/background.png" );
	background->setAnchorPoint( ccp(0, 0) );	
	background->setPosition( ccp(0, 0) );
	this->addChild( background );

	// panel 추가
	panel = CCSprite::create( "Images/option/panel.png" );
	panel->setScale( 0.9f );
	panel->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2) );
	background->addChild( panel );

	// scene name 추가
	sceneName = CCSprite::create( "Images/option/option.png" );
	sceneName->setPosition( ccp(panel->getPosition().x, panel->getPosition().y + 220));
	panel->addChild( sceneName );

	// sound label 삽입
	sound = CCSprite::create( "Images/option/sound.png" );
	sound->setPosition( ccp(panel->getPosition().x - 350, panel->getPosition().y) );
	panel->addChild( sound );

	// effect label 삽입
	effect = CCSprite::create( "Images/option/effect.png" );
	effect->setPosition( ccp(sound->getPosition().x, sound->getPosition().y - 100) );
	panel->addChild( effect );

	// back menu 추가
	backButton = CCMenuItemImage::create( "Images/option/exit_01.png", "Images/option/exit_02.png", this, menu_selector( OptionScene::returnMenu ) );
	CCMenu* back = CCMenu::create( backButton, NULL );
	back->setPosition( ccp( panel->getPosition().x + 95, panel->getPosition().y - 35 ) );
	back->setScale( 0.3f );
	panel->addChild( back, 5 );
}
void OptionScene::createBackgroundSoundButton()
{
	std::string backgroundSoundImageName[2][2];
	backgroundSoundImageName[0][0] = "Images/option/on_01.png";
	backgroundSoundImageName[0][1] = "Images/option/on_02.png";
	backgroundSoundImageName[1][0] = "Images/option/off_01.png";
	backgroundSoundImageName[1][1] = "Images/option/off_02.png";

	for( int i = 0; i < 2; i++ ) {
		backgroundSoundOn[i] = CCSprite::create( backgroundSoundImageName[0][i].c_str() );
		backgroundSoundOn[i]->setPosition( CCPointMake(sound->getPosition().x + 300, sound->getPosition().y) );
		backgroundSoundOn[i]->setScale( 0.3f );

		backgroundSoundOff[i] = CCSprite::create( backgroundSoundImageName[1][i].c_str() );
		backgroundSoundOff[i]->setPosition( CCPointMake(backgroundSoundOn[i]->getPosition().x + 250, sound->getPosition().y) );
		backgroundSoundOff[i]->setScale( 0.3f );

		if( backgroundSoundState ) {
			panel->addChild( backgroundSoundOn[i], i );
			panel->addChild( backgroundSoundOff[i], !i );
		}
		else {
			panel->addChild( backgroundSoundOn[i], !i );
			panel->addChild( backgroundSoundOff[i], i );
		}
	}
}
void OptionScene::createEffectSoundButton()
{
	std::string effectSoundImageName[2][2];
	effectSoundImageName[0][0] = "Images/option/on_01.png";
	effectSoundImageName[0][1] = "Images/option/on_02.png";
	effectSoundImageName[1][0] = "Images/option/off_01.png";
	effectSoundImageName[1][1] = "Images/option/off_02.png";

	for( int i = 0; i < 2; i++ ) {
		effectSoundOn[i] = CCSprite::create( effectSoundImageName[0][i].c_str() );
		effectSoundOn[i]->setPosition( ccp(effect->getPosition().x + 300, effect->getPosition().y) );
		effectSoundOn[i]->setScale( 0.3f );

		effectSoundOff[i] = CCSprite::create( effectSoundImageName[1][i].c_str() );
		effectSoundOff[i]->setPosition( ccp(effectSoundOn[i]->getPosition().x + 250, effect->getPosition().y) );
		effectSoundOff[i]->setScale( 0.3f );
		
		if( effectSoundState ) {
			panel->addChild( effectSoundOn[i], i );
			panel->addChild( effectSoundOff[i], !i );
		}
		else {
			panel->addChild( effectSoundOn[i], !i );
			panel->addChild( effectSoundOff[i], i );
		}
	}
}
void OptionScene::returnMenu( CCObject *pSender )
{
	// Main Scene 으로 돌아간다.
	CCScene *pScene = MainScene::scene(); 
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
}

// 터치
void OptionScene::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this, 0, true );
	CCLayer::onEnter();
}
void OptionScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate( this );
	CCLayer::onExit();
}
// 추후 수정
bool OptionScene::ccTouchBegan( CCTouch *pTouch, CCEvent *event )
{
	CCPoint touchPoint = pTouch->getLocation();
	
	// 눌린 이미지의 zOrder를 저장한다.
	zOrder[0] = backgroundSoundOn[1]->getZOrder();
	zOrder[1] = backgroundSoundOff[1]->getZOrder();
	zOrder[2] = effectSoundOn[1]->getZOrder();
	zOrder[3] = effectSoundOff[1]->getZOrder();

	CCLog("background sound button location : <%f, %f>", backgroundSoundOn[0]->getPosition().x, backgroundSoundOn[0]->getPosition().y);
	CCLog("user touch point : <%f, %f>", touchPoint.x, touchPoint.y);
	for( int index = 0; index < 4; index++ ) {
		CCSprite* pSprite;
		switch( index ) {
			case 0 : pSprite = backgroundSoundOn[0];	break;
			case 1 : pSprite = backgroundSoundOff[0];	break;
			case 2 : pSprite = effectSoundOn[0];		break;
			case 3 : pSprite = effectSoundOff[0];		break;
			default : break;
		}

		if( pSprite->boundingBox().containsPoint( touchPoint ) ) {
			if( !zOrder[index]) {
				for( int i = 0; i < 2; i++ ) {
					switch( index ) {
						case 0 : panel->reorderChild( backgroundSoundOn[i],  i ); panel->reorderChild( backgroundSoundOff[i], !i ); backgroundSoundState =  true; break;
						case 1 : panel->reorderChild( backgroundSoundOn[i], !i ); panel->reorderChild( backgroundSoundOff[i],  i ); backgroundSoundState = false; break;
						case 2 : panel->reorderChild( effectSoundOn[i],  i ); panel->reorderChild( effectSoundOff[i], !i );			effectSoundState =  true; break;
						case 3 : panel->reorderChild( effectSoundOn[i], !i ); panel->reorderChild( effectSoundOff[i],  i );			effectSoundState = false; break;
						default : break;
					}
				}
			}
			break;
		}
	}

	return true;
}
void OptionScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *event )
{
}