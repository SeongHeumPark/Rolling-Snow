#ifndef __CALCULATION_SCENE_H__
#define __CALCULATION_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "GameScene.h" 
#include "pugixml.hpp" // To Transfer the informations about score each Scene
#include "cocos-ext.h" // To use input of User
#include "RankingScene.h"

using namespace cocos2d;
using namespace pugi;

class CalculationScene : public CCLayerColor, public cocos2d::extension::CCEditBoxDelegate //After the game, it's SCENE calculate last score
{
public:

	CalculationScene();
	~CalculationScene();
    virtual bool init();  
    static CCScene* scene();

    CREATE_FUNC(CalculationScene);

	void ToGameScene(CCObject *pSender);
	void ToMainScene(CCObject *pSender);
	void ToRankingScene(CCObject *pSender);
	void SwapRanking();

	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;
	xml_node nodeTotalScore;
	xml_node nodeRankings;
	xml_node ranking;
	double EndTime;
	int TotalScore;
	int TotalMoney;

	char temptotalscore[20];
	/*About Input of User*/
	 
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox * editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox * editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox*editBox);
	
	void backGround();
	void takeRank();
	void notakeRank();
	void xml_Info(); 
	//랭킹 변수와 사용자 입력변수
	cocos2d::extension::CCEditBox * userName;
    char UserName_str[100];
	char* Ranking_Info[10]; 
	char Ranking_Info_2[10][10];  
	int ranking_Score[10];
	//사용자가 입력을 잘못했을 경우에 관한 변수
	CCMenuItemImage *userInputFault;
	CCMenu * userInputFaultMenu;
	void FaultUserName( CCObject *pSender);
	int FaultUserCount;

};

#endif