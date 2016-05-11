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
	/* ������, �Ҹ��� */
	GameScene();													// ������
	~GameScene();													// �Ҹ���

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

	/* ���, ĳ����, ��ֹ�, ������, �� ���� */
	std::string roadFileName[10];									// �� �̹��� ���� �̸� �迭
	std::string objectItemFileName[2];								// ��ֹ�, ������ ���� �̸� �迭
	std::string scoreItemName[3][4];								// ���� ������ �̹��� �̸�
	CCSprite* baseField[10];										// �⺻ �ʵ�
	CCSprite* sunField[10];											// ���� �ʵ�
	CCSprite* snowField[10];										// �ܿ� �ʵ�
	CCSprite* road[10];
	CCParallaxNode* backgroundNode[4];								// ��� �з����� ���
	int objCount;													// ��ֹ� ��
	int itemCount;													// ������ ��
	static float objPositionX[_MAXOBJ];								// ��ֹ� position x
	static float objPositionY[_MAXOBJ];								// ��ֹ� position y
	static float itemPositionX[_MAXITEM];							// ������ position x
	static float itemPositionY[_MAXITEM];							// ������ position y
	static int objCode[_MAXOBJ];									// ��ֹ� ����(code)
	CCSprite *sun;
	static int itemCode[_MAXITEM];									// ������ ����(code)
	float backgroundSpeed;											// ���ȭ�� �ӵ�
	float objectItemSpeed;											// ��ֹ�, ������ �ӵ�

	void initializeFileNameNumber();								// ��� ������, ��ֹ� ������ �����ϴ� �Լ�
	void createBackgroundPallaxNode();								// ��� ����
	void createRoadAndParsingXML();									// �� ���� �� xml parsing
	void addCharacter();											// ĳ���� ����
	void readObjectAndItemInfo();									// xml���� ��ֹ�, ������ ���� �б�
	void createObject();											// ��ֹ� ����
	void createSunAnimation();
	void createGearObject( int );									// ��ֹ� �� ��Ϲ��� �ִϸ��̼� ����
	CCAction* addActionToGear();									// ��Ϲ��� �׼�
	void createFireObject( int );									// ��ֹ� �� �Ҷ� �ִϸ��̼� ����
	CCAction* addActionToFire();									// �� �׼�
	void createItem();												// ������ ����
	void stopActionAllObjectAndRedefine( int, float, float );		// ���, ��ֹ�, ������ ��� �׼� ���� �� ������
	CCAction* setSpeed( float );									// ���, ��ֹ�, ������ �ӵ� ����
	
	/* ���� ���� ���� */
	float timeBarScalePerSecond;									// ���ҵǴ� Ÿ�� �� ����
	int currentTime;												// ���� �ð�
	CCLabelTTF* currentTimeBarName;									// TIME �̸� ���̺�
	CCSprite* currentTimeBarBackground;								// Ÿ�� �� ��� ��������Ʈ
	CCSprite* currentTimeBar;										// Ÿ�� �� ��������Ʈ
	float characterScalePerMeter;									// �þ ������ ����
	CCSprite* maxScaleBackground;									// ������ ��� ��������Ʈ
	CCSprite* currentScale;											// ������ ��������Ʈ
	CCLabelTTF* scoreLabel;											// ���� display ���̺�
	CCLabelTTF* coinLabel;											// ���� display ���̺�
	int meterStateTime;												//
	int meter;														// �Ÿ� ����
	int meterSpeed;													// ���Ͱ� ���ǵ�
	CCLabelTTF* meterLabel;											// �Ÿ� display ���̺�

	void createTimeBarState();										// Ÿ�� �� ����
	void timeBarSchedule( float );									// Ÿ�� �� display
	void createScaleState();										// ũ�� ���� ����
	void scaleSchedule( float );									// ũ�� ���� display
	void createScoreState();										// ���� ���� ����
	void scoreSchedule( float );									// ���� ���� display
	void createCoinState();											// ���� ���� ����
	void coinSchedule( float );										// ���� ���� display
	void createMeterState();										// �Ÿ� ���� ����
	void meterSchedule( float );									// �Ÿ� ���� display
	void stopAllSchedule();											// ��� ������ ����

	/* ��, ��ֹ�, ������ �浹 üũ */
	CCSprite* object[_MAXOBJ];										// ��ֹ� �迭
	bool checkColObj[_MAXOBJ];										// ��ֹ��� �ε����ų� ĳ���� �ڿ� ���� ���
	CCSprite* item[_MAXITEM];										// ������ �迭
	bool checkEatItem[_MAXITEM];									// ������ �ε����ų� ĳ���� �ڿ� ���� ���
	int colEffectTime;
	int number[10][8];
	int roadInfo[10][8];

	void initialinzeRoadInfo();
	void roadCollision( float );									// �� �浹 üũ
	void roadColEffect( float );									// �� �浹 ȿ��
	void objectCollision( float );									// ��ֹ� �浹 üũ
	void objectColEffect( float );									// ��Ź� �浹 ȿ��
	void itemCollision( float );									// ������ �浹 üũ

	/* ���� ������ ȹ�� �� ȿ�� */
	int boostWaitTime;												// �ν��� ���� �ð�
	int magneticWaitTime;											// �ڼ� ���� �ð�
	bool unbeatableState;											// ���� ����
	int unbeatableWaitTime;											// ���� ���� �ð�
	int fieldWaitTime;												// �ʵ� ���� �ð�

	void selectedItemByTag( int );									// ������ �з�
	void eatScoreItem();											// ���� ȹ��
	void eatCoinItem();												// ���� ȹ��
	void eatSizeUpItem();											// ũ�� ���� ȹ��
	void eatTimeItem();												// Ÿ�� ȹ��
	void eatBoostItem();											// �ν��� ȹ��
	void boostTime( float );										// �ν��� ����
	void eatUnbeatableItem();										// ���� ȹ��
	void unbeatableTime( float );									// ���� ����
	void eatMagneticItem();											// �ڼ� ȹ��
	void magneticTime( float );										// �ڼ� ����
	void eatRandomItem();											// ���� ȹ��
	void fieldTime( float );										// �ʵ� ����
	void transBounsScene();									// ���ʽ� �̵�
	void eatLargeTimeItem();										// ���� Ÿ�� ȹ��

	/* ���ǵ� ���� */
	int speedLevel;

	void speedUpByBackgroundPosition( float ); // ���� ����

	/* ��� �� �̵� */
	int waitGameOver;
	CCSprite *gameOver;

	void appearGameOver();
	void transCalculationScene( float );

	/* ��ġ */
	int jumpCount;													// ���� ī����

	virtual void onEnter();											// ����
	virtual void onExit();											// ����
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );				// ��ġ ����
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );			// ��ġ ���
	void ccTouchPositionCheck( float );								// ��ġ(����) �� ĳ���� ��ġ üũ

	float x;
};

#endif