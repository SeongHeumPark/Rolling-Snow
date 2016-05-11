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
	/* ������, �Ҹ��� */
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

	/* ���, ĳ����, ������ ���� */
	CCSprite* bonusField[5];
	std::string scoreItemName[3][4];								// ���� ������ �̹��� �̸�
	static float itemPosition_x[_MAXNUM];
	static float itemPosition_y[_MAXNUM];
	static int item_code[_MAXNUM];
	float backgroundSpeed;
	float ItemSpeed;
	int waitTime;

	void initializeFileNameNumber();								// ��� ������, ��ֹ� ������ �����ϴ� �Լ�
	void createBackground();
	void addCharacter();
	void createItem();
	void stopActionAllObjectAndRedefine( int, float, float );		// ���, ��ֹ�, ������ ��� �׼� ���� �� ������
	CCAction* setSpeed( float );
	void startBounsScene( float );
	
	/* ������ �浹 üũ */
	CCSprite* item[_MAXNUM];										// ������ �迭
	bool checkEatItem[_MAXNUM];										// ������ �ε����ų� ĳ���� �ڿ� ���� ���
	
	void itemCollision( float );									// ������ �浹 üũ

	/* ������ ȹ�� �� ȿ�� */
	int boostWaitTime;												// �ν��� ���� �ð�
	int magneticWaitTime;											// �ڼ� ���� �ð�
	int itemCount;													// ������ ���� XML
	void selectedItemByTag( int );									// ������ �з�
	void eatScoreItem();											// ���� ȹ��
	void eatCoinItem();												// ���� ȹ��

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