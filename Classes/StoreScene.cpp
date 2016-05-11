#include "StoreScene.h"
#include "Character.h"
#include "MainScene.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace pugi;

// ������, �Ҹ���
StoreScene::StoreScene()
{
	maxLevel[0] = 3;  // ���� ������ max level
	maxLevel[1] = 10; // ũ�� ���� ������ max level
	maxLevel[2] = 10; // �ð� ���� ������ max level
	maxLevel[3] = 10; // �ν�Ʈ ������ max level
	maxLevel[4] = 5;  // �ڼ� ������ max level
	maxLevel[5] = 5;  // ���� ������ max level
	maxLevel[6] = 10; // ���ʽ� ������ max level
}
StoreScene::~StoreScene()
{
}

// CCLayerColor Override
CCScene* StoreScene::scene()
{
	StoreScene* layer = StoreScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool StoreScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4( 255, 255, 255, 255) ) )
		return false;

	this->createStoreLayer();	  // Store Layer �����
	this->initializeScrollView(); // ��ũ�� �� �ʱ�ȭ
	this->createItemSprite();	  // ������ ��������Ʈ ����
	this->createExplainItem();	  // ������ ���� ����
	this->createMoneyLabel();	  // ���� �� ���̺� ����
	this->createItemLevelLabel(); // ������ ���� ���̺� ����
	this->createUpgradeMenu();	  // ������ ���׷��̵� �޴� ����

	return true;
}

// Store Scene ����
void StoreScene::createStoreLayer()
{
	// xml�� ����� ������ ���� ������ Total money�� �о�´�.
	this->xmlRead();

	// ���
	background = CCSprite::create( "Images/store/background.png" );
	background->setAnchorPoint( ccp(0, 0) );
	background->setPosition( ccp(0, 0) );
	this->addChild( background );

	// ���� �г�
	panel = CCSprite::create( "Images/store/panel.png" );
	panel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 ) );
	panel->setScale( 0.9f );	
	background->addChild( panel);

	// scene name �߰�
	sceneName = CCSprite::create( "Images/store/store.png" );
	sceneName->setPosition( ccp(panel->getPosition().x, panel->getPosition().y + 220));
	panel->addChild( sceneName );

	// back ��ư
	backButton = CCMenuItemImage::create( "Images/store/exit_01.png", "Images/store/exit_02.png", this, menu_selector( StoreScene::returnMenu ) );
	CCMenu* back = CCMenu::create( backButton, NULL );
	back->setPosition( ccp( panel->getPosition().x + 95, panel->getPosition().y - 35 ) );
	back->setScale( 0.3f );
	panel->addChild( back, 5 );
}
void StoreScene::returnMenu( CCObject *pSender )
{
	// Store scene�� ����鼭 ����� ������ ��ġ�� Total money�� xml�� �����Ѵ�.
	this->xmlWrite();
	// Main Scene ���� ���ư���.
	CCScene *pScene = MainScene::scene(); 
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
}
void StoreScene::xmlRead()
{
	// XML Pasing
	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/AbilityData.xml.txt");
	xml_parse_result result = xmlDoc.load_file(fileName.c_str());

	if( !result ) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
	}

	// read xml
	nodeResult =xmlDoc.child("Result");
	nodeItemAbility = nodeResult.child("ItemAbility");
	this->TOTALMONEY = nodeResult.child("TotalMoney").text().as_int();
	this->levelData[0] = nodeItemAbility.child("ScoreLevel").text().as_int();
	this->levelData[1] = nodeItemAbility.child("SizeUpLevel").text().as_int();
	this->levelData[2] = nodeItemAbility.child("PlayTimeLevel").text().as_int();
	this->levelData[3] = nodeItemAbility.child("BoostLevel").text().as_int();
	this->levelData[4] = nodeItemAbility.child("MagneticLevel").text().as_int();
	this->levelData[5] = nodeItemAbility.child("UnbeatableLevel").text().as_int();
	this->levelData[6] = nodeItemAbility.child("BonusLevel").text().as_int();
}
void StoreScene::xmlWrite()
{
	// write xml
	nodeResult.child("TotalMoney").text().set( this->TOTALMONEY );
	nodeItemAbility.child("ScoreLevel").text().set(			this->levelData[0] );
	nodeItemAbility.child("SizeUpLevel").text().set(		this->levelData[1] );
	nodeItemAbility.child("PlayTimeLevel").text().set(		this->levelData[2] );
	nodeItemAbility.child("BoostLevel").text().set(			this->levelData[3] );
	nodeItemAbility.child("MagneticLevel").text().set(		this->levelData[4] );
	nodeItemAbility.child("UnbeatableLevel").text().set(	this->levelData[5] );
	nodeItemAbility.child("BonusLevel").text().set(			this->levelData[6] );
	xmlDoc.save_file("data/AbilityData.xml.txt");
}

// ��ũ�� ��
void StoreScene::initializeScrollView()
{
	scrollLayer = CCLayerColor::create( ccc4(255, 255, 255, 255) );
	scrollLayer->setOpacity( 0 );
	scrollLayer->setAnchorPoint( CCPointZero );
	scrollLayer->setPosition( panel->getPosition() );
	scrollLayer->setContentSize( CCSizeMake(panel->getContentSize().width, 600) );

	scrollView = CCScrollView::create();
	scrollView->retain();
	scrollView->setDirection( kCCScrollViewDirectionVertical );
	scrollView->setViewSize( CCSizeMake(panel->getContentSize().width * 0.9, panel->getContentSize().height * 0.75) );
	scrollView->setContentSize( scrollLayer->getContentSize() );
	scrollView->setPosition( ccp(panel->getPosition().x - 465, panel->getPosition().y - 325) );
	scrollView->setContainer( scrollLayer );
	scrollView->setDelegate( this );
	scrollView->setContentOffset( CCPointMake(0, -(scrollLayer->getContentSize().height * 0.1) - 20), false );
	panel->addChild( scrollView );
}
void StoreScene::scrollViewDidScroll( CCScrollView *view )
{
}
void StoreScene::scrollViewDidZoom( CCScrollView *view )
{
}

// ������ ��������Ʈ ����
void StoreScene::createItemSprite()
{
	// ���� ������ ��������Ʈ �߰�
	switch ( Character::GetSingleTone()->getScoreLevel() ) {
		case 1 : scoreItem = CCSprite::create( "Images/store/score1.png" ); break;
		case 2 : scoreItem = CCSprite::create( "Images/store/score2.png" ); break;
		case 3 : scoreItem = CCSprite::create( "Images/store/score3.png" ); break;
		default : break;
	}
	scoreItem->setPosition( ccp( scrollLayer->getContentSize().width / 2 - 400, scrollLayer->getContentSize().height / 2 + 250 ) );
	scoreItem->setScale( 0.15f );
	scrollLayer->addChild( scoreItem );

	// ũ�� ���� ������ ��������Ʈ �߰�
	sizeUpItem = CCSprite::create( "Images/store/sizeUp.png" );
	sizeUpItem->setPosition( ccp( scoreItem->getPosition().x, scoreItem->getPosition().y - 75 ) );
	sizeUpItem->setScale( 0.1f );
	scrollLayer->addChild( sizeUpItem );

	// �ð� ���� ������ ��������Ʈ �߰�
	playTimeItem = CCSprite::create( "Images/store/playTime.png" );
	playTimeItem->setPosition( ccp( scoreItem->getPosition().x , scoreItem->getPosition().y - 150 ) );
	playTimeItem->setScale( 0.1f );
	scrollLayer->addChild( playTimeItem );
	
	// �ν��� ������ ��������Ʈ �߰�
	boostItem = CCSprite::create( "Images/store/boost.png" );
	boostItem->setPosition( ccp( scoreItem->getPosition().x, scoreItem->getPosition().y - 225 ) );
	boostItem->setScale( 0.1f );
	scrollLayer->addChild( boostItem );

	// �ڼ� ������ ��������Ʈ �߰�
	magneticItem = CCSprite::create( "Images/store/magnetic.png" );
	magneticItem->setPosition( ccp( scoreItem->getPosition().x, scoreItem->getPosition().y - 300 ) );
	magneticItem->setScale( 0.1f );
	scrollLayer->addChild( magneticItem );

	// ���� ������ ��������Ʈ �߰�
	unbeatableItem = CCSprite::create( "Images/store/unbeatable.png" );
	unbeatableItem->setPosition( ccp( scoreItem->getPosition().x, scoreItem->getPosition().y - 375 ) );
	unbeatableItem->setScale( 0.1f );
	scrollLayer->addChild( unbeatableItem );

	// ���ʽ� ������ ��������Ʈ �߰�
	bonusItem = CCSprite::create( "Images/store/bonus.png" );
	bonusItem->setPosition( ccp( scoreItem->getPosition().x, scoreItem->getPosition().y - 450 ) );
	bonusItem->setScale( 0.1f );
	scrollLayer->addChild( bonusItem );
}

// ������ ���� ����
void StoreScene::createExplainItem()
{
	// ���� ���� �߰�
	scoreExplain = CCSprite::create( "Images/store/scoreExplain.png" );
	scoreExplain->setPosition( ccp( scoreItem->getPosition().x + 200, scoreItem->getPosition().y ) );
	scoreExplain->setScale( 0.7f );
	scrollLayer->addChild( scoreExplain );

	// ũ�� ���� ���� �߰�
	sizeUpExplain = CCSprite::create( "Images/store/sizeUpExplain.png" );
	sizeUpExplain->setPosition( ccp( scoreExplain->getPosition().x, sizeUpItem->getPosition().y ) );
	sizeUpExplain->setScale( 0.5f );
	scrollLayer->addChild( sizeUpExplain );

	// �ð� ���� ���� �߰�
	playTimeExplain = CCSprite::create( "Images/store/playTimeExplain.png" );
	playTimeExplain->setPosition( ccp( scoreExplain->getPosition().x, playTimeItem->getPosition().y ) );
	playTimeExplain->setScale( 0.5f );
	scrollLayer->addChild( playTimeExplain );

	// �ν��� ���� �߰�
	boostExplain = CCSprite::create( "Images/store/boostExplain.png" );
	boostExplain->setPosition( ccp( scoreExplain->getPosition().x, boostItem->getPosition().y ) );
	boostExplain->setScale( 0.5f );
	scrollLayer->addChild( boostExplain );

	// �ڼ� ���� �߰�
	magneticExplain = CCSprite::create( "Images/store/magneticExplain.png" );
	magneticExplain->setPosition( ccp( scoreExplain->getPosition().x, magneticItem->getPosition().y ) );
	magneticExplain->setScale(  0.5f );
	scrollLayer->addChild( magneticExplain );

	// ���� ���� �߰�
	unbeatableExplain = CCSprite::create( "Images/store/unbeatableExplain.png" );
	unbeatableExplain->setPosition( ccp( scoreExplain->getPosition().x, unbeatableItem->getPosition().y ) );
	unbeatableExplain->setScale(  0.5f );
	scrollLayer->addChild( unbeatableExplain );

	// ���ʽ� ���� �߰�
	bonusExplain = CCSprite::create( "Images/store/bonusExplain.png" );
	bonusExplain->setPosition( ccp( scoreExplain->getPosition().x, bonusItem->getPosition().y ) );
	bonusExplain->setScale( 0.5f);
	scrollLayer->addChild( bonusExplain );
}

// ���� �� ���̺� ����
void StoreScene::inputMoneyData()
{
	sprintf( moneyArray[0], "%d", this->TOTALMONEY );
	for( int i = 0; i < 7; i++ ) {
		moneyData[i] = (levelData[i] + 1) * 1000;
		sprintf( moneyArray[i + 1], "%d", moneyData[i] );
	}
}
void StoreScene::createMoneyLabel()
{
	// �� ���� ���� �� ������ ���׷��̵� ����� string���� ��ȯ
	this->inputMoneyData();

	// �� ���� �� ���̺� �߰�
	totalMoney = CCLabelTTF::create( moneyArray[0], "Arial", 30 );
	totalMoney->setColor( ccBLACK );
	totalMoney->setAnchorPoint( ccp(1, 0) );
	totalMoney->setPosition( ccp( panel->getContentSize().width - 100, panel->getContentSize().height - 100) );
	panel->addChild( totalMoney );

	// ���� ������ �� ���̺� �߰�
	scoreMoney = CCLabelTTF::create( moneyArray[1], "Arial", 30 );
	scoreMoney->setColor( ccBLACK );
	scoreMoney->setAnchorPoint( ccp(1, 0) );
	scoreMoney->setPosition( ccp( scoreExplain->getPosition().x + 300, scoreExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( scoreMoney );

	// ũ�� ���� ������ �� ���̺� �߰�
	sizeUpMoney = CCLabelTTF::create( moneyArray[2], "Arial", 30 );
	sizeUpMoney->setColor( ccBLACK );
	sizeUpMoney->setAnchorPoint( ccp(1, 0) );
	sizeUpMoney->setPosition( ccp( scoreMoney->getPosition().x, sizeUpExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( sizeUpMoney );

	// �ð� ���� ������ �� ���̺� �߰�
	playTimeMoney = CCLabelTTF::create( moneyArray[3], "Arial", 30 );
	playTimeMoney->setColor( ccBLACK );
	playTimeMoney->setAnchorPoint( ccp(1, 0) );
	playTimeMoney->setPosition( ccp( scoreMoney->getPosition().x, playTimeExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( playTimeMoney );

	// �ν��� ������ �� ���̺� �߰�
	boostMoney = CCLabelTTF::create( moneyArray[4], "Arial", 30 );
	boostMoney->setColor( ccBLACK );
	boostMoney->setAnchorPoint( ccp(1, 0) );
	boostMoney->setPosition( ccp( scoreMoney->getPosition().x, boostExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( boostMoney );

	// �ڼ� ������ �� ���̺� �߰�
	magneticMoney = CCLabelTTF::create( moneyArray[5], "Arial", 30 );
	magneticMoney->setColor( ccBLACK );
	magneticMoney->setAnchorPoint( ccp(1, 0) );
	magneticMoney->setPosition( ccp( scoreMoney->getPosition().x, magneticExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( magneticMoney );

	// ���� ������ �� ���̺� �߰�
	unbeatableMoney = CCLabelTTF::create( moneyArray[6], "Arial", 30 );
	unbeatableMoney->setColor( ccBLACK );
	unbeatableMoney->setAnchorPoint( ccp(1, 0) );
	unbeatableMoney->setPosition( ccp( scoreMoney->getPosition().x, unbeatableExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( unbeatableMoney );

	// ���ʽ� ������ �� ���̺� �߰�
	bonusMoney = CCLabelTTF::create( moneyArray[7], "Arial", 30 );
	bonusMoney->setColor( ccBLACK );
	bonusMoney->setAnchorPoint( ccp(1, 0) );
	bonusMoney->setPosition( ccp( scoreMoney->getPosition().x, bonusExplain->getPosition().y - 20 ) );
	scrollLayer->addChild( bonusMoney );
}

// ������ ���� ���̺� ����
void StoreScene::inputLevelData()
{
	for( int i = 0; i < 7; i++ )
		sprintf( levelArray[i], "%d", levelData[i] );
}
void StoreScene::createItemLevelLabel()
{
	// �� ������ ���� �����͸� string���� ��ȯ
	this->inputLevelData();

	// ���� ������ ���� ���̺� �߰�
	if( levelData[0] == maxLevel[0] ) scoreLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else scoreLevel = CCLabelTTF::create( levelArray[0], "Arial", 30 );
	scoreLevel->setColor( ccBLACK );
	scoreLevel->setAnchorPoint( ccp(1, 0) );
	scoreLevel->setPosition( ccp( scoreMoney->getPosition().x + 150, scoreMoney->getPosition().y ) );
	scrollLayer->addChild( scoreLevel );

	// ũ�� ���� ������ ���� ���̺� �߰�
	if( levelData[1] == maxLevel[1] ) sizeUpLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else sizeUpLevel = CCLabelTTF::create( levelArray[1], "Arial", 30 );
	sizeUpLevel->setColor( ccBLACK );
	sizeUpLevel->setAnchorPoint( ccp(1, 0) );
	sizeUpLevel->setPosition( ccp( scoreLevel->getPosition().x, sizeUpMoney->getPosition().y ) );
	scrollLayer->addChild( sizeUpLevel );

	// �ð� ���� ������ ���� ���̺� �߰�
	if( levelData[2] == maxLevel[2] ) playTimeLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else playTimeLevel = CCLabelTTF::create( levelArray[2], "Arial", 30 );
	playTimeLevel->setColor( ccBLACK );
	playTimeLevel->setAnchorPoint( ccp(1, 0) );
	playTimeLevel->setPosition( ccp( scoreLevel->getPosition().x, playTimeMoney->getPosition().y ) );
	scrollLayer->addChild( playTimeLevel );

	// �ν��� ������ ���� ���̺� �߰�
	if( levelData[3] == maxLevel[3] ) boostLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else boostLevel = CCLabelTTF::create( levelArray[3], "Arial", 30 );
	boostLevel->setColor( ccBLACK );
	boostLevel->setAnchorPoint( ccp(1, 0) );
	boostLevel->setPosition( ccp( scoreLevel->getPosition().x, boostMoney->getPosition().y ) );
	scrollLayer->addChild( boostLevel );

	// �ڼ� ������ ���� ���̺� �߰�
	if( levelData[4] == maxLevel[4] ) magneticLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else magneticLevel = CCLabelTTF::create( levelArray[4], "Arial", 30 );
	magneticLevel->setColor( ccBLACK );
	magneticLevel->setAnchorPoint( ccp(1, 0) );
	magneticLevel->setPosition( ccp( scoreLevel->getPosition().x, magneticMoney->getPosition().y ) );
	scrollLayer->addChild( magneticLevel );

	// ���� ������ ���� ���̺� �߰�
	if( levelData[5] == maxLevel[5] ) unbeatableLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else unbeatableLevel = CCLabelTTF::create( levelArray[5], "Arial", 30 );
	unbeatableLevel->setColor( ccBLACK );
	unbeatableLevel->setAnchorPoint( ccp(1, 0) );
	unbeatableLevel->setPosition( ccp( scoreLevel->getPosition().x, unbeatableMoney->getPosition().y ) );
	scrollLayer->addChild( unbeatableLevel );

	// ���ʽ� ������ ���� ���̺� �߰�
	if( levelData[6] == maxLevel[6] ) bonusLevel = CCLabelTTF::create( "MAX", "Arial", 30 );
	else bonusLevel = CCLabelTTF::create( levelArray[6], "Arial", 30 );
	bonusLevel->setColor( ccBLACK );
	bonusLevel->setAnchorPoint( ccp(1, 0) );
	bonusLevel->setPosition( ccp( scoreLevel->getPosition().x, bonusMoney->getPosition().y ) );
	scrollLayer->addChild( bonusLevel );
}

// ������ ���׷��̵� �޴� ���� �� ����
void StoreScene::createUpgradeMenu()
{
	for( int i = 0; i < _MAXSTOREITEM; i++ ) {
		if( levelData[i] < maxLevel[i] ) {
			buttonImage[i] = CCMenuItemImage::create( "Images/store/up02.png", "Images/store/up03.png", this, menu_selector( StoreScene::upgradeManager ) );
			buttonImage[i]->setTag( i );
			buttonImage[i]->setScale(0.7f);

			button[i] = CCMenu::create( buttonImage[i], NULL );
			button[i]->setPosition( ccp( scoreLevel->getPosition().x + 75, scoreItem->getPosition().y - (75 * i) ) );
			scrollLayer->addChild( button[i] );
		}
	}
}
void StoreScene::upgradeManager( CCObject *pSender )
{
	CCMenuItemImage* temp = (CCMenuItemImage *)pSender;
	switch( temp->getTag() ) {
		case 0 : this->scoreItemUpgrade();		break;
		case 1 : this->sizeUpItemUpgrade();		break;
		case 2 : this->playTimeItemUpgrade();	break;
		case 3 : this->boostItemUpgrade();		break;
		case 4 : this->magneticItemUpgrade();	break;
		case 5 : this->unbeatableItemUpgrade();	break;
		case 6 : this->bonusItemUpgrade();		break;
		default : break;
	}
}
void StoreScene::subtractTotalMoney( int upgradeCost )
{
	char temp[10];
	this->TOTALMONEY -= upgradeCost;
	sprintf( temp, "%d", TOTALMONEY );
	this->totalMoney->setString( temp );
}
void StoreScene::scoreItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[0] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];
	
		// ���� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[0] );
	
		// ���� ������ ���� ��� �� ���̺� �� �̹��� ����
		this->levelData[0]++;
		scrollLayer->removeChild( scoreItem, true );
		switch( this->levelData[0] ) {
			case 2 : scoreItem = CCSprite::create( "Images/store/score2.png" ); break;
			case 3 : scoreItem = CCSprite::create( "Images/store/score3.png" ); break;
			default : break;
		}
		scoreItem->setPosition( ccp( scrollLayer->getContentSize().width / 2 - 400, scrollLayer->getContentSize().height / 2 + 250 ) );
		scoreItem->setScale( 0.1f );
		scrollLayer->addChild( scoreItem );
		sprintf( tempLevel, "%d", levelData[0] );

		this->scoreLevel->setString( tempLevel );
		// ���� ������ ���� 3�̸� MAX
		if( levelData[0] == 3 ) {
			strcpy( tempLevel, "MAX" );
			this->scoreLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[0], true );
		}
	
		// ���� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[0] = (levelData[0] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[0] );
		this->scoreMoney->setString( tempMoney );
	}
}
void StoreScene::sizeUpItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[1] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// ũ�� ���� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[1] );

		// ũ�� ���� ������ ���� ��� �� ���̺� ����
		this->levelData[1]++;
		sprintf( tempLevel, "%d", levelData[1] );
		this->sizeUpLevel->setString( tempLevel );

		// ũ�� ���� ������ ���� 10�̸� MAX
		if( levelData[1] == 10 ) {
			strcpy( tempLevel, "MAX" );
			this->sizeUpLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[1], true );
		}
	
		// ũ�� ���� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[1] = (levelData[1] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[1] );
		this->sizeUpMoney->setString( tempMoney );
	}
}
void StoreScene::playTimeItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[2] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// �ð� ���� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[2] );

		// �ð� ���� ������ ���� ��� �� ���̺� ����
		this->levelData[2]++;
		sprintf( tempLevel, "%d", levelData[2] );
		this->playTimeLevel->setString( tempLevel );

		// �ð� ���� ������ ���� 10�̸� MAX
		if( levelData[2] == 10 ) {
			strcpy( tempLevel, "MAX" );
			this->playTimeLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[2], true );
		}
	
		// �ð� ���� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[2] = (levelData[2] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[2] );
		this->playTimeMoney->setString( tempMoney );
	}
}
void StoreScene::boostItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[3] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// �ν��� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[3] );

		// �ν��� ������ ���� ��� �� ���̺� ����
		this->levelData[3]++;
		sprintf( tempLevel, "%d", levelData[3] );
		this->boostLevel->setString( tempLevel );

		// �ν��� ���� ������ ���� 10�̸� MAX
		if( levelData[3] == 10 ) {
			strcpy( tempLevel, "MAX" );
			this->boostLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[3], true );
		}
	
		// �ν��� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[3] = (levelData[3] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[3] );
		this->boostMoney->setString( tempMoney );
	}
}
void StoreScene::magneticItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[4] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// �ڼ� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[4] );

		// �ڼ� ������ ���� ��� �� ���̺� ����
		this->levelData[4]++;
		sprintf( tempLevel, "%d", levelData[4] );
		this->magneticLevel->setString( tempLevel );

		// �ڼ� ���� ������ ���� 5�� MAX
		if( levelData[4] == 5 ) {
			strcpy( tempLevel, "MAX" );
			this->magneticLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[4], true );
		}
	
		// �ڼ� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[4] = (levelData[4] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[4] );
		this->magneticMoney->setString( tempMoney );
	}
}
void StoreScene::unbeatableItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[5] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// ���� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[5] );

		// ���� ������ ���� ��� �� ���̺� ����
		this->levelData[5]++;
		sprintf( tempLevel, "%d", levelData[5] );
		this->unbeatableLevel->setString( tempLevel );

		// ���� ���� ������ ���� 5�� MAX
		if( levelData[5] == 5 ) {
			strcpy( tempLevel, "MAX" );
			this->unbeatableLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[5], true );
		}
	
		// ���� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[5] = (levelData[5] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[5] );
		this->unbeatableMoney->setString( tempMoney );
	}
}
void StoreScene::bonusItemUpgrade()
{
	if( this->TOTALMONEY - moneyData[6] >= 0 ) {
		// �ӽ� ����, ���� �� ����
		char tempLevel[10], tempMoney[10];

		// ���ʽ� ������ ���׷��̵� ����� �� ���� ������ ����.
		this->subtractTotalMoney( this->moneyData[6] );

		// ���ʽ� ������ ���� ��� �� ���̺� ����
		this->levelData[6]++;
		sprintf( tempLevel, "%d", levelData[6] );
		this->bonusLevel->setString( tempLevel );

		// ���ʽ� ���� ������ ���� 10�̸� MAX
		if( levelData[6] == 10 ) {
			strcpy( tempLevel, "MAX" );
			this->bonusLevel->setString( tempLevel );
			scrollLayer->removeChild( this->button[6], true );
		}
	
		// ���ʽ� ������ ���׷��̵� ��� ��� �� ���̺� ����
		this->moneyData[6] = (levelData[6] + 1) * 1000;
		sprintf( tempMoney, "%d", moneyData[6] );
		this->bonusMoney->setString( tempMoney );
	}
}

// ��ġ
void StoreScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1 ,true);
}
void StoreScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
bool StoreScene::ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *event)
{
	return true;
}
void StoreScene::ccTouchCancelled( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *event) 
{
}