#ifndef __BOUNS_SCENE_H__
#define __BOUNS_SCENE_H__

#include "cocos2d.h"
#include "pugixml.hpp"

#define _MAXNUM 1000

using namespace cocos2d;
using namespace pugi;

class BonusScene : public CCLayerColor
{
public :
	/* 생성자, 소멸자 */
	BonusScene();
	~BonusScene();

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(BonusScene);

	/* XML */
	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;
	xml_node itemNode;
	xml_node itemsNode;

	/* 배경, 캐릭터, 아이템 생성 */
	CCSprite* bonusField[5];
	std::string scoreItemName[3][4];								// 점수 아이템 이미지 이름
	static float itemPosition_x[_MAXNUM];
	static float itemPosition_y[_MAXNUM];
	static int item_code[_MAXNUM];
	float backgroundSpeed;
	float ItemSpeed;
	int waitTime;

	void initializeFileNameNumber();								// 길과 아이템, 장애물 파일을 지정하는 함수
	void createBackground();
	void addCharacter();
	void createItem();
	void stopActionAllObjectAndRedefine( int, float, float );		// 배경, 장애물, 아이템 모든 액션 중지 및 재지정
	CCAction* setSpeed( float );
	void startBounsScene( float );
	
	/* 아이템 충돌 체크 */
	CCSprite* item[_MAXNUM];										// 아이템 배열
	bool checkEatItem[_MAXNUM];										// 아이템 부딪혔거나 캐릭터 뒤에 있을 경우
	
	void itemCollision( float );									// 아이템 충돌 체크

	/* 아이템 획득 시 효과 */
	int boostWaitTime;												// 부스터 지속 시간
	int magneticWaitTime;											// 자석 지속 시간
	int itemCount;													// 아이템 갯수 XML
	void selectedItemByTag( int );									// 아이템 분류
	void eatScoreItem();											// 점수 획득
	void eatCoinItem();												// 코인 획득

	/* 터치 */
	int jumpCount;													// 점프 카운터

	virtual void onEnter();											// 시작
	virtual void onExit();											// 종료
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );				// 터치 시작
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );			// 터치 취소
	void ccTouchPositionCheck( float );								// 터치(점프) 시 캐릭터 위치 체크

	float x;
};

#endif