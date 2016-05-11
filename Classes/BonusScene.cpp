#include "BonusScene.h"
#include "GameScene.h"
#include "Character.h"

using namespace cocos2d;
using namespace pugi;

float BonusScene::itemPosition_x[_MAXNUM];
float BonusScene::itemPosition_y[_MAXNUM];
int BonusScene::item_code[_MAXNUM];

// ������, �Ҹ���
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

	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/BonusItems_01.xml.txt"); //���ʽ� ������ XML�ҷ���
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
	// ���� ������ ���ϸ� �ʱ�ȭ
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
	// ĳ���� �߰�
	this->addChild( Character::GetSingleTone(), 4 );
	Character::GetSingleTone()->setPosition( ccp( Character::GetSingleTone()->getInitPositionX(), Character::GetSingleTone()->getInitPositionY() ) );

	// �޸���
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

	// ������ ����
	for( int i = 0; i < itemCount; i++ ) {
		// ������ ���� ���� üũ
		checkEatItem[i] = false;

		switch( item_code[i] ) {
			case 0 : item[i] = CCSprite::create( this->scoreItemName[0][(int)(rand() % 4)].c_str() ); break; // ����
			case 1 : item[i] = CCSprite::create( this->scoreItemName[1][(int)(rand() % 4)].c_str() ); break; // ����
			case 2 : item[i] = CCSprite::create( this->scoreItemName[2][(int)(rand() % 4)].c_str() ); break; // ����
			case 3 : item[i] = CCSprite::create( "Images/item/coin.png" );							  break; // ����
			default : break;
		}

		item[i]->setPosition( ccp( itemPosition_x[i], itemPosition_y[i] ) );
		item[i]->setTag( item_code[i] ); // tag�� �������� ���������� �����ϱ� ���� setTag
		item[i]->setScale( 0.15f );       // ������ ���� �������� ������ ����

		this->addChild( item[i], 3 );

		// ������ �׼� �߰�
		item[i]->runAction( this->setSpeed( ItemSpeed ) );
	}
}
void BonusScene::stopActionAllObjectAndRedefine( int operationCode, float background, float item )
{
	float bgSpeed = 0.0f, oiSpeed = 0.0f;

	// ����
	switch( operationCode ) {
		case 0 : bgSpeed = this->backgroundSpeed + background; oiSpeed = this->ItemSpeed + item; break; // ����
		case 1 : bgSpeed = this->backgroundSpeed - background; oiSpeed = this->ItemSpeed - item; break; // ����
		case 2 : bgSpeed = this->backgroundSpeed * background; oiSpeed = this->ItemSpeed * item; break; // ����
		case 3 : bgSpeed = this->backgroundSpeed / background; oiSpeed = this->ItemSpeed / item; break; // ������
		case 4 : bgSpeed = background; oiSpeed = item; break;											// ����
		default : break;
	}

	// ���ǵ� ���
	CCLog( "speed < %.2f, %.2f >", bgSpeed, oiSpeed );

	// ��� ���� �� ������
	bonusField[0]->stopAllActions();
	bonusField[0]->runAction( this->setSpeed( bgSpeed ) );

	// ������ ���� �� ������
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
		// ĳ���� ����
		this->removeChild( Character::GetSingleTone(), true );

		// Game Scene �̵�
		CCScene* pScene = GameScene::scene();
		CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.8f, pScene) );
	}
}

// ������ �浹
void BonusScene::itemCollision( float f )
{
	for( int i = 0; i < itemCount; i++ ) {
		if( !checkEatItem[i] ) {
			int x = item[i]->getPosition().x - Character::GetSingleTone()->getPosition().x;
			float top = Character::GetSingleTone()->getPosition().y + (Character::GetSingleTone()->getContentSize().height * Character::GetSingleTone()->getScaleY());
			float bottom = Character::GetSingleTone()->getPosition().y;
			if( x > 0 && x < (90 + (Character::GetSingleTone()->getScaleX() * 10) )  ) { 
				if( item[i]->getPosition().y > bottom && item[i]->getPosition().y < top - 25 ) {
					// ������ �±׿� ���� �ൿ���ϰ� �������� ���� �Ѵ�.
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

// ���� ������ ȹ�� �� ȿ��
void BonusScene::selectedItemByTag( int itemCode )
{
	switch( itemCode ) {
		case 0 : 
		case 1 :
		case 2 :
				 this->eatScoreItem(); break; // ���� �߰�
		case 3 : this->eatCoinItem();  break; // ����
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

// ��ġ
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

		// ĳ���� ������ üũ
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
		// ���� ī��Ʈ �ʱ�ȭ
		jumpCount = 2;

		CCLog( "jump range < %f >", bonusField[0]->getPosition().x - x );

		// ĳ���� ������ üũ ����
		this->unschedule( schedule_selector( BonusScene::ccTouchPositionCheck ) );
	}
}