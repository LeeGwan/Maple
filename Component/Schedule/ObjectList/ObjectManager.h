#pragma once
#include"../../../pch.h"
#include"ObjectList.h"

class ObjectManager
{
private:
	int ObjCount;
	int MaxCount = 10;
	double DeleteTime = 0.0;
	CRITICAL_SECTION cs;
private:
	vector<unique_ptr<RoutineObject>> ObjectManagers;//����ȭ�� �ּҰ��� EachImageRender�� �������ִ´�
public:
	ObjectManager();
	~ObjectManager();
	void AddObjectManager(const string& Find_Name);
	ObjectClass* Get_Find_obj(const string& Find_Name);
	void Create_Object();
	vector<ObjectClass*>  Get_Find_Monster();
	vector<ObjectClass*> Get_Find_Object();
	void Check_Manager(double DeltaTime);
	void Remove_NullObj();

	void Update();

};