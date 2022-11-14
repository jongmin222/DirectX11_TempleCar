#include<vector>
#include<iostream>
#include<string>


#pragma once
using namespace std;
constexpr float PI = 3.141592654f;

enum class JumpType
{
	Null,
	Stop,
	Slide,
	Jump
};
enum class PositionType
{
	Null,
	Left,
	Center,
	Right
};


struct WorldInfo
{
	bool IsTurnPlace;
	bool LeftTurnPlace;
	bool RightTurnPlace;
	bool TTurnPlace;

	bool TurnLeft;
	bool TurnRight;
	bool MoveJump;
	bool MoveSlide;

	bool IsTurned;
	bool IsJumped;
	float LRposition;
	int CoinStack;

	JumpType JumpModel;
	PositionType MoveModel;
};


class Node
{
private:

	enum class Type
	{
		Null,
		Sequence,
		Selector,
		Action,
		Decorator
	};

	enum class State
	{
		Null,
		Running,
		Success,
		Failure
	};

	//data: 노드의 값
	//childNodes: 노드의 자식 노드로 동적 배열로 저장된다
	//type: 노드타입
	//state: 노드상태
	char data[20];
	Type type;
	State state;
	vector<Node*> childNodes;	
public:

	//데이터에 키값을 가진 노드를 생성한다
	Node(const char*);

	//데이터 설정
	void setData(char* ch)
	{
		strcpy_s(data, ch);
	}

	//데이터 조회
	char* getData()
	{
		return data;
	}

	//타입 설정
	void setType(Type m_type) {
		type = m_type;
	}

	//타입 조회
	Type getType() {
		return type;
	}

	//타입 설정
	void setState(State m_state) {
		state = m_state;
	}

	//타입 조회
	State getState() {
		return state;
	}

	//차일드 노드 추가
	void addSequenceNode(Node*);
	void addSelectorNode(Node*);
	void addActionNode(Node*);
	void addDecoratorNode(Node*);

	//노드 추가하여 행동트리 만들기
	void InitializeBT(Node*);

	//차일드 노드 목록 조회
	vector<Node*> getChildNodes() {
		return childNodes;
	}
	//트리 탐색
	void searchBT(Node*, int);

	//노드 상태 초기화
	void initState(Node*, int);

	//행동트리 정보 초기화
	void restartBT();

	//node로 부터 시작하여 해당 Key에 해당하는 데이터 값을 가진 노드를 재귀적으로 찾아 반환한다.
	Node* getNode(Node*, const char*);	

	//월드정보
	WorldInfo Info;


};