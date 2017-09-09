#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>

using namespace std;
int nodeCount;


int board[6][8]; //쌓을때 대신 위에서부터 쌓는다.
int height[8];
int totalmove;
int columnarr[8];

int omega = 0, opp = 0;

void play(int col) { //하나를 두었을 때
	height[col]++;
	board[height[col] - 1][col] = totalmove % 2 + 1; //홀수번째1 짝수번째2
	totalmove++;
}

void cancelplay(int col) { //둔 것 취소
	height[col]--;
	board[height[col]][col] = 0;
	totalmove--;
}


bool isfullcolumn(int column) {
	if (board[5][column] != 0)
		return true;
	else return false;
	//column이 꽉찼으면 true 아니면 false 
}

bool isthiswinningmove(int col) {//지금 column에 두면 현재 진행자가 이기나요?
	int player = totalmove % 2 + 1;
	if (height[col] >= 3 && board[height[col] - 1][col] == player && board[height[col] - 2][col] == player && board[height[col] - 3][col] == player) {
		return true;
	}//vertical check
	int cx = height[col]; //현재 착수점
	int cy = col;
	for (int dy = -1; dy <= 1; dy++) { //y방향 dir. dy == -1 || 1 이면 diagonal, 0이면 horizontal check
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

bool isthislosingmove(int col) { //지금 column에 두면 현재진행자가 지나요?
	int player = 3 - totalmove % 2 + 1;
	if (height[col] >= 3 && board[height[col] - 1][col] == player && board[height[col] - 2][col] == player && board[height[col] - 3][col] == player) {
		return true;
	}//vertical check
	int cx = height[col]; //현재 착수점
	int cy = col;
	for (int dy = -1; dy <= 1; dy++) { //y방향 dir. dy == -1 || 1 이면 diagonal, 0이면 horizontal check
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
int playsequence(string positionlist) { //여러가지를 두었을 때
	for (int i = 0; i < positionlist.size(); i++) {
		int col = positionlist[i];
		if ((col > 7) || (col < 1) || isfullcolumn(col) || isthiswinningmove(col))
			return -1;
		play(col);
	}
	return positionlist.size();
}

bool checkhorizontal() { //4개가 이루어지는게 있나 찾아본다.
	for (int i = 0; i < 6; i++) {
		for (int j = 1; j <= 4; j++) {
			if (board[i][j] != 0) { //착수가 되어있다면
				int stoneowner = board[i][j];
				if ((board[i][j + 1] == stoneowner) && (board[i][j + 2] == stoneowner) && (board[i][j + 3] == stoneowner))
					//수평 row에 4개가 연속이라면
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
	for (int i = 4; i <= 6; i++) { // 이렇게(/) 생긴 diagonal 이 4개가 되는게 있는지 검사한다.
		for (int row = 0; row < i - 3; row++) {
			if (board[row][i - row] != 0) { //현재 지점에 착수가 되어있다면
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

bool checkminusdiagonal() { // 이렇게(\) 되는 4개의 row가 있는지 검사한다
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
bool checkdiagonal() { //대각선 4개 있나 찾아본다.
	if ((checkplusdiagonal() == false) && (checkminusdiagonal == false))
		return false;
	else
		return true;
}

int whoiswinner(int i, int j) { //지금 착수한 놈이 이겼다고 하면, 그 놈이 누군지 return한다.
	return board[i][j];
}

void printboard() { //board상태를 프린트한다.
	for (int i = 5; i >= 0; i--) {
		for (int j = 1; j < 8; j++) {
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}


void check4(int player) { //4개가 되는 게 있는지 가로,세로, 대각선 살핀다.
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

//board play 함수
int ROWS = 6;

int COLS = 7;
/*
지금부터는 alpha - beta pruning 함수입니다.
알파와 베타와 밸류값의 싸움을 지켜볼까요? ^^*
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

//evaluation table은 각각에 해당하는 착수점을 가지고, 그를 이용해 만들수 있는 4row의 개수를 담아놓은 evaluation Table이다.


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

//evaluation함수는 여러개의 eval로 나누어져 있는데, 첫번째는 evaluation 함수이다.
//이 함수는 utility는 전체 element들의 합이고, 그에 해당하는 자리에 현재 player가 있다면 +, 다른 플레이어가 있다면 -를 해서 전체 sum을 출력한다

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

// 4개로 구성된 row를 봤을 때, 가운데에 hole이 있는 상태로 2개 1개로 구성된 row가 있는 줄이 많을 수록, 그 노드에 가중치를 둔다.

int evaluate() {
	int weight = 0;
	weight = nconnectedwhithhole(omega) - nconnectedwhithhole(opp); //나의 개수- 상대의 개수
	weight *= 100;
	weight += evaluate1();
	for (int x = 1; x <= 7; x++) {
		if (!isfullcolumn(x) && isthislosingmove(x))
			weight += -100000 + totalmove;
	}
	return weight;
}

//위에 언급된 eval함수 두개를 더한다. 대신 다음걸 상대가 두면 무조건 losing move일 경우에는, - 가중치를 둬서 피한다.

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
			return beta; //헷갈려서 바꿔놓음 beta
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

//alphabeta pruning 함수이다. 
//노드를 만드는 대신, 네가멕스값을 통해서 재귀적으로 구현했다.


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
		if (board[height[i]][i + 1] == player && (i + 1)<8) {//오른쪽 확인
			count++;
			if (board[height[i]][i + 2] == player && (i + 2)<8) {//오른쪽+1
				count++;
				if (board[height[i]][i + 3] == player && (i + 3)<8) {
					return i;
				}
			}
		}
		else if (board[height[i]][i - 1] == player && (i - 1)>0) {//왼쪽 확인
			if (count == 2) {
				return i;
			}
			count++;
			if (board[height[i]][i - 2] == player && (i - 2)>0) {//왼쪽+1
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
		//오른쪽
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
		//왼쪽
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
		//아래
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
		//왼위
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
		//왼아래
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
		//오른위
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
		//오른아래
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

	//휴리스틱의 일종. 탐색을 끝에서부터가 아니고, 중간 컬럼부터 해서 확률을 증가시킨다.

	int firstinput = 0;
	cout << "hi, first? second?(1 or 2)\n";
	cout << "오메가를 도와주세요. -01066118266507 하나-";
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
			cout << "오타. 다시 입력하렴";
		}
	}
}