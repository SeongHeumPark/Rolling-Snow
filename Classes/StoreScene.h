#ifndef __STORE_SCENE_H__
#define __STORE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "pugixml.hpp"
 
#define _MAXSTOREITEM 7

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace pugi;

class StoreScene : public CCLayerColor, public CCScrollViewDelegate
{
public :
	/* 생성자, 소멸자 */
	StoreScene();												// 생성자
	~StoreScene();												// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(StoreScene);

	/* Store Scene 구성 */
	CCSprite *background;										// 배경
	CCSprite *panel;											// 나무 패널
	CCSprite* sceneName;										// scene 이름
	CCMenuItemImage* backButton;									// back 버튼
	CCMenu* back;												// back 버튼 메뉴
	
	void createStoreLayer();									// Store Layer 생성
	void xmlRead();												// xml 파일 읽기
	void xmlWrite();											// xml 파일 쓰기
	void returnMenu( CCObject * );								// back 버튼 클릭 시 효과

	/* 스크롤 뷰 */
	CCLayerColor* scrollLayer;									// 스크롤 레이어
	CCScrollView *scrollView;									// 스크롤 뷰 변수

	void initializeScrollView();								// 스크롤 뷰 생성
	void scrollViewDidScroll( CCScrollView * );					// 스크롤
	void scrollViewDidZoom( CCScrollView * );					// 줌

	/* 아이템 스프라이트 생성 */
	CCSprite* scoreItem;										// 점수 아이템 스프라이트
	CCSprite* sizeUpItem;										// 크기 증가 아이템 스프라이트
	CCSprite* playTimeItem;										// 점수 증가 아이템 스프라이트
	CCSprite* boostItem;										// 부스터 아이템 스프라이트
	CCSprite* magneticItem;										// 자석 아이템 스프라이트
	CCSprite* unbeatableItem;									// 무적 아이템 스프라이트
	CCSprite* bonusItem;										// 보너스 아이템 스프라이트

	void createItemSprite();									// 아이템 스프라이트 생성 함수

	/* 아이템 설명 생성 */
	CCSprite* scoreExplain;										// 점수 아이템 설명 스프라이트
	CCSprite* sizeUpExplain;									// 크기 증가 아이템 설명 스프라이트
	CCSprite* playTimeExplain;									// 점수 증가 아이템 설명 스프라이트
	CCSprite* boostExplain;										// 부스터 아이템 설명 스프라이트
	CCSprite* magneticExplain;									// 자석 아이템 설명 스프라이트
	CCSprite* unbeatableExplain;								// 무적 아이템 설명 스프라이트
	CCSprite* bonusExplain;										// 보너스 아이템 설명 스프라이트

	void createExplainItem();									// 아이템 설명 생성 함수
	
	/* 게임 돈 레이블 생성 */
	int TOTALMONEY;												// 게임 전체 금액
	int moneyData[7];											// score부터 bonus까지 업그레이드 금액 배열
	char moneyArray[8][10];										// 아래 Label에 넣기 위한 배열
	CCLabelTTF* totalMoney;										// 전체 금액 레이블
	CCLabelTTF* scoreMoney;										// 점수 아이템 업그레이드 비용 레이블
	CCLabelTTF* sizeUpMoney;									// 크기 증가 아이템 업그레이드 비용 레이블
	CCLabelTTF* playTimeMoney;									// 시간 증가 아이템 업그레이드 비용 레이블
	CCLabelTTF* boostMoney;										// 부스터 아이템 업그레이드 비용 레이블
	CCLabelTTF* magneticMoney;									// 자석 아이템 업그레이드 비용 레이블
	CCLabelTTF* unbeatableMoney;								// 무적 아이템 업그레이드 비용 레이블
	CCLabelTTF* bonusMoney;										// 보너스 아이템 업그레이드 비용 레이블

	void inputMoneyData();										// int형 데이터들을 string으로 변환하여 집어 넣음
	void createMoneyLabel();									// Total money와 아이템들의 업그레이드 비용 레이블 생성

	/* 아이템 레벨 레이블 생성 */
	int levelData[7];											// score부터 bonus까지 레벨 데이터 배열
	char levelArray[7][10];										// 아래 Label에 넣기 위한 배열
	CCLabelTTF* scoreLevel;										// 점수 아이템 레벨 레이블
	CCLabelTTF* sizeUpLevel;									// 크기 증가 아이템 레벨 레이블
	CCLabelTTF* playTimeLevel;									// 시간 증가 아이템 레벨 레이블
	CCLabelTTF* boostLevel;										// 부스터 아이템 레벨 레이블
	CCLabelTTF* magneticLevel;									// 자석 아이템 레벨 레이블
	CCLabelTTF* unbeatableLevel;								// 무적 아이템 레벨 레이블
	CCLabelTTF* bonusLevel;										// 보너스 아이템 레벨 레이블

	void inputLevelData();										// int형 데이터들을 string으로 변환하여 집어 넣음
	void createItemLevelLabel();								// 아이템들의 레벨 레이블 생성

	/* 아이템 업그레이드 메뉴 생성 및 관리 */
	int maxLevel[_MAXSTOREITEM];
	CCMenuItemImage* buttonImage[_MAXSTOREITEM];							// 업그레이드 버튼 이미지 배열
	CCMenu* button[7];											// 업그레이드 버튼 메뉴 배열

	void createUpgradeMenu();									// 업그레이드 버튼 메뉴 생성
	void upgradeManager( CCObject * );							// 아이템 업그레이드를 관리하는 함수
	void subtractTotalMoney( int );								// Total money를 계산하는 함수
	void scoreItemUpgrade();									// 점수 아이템 업그레이드 함수
	void sizeUpItemUpgrade();									// 크기 증가 아이템 업그레이드 함수
	void playTimeItemUpgrade();									// 시간 증가 아이템 업그레이드 함수
	void boostItemUpgrade();									// 부스터 아이템 업그레이드 함수
	void magneticItemUpgrade();									// 자석 아이템 업그레이드 함수
	void unbeatableItemUpgrade();								// 무적 아이템 업그레이드 함수
	void bonusItemUpgrade();									// 보너스 아이템 업그레이드 함수
	
	/* XML 변수 */
	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;

	/* 터치 */
	virtual void onEnter();										// 시작
	virtual void onExit();										// 종료
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );			// 터치 시작
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );		// 터치 취소
};

#endif