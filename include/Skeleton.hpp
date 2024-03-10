#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "pch.hpp"

#define FRAMERATE 60.f

#define EULER_XYZ 0
#define EULER_XZY 1
#define EULER_YXZ 2
#define EULER_YZX 3
#define EULER_ZXY 4
#define EULER_ZYX 5

typedef struct Joint {
	uint id;
	std::string name;
	glm::vec3 position;
	glm::vec3 axis;
	int eulerOrder;

	glm::mat4 _C;
	glm::mat4 _Cinv;
	glm::mat4 _ROT; // = CinvMC (C * M * Cinv)
	glm::vec3 _B;
	// L = CinvMCB

	std::vector<std::string> dof;
	std::vector<glm::vec2> limits;

	glm::vec3 cor; // 프레임에서의 최종 좌표
} Joint;

typedef struct KeyFrame {
	uint sequence;
	float time;
	std::map<std::string, std::vector<float> > motions;
} KeyFrame;

class Skeleton {
	private:
		// parse ASF file
		std::string ASF_Filename;

		std::string version;
		std::string name;
		std::string documentation;

		float mass; // 질량 단위
		float length; // 길이 단위
		bool ASFisRad; // degree, radian 사용 여부
		glm::vec3 position; // 위치

		std::map<std::string, Joint> joints;

		std::map<std::string, std::vector<std::string> > hierarchy; // 부모 자식 관계도


		bool setUnit(std::fstream &file);

		bool setRoot(std::fstream &file);
		bool setRootOrder(std::fstream &file, Joint &root);

		bool setBoneData(std::fstream &file);
		bool setBoneDataDof(std::fstream &file, Joint &joint);
		bool setBoneDataLimits(std::fstream &file, Joint &joint);

		bool setHierarchy(std::fstream &file);
		// parse ASF file


		// parse AMC file
		std::string AMC_Filename;

		bool isSpecified;
		bool AMCisRad;
		std::vector<KeyFrame> keyFrame;
		// parse AMC file

		// for check error
		bool error;
		// for check error


		// for skeleton
		// std::map<std::string, sJoint> sJoints;
		void recursive(
			std::string const &parent,
			std::string const &name,
			std::stack<glm::mat4> rotateStack,
			std::stack<glm::vec3> translateStack);
		void recursive(
			std::string const &parent,
			std::string const &name,
			std::stack<glm::mat4> rotateStack,
			std::stack<glm::vec3> translateStack,
			glm::vec3 &pos,
			glm::mat4 &look);

		void setJointLocal(KeyFrame &first, KeyFrame &second, float t);
		// for skeleton

		// for get
		void recursiveLine(std::string const &parent, std::string const &name, std::vector<glm::vec3> &res);
		void recursiveCube(std::string const &parent, std::string const &name, std::vector<glm::mat4> &res);
		// for get

	public:
		Skeleton();
		~Skeleton();

		bool setASF(std::string const &filename);
		bool setAMC(std::string const &filename);
		bool setGlobalTransform();
		bool setGlobalTransform(glm::vec3 &pos, glm::mat4 &look);
		void setLocal(float curTime);

		std::vector<glm::vec3> getPoints();
		std::vector<glm::vec3> getLines();
		std::vector<glm::mat4> getcube();

};

static glm::mat4 eulerRotation (float x, float y, float z, int order);
static void skipWhiteSpace(std::fstream &file);
static std::string trim(std::string &str);
static std::string peekLine(std::fstream &file);
static std::string peekWord(std::fstream &file);

#endif