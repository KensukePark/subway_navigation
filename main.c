#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 552 //총 역 갯수
#define INF   9999   // 무한대 (연결이 없는 경우) 
#define NODES  552 // g->n 값, 저장할 총 역의 갯수
#define TRANS_VERTICES 150 //환승역 갯수

typedef struct GraphType {
	int n;   // 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES];// 시작정점으로부터의 최단경로 거리 
int found[MAX_VERTICES]; //* 방문한 정점 표시 
int path[NODES]; // 경로 저장 배열 
char station[MAX_VERTICES][50]; // 각 선 csv에서 읽은 역번호 저장 배열 
char st_num[MAX_VERTICES][50]; // 역이름 csv에서 읽은 역번호 저장 배열 
char st_name[MAX_VERTICES][50]; // 역이름 csv에서 읽은 역이름 저장 배열 
char st_trans_num[TRANS_VERTICES][50]; // 환승 csv 각 줄 첫번째 값인 역 번호 저장 배열
char st_trans_num_2[TRANS_VERTICES][50]; // 환승 csv 첫 가로줄에 있는 역 번호 저장 배열
GraphType g; //그래프 g

/*
함수명 : choose
인자 : int distance 배열, int n, int found 배열
리턴 : minpos
설명 : distance 배열에 있는 가장 작은 값의 노드를 선택하여 idx를 return하는 함수
*/
int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	//찾은 노드의 idx를 저장한 minpos를 반환
	return minpos;
}

/*
함수명 : print_path
인자 : int start, int end
설명 : 최단 경로를 출력하는 함수
*/
void print_path(int start, int end)
{
	int i = 0;
	int start_loc; //시작역의 노드 번호를 저장할 변수
	int end_loc; //도착역의 노드 번호를 저장할 변수
	while (i < 553) //while문으로 시작역과 도착역의 노드 번호를 찾음
	{
		if (strcmp(station[start], st_num[i]) == 0)
			start_loc = i;
		if (strcmp(station[end], st_num[i]) == 0)
			end_loc = i;
		i++;
	}
	int j = 1;
	int path_tmp[MAX_VERTICES]; //경로 임시 저장 변수
	int trans_time = 0; //총 환승 시간 출력용
	int trans_count = 0; //환승 횟수 저장용
	int total; //총 소요시간 출력용
	path_tmp[0] = end; //0에 도착역 저장
	while (path_tmp[j - 1] != start) //while 문으로 경로 저장
	{
		path_tmp[j] = path[path_tmp[j - 1]];
		j++;
	}
	printf("<출발>\n");
	char tmp[20];
	int j_tmp = j; //경로내의 역 갯수 임시 저장, 정거장 수 출력용
	int weight_tmp = 0; //쓸데없는 환승 경로로 인해 늘어난 환승시간들을 저장할 변수 ex) (경의중앙)용산->(1호선)용산 <도착>

	/*
	시작노드에서 바로 환승할 경우 처리 코드
	ex) 시작역을 용산으로 입력했을 때
	시작노드가 1호선 용산으로 지정되었는데
	로 경의중앙 용산으로 환승하는 Case
	위와 같은 경우 처리하는 함수
	*/
	if (abs(path_tmp[j - 1] - path_tmp[j - 2]) != 1)
	{
		weight_tmp = g.weight[path_tmp[j - 1]][path_tmp[j - 2]];  //시작역 -> 시작역(다른호선) 갈아타는데 소요된 시간 저장
		j--; //이는 필요없는 경로이므로 j-- 하여 환승한 상태에서 경로 출력 시작
		j_tmp--; //정거장 수도 빼줌
	}
	while (j > 1) //출발역부터 도착역 바로 전 역까지의 경로를 출력
	{
		if (path_tmp[j - 1] < 62) strcpy(tmp, "1호선");
		else if (62 <= path_tmp[j - 1] && path_tmp[j - 1] < 101) strcpy(tmp, "1지선");
		else if (101 <= path_tmp[j - 1] && path_tmp[j - 1] < 144) strcpy(tmp, "2호선");
		else if (144 <= path_tmp[j - 1] && path_tmp[j - 1] < 154) strcpy(tmp, "2지선");
		else if (154 <= path_tmp[j - 1] && path_tmp[j - 1] < 197) strcpy(tmp, "3호선");
		else if (197 <= path_tmp[j - 1] && path_tmp[j - 1] < 245) strcpy(tmp, "4호선");
		else if (245 <= path_tmp[j - 1] && path_tmp[j - 1] < 289) strcpy(tmp, "5호선");
		else if (289 <= path_tmp[j - 1] && path_tmp[j - 1] < 297) strcpy(tmp, "5지선");
		else if (297 <= path_tmp[j - 1] && path_tmp[j - 1] < 335) strcpy(tmp, "6지선");
		else if (335 <= path_tmp[j - 1] && path_tmp[j - 1] < 377) strcpy(tmp, "7호선");
		else if (377 <= path_tmp[j - 1] && path_tmp[j - 1] < 394) strcpy(tmp, "8호선");
		else if (394 <= path_tmp[j - 1] && path_tmp[j - 1] < 419) strcpy(tmp, "9호선");
		else if (101 <= path_tmp[j - 1] && path_tmp[j - 1] < 444) strcpy(tmp, "분당선");
		else if (144 <= path_tmp[j - 1] && path_tmp[j - 1] < 473) strcpy(tmp, "인천1선");
		else if (154 <= path_tmp[j - 1] && path_tmp[j - 1] < 501) strcpy(tmp, "중앙선");
		else if (197 <= path_tmp[j - 1] && path_tmp[j - 1] < 522) strcpy(tmp, "경춘선");
		else if (245 <= path_tmp[j - 1] && path_tmp[j - 1] < 542) strcpy(tmp, "경의선");
		else if (289 <= path_tmp[j - 1] && path_tmp[j - 1] < 552) strcpy(tmp, "공항철도");
		//csv를 읽고 저장한 순서를 이용해 해당 역이 몇호선인지 확인

		//환승한 경우 환승 출력 코드
		if ((abs(path_tmp[j - 1] - path_tmp[j - 2]) > 1 || strcmp(st_name[path_tmp[j - 1]], st_name[path_tmp[j - 2]]) == 0) && j != 2)
		{
			int time_temp = g.weight[path_tmp[j - 1]][path_tmp[j - 2]]; //환승 소요시간을 저장
			if (time_temp > 200) time_temp -= 200;  //소요시간이 200보다 큰 경우, 즉 최소환승으로 읽은 경우 200만큼 빼줌

			printf("-><환승 : 소요시간 %d 분> %s", time_temp, st_name[path_tmp[j - 1]]); // 환승 소요시간 출력
			trans_time = trans_time + g.weight[path_tmp[j - 1]][path_tmp[j - 2]]; //총 환승시간에 추가
			trans_count++; //환승 횟수 추가
			j = j - 2; //환승한 다음역을 넘기기위해 j=j-2
			j_tmp--; //정거장 수 빼줌
		}
		else //환승 아닌경우
		{
			printf("-><%s> %s", tmp, st_name[path_tmp[j - 1]]);
			j--;
		}
		if (j != 1) //줄바꿈 처리용 코드
			printf("\n");
	}

	/*
	도착노드가 1호선 용산으로 지정됬고
	도착역 전에 경의중앙 용산을 거쳐
	1호선 용산으로 환승하여 최종 도착하게 되는 경우
	*/
	if (strcmp(st_name[path_tmp[0]], st_name[path_tmp[1]]) == 0)
	{
		int total;
		weight_tmp = weight_tmp + g.weight[path_tmp[1]][path_tmp[0]];
		j_tmp--; //총 정거장수 하나 빼줌
		printf(" 도착\n");
		/*
		최소환승으로 계산된 경우
		소요시간을 정상적으로 수정하는 코드
		*/
		if (trans_time > 200) { //최소환승으로 계산됬으면 총 환승시간이 200보다 크므로
			trans_time -= trans_count * 200; //환승횟수*200을 빼주면 실제 환승시간이 나옴
			total = distance[path_tmp[0]] - weight_tmp; //쓸데없는 환승 경로로 생긴 환승 시간을 빼줌
			total -= trans_count * 200;
		}
		else total = distance[path_tmp[0]] - weight_tmp; //최단경로로 계산된 경우 위의 과정없이 바로 쓸데없는 환승시간을 빼주면 됨

		printf("\n소요시간 : %d (%d + 환승 소요시간 : %d) 분\n", total, total - trans_time, trans_time);
		printf("정거장 수 : %d 개\n", j_tmp);
		return; //총 소요시간 출력후 함수 종료
	}

	//정상적으로 도착역으로 끝나는 경우
	if (path_tmp[0] < 62) strcpy(tmp, "1호선");
	else if (62 <= path_tmp[0] && path_tmp[0] < 101) strcpy(tmp, "1지선");
	else if (101 <= path_tmp[0] && path_tmp[0] < 144) strcpy(tmp, "2호선");
	else if (144 <= path_tmp[0] && path_tmp[0] < 154) strcpy(tmp, "2지선");
	else if (154 <= path_tmp[0] && path_tmp[0] < 197) strcpy(tmp, "3호선");
	else if (197 <= path_tmp[0] && path_tmp[0] < 245) strcpy(tmp, "4호선");
	else if (245 <= path_tmp[0] && path_tmp[0] < 289) strcpy(tmp, "5호선");
	else if (289 <= path_tmp[0] && path_tmp[0] < 297) strcpy(tmp, "5지선");
	else if (297 <= path_tmp[0] && path_tmp[0] < 335) strcpy(tmp, "6지선");
	else if (335 <= path_tmp[0] && path_tmp[0] < 377) strcpy(tmp, "7호선");
	else if (377 <= path_tmp[0] && path_tmp[0] < 394) strcpy(tmp, "8호선");
	else if (394 <= path_tmp[0] && path_tmp[0] < 419) strcpy(tmp, "9호선");
	else if (101 <= path_tmp[0] && path_tmp[0] < 444) strcpy(tmp, "분당선");
	else if (144 <= path_tmp[0] && path_tmp[0] < 473) strcpy(tmp, "인천1선");
	else if (154 <= path_tmp[0] && path_tmp[0] < 501) strcpy(tmp, "중앙선");
	else if (197 <= path_tmp[0] && path_tmp[0] < 522) strcpy(tmp, "경춘선");
	else if (245 <= path_tmp[0] && path_tmp[0] < 542) strcpy(tmp, "경의선");
	else if (289 <= path_tmp[0] && path_tmp[0] < 552) strcpy(tmp, "공항철도");
	//csv를 읽고 저장한 순서를 이용해 마지막 도착역이 몇호선인지 확인
	printf("\n-><%s> %s <도착> \n", tmp, st_name[path_tmp[0]]);

	/*
	최소환승으로 계산된 경우
	소요시간을 정상적으로 수정하는 코드
	*/
	if (trans_time > 200) { //최소환승으로 계산됬으면 총 환승시간이 200보다 크므로
		trans_time -= trans_count * 200; //환승횟수*200을 빼주면 실제 환승시간이 나옴
		total = distance[path_tmp[0]] - weight_tmp; //쓸데없는 환승 경로로 생긴 환승 시간을 빼줌
		total -= trans_count * 200;
	}
	else total = distance[path_tmp[0]] - weight_tmp;  //최단경로로 계산된 경우 위의 과정없이 바로 쓸데없는 환승시간을 빼주면 됨

	printf("\n소요시간 : %d (%d + 환승 소요시간 : %d) 분\n", total, total - trans_time, trans_time);
	printf("정거장 수 : %d 개\n", j_tmp);
	return; //총 소요시간 출력후 함수 종료
}

/*
함수명 : shortest_path
인자 : GraphType* g, int start
설명 : 그래프 g를 이용해 시작 노드에서 시작하는 최단 경로를 찾는 함수
*/
void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	int s = start;
	for (i = 0; i < g->n; i++) //초기화 코드
	{
		distance[i] = g->weight[start][i];
		found[i] = FALSE;
		path[i] = s;
	}
	found[start] = TRUE; //시작 노드 방문 표시
	distance[start] = 0; //시작 노드로의 값은 0으로
	for (i = 0; i < g->n - 1; i++) {
		u = choose(distance, g->n, found); //가장 작은 값을 갖은 idx를 찾아 변수 u로
		found[u] = TRUE; //u 방문표시
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w])
				{ //기존의 거리보다 (u까지의 거리+ u에서 w까지의 거리)가 더 짧으면 distance와 path를 수정
					distance[w] = distance[u] + g->weight[u][w];
					path[w] = u;
				}
	}
}

/*
함수명 : load_line_csv
설명 : 모든 노선의 csv 파일을 읽어 그래프g에 저장하는 함수
*/
void load_line_csv()
{
	int max = 9999;
	char line[9999];
	char* p;
	g.n = 0; //노드수 0으로 초기화
	for (int k = 0; k < MAX_VERTICES; k++)
	{
		for (int l = 0; l < MAX_VERTICES; l++)
		{
			g.weight[k][l] = 9999; //모든 weight를 9999로 초기화
		}
	}
	int x = 0; //그래프에 weight를 저장할때 사용할 변수1
	int y; //그래프에 weight를 저장할때 사용할 변수2
	int j = 0; //읽은 역 개수를 저장할 변수
	//1호선 csv 로드
	FILE* in = fopen("1호선.csv", "r"); //파일 열음
	fgets(line, max, in); //첫 줄 읽음(사용안할 예정)
	while (!feof(in)) {
		y = 0;
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); //, 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 62) break; //1호선 총 역의 개수 62개이므로 노드의 갯수가 62개면 종료
	}
	fclose(in);//파일 닫음

	//1지선 csv 로드
	int temp_y = y; //현재 y값을 임시로 저장 
	in = fopen("1지선.csv", "r"); //파일 열음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한 줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 101) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 1지선의 역의 갯수인 101개면 종료
	}
	fclose(in); //파일 닫음

	//2호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("2호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ",");  // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 144) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 2호선의 역의 갯수인 144개면 종료
	}
	fclose(in); //파일 닫음

	//2지선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("2지선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 154) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 2지선의 역의 갯수인 154개면 종료
	}
	fclose(in); //파일 닫음

	//3호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("3호선.csv", "r");//파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 197) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 3호선의 역의 갯수인 197개면 종료
	}
	fclose(in); //파일 닫음

	//4호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("4호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p);//weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 245) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 4호선의 역의 갯수인 245개면 종료
	}
	fclose(in); //파일 닫음

	//5호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("5호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 289) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 5호선의 역의 갯수인 289개면 종료
	}
	fclose(in); //파일 닫음

	//5지선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("5지선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y;  //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 297) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 5지선의 역의 갯수인 297개면 종료
	}
	fclose(in); //파일 닫음

	//6호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("6호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y;  //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 335) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 6호선의 역의 갯수인 335개면 종료
	}
	fclose(in); //파일 닫음

	//7호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("7호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 377) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 7호선의 역의 갯수인 377개면 종료
	}
	fclose(in); //파일 닫음

	//8호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("8호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 394) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 8호선의 역의 갯수인 394개면 종료
	}
	fclose(in); //파일 닫음

	//9호선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("9호선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 419) break;  //노드의 갯수가 여태까지 저장한 역의 개수 + 9호선의 역의 갯수인 419개면 종료
	}
	fclose(in); //파일 닫음

	//분당선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("분당선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 444) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 분당선의 역의 갯수인 444개면 종료
	}
	fclose(in); //파일 닫음

	//인천1선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("인천1선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in);  //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 473) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 인천1호선의 역의 갯수인 473개면 종료
	}
	fclose(in); //파일 닫음

	//중앙선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("중앙선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ","); //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 501) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 중앙선의 역의 갯수인 501개면 종료
	}
	fclose(in); //파일 닫음

	//경춘선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("경춘선.csv", "r");  //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ",");  //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 522) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 경춘선의 역의 갯수인 522개면 종료
	}
	fclose(in); //파일 닫음

	//경의선 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("경의선.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ",");  //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 542) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 경의선의 역의 갯수인 542개면 종료
	}
	fclose(in); //파일 닫음

	//공항철도 csv 로드
	temp_y = y; //현재 y값을 임시로 저장
	in = fopen("공항철도.csv", "r"); //파일 읽음
	fgets(line, max, in); //첫 줄 읽음
	while (!feof(in)) {
		y = temp_y; //y를 임시저장했던 temp_y값으로 초기화
		fgets(line, max, in); //다음 한줄을 읽음
		p = strtok(line, ","); // , 기준으로 하나를 분리
		strcpy(station[j], p); //첫 문자열은 역의 번호이므로 이를 station 배열에 저장
		j++;
		p = strtok(NULL, ",");  //, 기준으로 다음을 얻어옴
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight를 그래프 g에 넣어줌
			y++;
			p = strtok(NULL, ","); // , 기준으로 다음을 얻어옴
		}
		x++;
		g.n++;
		if (g.n == 552) break; //노드의 갯수가 여태까지 저장한 역의 개수 + 공항철도의 역의 갯수인 552개면 종료
	}
	fclose(in); //파일 닫음

	//역이름 csv 로드
	char line_n[9999];
	char* p_n;
	int loc = 0; //배열의 인덱스로 이용할 변수
	in = fopen("역이름.csv", "r"); //파일 열음
	fgets(line_n, max, in); //첫 줄 읽음
	while (!feof(in)) {
		fgets(line_n, max, in); //다음 한 줄을 읽음
		p_n = strtok(line_n, ","); // , 기준으로 하나를 분리
		strcpy(st_num[loc], p_n); //첫 문자열은 역의 번호이므로 이를 st_num 배열에 저장
		p_n = strtok(NULL, "\n"); // , 기준으로 다음 하나를 분리
		strcpy(st_name[loc], p_n); //다음 문자열은 역의 한글 이름이므로 이를 st_name 배열에 저장
		loc++;
		if (loc == MAX_VERTICES) //총 역의 개수인 552개가 되면 while 루프 종료
			break;
	}
	fclose(in); //파일 닫음
}

/*
함수명 : load_trans_csv
인자 : int how
설명 : 경로 방식(최단거리or최소환승)에 따라
환승정보 csv 파일을 읽어 그래프 g에 저장하는 함수
*/
void load_trans_csv(int how)
{
	//환승정보 csv 로드
	//st_trans_num 배열에 역 번호들을 저장하고 닫을 예정
	srand(time(NULL)); //난수 생성을 위해 시드를 매번 변경
	char line_t[9999];
	char* p_t;
	int max = 9999;
	FILE* in = fopen("환승정보.csv", "r"); //파일 열음
	fgets(line_t, max, in); //첫 줄을 읽음
	int num_of_trans = 0; //역 번호를 저장할 때 인덱스로 사용할 변수
	while (num_of_trans < TRANS_VERTICES)
	{
		fgets(line_t, max, in); // 다음 한 줄을 읽음
		p_t = strtok(line_t, ","); // , 기준으로 하나를 분리
		strcpy(st_trans_num[num_of_trans], p_t); //st_trans_num 배열에 저장
		num_of_trans++;
	}
	fclose(in); //파일 닫음

	//환승정보 csv 로드2
	//환승시의 간선의 weight를 저장하기 위해 열음
	in = fopen("환승정보.csv", "r"); //파일 열음
	fgets(line_t, max, in); // 첫 줄을 읽음
	int y_pos; //
	int check_line = 0; //몇 번째 줄을 읽는지 체크하기위한 변수
	int find_x; //그래프 g의 노드의 위치로 사용할 변수1
	int find_y; //그래프 g의 노드의 위치로 사용할 변수1
	while (1)
	{
		fgets(line_t, max, in); //다음 한 줄을 읽음
		y_pos = 0;
		p_t = strtok(line_t, ","); // ,를 구분으로 하나를 읽음
		find_x = 0;
		while (1) //현재 줄의 노드가 그래프 g에서 어디에 있는지 위치를 찾는 코드
		{
			if (strcmp(st_trans_num[check_line], station[find_x]) == 0)
				break;
			else {
				find_x++;
			}
		}
		p_t = strtok(NULL, ","); // ,를 구분으로 하나를 읽음
		while (1)
		{
			if (atoi(p_t) < 9999 && atoi(p_t) > 0) { //환승 가능한 구간인 경우
				find_y = 0;
				while (find_y < 553) //반대편 도착지의 노드가 그래프 g에서 어디에 있는지 위치를 찾는 코드
				{
					if (strcmp(st_trans_num[y_pos], st_num[find_y]) == 0)
						break;
					else {
						find_y++;
					}
				}
				if (how == 1) //최단거리로 찾는 경우
				{
					int weight_tmp = atoi(p_t);
					int value = rand() % weight_tmp + 1; //환승시간을 읽어와 0~읽은시간 난수 생성
					g.weight[find_x][find_y] = value;
				}
				else if (how == 2) //최소환승으로 찾는 경우
				{
					int weight_tmp = atoi(p_t);
					int value = rand() % weight_tmp + 1; //환승시간을 읽어와 0~읽은시간 난수 생성
					value = value + 200; //최소환승 루트로 계산하기 위해 여기에 200을 더하여 최대한 환승을 덜하게 계산하도록 만듬
					g.weight[find_x][find_y] = value;
				}
				p_t = strtok(NULL, ",");
				y_pos++;
			}
			else if (atoi(p_t) == 9999 || atoi(p_t) == 0) { //환승경로가 아닌경우 그냥 pass
				p_t = strtok(NULL, ",");
				y_pos++;
			}
			if (y_pos == TRANS_VERTICES) break; //한 라인 다읽으면 종료
		}
		check_line++;
		if (check_line == TRANS_VERTICES) break; //모든 라인의 데이터 다읽으면 종료
	}
	fclose(in); //파일 닫음
}

int main() {
	load_line_csv(); //모든 노선의 csv를 읽어옴
	char start[20]; //시작역을 사용자로부터 입력받을 변수
	int start_i; //시작역의 노드 번호
	char end[20]; //도착역을 사용자로부터 입력받을 변수
	int end_i; //도착역의 노드 번호
	int how; //최단경로인지 최소환승인지 방식을 입력받을 변수
	while (1) {
		while (1) //출발역 입력받는 while 루프
		{
			start_i = 0;
			printf("출발역 : ");
			scanf("%s", start); //출발역 입력받아서 start 변수에 넣어줌
			while (start_i < 553) //출발역의 노드 번호를 찾는 while 루프 코드
			{
				if (strcmp(st_name[start_i], start) == 0) { //출발역으로 입력받은 이름과 st_name배열안에 데이터 중 같은 이름을 갖는 경우 
					break; //while 루프 종료
				}
				start_i++;
			}
			if (start_i == 553) //while 루프가 전부 돌았지만 입력받은 이름이 역 이름 배열에 없는 경우
				printf("[오류] 잘못된 이름입니다.\n");
			else if (start_i < 553)  //있는 경우
				break;
		}
		while (1) //도착역 입력받는 while 루프
		{
			end_i = 0;
			printf("도착역 : ");
			scanf("%s", end); //도착역 입력받아서 end 변수에 넣어줌
			while (end_i < 553)
			{
				if (strcmp(st_name[end_i], end) == 0) { //도착역으로 입력받은 이름과 st_name배열안에 데이터 중 같은 이름을 갖는 경우 
					break; //while 루프 종료
				}
				end_i++;
			}
			if (end_i == 553) //while 루프가 전부 돌았지만 입력받은 이름이 역 이름 배열에 없는 경우
				printf("[오류] 잘못된 이름입니다.\n");
			else if (end_i < 553) //있는 경우
				break;
		}
		if (start_i != end_i) //시작역과 출발역이 다른 경우에 정상적으로 진행
		{
			while (1) { //경로 방식을 물어봄
				printf("방식? 1. 최단경로 2. 최소환승\n:");
				scanf("%d", &how); //방식을 how 변수에 저장
				if (how == 1 || how == 2) //1이나 2를 입력받은 경우 while 루프 종료
					break;
				else //1이나 2가 아닌 다른 것을 입력했을 경우
					printf("1이나 2를 입력하십시요.\n");
			}
			break;	//출발역, 도착역, 경로 방식을 입력받는 전체 while 루프를 종료
		}
		else printf("[오류] 출발역과 도착역이 같습니다.\n"); //출발역과 도착역을 같은 이름으로 입력한 경우 다시 처음부터 while 루프
	}
	load_trans_csv(how); //경로 방식을 인자로 넣어주고 환승정보 csv파일을 읽어옴
	if (how == 1) printf("\n최단경로\n");
	else if (how == 2) printf("\n최소환승\n");

	shortest_path(&g, start_i); //시작역에 대하여 최단경로를 탐색
	print_path(start_i, end_i); //시작역에서 도착역까지의 경로를 출력
	return 0;
}
