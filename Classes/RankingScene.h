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
	/* ������, �Ҹ��� */
	RankingScene();													// ������
	~RankingScene();												// �Ҹ���

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(RankingScene);

	/* ���Ϸ� ���� Ranking ������ ���� */
	std::string rankingData[_MAX][2];								// ��ŷ ������
	void initializeRankingLabel();									// ��ŷ ������ ����

	/* Ranking Scene ���� */
	CCSprite* background;											// ���
	CCSprite* panel;												// ���� �г�
	CCSprite* sceneName;											// scene �̸�
	CCMenuItemImage* backButton;										// back ��ư font
	CCMenu* back;													// back ��ư �޴�

	void createRankingLayer();										// Ranking Layer ����
	void returnMenu( CCObject * );									// back ��ư Ŭ���� ȿ��
	
	/* ��ũ�� �� */
	CCArray* data;													// ��ũ�� �信 ���� ������ �迭
	CCLayerColor* scrollLayer;										// ��ũ�� �� ���̾� ����
	CCScrollView *scrollView;										// ��ũ�� �� ����
	
	void initializeScrollView();									// ��ũ�� �� ����
	void scrollViewDidScroll( CCScrollView * );						// ��ũ��
	void scrollViewDidZoom( CCScrollView * );						// ��

	/* ��ġ */
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );
	virtual void ccTouchMoved( CCTouch *, CCEvent * );
	virtual void ccTouchEnded( CCTouch *, CCEvent * );
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );

	//XML���� 
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