#pragma once
#ifndef _OBJ_HOLDER
#define _OBJ_HOLDER

#include "Artifact.h"
#include "Spaceship.h"
#include "NumEater.h"
#include "FlyingCreature.h"
//Will be used for the container of the objects
class ObjectHolder {
	Object *data;


public:
	ObjectHolder(Object *_data = nullptr) : data(_data) {}
	static ObjectHolder CreateNewArtifact(const Point &p) { 
	
		return new Artifact(p);
	}
	ObjectHolder(const ObjectHolder&) = delete;
	ObjectHolder& operator=(const ObjectHolder&) = delete;
	static ObjectHolder CreateNewFlyingCreature(const Point &p,Symbols symbol, Color color,Direction dir) { return new FlyingCreature(p,symbol, color,dir); }
	static ObjectHolder CreateNewNumEater(const Point &p) { return new NumEater(p); }
	bool operator==(const ObjectHolder &object) { return data == object.data; }
	//Move constructor for RValue , we only need R value ctor
	ObjectHolder(ObjectHolder&&obj) { 
	
		data = obj.data;
		obj.data = nullptr;
	}
	
	void Move() { data->Move(); }
	bool Died() { return data->GetMyVolume() == 0; }
	~ObjectHolder() { delete data;}
};

#endif
