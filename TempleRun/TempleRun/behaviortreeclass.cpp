#include "behaviortreeclass.h"

Node::Node(const char* key)
{
	strcpy_s(data, key);
	state = State::Null;
	type = Type::Null;
	Info.IsTurnPlace = false;
	Info.LeftTurnPlace = false;
	Info.RightTurnPlace = false;
	Info.TTurnPlace = false;

	Info.TurnLeft = false;
	Info.TurnRight = false;
	Info.MoveJump = false;
	Info.MoveSlide = false;

	Info.IsTurned = false;
	Info.IsJumped = false;
	Info.LRposition = 0.0f;
	Info.CoinStack = 0;

	Info.JumpModel = JumpType::Null;
	Info.MoveModel = PositionType::Null;
}
void Node::restartBT()
{
	Info.IsTurnPlace = false;
	Info.LeftTurnPlace = false;
	Info.RightTurnPlace = false;
	Info.TTurnPlace = false;

	Info.TurnLeft = false;
	Info.TurnRight = false;
	Info.MoveJump = false;
	Info.MoveSlide = false;

	Info.IsTurned = false;
	Info.IsJumped = false;
	Info.LRposition = 0.0f;
	Info.CoinStack = 0;

	Info.JumpModel = JumpType::Null;
	Info.MoveModel = PositionType::Null;
}
void Node::addSequenceNode(Node* node)
{
	childNodes.push_back(node);
	childNodes[childNodes.size() - 1]->setType(Type::Sequence);
	cout << "ChildNodes length: " << childNodes.size() <<  endl;

	return;
}
void Node::addSelectorNode(Node* node)
{
	childNodes.push_back(node);
	childNodes[childNodes.size() - 1]->setType(Type::Selector);
	cout << "ChildNodes length: " << childNodes.size() << endl;

	return;
}
void Node::addActionNode(Node* node)
{
	childNodes.push_back(node);
	childNodes[childNodes.size() - 1]->setType(Type::Action);
	cout << "ChildNodes length: " << childNodes.size() <<endl;

	return;
}
void Node::addDecoratorNode(Node* node)
{
	childNodes.push_back(node);
	childNodes[childNodes.size() - 1]->setType(Type::Decorator);
	cout << "ChildNodes length: " << childNodes.size() << endl;

	return;
}
void Node::InitializeBT(Node* root)
{
    root->addSequenceNode(new Node("sequence1"));    

    root->getNode(root, "sequence1")->addSelectorNode(new Node("selector1"));
            root->getNode(root, "selector1")->addDecoratorNode(new Node("decorator1"));
                    root->getNode(root, "decorator1")->addActionNode(new Node("action1"));
            root->getNode(root, "selector1")->addSelectorNode(new Node("selector4"));
                            root->getNode(root, "selector4")->addDecoratorNode(new Node("decorator2"));
                                    root->getNode(root, "decorator2")->addActionNode(new Node("action2"));
                            root->getNode(root, "selector4")->addDecoratorNode(new Node("decorator3"));
                                    root->getNode(root, "decorator3")->addActionNode(new Node("action3"));
                            root->getNode(root, "selector4")->addDecoratorNode(new Node("decorator4"));
                                    root->getNode(root, "decorator4")->addSelectorNode(new Node("selector5"));
                                            root->getNode(root, "selector5")->addDecoratorNode(new Node("decorator5"));
                                                    root->getNode(root, "decorator5")->addActionNode(new Node("action4"));
                                            root->getNode(root, "selector5")->addActionNode(new Node("action5"));

    root->getNode(root, "sequence1")->addSelectorNode(new Node("selector2"));
            root->getNode(root, "selector2")->addDecoratorNode(new Node("decorator6"));
                    root->getNode(root, "decorator6")->addActionNode(new Node("action6"));   
            root->getNode(root, "selector2")->addSelectorNode(new Node("selector6"));
                    root->getNode(root, "selector6")->addDecoratorNode(new Node("decorator8"));
                            root->getNode(root, "decorator8")->addActionNode(new Node("action7"));
                    root->getNode(root, "selector6")->addDecoratorNode(new Node("decorator9"));
							root->getNode(root, "decorator9")->addSelectorNode(new Node("selector7"));
									root->getNode(root, "selector7")->addDecoratorNode(new Node("decorator7"));
											root->getNode(root, "decorator7")->addActionNode(new Node("action8"));
									root->getNode(root, "selector7")->addActionNode(new Node("action14"));
                    root->getNode(root, "selector6")->addDecoratorNode(new Node("decorator10"));
                            root->getNode(root, "decorator10")->addActionNode(new Node("action9"));

    root->getNode(root, "sequence1")->addSelectorNode(new Node("selector3"));
            root->getNode(root, "selector3")->addDecoratorNode(new Node("decorator11"));
                    root->getNode(root, "decorator11")->addActionNode(new Node("action10"));
            root->getNode(root, "selector3")->addDecoratorNode(new Node("decorator12"));
                    root->getNode(root, "decorator12")->addActionNode(new Node("action11"));
            root->getNode(root, "selector3")->addDecoratorNode(new Node("decorator13"));
                    root->getNode(root, "decorator13")->addActionNode(new Node("action12"));
    root->getNode(root, "sequence1")->addDecoratorNode(new Node("decorator14"));
            root->getNode(root, "decorator14")->addActionNode(new Node("action13"));


	return;
}

void Node::initState(Node* node, int num)
{
	node->setState(State::Null);
	vector<Node*> childNodes = node->getChildNodes();
	for (int i = 0; i < childNodes.size(); i++)
	{
		initState(childNodes[i], num);
	}

	return;
}


void Node::searchBT(Node* node, int num)
{
	node->setState(State::Running);

	vector<Node*> childNodes = node->getChildNodes();

	if (node->type == Type::Action)
	{
		if (strcmp(node->data, "action1") == 0)
		{
			cout << "좌우 둘러보기\n";
		}
		else 	if (strcmp(node->data, "action2") == 0)
		{
			cout << "turnLeft\n";
			Info.TurnLeft = true;
			Info.IsTurned = true;
		}
		else 	if (strcmp(node->data, "action3") == 0)
		{
			cout << "turnLight\n";
			Info.TurnRight = true;
			Info.IsTurned = true;
		}
		else 	if (strcmp(node->data, "action4") == 0)
		{
			cout << "turnLeft\n";
			Info.TurnLeft = true;
			Info.IsTurned = true;
		}
		else 	if (strcmp(node->data, "action5") == 0)
		{
			cout << "turnRight\n";
			Info.TurnRight = true;
			Info.IsTurned = true;
		}
		else 	if (strcmp(node->data, "action6") == 0)
		{
			cout << "최대 속력 모션\n";
		}
		else 	if (strcmp(node->data, "action7") == 0)
		{
			cout << "moveLeft\n";
			Info.LRposition -= 0.15f;
		}
		else 	if (strcmp(node->data, "action8") == 0)
		{
			cout << "Left->Center\n";
			Info.LRposition += 0.15f;
		}
		else 	if (strcmp(node->data, "action9") == 0)
		{
			cout << "moveRight\n";
			Info.LRposition += 0.15f;
		}
		else 	if (strcmp(node->data, "action10") == 0)
		{
			cout << "jump\n";
			Info.MoveJump = true;
			Info.IsJumped = true;
		}
		else 	if (strcmp(node->data, "action11") == 0)
		{
			cout << "slide\n";
			Info.MoveSlide = true;
		}
		else 	if (strcmp(node->data, "action12") == 0)
		{
			cout << "가만히 있기\n";
		}
		else 	if (strcmp(node->data, "action13") == 0)
		{
			cout << "코인 20개\n";
		}
		else 	if (strcmp(node->data, "action14") == 0)
		{
			cout << "Right->Center\n";
			Info.LRposition -= 0.15f;
		}
		else
		{
		}
		node->setState(State::Success);
	}
	else if (node->type == Type::Selector)
	{
		int count = 0;
		for (int i = 0; i < childNodes.size(); i++)
		{
			if (i > 0 && childNodes[i - 1]->getState() == State::Success)
			{
				node->setState(State::Success);
				return;
			}
			searchBT(childNodes[i], num);
			if (childNodes[i]->getState() == State::Failure)
			{
				count++;
			}
		}
		if (count == childNodes.size())
		{
			node->setState(State::Failure);
		}
	}
	else if (node->type == Type::Sequence)
	{
		int count = 0;
		for (int i = 0; i < childNodes.size(); i++)
		{
			//cout << "\n";
			searchBT(childNodes[i], num);
			if (childNodes[i]->getState() == State::Failure)
			{
				node->setState(State::Failure);
				return;
			}

			if (childNodes[i]->getState() == State::Success)
			{
				count++;
			}
		}
		if (count == childNodes.size())
		{
			node->setState(State::Success);
		}
	}
	else if (node->type == Type::Decorator)
	{
		if (childNodes.size() == 0)
		{
			//0개인 노드 없음
		}
		else
		{
			if (strcmp(node->data, "decorator1") == 0)
			{
				if (Info.IsTurnPlace == true)
				{
					node->setState(State::Failure);
				}
				if (Info.IsTurnPlace == false)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);						
					}
				}

			}
			else if (strcmp(node->data, "decorator2") == 0)
			{
				if (Info.LeftTurnPlace == true)
				{
					if (Info.IsTurned== true)
						node->setState(State::Success);
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else if (Info.LeftTurnPlace == false)
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator3") == 0)
			{
				if (Info.RightTurnPlace == true)
				{
					if (Info.IsTurned == true)
						node->setState(State::Success);
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else if (Info.RightTurnPlace == false)
				{
					node->setState(State::Failure);
				}
			}

			else if (strcmp(node->data, "decorator4") == 0)
			{
				if (Info.TTurnPlace == true)
				{					
					if (Info.IsTurned == true)
						node->setState(State::Success);
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else if (Info.TTurnPlace == false)
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator5") == 0)
			{
				int temp;
				temp = rand() % 2;

				if (temp == 0)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}
				}
			}
			else if (strcmp(node->data, "decorator6") == 0)
			{
				if (Info.MoveModel == PositionType::Null)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator7") == 0)
			{
				if (Info.LRposition <= -0.1f)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator8") == 0)
			{
				if (Info.MoveModel == PositionType::Left)
				{
					if (Info.LRposition <= -0.8f)
					{
						node->setState(State::Success);
					}
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator9") == 0)
			{
				if (Info.MoveModel == PositionType::Center)
				{
					if (Info.LRposition < 0.1f && Info.LRposition > -0.1f)
					{
						node->setState(State::Success);
					}
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator10") == 0)
			{
				if (Info.MoveModel == PositionType::Right)
				{
					if (Info.LRposition >= 0.8f)
					{
						node->setState(State::Success);
					}
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator11") == 0)
			{
				if (Info.JumpModel == JumpType::Jump)
				{
					if (Info.IsJumped == true)
					{
						node->setState(State::Success);
					}
					else
					{
						searchBT(childNodes[0], num);
						if (childNodes[0]->getState() == State::Success)
						{
							node->setState(State::Success);
						}
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator12") == 0)
			{
				if (Info.JumpModel == JumpType::Slide)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}					
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator13") == 0)
			{
				if (Info.JumpModel == JumpType::Stop || Info.JumpModel == JumpType::Null)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else if (strcmp(node->data, "decorator14") == 0)
			{
				if (Info.CoinStack == 20)
				{
					searchBT(childNodes[0], num);
					if (childNodes[0]->getState() == State::Success)
					{
						node->setState(State::Success);
					}
				}
				else
				{
					node->setState(State::Failure);
				}
			}
			else
			{
				searchBT(childNodes[0], num);
				node->setState(State::Success);
			}
		}
	}
	else
	{		
		for (int i = 0; i < childNodes.size(); i++)
		{
			//cout << "나머지/n/n";
			searchBT(childNodes[i], num);
		}
	}

	return;
}

Node* Node::getNode(Node* node, const char* key)
{
	vector<Node*> v = node->getChildNodes();
	if (v.size() == 0) {
		return NULL;
	}

	for (int i = 0; i < v.size(); i++) {
		if (strcmp(v[i]->getData(), key) == 0)
		{
			return v[i];
		}
		else {
			Node* tn = getNode(v[i], key);
			if (NULL != tn)
				return tn;
		}
	}
	return NULL;
}

