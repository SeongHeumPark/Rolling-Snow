#include "GameScene.h"
#include "BonusScene.h"
#include "Character.h"
#include "MainScene.h"
#include "LogoScene.h" 
#include "OptionScene.h"
#include "CalculationScene.h"
#include "pugixml.hpp"

using namespace cocos2d;
using namespace pugi;

// static 변수 재선언
float GameScene::objPositionX[_MAXOBJ];
float GameScene::objPositionY[_MAXOBJ];
float GameScene::itemPositionX[_MAXITEM];
float GameScene::itemPositionY[_MAXITEM];
int GameScene::objCode[_MAXOBJ];
int GameScene::itemCode[_MAXITEM];

// 생성자, 소멸자
GameScene::GameScene()
{
	// 각종 변수 초기화
	srand( time(NULL) );
	backgroundSpeed = 5.0f;
	objectItemSpeed = 0.5f;
	meterSpeed = objectItemSpeed * 10;
	speedLevel = 1;
	unbeatableState = false;
	jumpCount = 2;

	// 게임 scene에서 정상적으로 item level이 적용됐나 확인
	CCLog( "Score		level		%d		LEVELING", Character::GetSingleTone()->getScoreLevel() );
	CCLog( "SizeUpp		level		%.0f%%		INCREASE", Character::GetSingleTone()->getSizeUpLevel() * 100 );
	CCLog( "PlayTime	level		%.1fsec	ADDITION", Character::GetSingleTone()->getPlayTimeLevel() / 60 );
	CCLog( "Boost		Time		%.1fsec	MAINTAIN", Character::GetSingleTone()->getBoostTime() / 60 );
	CCLog( "Magnetic	Time		%.1fsec	MAINTAIN", Character::GetSingleTone()->getMagneticTime() / 60 );
	CCLog( "Unbeatable	Time		%.1fsec	MAINTAIN", Character::GetSingleTone()->getUnbeatableTime() / 60 );
	CCLog( "Bonus		Time		%.1fsec	MAINTAIN", Character::GetSingleTone()->getBonusTime() / 60 );
}
GameScene::~GameScene()
{	
}

// CCLayerColor Override
CCScene* GameScene::scene()
{
	GameScene* layer = GameScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;

	// 배경, 캐릭터, 장애물, 아이템 오브젝트 생성
	this->initializeFileNameNumber();
	this->createBackgroundPallaxNode();
	this->addCharacter();
	this->createRoadAndParsingXML();

	// 각종 상태 생성
	this->createTimeBarState();
	this->createScaleState();
	this->createScoreState();
	this->createCoinState();
	this->createMeterState();

	// 길, 아이템, 장애물 충돌 체크
	this->initialinzeRoadInfo();
	this->schedule( schedule_selector( GameScene::roadCollision ) );
	this->schedule( schedule_selector( GameScene::itemCollision ) );
	this->schedule( schedule_selector( GameScene::objectCollision ) );

	// 스피드 증가 (추가 예정)
	// this->schedule( schedule_selector( GameScene::speedUpByBackgroundPosition ) );

	// 상태 비율 변수 초기화
	timeBarScalePerSecond = currentTimeBar->getScaleX() / _GAMEPLAYTIME;
	characterScalePerMeter = currentScale->getScale() / 1000;

	return true;
}

// 배경, 길, 캐릭터, 장애물, 아이템 생성
void GameScene::initializeFileNameNumber()
{
	// 길 이미지 파일명 초기화
	this->roadFileName[0] = "Images/road/road_01.png";
	this->roadFileName[1] = "Images/road/road_03.png";
	this->roadFileName[2] = "Images/road/road_05.png";
	this->roadFileName[3] = "Images/road/road_02.png";
	this->roadFileName[4] = "Images/road/road_03.png";
	this->roadFileName[5] = "Images/road/road_04.png";
	this->roadFileName[6] = "Images/road/road_05.png";
	this->roadFileName[7] = "Images/road/road_02.png";
	this->roadFileName[8] = "Images/road/road_03.png";
	this->roadFileName[9] = "Images/road/road_06.png";

	// 장애물 아이템 xml 파일명 초기화
	this->objectItemFileName[0] = "data/ItemsAndObjects_01.xml.txt";
	this->objectItemFileName[1] = "data/ItemsAndObjects_02.xml.txt";
	
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
void GameScene::createBackgroundPallaxNode()
{	
	// 배경화면 추가
	for( int i = 0; i < 10; i++ ) {
		// base field 추가
		baseField[i] = CCSprite::create( "Images/field/baseField.png"); 
		baseField[i]->setAnchorPoint( ccp(0, 0) );
		baseField[i]->setPosition( ccp(baseField[i]->getContentSize().width * i, 0) );

		// sun field 추가
		sunField[i] = CCSprite::create( "Images/field/sunField.png" );
		sunField[i]->setAnchorPoint( ccp(0, 0) );
		sunField[i]->setPosition( ccp(sunField[i]->getContentSize().width * i, 0 ) );

		// snow field 추가
		snowField[i] = CCSprite::create( "Images/field/snowField.png" );
		snowField[i]->setAnchorPoint( ccp(0, 0) );
		snowField[i]->setPosition( ccp(snowField[i]->getContentSize().width * i, 0 ) );

		if( i != 0 ) {
			baseField[0]->addChild( baseField[i] );
			sunField[0]->addChild( sunField[i] );
			snowField[0]->addChild( snowField[i] );
		}
	}

	// 패럴렉스 노드 생성
	for( int i = 0; i < 3; i++ ) {
		backgroundNode[i] = CCParallaxNode::create();

		// 패럴렉스 노드에 삽입
		switch ( i ) {
			case 0 : backgroundNode[0]->addChild( baseField[0], 0, ccp(1.5f, 1.5f), ccp(0, 0) );	break;
			case 1 : backgroundNode[1]->addChild( sunField[0], 0, ccp(1.5f, 1.5f), ccp(0, 0) );		break;
			case 2 : backgroundNode[2]->addChild( snowField[0], 0, ccp(1.5f, 1.5f), ccp(0, 0) );	break;
			default : break;
		}

		// 패럴렉스 노드 액션 시작
		backgroundNode[i]->runAction( this->setSpeed( backgroundSpeed ) );
		this->addChild( backgroundNode[i], 2 - i );
	}
}
void GameScene::addCharacter()
{
	// 캐릭터 추가
	this->addChild( Character::GetSingleTone(), 5 );
	if( Character::GetSingleTone()->bonusState == false ) {
		Character::GetSingleTone()->time = _GAMEPLAYTIME;
		Character::GetSingleTone()->meter = 0;
		Character::GetSingleTone()->score = 0;
		Character::GetSingleTone()->coin = 0;
		Character::GetSingleTone()->timeBarScale = 0.58f;
		Character::GetSingleTone()->setScale( 0.2f );
	}
	Character::GetSingleTone()->bonusState = false;
	Character::GetSingleTone()->setPosition( ccp(Character::GetSingleTone()->getInitPositionX(), Character::GetSingleTone()->getInitPositionY() ) );

	// 달리기
	Character::GetSingleTone()->doRun();
}
void GameScene::createRoadAndParsingXML()
{
	for( int i = 0; i < 10; i++ ) {
		road[i] = CCSprite::create( roadFileName[i].c_str() );
		road[i]->setAnchorPoint( CCPointZero );
		road[i]->setPosition( ccp( road[0]->getContentSize().width * i, 0 ) );
		if( i != 0 ) road[0]->addChild( road[i], 4 );
	}

	road[0]->runAction( this->setSpeed( this->objectItemSpeed ) );
	this->addChild( road[0], 4 );

	// XML Parsing
	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename( this->objectItemFileName[(int)(rand() % 2)].c_str() );
	xml_parse_result result = xmlDoc.load_file( fileName.c_str() );

	if(!result) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
	}

	ItemsAndObjects = xmlDoc.child("ItemsAndObjects");
	this->readObjectAndItemInfo();
	this->createObject();
	this->createItem();
}
void GameScene::readObjectAndItemInfo()
{
	// XML에서 아이템 위치 정보를 읽어들임
	Items = ItemsAndObjects.child("Items");
	itemCount = 0;
	for( Item = Items.child("Item"); Item; Item = Item.next_sibling("Item"), itemCount++ ) {
		this->itemCode[itemCount] = Item.child("CODE").text().as_int();
		this->itemPositionX[itemCount] = Item.child("X").text().as_int();
		this->itemPositionY[itemCount] = Item.child("Y").text().as_int();
	}

	// XML에서 장애물 위치 정보를 읽어들임
	Objects = ItemsAndObjects.child("Objects");
	objCount = 0;
	for( Object = Objects.child("Object"); Object; Object = Object.next_sibling("Object"), objCount++ ) {
		this->objCode[objCount] = Object.child("CODE").text().as_int();
		this->objPositionX[objCount] = Object.child("X").text().as_int();
		this->objPositionY[objCount] = Object.child("Y").text().as_int();
	}
}
void GameScene::createObject()
{
	// 장애물 생성
	for( int i = 0; i < objCount; i++ ) {
		checkColObj[i] = false;

		switch( objCode[i] ) {
			case 0 : object[i] = CCSprite::create( "Images/object/wood.png" );	break; // 1단 나무
			case 1 : object[i] = CCSprite::create( "Images/object/fence.png" ); break; // 2단 철조망
			case 2 : object[i] = CCSprite::create( "Images/object/straw.png" ); break; // 2단 짚단
			case 3 : object[i] = CCSprite::create( "Images/object/stone.png" ); break; // 2단 바위
			case 4 : this->createGearObject( i );								break; // 톱니바퀴
			case 5 : this->createFireObject( i );								break; // 불똥
			default : break;
		}

		object[i]->setTag( objCode[i] );
		object[i]->setAnchorPoint( ccp(0, 0) );
		object[i]->setPosition( ccp( objPositionX[i], objPositionY[i] ) );
		object[i]->setScaleY( 0.9f );
		if(	objCode[i] == 4 ) this->addChild( object[i], 3 );
		else this->addChild( object[i], 4 );

		// 장애물 액션 추가
		object[i]->runAction( this->setSpeed( objectItemSpeed ) );
	}
	
	// 태양 추가
	this->createSunAnimation();
}
void GameScene::createSunAnimation()
{
	sun = CCSprite::create();
	CCTexture2D* text = CCTextureCache::sharedTextureCache()->addImage( "Images/sun.png" );
	CCAnimation* animation = CCAnimation::create();
	animation->setDelayPerUnit( 0.2f );

	for( int i = 0 ; i < 12; i++ ) {
		int index = i % 4;
		int rowIndex = i / 4;
		animation->addSpriteFrameWithTexture( text, CCRectMake( index * 500, rowIndex * 500, 500, 500 ) );
	}

	sun->setAnchorPoint( ccp(0, 0) );
	sun->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width - 200, CCDirector::sharedDirector()->getWinSize().height - 300 ) );
	sun->setScale( 0.5f );
	this->addChild( sun, this->backgroundNode[1]->getZOrder() );
	
	CCAnimate* animate = CCAnimate::create( animation );
	CCAction* act = CCRepeatForever::create( animate );
	sun->runAction( act );
}
void GameScene::createGearObject( int index )
{
	object[index] = CCSprite::create();
	object[index]->runAction( this->addActionToGear() );
}
CCAction* GameScene::addActionToGear()
{
	CCTexture2D* text = CCTextureCache::sharedTextureCache()->addImage( "Images/object/gear.png" );
	CCAnimation* runAnimation = CCAnimation::create();
	runAnimation->setDelayPerUnit( 0.1f );

	for( int i = 0; i < 4; i++ ) {
		int index = i;
		int rowIndex = 0;
		runAnimation->addSpriteFrameWithTexture( text, CCRectMake( index * 200, rowIndex * 200, 200, 200 ) );
	}

	CCAnimate* runAnimate = CCAnimate::create( runAnimation );
	CCAction* actRep = CCRepeatForever::create( runAnimate );
	return actRep;
}
void GameScene::createFireObject( int index )
{
	object[index] = CCSprite::create();
}
CCAction* GameScene::addActionToFire()
{
	CCTexture2D* text = CCTextureCache::sharedTextureCache()->addImage( "Images/object/fire.png" );
	CCAnimation* runAnimation = CCAnimation::create();
	runAnimation->setDelayPerUnit( 0.1f );

	for( int i = 0; i < 2; i++ ) {
		for( int j = 0; j < 2; j++ ) {
			int index = j;
			int rowIndex = i;
			runAnimation->addSpriteFrameWithTexture( text, CCRectMake( index * 500, rowIndex * 500, 500, 500 ) );
		}
	}

	CCAnimate* runAnimate = CCAnimate::create( runAnimation );
	CCAction* actRep = CCRepeatForever::create( runAnimate );
	return actRep;
}
void GameScene::createItem()
{
	// 아이템 생성
	for( int i = 0; i < itemCount; i++ ) {
		checkEatItem[i] = false; // 아이템 습득 여부 체크 초기화
		switch( itemCode[i] ) {
			case  0 : item[i] = CCSprite::create( this->scoreItemName[0][(int)(rand() % 4)].c_str() ); break; // 점수
			case  1 : item[i] = CCSprite::create( this->scoreItemName[1][(int)(rand() % 4)].c_str() ); break; // 점수
			case  2 : item[i] = CCSprite::create( this->scoreItemName[2][(int)(rand() % 4)].c_str() ); break; // 점수
			case  3 : item[i] = CCSprite::create( "Images/item/coin.png" );																		    break; // 코인
			case  4 : item[i] = CCSprite::create( "Images/item/sizeUp.png" );																	    break; // 크기 증가
			case  5 : item[i] = CCSprite::create( "Images/item/playTime.png" );																	    break; // 시간 증가
			case  6 : item[i] = CCSprite::create( "Images/item/boost.png" );																		    break; // 부스터
			case  7 : item[i] = CCSprite::create( "Images/item/magnetic.png" );																	    break; // 마그네틱
			case  8 : item[i] = CCSprite::create( "Images/item/unbeatable.png" );																    break; // 무적
			case  9 : item[i] = CCSprite::create( "Images/item/randomItem.png" );																    break; // 랜덤
			case 10 : item[i] = CCSprite::create( "Images/item/bonus.png" );																		    break; // 보너스
			case 11 : item[i] = CCSprite::create( "Images/item/playTime.png" );											   						    break; // 라지 타임
			default : break;
		}

		item[i]->setPosition( ccp( itemPositionX[i], itemPositionY[i] ) );
		item[i]->setTag( itemCode[i] ); // tag로 아이템이 무엇인지를 구분하기 위한 setTag

		if( itemCode[i] <= 4 ) item[i]->setScale( 0.15f );			 // 점수와 코인 아이템의 스케일 지정
		else if( itemCode[i] == 11 ) item[i]->setScale( 0.2f );		 // 라지 타임 증가 아이템 스케일
		else item[i]->setScale( 0.15f );						     // 나머지 아이템 스케일 지정

		this->addChild( item[i], 3 );

		// 아이템 액션 추가
		item[i]->runAction( this->setSpeed( objectItemSpeed ) );
	}
}
void GameScene::stopActionAllObjectAndRedefine( int operationCode, float background, float objectItem )
{
	float bgSpeed = 0.0f, oiSpeed = 0.0f;

	// 속도에 맞는 미터기 조절을 위한 변수 조정
	meterSpeed = objectItem * 10;

	// 연산
	switch( operationCode ) {
	case 0 : bgSpeed = this->backgroundSpeed + background; oiSpeed = this->objectItemSpeed + objectItem; break; // 덧셈
	case 1 : bgSpeed = this->backgroundSpeed - background; oiSpeed = this->objectItemSpeed - objectItem; break; // 뺄셈
	case 2 : bgSpeed = this->backgroundSpeed * background; oiSpeed = this->objectItemSpeed * objectItem; break; // 곱셈
	case 3 : bgSpeed = this->backgroundSpeed / background; oiSpeed = this->objectItemSpeed / objectItem; break; // 나눗셈
	case 4 : bgSpeed = background; oiSpeed = objectItem; break;												    // 대입
	default : break;
	}

	// 배경 중지 및 재지정
	for( int i = 0; i < 3; i++ ) {
		backgroundNode[i]->stopAllActions();
		backgroundNode[i]->runAction( this->setSpeed( bgSpeed ) );
	}

	// 길 중지 및 재지정
	road[0]->stopAllActions();
	road[0]->runAction( this->setSpeed( oiSpeed ) );

	// 장애물 중지 및 재지정
	for( int i = 0; i < objCount; i++ ) {
		object[i]->stopAllActions();
		if( object[i]->getTag() == 4 ) object[i]->runAction( this->addActionToGear() );
		object[i]->runAction( this->setSpeed( oiSpeed ) );
	}

	// 아이템 중지 및 재지정
	for( int i = 0; i < itemCount; i++ ) {
		if( !checkEatItem[i] ) {
			item[i]->stopAllActions();
			item[i]->runAction( this->setSpeed( oiSpeed ) );
		}
	}
}
CCAction* GameScene::setSpeed( float speed )
{
	CCActionInterval* go = CCMoveBy::create( speed, ccp(-200, 0) );
	CCAction* act = CCRepeatForever::create( go );
	return act;
}

// 각종 상태 생성
void GameScene::createTimeBarState()
{
	// 타임 바 background 생성
	currentTimeBarBackground = CCSprite::create( "Images/state/timeBarImageBackground.png" );
	currentTimeBarBackground->setAnchorPoint( ccp(0, 0) );
	currentTimeBarBackground->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 + 200, CCDirector::sharedDirector()->getWinSize().height / 2 + 250 ) );
	currentTimeBarBackground->setScaleX( 0.6f );
	currentTimeBarBackground->setScaleY( 1.2f );	
	this->addChild( currentTimeBarBackground, 6 );

	// 타임 바 name 생성
	currentTimeBarName = CCLabelTTF::create( "TIME", "Helvetica", 32 );
	currentTimeBarName->setColor( ccBLACK );
	currentTimeBarName->setAnchorPoint( ccp(0, 0) );
	currentTimeBarName->setPosition( ccp(5, 25) );
	currentTimeBarName->setScaleY( 0.6f );
	currentTimeBarBackground->addChild( currentTimeBarName );

	// 타임 바 image 생성
	currentTimeBar = CCSprite::create( "Images/state/timeBarImage.png" );
	currentTimeBar->setAnchorPoint( ccp(0, 0) );
	currentTimeBar->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 + 205, CCDirector::sharedDirector()->getWinSize().height / 2 + 250 ) );
	currentTimeBar->setScaleX( Character::GetSingleTone()->timeBarScale );
	currentTimeBar->setScaleY( 1.17f );	
	this->addChild( currentTimeBar, 6 );

	// 스케쥴 실행
	this->schedule( schedule_selector( GameScene::timeBarSchedule ) );
}
void GameScene::timeBarSchedule( float f )
{
	(Character::GetSingleTone()->time)--;
	Character::GetSingleTone()->timeBarScale = currentTimeBar->getScaleX() - timeBarScalePerSecond;
	if( Character::GetSingleTone()->time <= 0 ) {	
		this->unschedule( schedule_selector( GameScene::timeBarSchedule ) );
		currentTimeBar->setScaleX( 0.0f );
		this->appearGameOver();
	}
	else currentTimeBar->setScaleX( Character::GetSingleTone()->timeBarScale );
}
void GameScene::createScaleState()
{
	// Max scale
	maxScaleBackground = CCSprite::create( "Images/state/characterScaleMax.png" );
	maxScaleBackground->setAnchorPoint( ccp(0, 0) );
	maxScaleBackground->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 - 490, CCDirector::sharedDirector()->getWinSize().height / 2 + 170 ) );
	maxScaleBackground->setScale( 0.3f );
	this->addChild( maxScaleBackground, 6 );

	// current scale
	currentScale = CCSprite::create( "Images/state/currentCharacterScale.png" );
	currentScale->setPosition( ccp( maxScaleBackground->getContentSize().width / 2, maxScaleBackground->getContentSize().height / 2 ) );
	// 초기 스케일 크기는 character class에서 정의되면 적용한다.(추후 수정)
	currentScale->setScale( Character::GetSingleTone()->getScale() + 0.3f );
	maxScaleBackground->addChild( currentScale );

	// 스케쥴 실행
	this->schedule( schedule_selector( GameScene::scaleSchedule ) );
}
void GameScene::scaleSchedule( float f )
{
	if( Character::GetSingleTone()->getScale() > 0.1f ) {
		int zorder = backgroundNode[0]->getZOrder();
		float scale = 0.0f;
		switch( zorder ) {
			case 0 : if( Character::GetSingleTone()->getScale() < 0.8f ) scale = this->characterScalePerMeter * 1.5f;  break; // snow field
			case 1 : scale = this->characterScalePerMeter * -0.5f; break; // sun field
			case 2 : if( Character::GetSingleTone()->getScale() < 0.8f ) scale = this->characterScalePerMeter;		   break; // base field
			default : break;
		}
		currentScale->setScale( currentScale->getScale() + scale );
		Character::GetSingleTone()->setScale( Character::GetSingleTone()->getScale() + scale );
	}
	else if( Character::GetSingleTone()->getScale() <= 0.1f ) {
		this->unschedule( schedule_selector( GameScene::scaleSchedule ) );
		this->appearGameOver();
	}
}
void GameScene::createScoreState()

{
	scoreLabel = CCLabelTTF::create( "0", "Helvetica", 30 );
	scoreLabel->setColor( ccWHITE );
	scoreLabel->setAnchorPoint( ccp(1, 0) );
	scoreLabel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 + 180, CCDirector::sharedDirector()->getWinSize().height / 2 + 250 ) );
	this->addChild( scoreLabel, 6 );

	// 스케쥴 실행
	this->schedule( schedule_selector( GameScene::scoreSchedule ) );
}
void GameScene::scoreSchedule( float f )
{
	char totalScore[100];
	sprintf( totalScore, "%d", Character::GetSingleTone()->score );
	scoreLabel->setString( totalScore );
}
void GameScene::createCoinState()
{
	coinLabel = CCLabelTTF::create( "0", "Helvetica", 30 );
	coinLabel->setColor( ccYELLOW );
	coinLabel->setAnchorPoint( ccp(1, 0) );
	coinLabel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 + 50, CCDirector::sharedDirector()->getWinSize().height / 2 + 250 ) );
	this->addChild( coinLabel, 6 );

	// 스케쥴 실행
	this->schedule( schedule_selector( GameScene::coinSchedule ) );
}
void GameScene::coinSchedule( float f )
{
	char totalCoin[100];
	sprintf( totalCoin, "%d", Character::GetSingleTone()->coin );
	coinLabel->setString( totalCoin );
}
void GameScene::createMeterState()
{
	meterLabel = CCLabelTTF::create( "0m", "Helvetica", 30 );
	meterLabel->setColor( ccBLUE );
	meterLabel->setAnchorPoint( ccp(1, 0) );
	meterLabel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2 - 100, CCDirector::sharedDirector()->getWinSize().height / 2 + 250 ) );
	this->addChild( meterLabel, 6 );

	// 스케쥴 실행
	meterStateTime = 0;
	this->schedule( schedule_selector( GameScene::meterSchedule ) );
}
void GameScene::meterSchedule( float f )
{
	char totalMeter[100];
	++meterStateTime;
	if( meterStateTime % meterSpeed == 0 ) {
		sprintf( totalMeter, "%dm", (Character::GetSingleTone()->meter) += 10 );
		meterLabel->setString( totalMeter );
	}
}
void GameScene::stopAllSchedule()
{
	this->unschedule( schedule_selector( GameScene::roadCollision ) );
	this->unschedule( schedule_selector( GameScene::objectCollision ) );
	this->unschedule( schedule_selector( GameScene::itemCollision ) );
	this->unschedule( schedule_selector( GameScene::scoreSchedule ) );
	this->unschedule( schedule_selector( GameScene::coinSchedule ) );
	this->unschedule( schedule_selector( GameScene::timeBarSchedule ) );
	this->unschedule( schedule_selector( GameScene::meterSchedule ) );
	this->unschedule( schedule_selector( GameScene::scaleSchedule ) );
}

// 길, 장애물, 아이템 충돌 체크
void GameScene::initialinzeRoadInfo()
{
	for( int i = 0; i < 10; i++ ) {
		switch( i ) {
			case 0 : // road_01
				for( int j = 0; j < 8; j++ ) {
					this->number[i][j] = 0;
					this->roadInfo[i][j] = 0;
				}
				break;
			case 1 : // road_03
			case 4 :
			case 8 :
				this->number[i][0] = 1;
				this->roadInfo[i][0] = 1200;
				this->number[i][1] = 1;
				this->roadInfo[i][1] = 1450;
				this->number[i][2] = 1;
				this->roadInfo[i][2] = 1700;
				this->number[i][3] = 2;
				this->roadInfo[i][3] = 2500;
				this->number[i][4] = 2;
				this->roadInfo[i][4] = 2900;
				this->number[i][5] = 2;
				this->roadInfo[i][5] = 5450;
				this->number[i][6] = 1;
				this->roadInfo[i][6] = 5850;
				this->number[i][7] = 2;
				this->roadInfo[i][7] = 6100;
				break;
			case 2 : // road_05
			case 6 :
				this->number[i][0] = 1;
				this->roadInfo[i][0] = 1200;
				this->number[i][1] = 1;
				this->roadInfo[i][1] = 2500;
				this->number[i][2] = 1;
				this->roadInfo[i][2] = 4350;
				this->number[i][3] = 2;
				this->roadInfo[i][3] = 5650;
				for( int j = 4; j < 8; j++ ) {
					this->number[i][j] = 0;
					this->roadInfo[i][j] = 0;
				}
				break;
			case 3 : // road_02
			case 7 :
				this->number[i][0] = 2;
				this->roadInfo[i][0] = 6150;
				for( int j = 1; j < 8; j++ ) {
					this->number[i][j] = 0;
					this->roadInfo[i][j] = 0;
				}
				break;
			case 5 : // road_04
				this->number[i][0] = 2;
				this->roadInfo[i][0] = 650;
				this->number[i][1] = 2;
				this->roadInfo[i][1] = 1050;
				this->number[i][2] = 2;
				this->roadInfo[i][2] = 3050;
				this->number[i][3] = 1;
				this->roadInfo[i][3] = 3450;
				this->number[i][4] = 1;
				this->roadInfo[i][4] = 5300;
				this->number[i][5] = 1;
				this->roadInfo[i][5] = 5550;
				this->number[i][6] = 1;
				this->roadInfo[i][6] = 6300;
				this->number[i][7] = 0;
				this->roadInfo[i][7] = 0;
				break;
			case 9 : // road_06
				this->number[i][0] = 1;
				this->roadInfo[i][0] = 1200;
				this->number[i][1] = 1;
				this->roadInfo[i][1] = 2500;
				this->number[i][2] = 1;
				this->roadInfo[i][2] = 4350;
				for( int j = 3; j < 8; j++ ) {
					this->number[i][j] = 0;
					this->roadInfo[i][j] = 0;
				}
				break;
			default : break;
		}
	}
}
void GameScene::roadCollision( float f )
{
	int x = -(road[0]->getPosition().x / 6480);
	for( int i = 0; i < 8; i++ ) {
		if( !unbeatableState ) {
			float info = Character::GetSingleTone()->getPosition().x - ( road[0]->getPosition().x + (road[0]->getContentSize().width * x + this->roadInfo[x][i]) );
			if( info > 0 && info < 20 * this->number[x][i] ) {
				if( Character::GetSingleTone()->getPosition().y == Character::GetSingleTone()->getInitPositionY() ) {
					Character::GetSingleTone()->stopAllActions();
					CCActionInterval* go = CCMoveBy::create( 0.5f, ccp(-200, -10) );
					CCAction* act = CCRepeatForever::create( go );
					Character::GetSingleTone()->runAction( act );
					this->schedule( schedule_selector( GameScene::roadColEffect ) );
				}
			}
		}
	}
}
void GameScene::roadColEffect( float f )
{
	if( Character::GetSingleTone()->getPosition().y <= -1000 ) {
		this->unschedule( schedule_selector( GameScene::roadCollision ) );
		this->unschedule( schedule_selector ( GameScene::roadColEffect ) );
		this->appearGameOver();
	}
	else Character::GetSingleTone()->setPositionY( Character::GetSingleTone()->getPosition().y - 10 );
}
void GameScene::objectCollision( float f )
{
	if( !unbeatableState ) {
		for( int i = 0; i < objCount; i++ ) {
			if( !checkColObj[i] ) {
				float left = object[i]->getPosition().x - 20;
				float right = object[i]->getPosition().x + object[i]->getContentSize().width - 50;
				float top = object[i]->getPosition().y + object[i]->getContentSize().height - 20;
				float bottom = object[i]->getPosition().y;
				float y = Character::GetSingleTone()->getPosition().y + (Character::GetSingleTone()->getContentSize().height * Character::GetSingleTone()->getScaleY());
				if( left - Character::GetSingleTone()->getPosition().x < 5 && right - Character::GetSingleTone()->getPosition().x > 10 ) { 
					if( top - Character::GetSingleTone()->getPosition().y > 50  ) {
						checkColObj[i] = true;
						CCLog( "Object Collision" );
						// 장애물에 충돌하면 배경화면과 아이템은 느려지고 서서히 가속한다.
						colEffectTime = 0;
						this->unbeatableState = true;
						this->stopActionAllObjectAndRedefine( 2, 2.0f, 2.0f );
						this->schedule( schedule_selector( GameScene::objectColEffect ), 1.0f );

						// 캐릭터는 반짝이며 통과하고 크기와 시간이 줄어든다.
						Character::GetSingleTone()->doCollision();
						Character::GetSingleTone()->setScale( Character::GetSingleTone()->getScale() / 2.0f );
						this->currentScale->setScale( currentScale->getScale() / 2.0f );
						
						if( currentTimeBar->getScaleX() - (timeBarScalePerSecond * 300) <= 0) {
							Character::GetSingleTone()->time = 0;
							this->currentTimeBar->setScaleX( 0.0f );
						}
						else {
							(Character::GetSingleTone()->time) -= 300;
							this->currentTimeBar->setScaleX( currentTimeBar->getScaleX() - (timeBarScalePerSecond * 300) );
						}
						break;
					}
				}
				else if( left < -10 ) checkColObj[i] = true;
			}
		}
	}
}
void GameScene::objectColEffect( float f )
{
	colEffectTime++;
	if( colEffectTime >= 2 ) {
		this->unschedule( schedule_selector( GameScene::objectColEffect ) );
		this->unbeatableState = false;
		this->stopActionAllObjectAndRedefine( 4, this->backgroundSpeed, this->objectItemSpeed );
	}
	else if( colEffectTime == 1 )
		this->stopActionAllObjectAndRedefine( 4, this->backgroundSpeed * 1.5f, this->objectItemSpeed * 1.5f );
}
void GameScene::itemCollision( float f )
{
	for( int i = 0; i < itemCount; i++ ) {
		if( !checkEatItem[i] ) {
			float x = item[i]->getPosition().x - Character::GetSingleTone()->getPosition().x;
			float top = Character::GetSingleTone()->getPosition().y + (Character::GetSingleTone()->getContentSize().height * Character::GetSingleTone()->getScaleY());
			float bottom = Character::GetSingleTone()->getPosition().y;
			if( x > 0 && x < (90 + (Character::GetSingleTone()->getScaleX() * 10) ) ) { 
				if( item[i]->getPosition().y > bottom && item[i]->getPosition().y < top - 25 ) {
					// 아이템 태그에 따른 행동을하고 아이템을 제거 한다.
					checkEatItem[i] = true;
					this->selectedItemByTag( item[i]->getTag() );
					this->removeChild( item[i], true );
					break;
				}
			}
			else if( item[i]->getPosition().x < 0 ) checkEatItem[i] = true;
		}
	}
}

// 각종 아이템 획득 시 효과
void GameScene::selectedItemByTag( int itemCode )
{
	switch( itemCode ) {
		case  0 : 
		case  1 :
		case  2 :
				  this->eatScoreItem();		  break; // 점수 추가
		case  3 : this->eatCoinItem();		  break; // 코인
		case  4 : this->eatSizeUpItem();	  break; // 크기 증가
		case  5 : this->eatTimeItem();		  break; // 시간 증가
		case  6 : this->eatBoostItem();		  break; // 부스터
		case  7 : this->eatMagneticItem();	  break; // 마그네틱
		case  8 : this->eatUnbeatableItem();  break; // 무적
		case  9 : this->eatRandomItem();	  break; // 랜덤
		case 10 : this->transBounsScene(); break; // 보너스
		case 11 : this->eatLargeTimeItem();	  break; // 시간 큰폭 증가
		default : break;
	}
}
void GameScene::eatScoreItem()
{
	CCLog( "add score" );
	int tempScore;
	switch( Character::GetSingleTone()->getScoreLevel() ) {
		case 1 : tempScore = 100; break;
		case 2 : tempScore = 200; break;
		case 3 : tempScore = 300; break;
		default : break;
	}
	if( Character::GetSingleTone()->getScale() >= 0.8f )
		Character::GetSingleTone()->score += tempScore * 2;
	else
		Character::GetSingleTone()->score += tempScore;
}
void GameScene::eatCoinItem()
{
	CCLog( "add coin" );
	Character::GetSingleTone()->coin += 100;
}
void GameScene::eatSizeUpItem()
{
	// 크기 10% 증가
	CCLog( "size up" );
	Character::GetSingleTone()->setScale( Character::GetSingleTone()->getScale() + Character::GetSingleTone()->getSizeUpLevel() );
	this->currentScale->setScale( currentScale->getScale() + Character::GetSingleTone()->getSizeUpLevel() );
}
void GameScene::eatTimeItem()
{
	float tempScale;

	CCLog( "increase time" );
	tempScale = this->currentTimeBar->getScaleX() + ( this->timeBarScalePerSecond * Character::GetSingleTone()->getPlayTimeLevel() );
	if( tempScale >= 0.58f ) {
		Character::GetSingleTone()->time = _GAMEPLAYTIME;
		this->currentTimeBar->setScaleX( 0.58f );
	}
	else {
		Character::GetSingleTone()->time = Character::GetSingleTone()->time + Character::GetSingleTone()->getPlayTimeLevel();
		this->currentTimeBar->setScaleX( tempScale );
	}
}
void GameScene::eatBoostItem()
{
	CCLog( "boost" );
	// 모든 오브젝트 액션 중지 및 재지정
	this->stopActionAllObjectAndRedefine( 4, 0.3f, 0.1f );
	this->unschedule( schedule_selector( GameScene::objectCollision ) );
	this->unschedule( schedule_selector( GameScene::roadCollision ) );

	// 스케쥴 시작
	boostWaitTime = 0;
	this->unbeatableState = true;
	this->schedule( schedule_selector( GameScene::boostTime ) );
}
void GameScene::boostTime( float f )
{
	boostWaitTime++;
	if( boostWaitTime > Character::GetSingleTone()->getBoostTime() ) {
		this->unschedule( schedule_selector( GameScene::boostTime ) );
		this->unbeatableState = false;
		this->stopActionAllObjectAndRedefine( 4, this->backgroundSpeed, this->objectItemSpeed );
		this->schedule( schedule_selector( GameScene::objectCollision ) );
		this->schedule( schedule_selector( GameScene::roadCollision ) );
	}
}
void GameScene::eatMagneticItem()
{
	CCLog( "magnetic" );
	magneticWaitTime = 0;
	this->schedule( schedule_selector( GameScene::magneticTime ) );
}
void GameScene::magneticTime( float f )
{
	magneticWaitTime++;
	if( magneticWaitTime > Character::GetSingleTone()->getMagneticTime() ) {
		CCLog( "<< End Magnetic >>" );
		this->unschedule( schedule_selector( GameScene::magneticTime ) );
	}
	else {
		for( int i = 0; i < _MAXITEM; i++ ) {
			if( !checkEatItem[i] ) {
				int x = item[i]->getPosition().x - Character::GetSingleTone()->getPosition().x;
				int y = item[i]->getPosition().y - Character::GetSingleTone()->getPosition().y;
				if( x > 50 && x < 200 ) {
					item[i]->setPositionX( item[i]->getPosition().x - 5 );
					if( y < 50 )
						item[i]->setPositionY( itemPositionY[i] += 5 );
					else if( y > Character::GetSingleTone()->getScale() * 100 + 50 )
						item[i]->setPositionY( itemPositionY[i] -= 5 );
				}
				else if( x < 50 ) {
					item[i]->setPositionX( item[i]->getPosition().x + 5 );
					if( y < 50 )
						item[i]->setPositionY( itemPositionY[i] += 5 );
					else if( y > Character::GetSingleTone()->getScale() * 100 + 50 )
						item[i]->setPositionY( itemPositionY[i] -= 5 );
				}
			}
		}
	}
}
void GameScene::eatUnbeatableItem()
{
	// 무적
	CCLog( "unbeatable");
	unbeatableWaitTime = 0;
	unbeatableState = true;
	Character::GetSingleTone()->setScale( 0.8f );
	this->unschedule( schedule_selector( GameScene::scaleSchedule ) );
	this->unschedule( schedule_selector( GameScene::objectCollision ) );
	this->schedule( schedule_selector( GameScene::unbeatableTime ) );
}
void GameScene::unbeatableTime( float f )
{
	unbeatableWaitTime++;
	if( unbeatableWaitTime > Character::GetSingleTone()->getUnbeatableTime() ) {
		this->unschedule( schedule_selector( GameScene::unbeatableTime ) );
		unbeatableState = false;
		Character::GetSingleTone()->setScale( this->currentScale->getScale() );
		this->schedule( schedule_selector( GameScene::objectCollision ) );
		this->schedule( schedule_selector( GameScene::scaleSchedule ) );
		CCLog( "<< Unbeatable End >>" );
	}
}
void GameScene::eatRandomItem()
{
	// 아이템 지정과 필드 지정(랜덤), 필드는 총 네가지가 있음(보너스 포함)
	fieldWaitTime = 0;
	int fieldNumber = rand() % 9;
	int itemNumber = rand() % 5 + 4;
	int tempZOrder = backgroundNode[0]->getZOrder();

	// 아이템 선택
	this->selectedItemByTag( itemNumber );

	// 효과를 case문에 추가하고 레이어 추가(몇 초후 기본 레이어로 돌아옴)
	// 기본, 태양, 초원 필드는 30%씩, 보너스는 10%로 지정(추후 수정)
	switch( fieldNumber ) {
		case 0 :
		case 3 :
		case 6 :
			CCLog( "base field" );
			break; // 기본
		case 1 :
		case 4 :
		case 7 :
			CCLog( "sun field" );
			backgroundNode[0]->setZOrder( 1 );
			backgroundNode[1]->setZOrder( 2 );
			sun->setZOrder( this->backgroundNode[1]->getZOrder() );
			break; // 태양
		case 2 :
		case 5 :
		case 8 :
			CCLog( "snow field" );
			backgroundNode[0]->setZOrder( 0 );
			backgroundNode[2]->setZOrder( 2 );
			break; // 눈
		default : break;
	}

	this->schedule( schedule_selector( GameScene::fieldTime ) );
}
void GameScene::fieldTime( float f )
{
	fieldWaitTime++;

	if( fieldWaitTime >= 300 ) {
		this->unschedule( schedule_selector( GameScene::fieldTime ) );
		int tempZOrder = backgroundNode[0]->getZOrder();

		switch( tempZOrder ) {
		case 0 :
			backgroundNode[0]->setZOrder( 2 );
			backgroundNode[2]->setZOrder( 0 );
			break;
		case 1 :
			backgroundNode[0]->setZOrder( 2 );
			backgroundNode[1]->setZOrder( 1 );
			sun->setZOrder( this->backgroundNode[1]->getZOrder() );
			break;
		}
	}
}
void GameScene::transBounsScene()
{
	// 캐릭터 클래스 bonus state 변환
	this->stopAllSchedule();
	Character::GetSingleTone()->bonusState = true;

	// 캐릭터 제거
	this->removeChild( Character::GetSingleTone(), false );

	// Bouns Scene 이동
	CCScene* pScene = BonusScene::scene();
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.8f, pScene) );
}
void GameScene::eatLargeTimeItem()
{
	float tempScale;

	CCLog( "large increase time" );
	tempScale = this->currentTimeBar->getScaleX() + ( this->timeBarScalePerSecond * 600 );
	if( tempScale >= 0.58f ) {
		Character::GetSingleTone()->time = _GAMEPLAYTIME;
		this->currentTimeBar->setScaleX( 0.58f );
	}
	else {
		Character::GetSingleTone()->time = Character::GetSingleTone()->time + 600;
		this->currentTimeBar->setScaleX( tempScale );
	}
}

// 계산 씬 이동
void GameScene::appearGameOver()
{
	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/AbilityData.xml.txt"); //아이템과 장애물 XML불러옴
	xml_document xmlDoc2;
	xml_parse_result result = xmlDoc2.load_file(fileName.c_str());

	if( !result ) {
		CCLog( "Error description %s", result.description() );
		CCLog( "Error offset %d", result.offset ); 
	}
	xml_node nodeResult = xmlDoc2.child("Result"); 

	int totalCoin = nodeResult.child("TotalMoney").text().as_int(); 
	int totalScore = nodeResult.child("TotalScore").text().as_int();
	totalCoin += Character::GetSingleTone()->coin;
	totalScore = Character::GetSingleTone()->meter + Character::GetSingleTone()->score;

	nodeResult.child("TotalMoney").text().set(totalCoin);
	nodeResult.child("TotalScore").text().set(totalScore);
	xmlDoc2.save_file("data/AbilityData.xml.txt");	

	this->jumpCount = 0;
	gameOver = CCSprite::create( "Images/gameover.png" );
	gameOver->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 ) );
	this->addChild( gameOver, 10 );
	waitGameOver = 0;
	this->stopAllSchedule();
	this->schedule( schedule_selector( GameScene::transCalculationScene ) );
}
void GameScene::transCalculationScene( float f )
{
	waitGameOver++;
	if( waitGameOver >= 60 ) {
		this->unschedule( schedule_selector (GameScene::transCalculationScene ) );
		CCScene* pScene = CalculationScene::scene();
		CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
	}
}

// 터치
void GameScene::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate( this, 0, true );
	CCLayer::onEnter();
}
void GameScene::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate( this );
	CCLayer::onExit();
}
bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *event )
{
	if( jumpCount == 2 ) {
		--jumpCount;
		Character::GetSingleTone()->doJump();

		x = baseField[0]->getPosition().x;

		// 캐릭터 포지션 체크
		this->schedule( schedule_selector( GameScene::ccTouchPositionCheck ) );
	}
	else if( jumpCount == 1 ) {
		--jumpCount;
		Character::GetSingleTone()->doDoubleJump();
	}

	return true;
}
void GameScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *event )
{
}
void GameScene::ccTouchPositionCheck( float f )
{
	if( Character::GetSingleTone()->getPosition().y == Character::GetSingleTone()->getInitPositionY() ) {
		// 점프 카운트 초기화
		jumpCount = 2;

		CCLog( "jump range < %f >", baseField[0]->getPosition().x - x );

		// 캐릭터 포지션 체크 중지
		this->unschedule( schedule_selector( GameScene::ccTouchPositionCheck ) );
	}
}

// 백그라운드 포지션에 따른 속도 증가
void GameScene::speedUpByBackgroundPosition( float f )
{
	// max speed는 0.4f, 0.2f
	if( speedLevel == 6 ) {
		CCLog( "speed schedule end" );
		this->unschedule( schedule_selector( GameScene::speedUpByBackgroundPosition ) );
		this->backgroundSpeed = 0.8f; // 1.0f
		this->objectItemSpeed = 0.4f; // 0.5f
		this->stopActionAllObjectAndRedefine( 4, 0.8f, 0.4f );
	}

	// Big time 아이템 position을 지나칠 시 속도를 증가시킨다.
	if( this->backgroundNode[0]->getPosition().x < ((-6400 * speedLevel) + ((-6400 * 0.12f * (speedLevel - 1)))) ) {
		CCLog( "speed up" );
		CCLog( "%f", this->backgroundNode[0]->getPosition().x );
		this->backgroundSpeed -= 0.02f;
		this->objectItemSpeed -= 0.05f;
		this->stopActionAllObjectAndRedefine( 4, this->backgroundSpeed, this->objectItemSpeed );
		speedLevel++;
	}
}