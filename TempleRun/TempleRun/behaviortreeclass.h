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

	//data: ����� ��
	//childNodes: ����� �ڽ� ���� ���� �迭�� ����ȴ�
	//type: ���Ÿ��
	//state: ������
	char data[20];
	Type type;
	State state;
	vector<Node*> childNodes;	
public:

	//�����Ϳ� Ű���� ���� ��带 �����Ѵ�
	Node(const char*);

	//������ ����
	void setData(char* ch)
	{
		strcpy_s(data, ch);
	}

	//������ ��ȸ
	char* getData()
	{
		return data;
	}

	//Ÿ�� ����
	void setType(Type m_type) {
		type = m_type;
	}

	//Ÿ�� ��ȸ
	Type getType() {
		return type;
	}

	//Ÿ�� ����
	void setState(State m_state) {
		state = m_state;
	}

	//Ÿ�� ��ȸ
	State getState() {
		return state;
	}

	//���ϵ� ��� �߰�
	void addSequenceNode(Node*);
	void addSelectorNode(Node*);
	void addActionNode(Node*);
	void addDecoratorNode(Node*);

	//��� �߰��Ͽ� �ൿƮ�� �����
	void InitializeBT(Node*);

	//���ϵ� ��� ��� ��ȸ
	vector<Node*> getChildNodes() {
		return childNodes;
	}
	//Ʈ�� Ž��
	void searchBT(Node*, int);

	//��� ���� �ʱ�ȭ
	void initState(Node*, int);

	//�ൿƮ�� ���� �ʱ�ȭ
	void restartBT();

	//node�� ���� �����Ͽ� �ش� Key�� �ش��ϴ� ������ ���� ���� ��带 ��������� ã�� ��ȯ�Ѵ�.
	Node* getNode(Node*, const char*);	

	//��������
	WorldInfo Info;


};