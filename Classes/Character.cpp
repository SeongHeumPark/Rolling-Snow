#include"Character.h"

using namespace cocos2d;

Character* Character::snowMan;
CCTexture2D* Character::text;

Character::Character()
{
	this->bonusState = false;
	// XML Pasing
	std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/AbilityData.xml.txt");
	xml_parse_result result = xmlDoc.load_file(fileName.c_str());

	if( !result ) {
		CCLog("Error description %s ",result.description());
		CCLog("Error offset %d",result.offset);
	}

	// read xml
	nodeResult =xmlDoc.child("Result");
	nodeItemAbility = nodeResult.child("ItemAbility");
	this->setScoreLevel( nodeItemAbility.child("ScoreLevel").text().as_int() );
	this->setSizeUpLevel( nodeItemAbility.child("SizeUpLevel").text().as_float() );
	this->setPlayTimeLevel( nodeItemAbility.child("PlayTimeLevel").text().as_float() );
	this->setBoostTime( nodeItemAbility.child("BoostLevel").text().as_float() );
	this->setMagneticTime( nodeItemAbility.child("MagneticLevel").text().as_float() );
	this->setUnbeatableTime( nodeItemAbility.child("UnbeatableLevel").text().as_float() );
	this->setBonusTime( nodeItemAbility.child("BonusLevel").text().as_float() );
}
Character::~Character()
{
}

Character* Character::GetSingleTone()
{
	if( !snowMan ) Character::createCharacter();
	return snowMan;
}
Character* Character::createWithType( const char *pszFileName )
{
	Character *pCharacter = new Character();
	if( pCharacter && pCharacter->initWithFile( pszFileName ) ) {
		return pCharacter;
	}
	CC_SAFE_DELETE( pCharacter );
	return NULL;
}
void Character::createCharacter()
{
	text = CCTextureCache::sharedTextureCache()->addImage( "Images/character_animation.png" );
	CCAnimation* Animation = CCAnimation::create();
	Animation->setDelayPerUnit( 0.1f );
	 
	// 캐릭터 애니매이션 추가
	for( int i = 0; i < 4; i++ ) {
		int index = i;
		int rowIndex = 0;
		Animation->addSpriteFrameWithTexture(text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
	}

	// 캐릭터 생성
	snowMan = createWithType( "Images/character_animation.png" );
	snowMan->setAnchorPoint( ccp(0.3f, 0.1f) );
	snowMan->setInitPositionX( 150.0f );
	snowMan->setInitPositionY( 90.0f );
	snowMan->setScale( 0.2f );

	CCAnimate* Animate = CCAnimate::create( Animation );
	CCAction* actRep = CCRepeatForever::create( Animate );
	snowMan->runAction( actRep );
}
void Character::doRun()
{  
	CCAnimation* runAnimation = CCAnimation::create();
	runAnimation->setDelayPerUnit( 0.1f );

	// 달리기
	for( int i = 0; i < 4; i++ ) {
		int index = i;
		int rowIndex = 1;
		runAnimation->addSpriteFrameWithTexture(text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
	}

	// 액션 모두 정지
	Character::snowMan->stopAllActions();

	CCAnimate* runAnimate = CCAnimate::create( runAnimation );
	CCAction* actRep = CCRepeatForever::create( runAnimate );
	Character::snowMan->runAction( actRep );
}
void Character::doJump()
{
	CCActionInterval *Jump = CCJumpTo::create( 0.6f, ccp(initPositionX, initPositionY), 200, 1 );
	CCAnimation* jumpAnimation = CCAnimation::create();
 	jumpAnimation->setDelayPerUnit( 0.07f );

	// 점프
	for( int i = 0; i < 7; i++ ) {
		int index = i;
		int rowIndex = 2;
		jumpAnimation->addSpriteFrameWithTexture(text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
	}
	
	// 액션 모두 정지
	Character::snowMan->stopAllActions();
	 
	// perform the selector call
	CCAnimate* jumpAnimate = CCAnimate::create( jumpAnimation );
	CCCallFunc *callSelectorAction = CCCallFunc::create( this, callfunc_selector( Character::JumpEnd ) );
	Character::snowMan->runAction( jumpAnimate );
	CCFiniteTimeAction* seq = CCSequence::create( Jump, callSelectorAction, NULL );
	Character::snowMan->runAction( seq );
}
void Character::doDoubleJump()
{
	CCActionInterval *Jump = CCJumpTo::create( 0.6f, ccp(initPositionX, initPositionY), 250, 1 );
	CCAnimation* doubleJumpAnimation = CCAnimation::create();
	doubleJumpAnimation->setDelayPerUnit( 0.07f );

	// 2단 점프
	for( int i = 0; i < 9; i++ ) {
		int index = i;
		int rowIndex = 3;
		doubleJumpAnimation->addSpriteFrameWithTexture( text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
	}	 
	
	// 액션 모두 정지
	Character::snowMan->stopAllActions();
	
    // perform the selector call
	CCAnimate* doubleJumpAnimate = CCAnimate::create( doubleJumpAnimation );
	CCCallFunc *callSelectorAction = CCCallFunc::create( this, callfunc_selector( Character::DoubleJumpEnd ) );
	Character::snowMan->runAction( doubleJumpAnimate );
	CCFiniteTimeAction* seq = CCSequence::create( Jump, callSelectorAction, NULL );
	Character::snowMan->runAction( seq );
}
void Character::JumpEnd()
{
	doRun();
}
void Character::DoubleJumpEnd()
{
	CCAnimation* touchdownAnimation = CCAnimation::create();
	touchdownAnimation->setDelayPerUnit( 0.05f );

	// 착지
	for( int i = 0; i < 3; i++ ) {
		int index = i;
		int rowIndex = 4;
		touchdownAnimation->addSpriteFrameWithTexture( text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
	}

	// 액션 모두 정지
	if( snowMan->getPosition().y == snowMan->getInitPositionY() ) {
		Character::snowMan->stopAllActions();

		CCAnimate* touchdownAnimate = CCAnimate::create( touchdownAnimation );
		CCCallFunc* callSelectorAction = CCCallFunc::create( this, callfunc_selector( Character::collisionEnd ) );
		CCFiniteTimeAction* seq = CCSequence::create( touchdownAnimate, callSelectorAction, NULL );
		Character::snowMan->runAction( seq );
	}
}
void Character::doCollision()
{
	CCActionInterval* go = CCMoveTo::create( 0.1f, ccp(initPositionX, initPositionY) );
	CCAnimation* collisionAnimation = CCAnimation::create();
	collisionAnimation->setDelayPerUnit( 0.14f );

	// 충돌
	for( int j = 0; j < 2; j++) {
		for( int i = 4 ; i < 8; i++ ) {
			int index = i;
			int rowIndex = 0;
			collisionAnimation->addSpriteFrameWithTexture( text, CCRectMake( index * 600, rowIndex * 640, 600, 640 ) );
		}
	}

	// 액션 모두 정지
	Character::snowMan->stopAllActions();

	CCAnimate* collisionAnimate = CCAnimate::create( collisionAnimation );
	CCCallFunc* callSelectorAction = CCCallFunc::create( this, callfunc_selector( Character::collisionEnd ) );
	Character::snowMan->runAction( collisionAnimate );
	CCFiniteTimeAction* seq = CCSequence::create( go, callSelectorAction, NULL );
	Character::snowMan->runAction( seq );
}
void Character::collisionEnd()
{
	doRun();
}

void Character::setInitPositionX( float x )
{
	this->initPositionX = x;
	snowMan->setPositionX( this->initPositionX );
}
float Character::getInitPositionX()
{
	return this->initPositionX;
}
void Character::setInitPositionY( float y )
{
	this->initPositionY = y;
	snowMan->setPositionY( this->initPositionY );
}
float Character::getInitPositionY()
{
	return this->initPositionY;
}
void Character::setScoreLevel( int level )
{
	this->scoreLevel = level;
}
int Character::getScoreLevel()
{
	return this->scoreLevel;
}
void Character::setSizeUpLevel( float level )
{
	this->sizeUpLevel = level / 50;
}
float Character::getSizeUpLevel()
{
	return this->sizeUpLevel;
}
void Character::setPlayTimeLevel( float level )
{
	this->playTimeLevel = 120 + ( level * 60 );
}
float Character::getPlayTimeLevel()
{
	return this->playTimeLevel;
}
void Character::setBoostTime( float level )
{
	this->boostTime = 120 + ( level * 60 );
}
float Character::getBoostTime()
{
	return this->boostTime;
}
void Character::setMagneticTime( float level )
{
	this->magneticTime = 120 + ( level * 60 );
}
float Character::getMagneticTime()
{
	return this->magneticTime;
}
void Character::setUnbeatableTime( float level )
{
	this->unbeatableTime = 120 + ( level * 60 );
}
float Character::getUnbeatableTime()
{
	return this->unbeatableTime;
}
void Character::setBonusTime( float level )
{
	this->bonusTime = 120 + ( level * 60 );
}
float Character::getBonusTime()
{
	return this->bonusTime;
}