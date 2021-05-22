#include "pch.h"
#include "Hand.h"

Hand::Hand(GLFrame* obj) {
	object = obj;
	object->SetOrigin(0, 0, -1);
	palm = glmReadOBJ("hand.obj");
	arm = glmReadOBJ("arm.obj");
	glmUnitize(palm);
	glmFacetNormals(palm);
	glmVertexNormals(palm, 90);
	glmUnitize(arm);
	glmFacetNormals(arm);
	glmVertexNormals(arm, 90);

	index = 0;// INDEX: 0~2:litte finger ; 3~5:right finger ; 6~8:middle ; 9~11: fore ; 12~13: thumb
	finger_index = 0;

	/// ///////////////////////////////////////////////////////////////////////
	root[0] = 15;
	first[0] = 15;
	second[0] = 15;

	root[1] = 15;
	first[1] = 15;
	second[1] = 15;

	root[2] = 15;
	first[2] = 15;
	second[2] = 15;

	root[3] = 15;
	first[3] = 15;
	second[3] = 15;

	first[4] = 15;
	second[4] = 15;

	/// ///////////////////////////////////////////////////////////////////////
	root_init[0] = 15;
	first_init[0] = 15;
	second_init[0] = 15;

	root_init[1] = 15;
	first_init[1] = 15;
	second_init[1] = 15;

	root_init[2] = 15;
	first_init[2] = 15;
	second_init[2] = 15;

	root_init[3] = 15;
	first_init[3] = 15;
	second_init[3] = 15;

	first_init[4] = 15;
	second_init[4] = 15;

	/// ///////////////////////////////////////////////////////////////////////
	root_min[0] = 0;
	first_min[0] = 0;
	second_min[0] = 0;

	root_min[1] = 0;
	first_min[1] = 0;
	second_min[1] = 0;

	root_min[2] = 0;
	first_min[2] = 0;
	second_min[2] = 0;

	root_min[3] = 0;
	first_min[3] = 0;
	second_min[3] = 0;

	first_min[4] = 0;
	second_min[4] = 0;

	/// ///////////////////////////////////////////////////////////////////////
	root_max[0] = 90;
	first_max[0] = 90;
	second_max[0] = 90;

	root_max[1] = 90;
	first_max[1] = 90;
	second_max[1] = 90;

	root_max[2] = 90;
	first_max[2] = 90;
	second_max[2] = 90;

	root_max[3] = 90;
	first_max[3] = 90;
	second_max[3] = 90;

	first_max[4] = 90;
	second_max[4] = 90;

	armDeg = 0;
}

Hand::~Hand() {

}

void Hand::setJointIndex(int num) {
	index = num;
}

void Hand::WristRotatePos(float pos1) {
	index = 14;
	setJointIndex(index); // Wrist index
	armDeg = pos1;
	if (armDeg >= 84)
		armDeg = 84;
	else if (armDeg <= -50)
		armDeg = -50;
}

void Hand::ThumbRotatePos(float pos1, float pos2) {
	finger_index = 4;
	index = 12;
	setJointIndex(index); // Thumb - first index
	first[finger_index] = pos1;
	if (first[finger_index] >= first_max[finger_index])
		first[finger_index] = first_max[finger_index];
	else if (first[finger_index] <= first_min[finger_index])
		first[finger_index] = first_min[finger_index];

	index = 13;
	setJointIndex(index); // Thumb - second index
	second[finger_index] = pos2;
	if (second[finger_index] >= second_max[finger_index])
		second[finger_index] = second_max[finger_index];
	else if (second[finger_index] <= second_min[finger_index])
		second[finger_index] = second_min[finger_index];
}

void Hand::IndexRotatePos(float pos1, float pos2, float pos3) {
	finger_index = 3;
	index = 9;
	setJointIndex(index); // Thumb - first index
	root[finger_index] = pos1;
	if (root[finger_index] >= root_max[finger_index])
		root[finger_index] = root_max[finger_index];
	else if (root[finger_index] <= root_min[finger_index])
		root[finger_index] = root_min[finger_index];

	index = 10;
	setJointIndex(index); // Thumb - first index
	first[finger_index] = pos1;
	if (first[finger_index] >= first_max[finger_index])
		first[finger_index] = first_max[finger_index];
	else if (first[finger_index] <= first_min[finger_index])
		first[finger_index] = first_min[finger_index];

	index = 11;
	setJointIndex(index); // Thumb - second index
	second[finger_index] = pos2;
	if (second[finger_index] >= second_max[finger_index])
		second[finger_index] = second_max[finger_index];
	else if (second[finger_index] <= second_min[finger_index])
		second[finger_index] = second_min[finger_index];
}

void Hand::MiddleRotatePos(float pos1, float pos2, float pos3) {
	finger_index = 2;
	index = 6;
	setJointIndex(index); // Thumb - first index
	root[finger_index] = pos1;
	if (root[finger_index] >= root_max[finger_index])
		root[finger_index] = root_max[finger_index];
	else if (root[finger_index] <= root_min[finger_index])
		root[finger_index] = root_min[finger_index];

	index = 7;
	setJointIndex(index); // Thumb - first index
	first[finger_index] = pos1;
	if (first[finger_index] >= first_max[finger_index])
		first[finger_index] = first_max[finger_index];
	else if (first[finger_index] <= first_min[finger_index])
		first[finger_index] = first_min[finger_index];

	index = 8;
	setJointIndex(index); // Thumb - second index
	second[finger_index] = pos2;
	if (second[finger_index] >= second_max[finger_index])
		second[finger_index] = second_max[finger_index];
	else if (second[finger_index] <= second_min[finger_index])
		second[finger_index] = second_min[finger_index];
}

void Hand::RingRotatePos(float pos1, float pos2, float pos3) {
	finger_index = 1;
	index = 3;
	setJointIndex(index); // Thumb - first index
	root[finger_index] = pos1;
	if (root[finger_index] >= root_max[finger_index])
		root[finger_index] = root_max[finger_index];
	else if (root[finger_index] <= root_min[finger_index])
		root[finger_index] = root_min[finger_index];

	index = 4;
	setJointIndex(index); // Thumb - first index
	first[finger_index] = pos1;
	if (first[finger_index] >= first_max[finger_index])
		first[finger_index] = first_max[finger_index];
	else if (first[finger_index] <= first_min[finger_index])
		first[finger_index] = first_min[finger_index];

	index = 5;
	setJointIndex(index); // Thumb - second index
	second[finger_index] = pos2;
	if (second[finger_index] >= second_max[finger_index])
		second[finger_index] = second_max[finger_index];
	else if (second[finger_index] <= second_min[finger_index])
		second[finger_index] = second_min[finger_index];
}

void Hand::PinkyRotatePos(float pos1, float pos2, float pos3) {
	finger_index = 0;
	index = 0;
	setJointIndex(index); // Thumb - first index
	root[finger_index] = pos1;
	if (root[finger_index] >= root_max[finger_index])
		root[finger_index] = root_max[finger_index];
	else if (root[finger_index] <= root_min[finger_index])
		root[finger_index] = root_min[finger_index];

	index = 1;
	setJointIndex(index); // Thumb - first index
	first[finger_index] = pos1;
	if (first[finger_index] >= first_max[finger_index])
		first[finger_index] = first_max[finger_index];
	else if (first[finger_index] <= first_min[finger_index])
		first[finger_index] = first_min[finger_index];

	index = 2;
	setJointIndex(index); // Thumb - second index
	second[finger_index] = pos2;
	if (second[finger_index] >= second_max[finger_index])
		second[finger_index] = second_max[finger_index];
	else if (second[finger_index] <= second_min[finger_index])
		second[finger_index] = second_min[finger_index];
}

void Hand::fingerRotatePos(float pos) {
	if (index == 12) {
		first[4] = pos;
		if (first[4] >= 90)
			first[4] = 90;
		else if (first[4] <= 0)
			first[4] = 0;
	}
	else if (index == 13) {
		second[4] = pos;
		if (second[4] >= 90)
			second[4] = 90;
		else if (second[4] <= 0)
			second[4] = 0;
	}
	else if (index == 14) {
		armDeg = pos;
		if (armDeg >= 84)
			armDeg = 84;
		if (armDeg <= -50)
			armDeg = -50;
	}
	else if (index % 3 == 0) {
		root[index / 3] = pos;
		if (root[index / 3] >= 78)
			root[index / 3] = 78;
		if (root[index / 3] <= 0)
			root[index / 3] = 0;
	}
	else if (index % 3 == 1) {
		first[index / 3] = pos;
		if (first[index / 3] >= 99)
			first[index / 3] = 99;
		if (first[index / 3] <= 0)
			first[index / 3] = 0;
	}
	else if (index % 3 == 2) {
		second[index / 3] = pos;
		if (second[index / 3] >= 90)
			second[index / 3] = 90;
		if (second[index / 3] <= 0)
			second[index / 3] = 0;
	}
}

void Hand::fingerRotateDown() {
	if (index == 12) {
		first[4] += 3;
		if (first[4] >= 90)
			first[4] = 90;
	}
	else if (index == 13) {
		second[4] += 3;
		if (second[4] >= 90)
			second[4] = 90;
	}
	else if (index == 14) {
		armDeg += 3;
		if (armDeg >= 84)
			armDeg = 84;
	}
	else if (index % 3 == 0) {
		root[index / 3] += 3;
		if (root[index / 3] >= 78)
			root[index / 3] = 78;
	}
	else if (index % 3 == 1) {
		first[index / 3] += 3;
		if (first[index / 3] >= 99)
			first[index / 3] = 99;
	}
	else if (index % 3 == 2) {
		second[index / 3] += 3;
		if (second[index / 3] >= 90)
			second[index / 3] = 90;
	}
}

void Hand::fingerRotateUp() {
	if (index == 12) {
		first[4] -= 3;
		if (first[4] <= 0)
			first[4] = 0;
	}
	else if (index == 13) {
		second[4] -= 3;
		if (second[4] <= 0)
			second[4] = 0;
			second[4] = 0;
	}
	else if (index == 14) {
		armDeg -= 3;
		if (armDeg <= -50)
			armDeg = -50;
	}
	else if (index % 3 == 0) {
		root[index / 3] -= 3;
		if (root[index / 3] <= 0)
			root[index / 3] = 0;
	}
	else if (index % 3 == 1) {
		first[index / 3] -= 3;
		if (first[index / 3] <= 0)
			first[index / 3] = 0;
	}
	else if (index % 3 == 2) {
		second[index / 3] -= 3;
		if (second[index / 3] <= 0)
			second[index / 3] = 0;
	}
}

void Hand::plamRotate(int rotateType) { //type1: vertical ; type2 : armRotate
	if (index != 14)
		return;
}

void Hand::Render() {
	glPushMatrix();
	glLineWidth(2);
	object->ApplyActorTransform();
	glTranslatef(0, 1, 0);

#pragma region Arm
	glPushMatrix();
	glLineWidth(2);
	glScalef(12, 12, 12);
	//glRotatef(-90, 0, 0, 1);
	glTranslatef(-0.095, 0.06, 0.95);
	glRotatef(15, 0, 1, 0);
	glRotatef(90, 1, 0, 0);

	glColor3ub(0, 200, 0); glmDraw(arm, GL_SMOOTH);
	glPopMatrix();
#pragma endregion

	glTranslatef(0, 0.2, 0);
	glRotatef(-90, 0, 0, 1);

#pragma region Palm
	glLineWidth(1);
	glScalef(1.5, 1.5, -1.5);
	glTranslatef(0, 0, -3.2);
	glRotatef(armDeg, 1, 0, 0);
	index == 14 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.5, 50, 50);

	glTranslatef(0, 0, 0.5);
	glColor3ub(0, 200, 0); glmDraw(palm, GL_SMOOTH);

	glLineWidth(2);
	glTranslatef(0.08, 0, -0.6);
	glScalef(2.0 / 3.0, 2.0 / 3.0, -2.0 / 3.0);
	glTranslatef(-0.1, 0, -4.5);

#pragma endregion

#pragma region litteFinger
	glPushMatrix();
	glTranslatef(1, -0.2, 1.1);
	glScalef(1.2, 1.2, 1.2);
	glRotatef(-33, 0, 1, 0);
	glTranslatef(0.95, 0.15, -0.45);

	//glColor3ub(255, 255, 255); glutWireSphere(0.15, 10, 10);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.35, 10, 10);
	//glTranslatef(0, 0, 0.35);

	//glColor3ub(255, 0, 0); glutWireSphere(0.15, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.45, 10, 10);
	//glTranslatef(0, 0, 0.45);

	//glColor3ub(255, 0, 0); glutWireSphere(0.15, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.6, 10, 10);
	//glTranslatef(0, 0, 0.6);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 30, 30);
	glTranslatef(0, 0, 0.35 + 0.45 + 0.6);
	glScalef(1, 1, -1);

	glRotatef(root[0], 1, 0, 0);
	index == 0 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.6, 10, 10);
	glTranslatef(0, 0, 0.6);

	glRotatef(first[0], 1, 0, 0);
	index == 1 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.15, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.45, 10, 10);
	glTranslatef(0, 0, 0.45);

	glRotatef(second[0], 1, 0, 0);
	index == 2 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.15, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.15, 0.35, 10, 10);
	glTranslatef(0, 0, 0.35);

	glColor3ub(255, 255, 255); glutWireSphere(0.15, 10, 10);
	glPopMatrix();
#pragma endregion

#pragma region rightFinger 
	//�L�W?
	glPushMatrix();
	glTranslatef(0.5, -0.1, 0.4);

	glScalef(1.2, 1.2, 1.2);
	glRotatef(-9, 0, 1, 0);
	glTranslatef(0.4, 0.35, -0.5);

	//glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.4, 10, 10);
	//glTranslatef(0, 0, 0.4);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.7, 10, 10);
	//glTranslatef(0, 0, 0.7);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.1, 10, 10);
	//glTranslatef(0, 0, 1.1);

	//glColor3ub(255, 0, 0); glutWireSphere(0.25, 30, 30);
	glTranslatef(0, 0, 0.4 + 0.7 + 1.1);
	glScalef(1, 1, -1);

	glRotatef(root[1], 1, 0, 0);
	index == 3 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.25, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.1, 10, 10);
	glTranslatef(0, 0, 1.1);

	glRotatef(first[1], 1, 0, 0);
	index == 4 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.7, 10, 10);
	glTranslatef(0, 0, 0.7);

	glRotatef(second[1], 1, 0, 0);
	index == 5 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.4, 10, 10);
	glTranslatef(0, 0, 0.4);

	glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);
	glPopMatrix();
#pragma endregion

#pragma region middleFinger
	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	glTranslatef(0.07, 0.35, -0.4);
	//glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.5, 10, 10);
	//glTranslatef(0, 0, 0.5);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.7, 10, 10);
	//glTranslatef(0, 0, 0.7);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.3, 10, 10);
	//glTranslatef(0, 0, 1.3);

	//glColor3ub(255, 0, 0); glutWireSphere(0.25, 30, 30);
	glTranslatef(0, 0, 2.5);
	glScalef(1, 1, -1);

	glRotatef(root[2], 1, 0, 0);
	index == 6 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.25, 30, 30);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.3, 10, 10);
	glTranslatef(0, 0, 1.3);

	glRotatef(first[2], 1, 0, 0);
	index == 7 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.7, 10, 10);
	glTranslatef(0, 0, 0.7);

	glRotatef(second[2], 1, 0, 0);
	index == 8 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 40, 40);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.5, 10, 10);
	glTranslatef(0, 0, 0.5);

	glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);

	glPopMatrix();
#pragma endregion

#pragma region foreFinger
	glPushMatrix();
	glTranslatef(-0.5, -0.1, 0.4);
	glScalef(1.2, 1.2, 1.2);
	glRotatef(12, 0, 1, 0);
	glTranslatef(-0.4, 0.5, -0.4);


	//glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.5, 10, 10);
	//glTranslatef(0, 0, 0.5);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.55, 10, 10);
	//glTranslatef(0, 0, 0.55);

	//glColor3ub(255, 0, 0); glutWireSphere(0.2, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.1, 10, 10);
	//glTranslatef(0, 0, 1.1);

	//glColor3ub(255, 0, 0); glutWireSphere(0.25, 30, 30);
	glTranslatef(0, 0, 0.5 + 0.55 + 1.1);
	glScalef(1, 1, -1);

	glRotatef(root[3], 1, 0, 0);
	index == 9 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.25, 30, 30);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 1.1, 10, 10);
	glTranslatef(0, 0, 1.1);

	glRotatef(first[3], 1, 0, 0);
	index == 10 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 30, 30);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.55, 10, 10);
	glTranslatef(0, 0, 0.55);

	glRotatef(second[3], 1, 0, 0);
	index == 11 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0); glutWireSphere(0.2, 30, 30);
	glColor3ub(255, 255, 255); glutWireCylinder(0.2, 0.5, 10, 10);
	glTranslatef(0, 0, 0.5);

	glColor3ub(255, 255, 255); glutWireSphere(0.2, 10, 10);
	glPopMatrix();
#pragma endregion

#pragma region Thumb
	glPushMatrix();
	glTranslatef(-1.1, -0.1, 1.7);

	glScalef(1.2, 1.2, 1.2);
	glRotatef(60, 0, 1, 0);
	glTranslatef(-1.35, -0.05, -0.6);

	//glColor3ub(255, 255, 255); glutWireSphere(0.25, 10, 10);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.25, 0.5, 10, 10);
	//glTranslatef(0, 0, 0.4);

	//glColor3ub(255, 0, 0); glutWireSphere(0.25, 20, 20);
	////glRotatef(20, 1, 0, 0);
	//glColor3ub(255, 255, 255); glutWireCylinder(0.25, 0.7, 10, 10);
	//glTranslatef(0, 0, 0.7);

	//glColor3ub(255, 0, 0); glutWireSphere(0.3, 30, 30);

	glTranslatef(0, 0, 0.5 + 0.7);
	glScalef(1, 1, -1);

	glRotatef(first[4], 1, 0, 0);
	index == 12 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0);
	glutWireSphere(0.3, 30, 30);
	glColor3ub(255, 255, 255);
	glutWireCylinder(0.25, 0.7, 10, 10);
	glTranslatef(0, 0, 0.7);

	glRotatef(second[4], 1, 0, 0);
	index == 13 ? glColor3ub(255, 255, 0) : glColor3ub(255, 0, 0);
	glutWireSphere(0.25, 20, 20);
	glColor3ub(255, 255, 255);
	glutWireCylinder(0.25, 0.5, 10, 10);
	glTranslatef(0, 0, 0.5);

	glColor3ub(255, 255, 255);
	glutWireSphere(0.25, 10, 10);
	glPopMatrix();
#pragma endregion

	glPopMatrix();
}

const float* Hand::Get_root_init() {
	return root_init;
}

const float* Hand::Get_first_init() {
	return first_init;
}

const float* Hand::Get_second_init() {
	return second_init;
}

const float* Hand::Get_root_min() {
	return root_min;
}

const float* Hand::Get_first_min() {
	return first_min;
}

const float* Hand::Get_second_min() {
	return second_min;
}

const float* Hand::Get_root_max() {
	return root_max;
}

const float* Hand::Get_first_max() {
	return first_max;
}

const float* Hand::Get_second_max() {
	return second_max;
}