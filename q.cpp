#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

const int qSize = 7;
const double gamma = 0.8;
const double learningRate = 1;
const int episodeStep = qSize * qSize;
float epsilon = 0.5;
const int iterations = 50;
bool needLearn = true;

int R[qSize][qSize] =
{
	{ -1, -1, -1, -1,  0, -1,  -1 },
	{ -1, -1, -1,  0, -1, 100,  0 },
	{ -1, -1, -1,  0, -1, -1,   0 },
	{ -1,  0,  0, -1,  0, -1,   0 },
	{  0, -1, -1,  0, -1, 100,  0 },
	{ -1,  0, -1, -1,  0, -1,   0 },
	{ -1,  0,  0,  0,  0, 100, -1 }

};

int Q[qSize][qSize];
int currentState;
int goalState = 5;
void episode(int initialState);
void chooseAnAction();
int getRandomAction(int upperBound, int lowerBound);
void initialize();
int maximum(int state, bool returnIndexOnly);
int reward(int action);
void printQMatrix();

int main()
{

	int newState;

	initialize();

	//Perform learning trials starting at  initial state 0.
	for (int i = 1; i <= iterations ; i++)
	{
		currentState = 0;
        if(needLearn)
        {
            needLearn = false;
            episode(currentState);

        }
        else
        {
            //If last episode learn nothing,decrease epsilon and continue learning.
            epsilon = sqrtf(1.0 / i);
            needLearn = true;

        }

		//printQMatrix();
	}

    printQMatrix();

	//Perform tests, starting at all initial states.
	for (int i = 0; i <= (qSize - 1); i++)
	{
		currentState = i;
		newState = 0;
		while (currentState != goalState)
		{
			newState = maximum(currentState, true);
			cout << currentState << ", ";
			currentState = newState;
		}
		cout << goalState << endl;
	}

	return 0;
}
void printQMatrix()
{
	//Print out Q matrix.
	for (int i = 0; i <= (qSize - 1); i++)
	{
		for (int j = 0; j <= (qSize - 1); j++)
		{
			cout << setw(5) << Q[i][j];
			if (j < qSize - 1)
			{
				cout << ",";
			}
		} // j
		cout << "\n";
	} // i
	cout << "\n";
}

void episode(int initialState)
{
	for (int i = 0; i <= episodeStep; i++)
	{
		chooseAnAction();
	}
}

void chooseAnAction()
{

	int possibleAction;
	if (rand() / (RAND_MAX + 1.0) < epsilon)
	{
		//Randomly choose a possible action connected to the current state.
		possibleAction = getRandomAction(qSize, 0);
	}
	else
	{
		possibleAction = maximum(currentState, true);
		if (R[currentState][possibleAction] < 0)
		{

			possibleAction = getRandomAction(qSize, 0);
		}

	}

	Q[currentState][possibleAction] = reward(possibleAction);
	currentState = possibleAction;

}


int getRandomAction(int upperBound, int lowerBound)
{

	int action;
	bool choiceIsValid = false;
	int range = (upperBound - lowerBound) + 1;

	//Randomly choose a possible action connected to the current state.
	do
	{
		//Get a random value between 0 and 6.
		action = lowerBound + int(range * rand() / (RAND_MAX + 1.0));
		if (R[currentState][action] > -1)
		{
			choiceIsValid = true;
		}
	} while (choiceIsValid == false);

	return action;
}

void initialize()
{

	srand((unsigned)time(0));

	for (int i = 0; i <= (qSize - 1); i++)
	{
		for (int j = 0; j <= (qSize - 1); j++)
		{
			Q[i][j] = 0;
		} // j
	} // i
}

int maximum(int state, bool returnIndexOnly)
{
	// if returnIndexOnly = true, a Q matrix index is returned.
	// if returnIndexOnly = false, a Q matrix element is returned.

	int winner;
	bool foundNewWinner;
	bool done = false;

	winner = 0;

	do {
		foundNewWinner = false;
		for (int i = 0; i <= (qSize - 1); i++)
		{
			if ((i < winner) || (i > winner))
			{     //Avoid self-comparison.
				if (Q[state][i] > Q[state][winner])
				{
					winner = i;
					foundNewWinner = true;
				}
				if (Q[state][i] == Q[state][winner])
				{
					if (rand() / (RAND_MAX + 1.0) < 0.5)
					{
						winner = i;
						foundNewWinner = true;
					}
				}
			}
		} // i

		if (foundNewWinner == false)
		{
			done = true;
		}

	} while (done = false);

	if (returnIndexOnly == true)
	{
		return winner;
	}
	else
	{
		return Q[state][winner];
	}
}

int reward(int action)
{
    int last = Q[currentState][action];
    int cur = static_cast<int>(Q[currentState][action] + learningRate *(R[currentState][action] + gamma * maximum(action, false) - Q[currentState][action]));
    if(cur != last) needLearn = true;
	return  cur;
}
