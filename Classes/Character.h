#pragma once

#include "cocos2d.h"
#include "pugixml.hpp"

using namespace cocos2d;
using namespace pugi;

class Character : public cocos2d::CCSprite
{
private:
	/* 캐릭터 관련 변수 */
	static Character* snowMan;							// 정적 캐릭터 변수
	static CCTexture2D *text;							// 캐릭터 action image 변수
	float initPositionX;								// 캐릭터 최초 위치 x좌표 
	float initPositionY;								// 캐릭터 최초 위치 y좌표

	/* 아이템 수치 및 시간 변수 */
	int scoreLevel;										// 점수 수치
	float sizeUpLevel;									// 크기 증가 수치
	float playTimeLevel;								// 시간 증가 수치
	float boostTime;									// 부스터 유지 시간
	float magneticTime;									// 자석 유지 시간
	float unbeatableTime;								// 무적 유지 시간
	float bonusTime;									// 보너스 유지 시간

	/* 상태 출력 변수 */
public:
	/* 생성자, 소멸자 */
	Character();										// 생성자
	~Character();										// 소멸자

	/* 캐릭터 생성 및 행동 지정 */
	static Character* GetSingleTone();					// singletone 반환
	static Character* createWithType( const char* );	// 
	static void createCharacter();						// 캐릭터 생성
	void doRun();										// 캐릭터 달리기
	void doJump();										// 캐릭터 1단 점프
	void doDoubleJump();								// 캐릭터 2단 점프
	void JumpEnd();										// 캐릭터 1단 점프 끝
	void DoubleJumpEnd();								// 캐릭터 2단 점프 끝
	void doCollision();									// 장애물 충돌 시 행동
	void collisionEnd();
	
	/* 캐릭터 초기 위치 지정 */
	void setInitPositionX( float );						// 캐릭터 최초 위치 x좌표 지정
	float getInitPositionX();							// 캐릭터 최초 위치 x좌표 접근
	void setInitPositionY( float );						// 캐릭터 최초 위치 y좌표 지정
	float getInitPositionY();							// 캐릭터 최초 위치 y좌표 접근

	/* 상점에서 적용되는 아이템 수치 */
	void setScoreLevel( int );							// 점수 아이템 수치 지정
	int getScoreLevel();								// 점수 아이템 수치 접근
	void setSizeUpLevel( float );						// 크기 증가 아이템 수치 지정
	float getSizeUpLevel();								// 크기 증가 아이템 수치 접근
	void setPlayTimeLevel( float );						// 시간 증가 아이템 수치 지정
	float getPlayTimeLevel();							// 시간 증가 아이템 수치 접근
	void setBoostTime( float );							// 부스터 아이템 유지 시간 지정
	float getBoostTime();								// 부스터 아이템 유지 시간 접근
	void setMagneticTime( float );						// 자석 아이템 유지 시간 지정
	float getMagneticTime();							// 자석 아이템 유지 시간 접근
	void setUnbeatableTime( float );					// 무적 아이템 유지 시간 지정
	float getUnbeatableTime();							// 무적 아이템 유지 시간 접근
	void setBonusTime( float );							// 보너스 아이템 유지 시간 지정
	float getBonusTime();								// 보너스 아이템 유지 시간 접근
	
	/* XML 변수 */
	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;

	int meter;
	int coin;
	int score;
	int time;
	bool bonusState;
	float timeBarScale;
};
 