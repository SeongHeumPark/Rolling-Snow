#include "CalculationScene.h"
#include "MainScene.h"
#include "GameScene.h"
#include "Character.h"
#include "LogoScene.h"

using namespace cocos2d;
using namespace pugi;
using namespace cocos2d::extension;

CalculationScene::CalculationScene()
{ 
}
CalculationScene::~CalculationScene()
{
}

CCScene* CalculationScene::scene()
{
	CCScene* scene = CCScene::create();
	CalculationScene *layer = CalculationScene::create();
	scene->addChild(layer);
	return scene;
}
bool CalculationScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255 ,255, 255) ) )
		return false;

	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/AbilityData.xml.txt");
	xml_parse_result result = xmlDoc.load_file(fileName.c_str());

	if( !result ) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
		return false;
	}

	nodeResult = xmlDoc.child("Result"); 
	TotalScore = nodeResult.child("TotalScore").text().as_int();  
	sprintf(temptotalscore,"%d",TotalScore);

	backGround();																							//배경
	xml_Info();																								//XML 변수를 받아오는 함수
	 
	if(TotalScore>=ranking_Score[9]) takeRank();
	else							 notakeRank(); 

	return true;
}

void CalculationScene::backGround()																	//배경 및 최종 점수만을 알려주는 함수
{
	CCSprite *background = CCSprite::create( "Images/calculation/background.png" );
	background->setAnchorPoint( ccp(0, 0) );
	background->setPosition( ccp(0, 0) );
	this->addChild( background );

	CCSprite *panel = CCSprite::create( "Images/calculation/scorePanel.png" );
	panel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 ) );
	panel->setScaleX( 0.8f );
	panel->setScaleY( 0.6f );
	background->addChild( panel );
}
void CalculationScene::xml_Info()
{
	int i = 0;
	nodeRankings = nodeResult.child("nodeRankings");                                                   //Accept Ranking list
	ranking = nodeRankings.child("Ranking");

	for(ranking = nodeRankings.child("Ranking"); ranking; ranking = ranking.next_sibling("Ranking"), i++ ) {
		Ranking_Info[i] = (char*) ranking.child("Name").text().get();
		ranking_Score[i] = ranking.child("Score").text().as_int();
	}
}

void CalculationScene::takeRank()
{
	//LogoScene::takeRankSound();
	
	CCSprite * yourScore = CCSprite::create( "Images/calculation/yourScore.png");								//점수 스프라이트
	yourScore->setPosition( ccp(310, 470) );
	this->addChild( yourScore );

	CCLabelTTF *ScoreLabel = CCLabelTTF::create( temptotalscore, "Helvetica", 50 );
	ScoreLabel->setPosition( ccp(572, 455) );
	ScoreLabel->setColor( ccc3(255, 255, 0) );
	this->addChild( ScoreLabel );

	CCSprite *userNameSprite = CCSprite::create( "Images/calculation/userName.png" );
	userNameSprite->setPosition( ccp(325, 165) );
	this->addChild( userNameSprite );

	CCSize editBoxSize = CCSizeMake( 300, 60 );													    //About the Input of Username
	userName = CCEditBox::create( editBoxSize, CCScale9Sprite::create( "Images/calculation/userName.png" ) );
	userName->setPosition( ccp(340, 160) );
	userName->setFontColor( ccBLACK );
	userName->setPlaceHolder( "User Name" );
	userName->setPlaceholderFontColor( ccBLACK );
	userName->setMaxLength( 8 );
	userName->setReturnType( kKeyboardReturnTypeDone );
	userName->setDelegate( this );
	userName->setOpacity( 0 );
	userName->setScale( 0.85f );
	addChild( userName );

	CCSprite * yourRank = CCSprite::create( "Images/calculation/yourRank.png" );									//등수 스프라이트
	yourRank->setPosition( ccp(310, 370) );
	yourRank->setColor( ccc3(255, 0, 0) );
	this->addChild( yourRank );

	CalculationScene::editBoxTextChanged( userName, this->UserName_str );

	int grade = 9;
	while(1) {
		if( TotalScore<ranking_Score[grade] || grade == -1 ) break; 
		grade--;
	} 					
	 
	char finalGrade[3];
	sprintf( finalGrade , "%d" , grade + 2 );
	CCLabelTTF *YourGrade = CCLabelTTF::create( "0", "Helvetica", 50 );
	YourGrade->setString( finalGrade );
	YourGrade->setPosition( ccp(572, 355) );
	this->addChild(YourGrade);

	CCMenuItemImage *ToRankingScene = CCMenuItemImage::create("Images/calculation/rank.png","Images/calculation/rank.png", this, menu_selector( CalculationScene::ToRankingScene ) );
	ToRankingScene->setScale(0.57f);

	FaultUserCount = 0;

	CCMenu *pmenu = CCMenu::create( ToRankingScene, NULL );
	pmenu->setPosition( ccp(700, 170) );
	this->addChild( pmenu ); 
}
void CalculationScene::notakeRank()
{
	//LogoScene::notakeRankSound();

	CCSprite * yourScore = CCSprite::create( "Images/calculation/yourScore.png" );								//점수 스프라이트
	yourScore->setPosition( ccp(310, 430) ); 
	yourScore->setColor( ccc3(255, 0, 0) );
	this->addChild (yourScore );

	CCLabelTTF *ScoreLabel = CCLabelTTF::create( temptotalscore, "Helvetica" ,50 );
	ScoreLabel->setPosition( ccp(550, 415) );
	ScoreLabel->setColor( ccc3(255, 255, 0) );
	this->addChild( ScoreLabel );

	CCMenuItemImage *ToGameScene = CCMenuItemImage::create("Images/calculation/restart.png", "Images/calculation/restart.png", this, menu_selector( CalculationScene::ToGameScene ) );
	ToGameScene->setScale(0.57f);

	CCMenuItemImage *ToMainScene = CCMenuItemImage::create("Images/calculation/tomain.png", "Images/calculation/tomain.png", this, menu_selector( CalculationScene::ToMainScene ) );
	ToMainScene->setScale(0.57f);

	CCMenu *pmenu = CCMenu::create( ToMainScene, ToGameScene, NULL ); 
	pmenu->alignItemsHorizontallyWithPadding( 150 );
	pmenu->setPosition( ccp(510, 170) );
	this->addChild( pmenu );
} 
void CalculationScene::ToGameScene(CCObject *pSender)
{
	LogoScene::gameSound();
	CCScene* pScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
}
void CalculationScene::ToMainScene(CCObject *pSender)
{
	LogoScene::mainSound();
	CCScene* pScene = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
}
void CalculationScene::ToRankingScene(CCObject *pSender)
{
	if( strlen( UserName_str ) <= 12) {
		SwapRanking();   //Swap!! because of satisfying " (TotalScore>=Ranking_Score[9]) "

		CCScene* pScene = RankingScene::scene();
		CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create( 0.5f, pScene ) );
	} 
	else if( FaultUserCount == 0 ) {
		FaultUserCount++;
		userInputFault = CCMenuItemImage::create( "Images/calculation/enterWithCaution.png", "Images/calculation/enterWithCaution.png", this, menu_selector( CalculationScene::FaultUserName ) );
		userInputFaultMenu = CCMenu::create( userInputFault, NULL );
		userInputFaultMenu->setPosition( ccp(500, 250) );
		this->addChild( userInputFaultMenu );
	}
}

void CalculationScene::SwapRanking()
{ 
	int tempNum = 9;
	int count = 9;

	while(1) {
		if( TotalScore<ranking_Score[tempNum] || tempNum == -1 ) break; 
		tempNum--;
	} 

	for( ; count > tempNum + 1; count-- ) {
		ranking_Score[count] = ranking_Score[count - 1];
		strcpy(Ranking_Info[count], Ranking_Info[count - 1]);
	}

	ranking_Score[tempNum + 1] = TotalScore;
	Ranking_Info[tempNum + 1] = UserName_str;
	int i = 0;
	for( ranking = nodeRankings.child("Ranking"); ranking; ranking = ranking.next_sibling("Ranking"), i++ ) {
		ranking.child("Name").text().set(Ranking_Info[i]); 
		ranking.child("Score").text().set(ranking_Score[i]);
	}

	xmlDoc.save_file("data/AbilityData.xml.txt", "\t", 1U, encoding_utf8);
}

void CalculationScene::FaultUserName(CCObject *pSender)
{
	this->removeChild( userInputFaultMenu, true );
	FaultUserCount--;
} 
void CalculationScene::editBoxEditingDidBegin(cocos2d::extension::CCEditBox * editBox)
{
}
void CalculationScene::editBoxEditingDidEnd(cocos2d::extension::CCEditBox * editBox)
{
}
void CalculationScene::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string& text)
{
	strcpy( this->UserName_str, text.c_str() );      
	nodeResult.child("UserName").text().set(this->UserName_str);				//Return 할때 정보저장해준다.
	xmlDoc.save_file( "data/AbilityData.xml.txt" );								//The name of Username into Variable
}
void CalculationScene::editBoxReturn(cocos2d::extension::CCEditBox*editBox)
{
}
 