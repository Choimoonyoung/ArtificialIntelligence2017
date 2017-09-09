#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>

using namespace std;
int nodeCount;


int board[6][8]; //������ ��� ���������� �״´�.
int height[8];
int totalmove;
int columnarr[8];

int omega = 0, opp = 0;

void play(int col) { //�ϳ��� �ξ��� ��
	height[col]++;
	board[height[col] - 1][col] = totalmove % 2 + 1; //Ȧ����°1 ¦����°2
	totalmove++;
}

void cancelplay(int col) { //�� �� ���
	height[col]--;
	board[height[col]][col] = 0;
	totalmove--;
}


bool isfullcolumn(int column) {
	if (board[5][column] != 0)
		return true;
	else return false;
	//column�� ��á���� true �ƴϸ� false 
}

bool isthiswinningmove(int col) {//���� column�� �θ� ���� �����ڰ� �̱⳪��?
	int player = totalmove % 2 + 1;
	if (height[col] >= 3 && board[height[col] - 1][col] == player && board[height[col] - 2][col] == player && board[height[col] - 3][col] == player) {
		return true;
	}//vertical check
	int cx = height[col]; //���� ������
	int cy = col;
	for (int dy = -1; dy <= 1; dy++) { //y���� dir. dy == -1 || 1 �̸� diagonal, 0�̸� horizontal check
		int nb = 0;
		for (int dx = -1; dx <= 1; dx += 2) {
			for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
				x += dx;
				y += dy*dx;
			}
		}
		if (nb >= 3) return true;
	}
	return false;
}

bool isthislosingmove(int col) { //���� column�� �θ� ���������ڰ� ������?
	int player = 3 - totalmove % 2 + 1;
	if (height[col] >= 3 && board[height[col] - 1][col] == player && board[height[col] - 2][col] == player && board[height[col] - 3][col] == player) {
		return true;
	}//vertical check
	int cx = height[col]; //���� ������
	int cy = col;
	for (int dy = -1; dy <= 1; dy++) { //y���� dir. dy == -1 || 1 �̸� diagonal, 0�̸� horizontal check
		int nb = 0;
		for (int dx = -1; dx <= 1; dx += 2) {
			for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
				x += dx;
				y += dy*dx;
			}
		}
		if (nb >= 3) return true;
	}
	return false;
}
int playsequence(string positionlist) { //���������� �ξ��� ��
	for (int i = 0; i < positionlist.size(); i++) {
		int col = positionlist[i];
		if ((col > 7) || (col < 1) || isfullcolumn(col) || isthiswinningmove(col))
			return -1;
		play(col);
	}
	return positionlist.size();
}

bool checkhorizontal() { //4���� �̷�����°� �ֳ� ã�ƺ���.
	for (int i = 0; i < 6; i++) {
		for (int j = 1; j <= 4; j++) {
			if (board[i][j] != 0) { //������ �Ǿ��ִٸ�
				int stoneowner = board[i][j];
				if ((board[i][j + 1] == stoneowner) && (board[i][j + 2] == stoneowner) && (board[i][j + 3] == stoneowner))
					//���� row�� 4���� �����̶��
					return true;
			}
		}
	}
	return false;
}

bool checkvertical() {
	for (int j = 1; j <= 7; j++) {
		for (int i = 0; i <= 6; i++) {
			if (board[i][j] != 0) {
				int owner = board[i][j];
				if (board[i + 1][j] == owner &&board[i + 2][j] == owner && board[i + 3][j] == owner)
					return true;
			}
		}
	}
	return false;
}

bool checkplusdiagonal() {
	for (int i = 4; i <= 6; i++) { // �̷���(/) ���� diagonal �� 4���� �Ǵ°� �ִ��� �˻��Ѵ�.
		for (int row = 0; row < i - 3; row++) {
			if (board[row][i - row] != 0) { //���� ������ ������ �Ǿ��ִٸ�
				int owner = board[row][i - row];
				if ((board[row + 1][i - row - 1] == owner) && (board[row + 2][i - row - 2] == owner) && (board[row + 3][i - row - 3] == owner)) {
					return true;
				}
			}
		}
	}
	for (int i = 7; i <= 9; i++) {
		for (int column = 7; column >= i - 2; column--) {
			if (board[i - column][column] != 0) {
				int owner = board[i - column][column];
				if ((board[i + 1 - column][column - 1] == owner) && (board[i + 2 - column][column - 2] == owner) && (board[i + 3 - column][column - 3] == owner)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool checkminusdiagonal() { // �̷���(\) �Ǵ� 4���� row�� �ִ��� �˻��Ѵ�
	for (int i = 2; i >= 0; i--) {
		for (int j = 1; j <= 3 - i; j++) {
			if (board[i + j - 1][j] != 0) {
				int owner = board[i + j - 1][j];
				if ((board[i + j][j + 1] == owner) && (board[i + j + 1][j + 2] == owner) && (board[i + j + 2][j + 3] == owner))
					return true;
			}
		}
	}
	for (int i = 2; i <= 4; i++) {
		for (int j = 0; j <= 4 - i; j++) {
			if (board[j][i + j] != 0) {
				int owner = board[j][i + j];
				if ((board[j + 1][i + j + 1] == owner) && (board[j + 2][i + j + 2] == owner) && (board[j + 3][i + j + 3] == owner))
					return true;
			}
		}
	}
	return false;
}
bool checkdiagonal() { //�밢�� 4�� �ֳ� ã�ƺ���.
	if ((checkplusdiagonal() == false) && (checkminusdiagonal == false))
		return false;
	else
		return true;
}

int whoiswinner(int i, int j) { //���� ������ ���� �̰�ٰ� �ϸ�, �� ���� ������ return�Ѵ�.
	return board[i][j];
}

void printboard() { //board���¸� ����Ʈ�Ѵ�.
	for (int i = 5; i >= 0; i--) {
		for (int j = 1; j < 8; j++) {
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}


void check4(int player) { //4���� �Ǵ� �� �ִ��� ����,����, �밢�� ���ɴ�.
	bool wow;
	if (checkdiagonal() == false && checkhorizontal() == false && checkvertical() == false)
		wow = false;
	else
		wow = true;
	if (wow == true) {
		printf("wow! player %d are win!!\n", player);
		printf("%d %d %d %d\n", checkplusdiagonal(), checkminusdiagonal(), checkhorizontal(), checkvertical());
	}
}

//board play �Լ�
int ROWS = 6;

int COLS = 7;
/*
���ݺ��ʹ� alpha - beta pruning �Լ��Դϴ�.
���Ŀ� ��Ÿ�� ������� �ο��� ���Ѻ����? ^^*
*/

/*
int ROWS = 6;
int COLS = 7;

int score(int row, int col) {
int score = 0;
bool unblocked = true;
int tally = 0;
//int r, c;
int CHECKERS[100] = { 0, };
int playerToMoveNum = 0;

if (row < ROWS - 3) {
//check up
unblocked = true;
tally = 0;

for (int r = row; r<row + 4; r++) {
if (board[r][col] == CHECKERS[1 - playerToMoveNum]) {
unblocked = false;
}
if (board[r][col] == CHECKERS[playerToMoveNum]) {
tally++;
}
}
if (unblocked == true) {
score = score + (tally*tally*tally*tally);
}
if (col < COLS - 3) {
//check up and to the right
unblocked = true;
tally = 0;
for (int r = row, c = col; r<row + 4; r++, c++) {
if (board[r][c] == CHECKERS[1 - playerToMoveNum]) {
unblocked = false;
}
if (board[r][c] == CHECKERS[playerToMoveNum]) {
tally++;
}
}
if (unblocked == true) {
score = score + (tally*tally*tally*tally);
}
}
}
if (col < COLS - 3) {
//check right
unblocked = true;
tally = 0;
for (int c = col; c<col + 4; c++) {
if (board[row][c] == CHECKERS[1 - playerToMoveNum]) {
unblocked = false;
}
if (board[row][c] == CHECKERS[playerToMoveNum]) {
tally++;
}
}
if (unblocked == true) {
score = score + (tally*tally*tally*tally);
}
if (row > 2) {
//check down and to the right
unblocked = true;
tally = 0;
for (int r = row, c = col; c<col + 4; r--, c++) {
if (board[r][c] == CHECKERS[1 - playerToMoveNum]) {
unblocked = false;
}
if (board[r][c] == CHECKERS[playerToMoveNum]) {
tally++;
}
}
if (unblocked == true) {
score = score + (tally*tally*tally*tally);
}
}
}
return score;
}

int evaluate1() {
int evaluation = 0;
for (int r = 0; r < ROWS; r++) {
if (r <= ROWS - 4) {
for (int c = 0; c < COLS; c++) {
evaluation += score(r, c);
}
}
else {
for (int c = 0; c <= COLS - 4; c++) {
evaluation += score(r, c);
}
}
}
return evaluation;
}
*/

/*int negamax(int depth, int alpha, int beta,int* xx, int* yy)
{
if (depth == 3)
return evaluate();
//GenerateLegalMoves();
nodeCount++;
if (totalmove == 6 * 7)
return 0;
for (int x = 1; x <= 7; x++) {
if (!isfullcolumn(x) && isthiswinningmove(x))
return (43 - totalmove)*1000;
}

for (int x = 1; x <= 7;x++) {
if (!isfullcolumn(x)) {
play(x);
//printboard();
//system("pause");
int val = -negamax(depth + 1, -beta, -alpha, xx, yy); // <-- ***NEGATE HERE***
cancelplay(x);
if (val >= beta) {
return beta;
}
if (val > alpha) {
*xx = height[x] - 1;
*yy = x;
alpha = val;
}
}
}
return alpha;
}*/
/*
int bestscore = -9999;
for (int x = 1; x <= 7; x++) {
if (!isfullcolumn(x)) {
play(x);
int score = -negamax(alpha,beta,depth+1,xx,yy);
if (score > bestscore) {
*yy = x;
*xx = height[x] - 1;
bestscore = score;
}
cancelplay(x);
}
}

return bestscore;
}
*/ // minmax
//minmax

int evaluationTable[6][7] =
{ { 3, 4, 5, 7, 5, 4, 3 },
{ 4, 6, 8, 10, 8, 6, 4 },
{ 5, 8, 11, 13, 11, 8, 5 },
{ 5, 8, 11, 13, 11, 8, 5 },
{ 4, 6, 8, 10, 8, 6, 4 },
{ 3, 4, 5, 7, 5, 4, 3 } };

//evaluation table�� ������ �ش��ϴ� �������� ������, �׸� �̿��� ����� �ִ� 4row�� ������ ��Ƴ��� evaluation Table�̴�.


int evaluate1() {
	int utility = 138;
	int sum = 0;
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 7; j++) {
			if (board[i][j] == totalmove % 2 + 1)
				sum += evaluationTable[i][j];
			else if (board[i][j] == totalmove % 2 + 2)
				sum -= evaluationTable[i][j];
		}
	return sum;
}

//evaluation�Լ��� �������� eval�� �������� �ִµ�, ù��°�� evaluation �Լ��̴�.
//�� �Լ��� utility�� ��ü element���� ���̰�, �׿� �ش��ϴ� �ڸ��� ���� player�� �ִٸ� +, �ٸ� �÷��̾ �ִٸ� -�� �ؼ� ��ü sum�� ����Ѵ�

	int nconnectedwhithhole(int currplayer) {
		int sum = 0;
		int weight = 0;
		for (int j = 1; j <= 4; j++) {
			for (int i = 0; i < height[j]; i++) {
				if (board[i][j] == currplayer) {
					for (int dy = -1; dy <= 1; dy++) {
						for (int dx = 1, count = 1, x = j + dx, y = i + dy; x <= 7 && x >= 1 && y <= 5 && y >= 0 && count <= 3; count++, x += dx, y += dy) {
							if (board[x][y] == currplayer)
								sum += 10;
							if (board[x][y] == 0)
								sum -= 1;
						}
						if (sum == 19)
							weight++;
					}
				}
			}
		}
		return weight;
	}

// 4���� ������ row�� ���� ��, ����� hole�� �ִ� ���·� 2�� 1���� ������ row�� �ִ� ���� ���� ����, �� ��忡 ����ġ�� �д�.

int evaluate() {
	int weight = 0;
	weight = nconnectedwhithhole(omega) - nconnectedwhithhole(opp); //���� ����- ����� ����
	weight *= 100;
	weight += evaluate1();
	for (int x = 1; x <= 7; x++) {
		if (!isfullcolumn(x) && isthislosingmove(x))
			weight += -100000 + totalmove;
	}
	return weight;
}

//���� ��޵� eval�Լ� �ΰ��� ���Ѵ�. ��� ������ ��밡 �θ� ������ losing move�� ��쿡��, - ����ġ�� �ּ� ���Ѵ�.

int negamax(int alpha, int beta, int depth, int* xx, int* yy) {
	nodeCount++;
	if (totalmove == 0) {
		*yy = 4;
		return 0;
	}
	if (totalmove == 6 * 7)
		return 0;

	for (int x = 1; x <= 7; x++) {
		if (!isfullcolumn(x) && isthiswinningmove(x))
			return 100000 - totalmove;
	}

	if (depth == 16)
		return evaluate();

	int ceiling = 90000 - totalmove;
	if (beta > ceiling) {
		beta = ceiling;
		if (alpha >= beta)
			return beta; //�򰥷��� �ٲ���� beta
	}

	for (int x = 1; x <= 7; x++) {
		if (!isfullcolumn(columnarr[x])) {

			play(columnarr[x]);
			//printboard();
			//system("pause");
			int score = -negamax(-beta, -alpha, depth + 1, xx, yy);
			cancelplay(columnarr[x]);
			if (score >= beta) {
				//printboard();
				//system("pause");
				return score;
			}
			if (score > alpha) {
				alpha = score;
				*xx = height[columnarr[x]] - 1;
				*yy = columnarr[x];
			}
		}
	}
	return alpha;
}

//alphabeta pruning �Լ��̴�. 
//��带 ����� ���, �װ��߽����� ���ؼ� ��������� �����ߴ�.


/*int negamax(int alpha, int beta, int depth, int* xx, int* yy) {
nodeCount++;
if (totalmove == 0) {
*yy = 4;
return 1;
}
if (totalmove == 6 * 7)
return 0;
for (int x = 1; x <= 7; x++) {
if (!isfullcolumn(x) && isthiswinningmove(x))
return 10000;
}

if (depth == 6)
return evaluate();

int bestscore = -9999;
for (int x = 1; x <= 7; x++) {
if (!isfullcolumn(x)) {
play(x);
int score = -negamax(alpha, beta, depth + 1, xx, yy);
if (score > bestscore) {
*yy = x;
*xx = height[x] - 1;
bestscore = score;
}
cancelplay(x);
}
}

return bestscore;
}
*/

int ruleflag[8] = { 0, };

int rule() {
	int weight = 0;
	int max = 0;
	int max_col = 4;

	int player = omega;
	int count;
	//Offense1

	for (int i = 1; i < 8; i++) {
		count = 0;
		if (isfullcolumn(i)) {
			ruleflag[i] = 1;
			continue;
		}
		else {
			if (height[i] >= 3
				&& board[height[i] - 1][i] == player && board[height[i] - 2][i] == player && board[height[i] - 3][i] == player) {
				return i;
			}

			int cx = height[i];
			int cy = i;
			for (int dy = -1; dy <= 1; dy++) {
				int nb = 0;
				for (int dx = -1; dx <= 1; dx += 2) {
					for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
						x += dx;
						y += dy*dx;
					}
				}
				if (nb >= 3) return i;
			}

		}
	}
	player = opp;
	//Defense1

	for (int i = 1; i < 8; i++) {
		if (isfullcolumn(i)) {
			ruleflag[i] = 1;
			continue;
		}
		else {
			if (height[i] >= 3 && board[height[i] - 1][i] == player && board[height[i] - 2][i] == player
				&& board[height[i] - 3][i] == player) {
				return i;
			}
			int cx = height[i];
			int cy = i;
			for (int dy = -1; dy <= 1; dy++) {
				int nb = 0;
				for (int dx = -1; dx <= 1; dx += 2) {
					for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
						x += dx;
						y += dy*dx;
					}
				}
				if (nb >= 3) return i;
			}
		}
	}
	for (int i = 1; i < 8; i++) {
		count = 0;
		if (board[height[i]][i + 1] == player && (i + 1)<8) {//������ Ȯ��
			count++;
			if (board[height[i]][i + 2] == player && (i + 2)<8) {//������+1
				count++;
				if (board[height[i]][i + 3] == player && (i + 3)<8) {
					return i;
				}
			}
		}
		else if (board[height[i]][i - 1] == player && (i - 1)>0) {//���� Ȯ��
			if (count == 2) {
				return i;
			}
			count++;
			if (board[height[i]][i - 2] == player && (i - 2)>0) {//����+1
				if (count == 2) {
					return i;
				}
				count++;
				if (board[height[i]][i - 3] == player && (i - 3)>0) {
					return i;
				}
			}
		}
	}


	//Defense2
	for (int i = 1; i < 8; i++) {
		height[i]++;
		if (height[i] >= 3
			&& board[height[i] - 1][i] == player && board[height[i] - 2][i] == player && board[height[i] - 3][i] == player) {
			return i;
		}
		int cx = height[i];
		int cy = i;
		for (int dy = -1; dy <= 1; dy++) {
			int nb = 0;
			for (int dx = -1; dx <= 1; dx += 2) {
				for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
					x += dx;
					y += dy*dx;
				}
			}
			if (nb >= 3) {
				ruleflag[i] = 1;
				break;
			}
		}
		height[i]--;
	}

	player = omega;
	//Offense2
	for (int i = 1; i < 8; i++) {
		height[i]++;
		if (height[i] >= 3 && board[height[i] - 1][i] == player
			&& board[height[i] - 2][i] == player && board[height[i] - 3][i] == player) {
			return i;
		}
		int cx = height[i];
		int cy = i;
		for (int dy = -1; dy <= 1; dy++) {
			int nb = 0;
			for (int dx = -1; dx <= 1; dx += 2) {
				for (int x = cx + dx, y = cy + dx*dy; x >= 1 && x <= 7 && y <= 5 && y >= 0 && board[x][y] == player; nb++) {
					x += dx;
					y += dy*dx;
				}
			}
			if (nb >= 3) {
				ruleflag[i] = 1;
				break;
			}
		}
		height[i]--;
	}
	//Offense3

	for (int i = 1; i < 8; i++) {
		weight = 0;
		if (ruleflag[i] == 1) {
			continue;
		}
		//������
		if ((i + 1) < 8 && board[height[i]][i + 1] == player) {
			if (board[height[i]][i + 2] == player) {
				weight += 12;
			}
			else if ((i + 2) < 8 && board[height[i]][i + 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i + 1) < 8 && board[height[i]][i + 1] == opp) {
			weight -= 2;
		}
		//����
		if ((i - 1) > 0 && board[height[i]][i - 1] == player) {
			if (board[height[i]][i - 2] == player) {
				weight += 12;
			}
			else if ((i - 2) > 0 && board[height[i]][i - 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i - 1) > 0 && board[height[i]][i - 1] == opp) {
			weight -= 2;
		}
		//�Ʒ�
		if ((i - 1) > 0 && board[height[i] - 1][i] == player) {
			if ((i - 2) > 0 && board[height[i] - 2][i] == player) {
				weight += 12;
			}
			else if (board[height[i] - 1][i] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if (board[height[i] - 1][i] == opp) {
			weight -= 2;
		}
		//����
		if ((i - 1) > 0 && board[height[i] + 1][i - 1] == player) {
			if ((i - 2) > 0 && board[height[i] + 2][i - 2] == player) {
				weight += 12;
			}
			else if ((i - 2) > 0 && board[height[i] + 2][i - 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i - 1) > 0 && board[height[i] + 1][i - 1] == opp) {
			weight -= 2;
		}
		//�޾Ʒ�
		if ((i - 1) > 0 && board[height[i] - 1][i - 1] == player) {
			if ((i - 2) > 0 && board[height[i] - 2][i - 2] == player) {
				weight += 12;
			}
			else if ((i - 2) > 0 && board[height[i] - 2][i - 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i - 1) > 0 && board[height[i] - 1][i - 1] == opp) {
			weight -= 2;
		}
		//������
		if ((i + 1) < 8 && board[height[i] + 1][i + 1] == player) {
			if ((i + 2) < 8 && board[height[i] + 2][i + 2] == player) {
				weight += 12;
			}
			else if ((i + 2) < 8 && board[height[i] + 2][i + 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i + 1) < 8 && board[height[i] + 1][i + 1] == opp) {
			weight -= 2;
		}
		//�����Ʒ�
		if ((i + 1) < 8 && board[height[i] - 1][i + 1] == player) {
			if ((i + 2) < 8 && board[height[i] - 2][i + 2] == player) {
				weight += 12;
			}
			else if ((i + 2) < 8 && board[height[i] - 2][i + 2] == opp) {
				weight += 2;
			}
			else weight += 4;
		}
		else if ((i + 1) < 8 && board[height[i] - 1][i + 1] == opp) {
			weight -= 2;
		}

		if (weight > max) {
			max = weight;
			max_col = i;
		}
	}
	return max_col;
}


int main() {
	int xx = 0;
	int yy = 0;
	for (int i = 1; i <= 7; i++) {
		columnarr[i] = 7 / 2 + (1 - 2 * ((i - 1) % 2))*i / 2 + 1;
		printf("columnarr %d is %d\n", i, columnarr[i]);
	}

	//�޸���ƽ�� ����. Ž���� ���������Ͱ� �ƴϰ�, �߰� �÷����� �ؼ� Ȯ���� ������Ų��.

	int firstinput = 0;
	cout << "hi, first? second?(1 or 2)\n";
	cout << "���ް��� �����ּ���. -01066118266507 �ϳ�-";
	cin >> omega;
	if (omega == 1) {
		opp = 2;
	}
	else {
		opp = 1;
		cin >> firstinput;
		play(firstinput);
		printboard();
	}
	while (1) {
		char letter;
		int oppinput = 0;
		cout << "auto? rule?(a or r)";
		cin >> letter;
		if (letter == 'a') { //auto
			negamax(-100000, 100000, 1, &xx, &yy);
			printf("%d\n", yy);
			play(yy);
			printboard();
			cin >> oppinput;
			play(oppinput);
		}
		else if (letter == 'r') { //rule
			printf("%d \n", rule());
			play(rule());
			printboard();
			cin >> oppinput;
			play(oppinput);
		}
		else {
			cout << "��Ÿ. �ٽ� �Է��Ϸ�";
		}
	}
}