#ifndef __RANKING_SCENE_H__
#define __RANKING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MainScene.h"
#include "pugixml.hpp"

#define _MAX 20

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace pugi;

USING_NS_CC;
USING_NS_CC_EXT;

class RankingScene : public CCLayerColor, public CCScrollViewDelegate
{
public :
	/* 생성자, 소멸자 */
	RankingScene();													// 생성자
	~RankingScene();												// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(RankingScene);

	/* 파일로 부터 Ranking 데이터 적재 */
	std::string rankingData[_MAX][2];								// 랭킹 데이터
	void initializeRankingLabel();									// 랭킹 데이터 삽입

	/* Ranking Scene 구성 */
	CCSprite* background;											// 배경
	CCSprite* panel;												// 나무 패널
	CCSprite* sceneName;											// scene 이름
	CCMenuItemImage* backButton;										// back 버튼 font
	CCMenu* back;													// back 버튼 메뉴

	void createRankingLayer();										// Ranking Layer 생성
	void returnMenu( CCObject * );									// back 버튼 클릭시 효과
	
	/* 스크롤 뷰 */
	CCArray* data;													// 스크롤 뷰에 얹일 데이터 배열
	CCLayerColor* scrollLayer;										// 스크롤 뷰 레이어 변수
	CCScrollView *scrollView;										// 스크롤 뷰 변수
	
	void initializeScrollView();									// 스크롤 뷰 생성
	void scrollViewDidScroll( CCScrollView * );						// 스크롤
	void scrollViewDidZoom( CCScrollView * );						// 줌

	/* 터치 */
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );
	virtual void ccTouchMoved( CCTouch *, CCEvent * );
	virtual void ccTouchEnded( CCTouch *, CCEvent * );
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );

	//XML변수 
	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;
	xml_node nodeTotalScore;
	xml_node nodeRankings;
	xml_node Ranking;
	char *Ranking_Info[10];
	int Ranking_Score[10];
	char *ScoreIntToAsc[10];
};

#endif