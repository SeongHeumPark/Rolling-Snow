#include "BonusScene.h"
#include "GameScene.h"
#include "Character.h"

using namespace cocos2d;
using namespace pugi;

float BonusScene::itemPosition_x[_MAXNUM];
float BonusScene::itemPosition_y[_MAXNUM];
int BonusScene::item_code[_MAXNUM];

// 생성자, 소멸자
BonusScene::BonusScene()
{
	srand( time(NULL) );
	backgroundSpeed = 0.5f;
	ItemSpeed = 0.25f;
	waitTime = 0;
	jumpCount = 2;
}
BonusScene::~BonusScene()
{
	this->unschedule( schedule_selector(BonusScene::itemCollision) );
}

// CCLayerColor Override
CCScene* BonusScene::scene()
{
	BonusScene* layer = BonusScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool BonusScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;

	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/BonusItems_01.xml.txt"); //보너스 아이템 XML불러옴
	xml_parse_result result = xmlDoc.load_file(fileName.c_str());

	if(!result) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
		return false;
	}

	itemsNode = xmlDoc.child("Items");
	itemNode = itemsNode.child("item");

	this->initializeFileNameNumber();
	this->createBackground();
	this->addCharacter();
	this->createItem();

	this->schedule( schedule_selector( BonusScene::itemCollision) );
	this->schedule( schedule_selector( BonusScene::startBounsScene ) );
	
	return true;
}

void BonusScene::initializeFileNameNumber()
{
	// 점수 아이템 파일명 초기화
	this->scoreItemName[0][0] = "Images/item/score/score1-1.png";
	this->scoreItemName[0][1] = "Images/item/score/score1-2.png";
	this->scoreItemName[0][2] = "Images/item/score/score1-3.png";
	this->scoreItemName[0][3] = "Images/item/score/score1-4.png";
	this->scoreItemName[1][0] = "Images/item/score/score2-1.png";
	this->scoreItemName[1][1] = "Images/item/score/score2-2.png";
	this->scoreItemName[1][2] = "Images/item/score/score2-3.png";
	this->scoreItemName[1][3] = "Images/item/score/score2-4.png";
	this->scoreItemName[2][0] = "Images/item/score/score3-1.png";
	this->scoreItemName[2][1] = "Images/item/score/score3-2.png";
	this->scoreItemName[2][2] = "Images/item/score/score3-3.png";
	this->scoreItemName[2][3] = "Images/item/score/score3-4.png";
}
void BonusScene::createBackground()
{
	for( int i = 0; i < 5; i++ ) {
		bonusField[i] = CCSprite::create( "Images/field/bonusField.png" );
		bonusField[i]->setAnchorPoint( ccp(0, 0) );
		bonusField[i]->setPosition( ccp(bonusField[0]->getContentSize().width * i, 0) );
		if( i != 0 ) bonusField[0]->addChild( bonusField[i] );
	}
	bonusField[0]->runAction( this->setSpeed( this->backgroundSpeed ) );
	this->addChild( bonusField[0] );
}
void BonusScene::addCharacter()
{
	// 캐릭터 추가
	this->addChild( Character::GetSingleTone(), 4 );
	Character::GetSingleTone()->setPosition( ccp( Character::GetSingleTone()->getInitPositionX(), Character::GetSingleTone()->getInitPositionY() ) );

	// 달리기
	Character::GetSingleTone()->doRun();
}
void BonusScene::createItem()
{
	itemCount = 0;
	for(itemNode = itemsNode.child("Item"); itemNode; itemNode = itemNode.next_sibling("Item"), itemCount++ ) { 
		this->itemPosition_x[itemCount] = itemNode.child("X").text().as_float();
		this->itemPosition_y[itemCount] = itemNode.child("Y").text().as_float();
	    this->item_code[itemCount] = itemNode.child("CODE").text().as_int();
	}

	// 아이템 생성
	for( int i = 0; i < itemCount; i++ ) {
		// 아이템 습득 여부 체크
		checkEatItem[i] = false;

		switch( item_code[i] ) {
			case 0 : item[i] = CCSprite::create( this->scoreItemName[0][(int)(rand() % 4)].c_str() ); break; // 점수
			case 1 : item[i] = CCSprite::create( this->scoreItemName[1][(int)(rand() % 4)].c_str() ); break; // 점수
			case 2 : item[i] = CCSprite::create( this->scoreItemName[2][(int)(rand() % 4)].c_str() ); break; // 점수
			case 3 : item[i] = CCSprite::create( "Images/item/coin.png" );							  break; // 코인
			default : break;
		}

		item[i]->setPosition( ccp( itemPosition_x[i], itemPosition_y[i] ) );
		item[i]->setTag( item_code[i] ); // tag로 아이템이 무엇인지를 구분하기 위한 setTag
		item[i]->setScale( 0.15f );       // 점수와 코인 아이템의 스케일 지정

		this->addChild( item[i], 3 );

		// 아이템 액션 추가
		item[i]->runAction( this->setSpeed( ItemSpeed ) );
	}
}
void BonusScene::stopActionAllObjectAndRedefine( int operationCode, float background, float item )
{
	float bgSpeed = 0.0f, oiSpeed = 0.0f;

	// 연산
	switch( operationCode ) {
		case 0 : bgSpeed = this->backgroundSpeed + background; oiSpeed = this->ItemSpeed + item; break; // 덧셈
		case 1 : bgSpeed = this->backgroundSpeed - background; oiSpeed = this->ItemSpeed - item; break; // 뺄셈
		case 2 : bgSpeed = this->backgroundSpeed * background; oiSpeed = this->ItemSpeed * item; break; // 곱셈
		case 3 : bgSpeed = this->backgroundSpeed / background; oiSpeed = this->ItemSpeed / item; break; // 나눗셈
		case 4 : bgSpeed = background; oiSpeed = item; break;											// 대입
		default : break;
	}

	// 스피드 출력
	CCLog( "speed < %.2f, %.2f >", bgSpeed, oiSpeed );

	// 배경 중지 및 재지정
	bonusField[0]->stopAllActions();
	bonusField[0]->runAction( this->setSpeed( bgSpeed ) );

	// 아이템 중지 및 재지정
	for( int i = 0; i < itemCount; i++ ) {
		if( !checkEatItem[i] ) {
			this->item[i]->stopAllActions();
			this->item[i]->runAction( this->setSpeed( oiSpeed ) );
		}
	}
}
CCAction* BonusScene::setSpeed( float speed )
{
	CCActionInterval* go = CCMoveBy::create( speed, ccp(-200, 0) );
	CCAction* act = CCRepeatForever::create( go );
	return act;
}

void BonusScene::startBounsScene( float f )
{
	++waitTime;
	if( waitTime >= 300 ) {
		this->unschedule( schedule_selector( BonusScene::startBounsScene ) );
		// 캐릭터 제거
		this->removeChild( Character::GetSingleTone(), true );

		// Game Scene 이동
		CCScene* pScene = GameScene::scene();
		CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.8f, pScene) );
	}
}

// 아이템 충돌
void BonusScene::itemCollision( float f )
{
	for( int i = 0; i < itemCount; i++ ) {
		if( !checkEatItem[i] ) {
			int x = item[i]->getPosition().x - Character::GetSingleTone()->getPosition().x;
			float top = Character::GetSingleTone()->getPosition().y + (Character::GetSingleTone()->getContentSize().height * Character::GetSingleTone()->getScaleY());
			float bottom = Character::GetSingleTone()->getPosition().y;
			if( x > 0 && x < (90 + (Character::GetSingleTone()->getScaleX() * 10) )  ) { 
				if( item[i]->getPosition().y > bottom && item[i]->getPosition().y < top - 25 ) {
					// 아이템 태그에 따른 행동을하고 아이템을 제거 한다.
					checkEatItem[i] = true;
					this->selectedItemByTag( item[i]->getTag() );
					this->removeChild( item[i], true );
					break;
				}
			}
			else if( item[i]->getPosition().x < -10 ) checkEatItem[i] = true;
		}
	}
}

// 각종 아이템 획득 시 효과
void BonusScene::selectedItemByTag( int itemCode )
{
	switch( itemCode ) {
		case 0 : 
		case 1 :
		case 2 :
				 this->eatScoreItem(); break; // 점수 추가
		case 3 : this->eatCoinItem();  break; // 코인
		default : break;
	}
}
void BonusScene::eatScoreItem()
{
	CCLog( "add score" );
	switch( Character::GetSingleTone()->getScoreLevel() ) {
		case 1 : Character::GetSingleTone()->score += 100; break;
		case 2 : Character::GetSingleTone()->score += 200; break;
		case 3 : Character::GetSingleTone()->score += 300; break;
		default : break;
	}
}
void BonusScene::eatCoinItem()
{
	CCLog( "add coin" );
	Character::GetSingleTone()->coin += 100;
}

// 터치
void BonusScene::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate( this, 0, true );
	CCLayer::onEnter();
}
void BonusScene::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate( this );
	CCLayer::onExit();
}
bool BonusScene::ccTouchBegan( CCTouch *pTouch, CCEvent *event )
{
	if( jumpCount == 2 ) {
		--jumpCount;
		Character::GetSingleTone()->doJump();

		x = bonusField[0]->getPosition().x;

		// 캐릭터 포지션 체크
		this->schedule( schedule_selector( BonusScene::ccTouchPositionCheck ) );
	}
	else if( jumpCount == 1 ) {
		--jumpCount;
		Character::GetSingleTone()->doDoubleJump();
	}

	return true;
}
void BonusScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *event )
{
}
void BonusScene::ccTouchPositionCheck( float f )
{
	if( Character::GetSingleTone()->getPosition().y == Character::GetSingleTone()->getInitPositionY() ) {
		// 점프 카운트 초기화
		jumpCount = 2;

		CCLog( "jump range < %f >", bonusField[0]->getPosition().x - x );

		// 캐릭터 포지션 체크 중지
		this->unschedule( schedule_selector( BonusScene::ccTouchPositionCheck ) );
	}
}