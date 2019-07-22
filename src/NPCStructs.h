#ifndef NPC_STRUCTS
#define NPC_STRUCTS

struct sPatrolNode {
 D3DXVECTOR3 pos;
 int		 index;
};


enum Status{
 STATIONARY,
 IDLE,
 BLOCKED,
 WALKING,
 FOLLOW,
 FOLLOW_IDLE
};

enum EntityType{
 DOCTOR,
 ROBBERY,
 BAT_ATTACK,
 SWORD_FIGHT,
 BEAT_UP,
 VINCENT_SUICIDE,
 CLERK,
 INSPECTOR,
 WITNESS,
 JANE,
 VINCENT,
 ANITA,
 GUARD,
 COP = 12
};

struct sBoundingBox {
  float height, width, length;

  sBoundingBox(EntityType type){
    switch(type){
      case ROBBERY:
        height = 38; width = 46; length = 24;
		break;
	  case BAT_ATTACK:
        height = 42; width = 45; length = 50;
		break;
	  case SWORD_FIGHT:
        height = 40; width = 30; length = 85;
		break;
	  case BEAT_UP:
        height = 41; width = 39; length = 26;
		break;
	
      default:
		height = 36; width = length = 13;
		break;
    }
    
  }	
};

struct sFileName {
  LPCWSTR fileName;
  int	  numEntities;
};

#endif