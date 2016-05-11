#pragma once

#include "cocos2d.h"
#include "pugixml.hpp"

using namespace cocos2d;
using namespace pugi;

class Character : public cocos2d::CCSprite
{
private:
	/* ĳ���� ���� ���� */
	static Character* snowMan;							// ���� ĳ���� ����
	static CCTexture2D *text;							// ĳ���� action image ����
	float initPositionX;								// ĳ���� ���� ��ġ x��ǥ 
	float initPositionY;								// ĳ���� ���� ��ġ y��ǥ

	/* ������ ��ġ �� �ð� ���� */
	int scoreLevel;										// ���� ��ġ
	float sizeUpLevel;									// ũ�� ���� ��ġ
	float playTimeLevel;								// �ð� ���� ��ġ
	float boostTime;									// �ν��� ���� �ð�
	float magneticTime;									// �ڼ� ���� �ð�
	float unbeatableTime;								// ���� ���� �ð�
	float bonusTime;									// ���ʽ� ���� �ð�

	/* ���� ��� ���� */
public:
	/* ������, �Ҹ��� */
	Character();										// ������
	~Character();										// �Ҹ���

	/* ĳ���� ���� �� �ൿ ���� */
	static Character* GetSingleTone();					// singletone ��ȯ
	static Character* createWithType( const char* );	// 
	static void createCharacter();						// ĳ���� ����
	void doRun();										// ĳ���� �޸���
	void doJump();										// ĳ���� 1�� ����
	void doDoubleJump();								// ĳ���� 2�� ����
	void JumpEnd();										// ĳ���� 1�� ���� ��
	void DoubleJumpEnd();								// ĳ���� 2�� ���� ��
	void doCollision();									// ��ֹ� �浹 �� �ൿ
	void collisionEnd();
	
	/* ĳ���� �ʱ� ��ġ ���� */
	void setInitPositionX( float );						// ĳ���� ���� ��ġ x��ǥ ����
	float getInitPositionX();							// ĳ���� ���� ��ġ x��ǥ ����
	void setInitPositionY( float );						// ĳ���� ���� ��ġ y��ǥ ����
	float getInitPositionY();							// ĳ���� ���� ��ġ y��ǥ ����

	/* �������� ����Ǵ� ������ ��ġ */
	void setScoreLevel( int );							// ���� ������ ��ġ ����
	int getScoreLevel();								// ���� ������ ��ġ ����
	void setSizeUpLevel( float );						// ũ�� ���� ������ ��ġ ����
	float getSizeUpLevel();								// ũ�� ���� ������ ��ġ ����
	void setPlayTimeLevel( float );						// �ð� ���� ������ ��ġ ����
	float getPlayTimeLevel();							// �ð� ���� ������ ��ġ ����
	void setBoostTime( float );							// �ν��� ������ ���� �ð� ����
	float getBoostTime();								// �ν��� ������ ���� �ð� ����
	void setMagneticTime( float );						// �ڼ� ������ ���� �ð� ����
	float getMagneticTime();							// �ڼ� ������ ���� �ð� ����
	void setUnbeatableTime( float );					// ���� ������ ���� �ð� ����
	float getUnbeatableTime();							// ���� ������ ���� �ð� ����
	void setBonusTime( float );							// ���ʽ� ������ ���� �ð� ����
	float getBonusTime();								// ���ʽ� ������ ���� �ð� ����
	
	/* XML ���� */
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
 