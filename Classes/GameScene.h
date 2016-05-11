#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <time.h>
#include "pugixml.hpp"

#define _MAXOBJ 2000
#define _MAXITEM 2000
#define _GAMEPLAYTIME 3600

using namespace cocos2d;
using namespace pugi;

class GameScene : public CCLayerColor
{
public :
	/* 생성자, 소멸자 */
	GameScene();													// 생성자
	~GameScene();													// 소멸자

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(GameScene); 

	/* XML */
	xml_document xmlDoc;
	xml_node ItemsAndObjects;
	xml_node Items;
	xml_node Objects;
	xml_node Item;
	xml_node Object;

	/* 배경, 캐릭터, 장애물, 아이템, 길 생성 */
	std::string roadFileName[10];									// 길 이미지 파일 이름 배열
	std::string objectItemFileName[2];								// 장애물, 아이템 파일 이름 배열
	std::string scoreItemName[3][4];								// 점수 아이템 이미지 이름
	CCSprite* baseField[10];										// 기본 필드
	CCSprite* sunField[10];											// 여름 필드
	CCSprite* snowField[10];										// 겨울 필드
	CCSprite* road[10];
	CCParallaxNode* backgroundNode[4];								// 배경 패럴랙스 노드
	int objCount;													// 장애물 수
	int itemCount;													// 아이템 수
	static float objPositionX[_MAXOBJ];								// 장애물 position x
	static float objPositionY[_MAXOBJ];								// 장애물 position y
	static float itemPositionX[_MAXITEM];							// 아이템 position x
	static float itemPositionY[_MAXITEM];							// 아이템 position y
	static int objCode[_MAXOBJ];									// 장애물 종류(code)
	CCSprite *sun;
	static int itemCode[_MAXITEM];									// 아이템 종류(code)
	float backgroundSpeed;											// 배경화면 속도
	float objectItemSpeed;											// 장애물, 아이템 속도

	void initializeFileNameNumber();								// 길과 아이템, 장애물 파일을 지정하는 함수
	void createBackgroundPallaxNode();								// 배경 생성
	void createRoadAndParsingXML();									// 길 생성 및 xml parsing
	void addCharacter();											// 캐릭터 생성
	void readObjectAndItemInfo();									// xml에서 장애물, 아이템 정보 읽기
	void createObject();											// 장애물 생성
	void createSunAnimation();
	void createGearObject( int );									// 장애물 중 톱니바퀴 애니매이션 생성
	CCAction* addActionToGear();									// 톱니바퀴 액션
	void createFireObject( int );									// 장애물 중 불똥 애니매이션 생성
	CCAction* addActionToFire();									// 불 액션
	void createItem();												// 아이템 생성
	void stopActionAllObjectAndRedefine( int, float, float );		// 배경, 장애물, 아이템 모든 액션 중지 및 재지정
	CCAction* setSpeed( float );									// 배경, 장애물, 아이템 속도 지정
	
	/* 각종 상태 생성 */
	float timeBarScalePerSecond;									// 감소되는 타임 바 비율
	int currentTime;												// 현재 시간
	CCLabelTTF* currentTimeBarName;									// TIME 이름 레이블
	CCSprite* currentTimeBarBackground;								// 타임 바 배경 스프라이트
	CCSprite* currentTimeBar;										// 타임 바 스프라이트
	float characterScalePerMeter;									// 늘어날 스케일 비율
	CCSprite* maxScaleBackground;									// 스케일 배경 스프라이트
	CCSprite* currentScale;											// 스케일 스프라이트
	CCLabelTTF* scoreLabel;											// 점수 display 레이블
	CCLabelTTF* coinLabel;											// 코인 display 레이블
	int meterStateTime;												//
	int meter;														// 거리 증가
	int meterSpeed;													// 미터계 스피드
	CCLabelTTF* meterLabel;											// 거리 display 레이블

	void createTimeBarState();										// 타임 바 생성
	void timeBarSchedule( float );									// 타임 바 display
	void createScaleState();										// 크기 상태 생성
	void scaleSchedule( float );									// 크기 상태 display
	void createScoreState();										// 점수 상태 생성
	void scoreSchedule( float );									// 점수 상태 display
	void createCoinState();											// 코인 상태 생성
	void coinSchedule( float );										// 코인 상태 display
	void createMeterState();										// 거리 상태 생성
	void meterSchedule( float );									// 거리 상태 display
	void stopAllSchedule();											// 모든 스케줄 중지

	/* 길, 장애물, 아이템 충돌 체크 */
	CCSprite* object[_MAXOBJ];										// 장애물 배열
	bool checkColObj[_MAXOBJ];										// 장애물을 부딪혔거나 캐릭터 뒤에 있을 경우
	CCSprite* item[_MAXITEM];										// 아이템 배열
	bool checkEatItem[_MAXITEM];									// 아이템 부딪혔거나 캐릭터 뒤에 있을 경우
	int colEffectTime;
	int number[10][8];
	int roadInfo[10][8];

	void initialinzeRoadInfo();
	void roadCollision( float );									// 길 충돌 체크
	void roadColEffect( float );									// 길 충돌 효과
	void objectCollision( float );									// 장애물 충돌 체크
	void objectColEffect( float );									// 장매물 충돌 효과
	void itemCollision( float );									// 아이템 충돌 체크

	/* 각종 아이템 획득 시 효과 */
	int boostWaitTime;												// 부스터 지속 시간
	int magneticWaitTime;											// 자석 지속 시간
	bool unbeatableState;											// 무적 상태
	int unbeatableWaitTime;											// 무적 지속 시간
	int fieldWaitTime;												// 필드 지속 시간

	void selectedItemByTag( int );									// 아이템 분류
	void eatScoreItem();											// 점수 획득
	void eatCoinItem();												// 코인 획득
	void eatSizeUpItem();											// 크기 증가 획득
	void eatTimeItem();												// 타임 획득
	void eatBoostItem();											// 부스터 획득
	void boostTime( float );										// 부스터 지속
	void eatUnbeatableItem();										// 무적 획득
	void unbeatableTime( float );									// 무적 지속
	void eatMagneticItem();											// 자석 획득
	void magneticTime( float );										// 자석 지속
	void eatRandomItem();											// 랜덤 획득
	void fieldTime( float );										// 필드 지속
	void transBounsScene();									// 보너스 이동
	void eatLargeTimeItem();										// 라지 타임 획득

	/* 스피드 증가 */
	int speedLevel;

	void speedUpByBackgroundPosition( float ); // 추후 수정

	/* 계산 씬 이동 */
	int waitGameOver;
	CCSprite *gameOver;

	void appearGameOver();
	void transCalculationScene( float );

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