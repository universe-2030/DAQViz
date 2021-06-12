#pragma once

#include <gltools.h>	// OpenGL toolkit
#include <glframe.h>   // Frame Class
#include "glm.h"
#include "GL/glew.h"
#include "GL/freeglut_std.h"

class Hand {
public:
	Hand(GLFrame* obj);
	~Hand();

	void setJointIndex(int num);
	
	void WristFERotatePos(float pos);
	void WristRURotatePos(float pos);

	void ElbowFERotatePos(float pos);
	void ElbowIERotatePos(float pos);
	void ElbowAARotatePos(float pos);

	void ShoulderFERotatePos(float pos);
	void ShoulderIERotatePos(float pos);
	void ShoulderAARotatePos(float pos);

	void ThumbRotatePos(float pos1, float pos2);
	void IndexRotatePos(float pos1, float pos2, float pos3);
	void MiddleRotatePos(float pos1, float pos2, float pos3);
	void RingRotatePos(float pos1, float pos2, float pos3);
	void PinkyRotatePos(float pos1, float pos2, float pos3);

	void fingerRotatePos(float pos);
	void fingerRotateDown();
	void fingerRotateUp();

	//type1: vertical ; type2 : armRotate
	void plamRotate(int rotateType);
	void Render();

	const float* Get_root_init();
	const float* Get_first_init();
	const float* Get_second_init();
	float Get_wrist_FE_init();
	float Get_wrist_RU_init();
	float Get_elbow_FE_init();
	float Get_shoulder_FE_init();
	float Get_shoulder_IE_init();
	float Get_shoulder_AA_init();

	const float* Get_root_min();
	const float* Get_first_min();
	const float* Get_second_min();
	float Get_wrist_FE_min();
	float Get_wrist_RU_min();
	float Get_elbow_FE_min();
	float Get_shoulder_FE_min();
	float Get_shoulder_IE_min();
	float Get_shoulder_AA_min();

	const float* Get_root_max();
	const float* Get_first_max();
	const float* Get_second_max();
	float Get_wrist_FE_max();
	float Get_wrist_RU_max();
	float Get_elbow_FE_max();
	float Get_shoulder_FE_max();
	float Get_shoulder_IE_max();
	float Get_shoulder_AA_max();

private:
	GLFrame* object;
	GLMmodel* palm;
	GLMmodel* arm;
	GLMmodel* Lower_arm;
	GLMmodel* Upper_arm;

	float root[4];
	float first[5];
	float second[5];

	float root_init[4];
	float first_init[5];
	float second_init[5];

	float root_min[4];
	float first_min[5];
	float second_min[5];

	float root_max[4];
	float first_max[5];
	float second_max[5];

	// Wrist (2 DoF)
	float wrist_FE;
	float wrist_RU;

	float wrist_FE_init;
	float wrist_RU_init;

	float wrist_FE_min;
	float wrist_RU_min;

	float wrist_FE_max;
	float wrist_RU_max;

	// Elbow (1 DoF)
	float elbow_FE;
	float elbow_IE;
	float elbow_AA;

	float elbow_FE_init;
	float elbow_IE_init;
	float elbow_AA_init;

	float elbow_FE_min;
	float elbow_IE_min;
	float elbow_AA_min;

	float elbow_FE_max;
	float elbow_IE_max;
	float elbow_AA_max;

	// Shoulder (3 DoF)
	float shoulder_FE;
	float shoulder_IE;
	float shoulder_AA;

	float shoulder_FE_init;
	float shoulder_IE_init;
	float shoulder_AA_init;

	float shoulder_FE_min;
	float shoulder_IE_min;
	float shoulder_AA_min;

	float shoulder_FE_max;
	float shoulder_IE_max;
	float shoulder_AA_max;

	int index;
	int finger_index;
};