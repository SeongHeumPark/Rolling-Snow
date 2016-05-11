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
	/* ������, �Ҹ��� */
	StoreScene();												// ������
	~StoreScene();												// �Ҹ���

	/* CCLayerColor Override */
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(StoreScene);

	/* Store Scene ���� */
	CCSprite *background;										// ���
	CCSprite *panel;											// ���� �г�
	CCSprite* sceneName;										// scene �̸�
	CCMenuItemImage* backButton;									// back ��ư
	CCMenu* back;												// back ��ư �޴�
	
	void createStoreLayer();									// Store Layer ����
	void xmlRead();												// xml ���� �б�
	void xmlWrite();											// xml ���� ����
	void returnMenu( CCObject * );								// back ��ư Ŭ�� �� ȿ��

	/* ��ũ�� �� */
	CCLayerColor* scrollLayer;									// ��ũ�� ���̾�
	CCScrollView *scrollView;									// ��ũ�� �� ����

	void initializeScrollView();								// ��ũ�� �� ����
	void scrollViewDidScroll( CCScrollView * );					// ��ũ��
	void scrollViewDidZoom( CCScrollView * );					// ��

	/* ������ ��������Ʈ ���� */
	CCSprite* scoreItem;										// ���� ������ ��������Ʈ
	CCSprite* sizeUpItem;										// ũ�� ���� ������ ��������Ʈ
	CCSprite* playTimeItem;										// ���� ���� ������ ��������Ʈ
	CCSprite* boostItem;										// �ν��� ������ ��������Ʈ
	CCSprite* magneticItem;										// �ڼ� ������ ��������Ʈ
	CCSprite* unbeatableItem;									// ���� ������ ��������Ʈ
	CCSprite* bonusItem;										// ���ʽ� ������ ��������Ʈ

	void createItemSprite();									// ������ ��������Ʈ ���� �Լ�

	/* ������ ���� ���� */
	CCSprite* scoreExplain;										// ���� ������ ���� ��������Ʈ
	CCSprite* sizeUpExplain;									// ũ�� ���� ������ ���� ��������Ʈ
	CCSprite* playTimeExplain;									// ���� ���� ������ ���� ��������Ʈ
	CCSprite* boostExplain;										// �ν��� ������ ���� ��������Ʈ
	CCSprite* magneticExplain;									// �ڼ� ������ ���� ��������Ʈ
	CCSprite* unbeatableExplain;								// ���� ������ ���� ��������Ʈ
	CCSprite* bonusExplain;										// ���ʽ� ������ ���� ��������Ʈ

	void createExplainItem();									// ������ ���� ���� �Լ�
	
	/* ���� �� ���̺� ���� */
	int TOTALMONEY;												// ���� ��ü �ݾ�
	int moneyData[7];											// score���� bonus���� ���׷��̵� �ݾ� �迭
	char moneyArray[8][10];										// �Ʒ� Label�� �ֱ� ���� �迭
	CCLabelTTF* totalMoney;										// ��ü �ݾ� ���̺�
	CCLabelTTF* scoreMoney;										// ���� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* sizeUpMoney;									// ũ�� ���� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* playTimeMoney;									// �ð� ���� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* boostMoney;										// �ν��� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* magneticMoney;									// �ڼ� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* unbeatableMoney;								// ���� ������ ���׷��̵� ��� ���̺�
	CCLabelTTF* bonusMoney;										// ���ʽ� ������ ���׷��̵� ��� ���̺�

	void inputMoneyData();										// int�� �����͵��� string���� ��ȯ�Ͽ� ���� ����
	void createMoneyLabel();									// Total money�� �����۵��� ���׷��̵� ��� ���̺� ����

	/* ������ ���� ���̺� ���� */
	int levelData[7];											// score���� bonus���� ���� ������ �迭
	char levelArray[7][10];										// �Ʒ� Label�� �ֱ� ���� �迭
	CCLabelTTF* scoreLevel;										// ���� ������ ���� ���̺�
	CCLabelTTF* sizeUpLevel;									// ũ�� ���� ������ ���� ���̺�
	CCLabelTTF* playTimeLevel;									// �ð� ���� ������ ���� ���̺�
	CCLabelTTF* boostLevel;										// �ν��� ������ ���� ���̺�
	CCLabelTTF* magneticLevel;									// �ڼ� ������ ���� ���̺�
	CCLabelTTF* unbeatableLevel;								// ���� ������ ���� ���̺�
	CCLabelTTF* bonusLevel;										// ���ʽ� ������ ���� ���̺�

	void inputLevelData();										// int�� �����͵��� string���� ��ȯ�Ͽ� ���� ����
	void createItemLevelLabel();								// �����۵��� ���� ���̺� ����

	/* ������ ���׷��̵� �޴� ���� �� ���� */
	int maxLevel[_MAXSTOREITEM];
	CCMenuItemImage* buttonImage[_MAXSTOREITEM];							// ���׷��̵� ��ư �̹��� �迭
	CCMenu* button[7];											// ���׷��̵� ��ư �޴� �迭

	void createUpgradeMenu();									// ���׷��̵� ��ư �޴� ����
	void upgradeManager( CCObject * );							// ������ ���׷��̵带 �����ϴ� �Լ�
	void subtractTotalMoney( int );								// Total money�� ����ϴ� �Լ�
	void scoreItemUpgrade();									// ���� ������ ���׷��̵� �Լ�
	void sizeUpItemUpgrade();									// ũ�� ���� ������ ���׷��̵� �Լ�
	void playTimeItemUpgrade();									// �ð� ���� ������ ���׷��̵� �Լ�
	void boostItemUpgrade();									// �ν��� ������ ���׷��̵� �Լ�
	void magneticItemUpgrade();									// �ڼ� ������ ���׷��̵� �Լ�
	void unbeatableItemUpgrade();								// ���� ������ ���׷��̵� �Լ�
	void bonusItemUpgrade();									// ���ʽ� ������ ���׷��̵� �Լ�
	
	/* XML ���� */
	xml_document xmlDoc;
	xml_node nodeResult;
	xml_node nodeItemAbility;

	/* ��ġ */
	virtual void onEnter();										// ����
	virtual void onExit();										// ����
	virtual bool ccTouchBegan( CCTouch *, CCEvent * );			// ��ġ ����
	virtual void ccTouchCancelled( CCTouch *, CCEvent * );		// ��ġ ���
};

#endif