#include "RankingScene.h"

using namespace cocos2d;
using namespace cocos2d::extension;

// 생성자, 소멸자
RankingScene::RankingScene()
{
}
RankingScene::~RankingScene()
{
}

// CCLayerColor Override
CCScene* RankingScene::scene()
{
	RankingScene* layer = RankingScene::create();
	CCScene* scene = CCScene::create();
	scene->addChild(layer);
	return scene;
}
bool RankingScene::init()
{
	if( !CCLayerColor::initWithColor( ccc4(255, 255, 255, 255) ) )
		return false;

	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/AbilityData.xml.txt");
	xml_parse_result result = xmlDoc.load_file(fileName.c_str());
	
	if(!result) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
		return false;
	}
	
	nodeResult =xmlDoc.child("Result");

	
	nodeRankings = nodeResult.child("nodeRankings");                                                   //Accept Ranking list
	Ranking = nodeRankings.child("Ranking");

	int i=0;
	for( Ranking = nodeRankings.child("Ranking"); Ranking; Ranking = Ranking.next_sibling("Ranking"), i++ )
	{
		Ranking_Info[i] =  (char*)Ranking.child("Name").text().get();								
		Ranking_Score[i] = Ranking.child("Score").text().as_int();
	}

	this->createRankingLayer();
	this->initializeScrollView();
	this->initializeRankingLabel();
	 
	return true;
}

// 파일로 부터 Ranking 데이터 적재
void RankingScene::initializeRankingLabel()
{
	CCString* grade;
	for(int i = 0; i < 10; i++) {
		char temp[10];
		if( strcmp( Ranking_Info[i], "\0" ) && Ranking_Score[i] == 0 ) sprintf( temp, "%c", ' ' );
		else														   sprintf( temp, "%d", Ranking_Score[i] );
		data = CCArray::create();
		data->addObject( CCString::create( Ranking_Info[i]) );
		data->addObject( CCString::create( temp ) );

		grade = CCString::createWithFormat("%02d.", i + 1);
		CCLabelTTF* gradeLabel = CCLabelTTF::create( grade->getCString(), "Helvetica", 64 );
		gradeLabel->setColor( ccBLACK );
		gradeLabel->setAnchorPoint( ccp(0, 0) );
		gradeLabel->setPosition( ccp(50, scrollLayer->getContentSize().height - (80 * (i + 1))) );
		scrollLayer->addChild( gradeLabel );

		double location = 0;
		CCObject* object;
		CCARRAY_FOREACH(data, object) {
			CCString* temp = (CCString *)object;
			CCLabelTTF* rankingLabel = CCLabelTTF::create( temp->getCString(), "Arial", 64 );
			rankingLabel->setColor( ccBLACK );
			rankingLabel->setAnchorPoint( ccp(1, 0) );
			rankingLabel->setPosition( ccp(400 + location, scrollLayer->getContentSize().height - (80 * (i + 1))) );
			scrollLayer->addChild( rankingLabel );
			location += 400;
		}
	}
}

// Ranking Scene 구성
void RankingScene::createRankingLayer()
{
	// background 삽입
	background = CCSprite::create( "Images/ranking/background.png" );
	background->setAnchorPoint( ccp(0, 0) );
	background->setPosition( ccp(0, 0) );
	this->addChild( background );

	// panel 삽입
	panel = CCSprite::create( "Images/ranking/panel.png" );
	panel->setPosition( ccp( CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2 ) );
	panel->setScale( 0.9f );
	background->addChild( panel );

	// scene name 추가
	sceneName = CCSprite::create( "Images/ranking/ranking.png");
	sceneName->setPosition( ccp(panel->getPosition().x, panel->getPosition().y + 220));
	panel->addChild( sceneName );

	// back 버튼 삽입
	backButton = CCMenuItemImage::create( "Images/ranking/exit_01.png", "Images/ranking/exit_02.png", this, menu_selector( RankingScene::returnMenu ) );
	CCMenu* back = CCMenu::create( backButton, NULL );
	back->setPosition( ccp( panel->getPosition().x + 95, panel->getPosition().y - 35 ) );
	back->setScale( 0.3f );
	panel->addChild( back, 5 );
}
void RankingScene::returnMenu( CCObject *pSender )
{
	CCScene *pScene = MainScene::scene(); 
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create( 0.5f, pScene ));
}

// 스크롤 뷰
void RankingScene::initializeScrollView()
{	
	scrollLayer = CCLayerColor::create( ccc4(255, 255, 255, 255) );
	scrollLayer->setOpacity( 0 );
	scrollLayer->setAnchorPoint( CCPointZero );
	scrollLayer->setPosition( panel->getPosition() );
	scrollLayer->setContentSize( CCSizeMake(panel->getContentSize().width, 800) );

	scrollView = CCScrollView::create();
	scrollView->retain();
	scrollView->setDirection( kCCScrollViewDirectionVertical );
	scrollView->setViewSize( CCSizeMake(panel->getContentSize().width * 0.9, panel->getContentSize().height * 0.75) );
	scrollView->setContentSize( scrollLayer->getContentSize() );
	scrollView->setPosition( ccp(panel->getPosition().x - 465, panel->getPosition().y - 325) );
	scrollView->setContainer( scrollLayer );
	scrollView->setDelegate( this );
	scrollView->setContentOffset( CCPointMake(0, -(scrollLayer->getContentSize().height * 0.4) + 35), false );
	panel->addChild( scrollView );
}
void RankingScene::scrollViewDidScroll( CCScrollView *view )
{
}
void RankingScene::scrollViewDidZoom( CCScrollView *view )
{
}

// 터치
void RankingScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( this, 1, true );
}
void RankingScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate( this );
	CCLayer::onExit();
}
bool RankingScene::ccTouchBegan( CCTouch *pTouch, CCEvent *event )
{
	return true;
}
void RankingScene::ccTouchMoved( CCTouch *pTouch, CCEvent *event )
{
}
void RankingScene::ccTouchEnded( CCTouch *pTouch, CCEvent *event )
{
}
void RankingScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *event )
{
}