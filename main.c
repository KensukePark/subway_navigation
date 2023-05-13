#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 552 //�� �� ����
#define INF   9999   // ���Ѵ� (������ ���� ���) 
#define NODES  552 // g->n ��, ������ �� ���� ����
#define TRANS_VERTICES 150 //ȯ�¿� ����

typedef struct GraphType {
	int n;   // ������ ����
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES];// �����������κ����� �ִܰ�� �Ÿ� 
int found[MAX_VERTICES]; //* �湮�� ���� ǥ�� 
int path[NODES]; // ��� ���� �迭 
char station[MAX_VERTICES][50]; // �� �� csv���� ���� ����ȣ ���� �迭 
char st_num[MAX_VERTICES][50]; // ���̸� csv���� ���� ����ȣ ���� �迭 
char st_name[MAX_VERTICES][50]; // ���̸� csv���� ���� ���̸� ���� �迭 
char st_trans_num[TRANS_VERTICES][50]; // ȯ�� csv �� �� ù��° ���� �� ��ȣ ���� �迭
char st_trans_num_2[TRANS_VERTICES][50]; // ȯ�� csv ù �����ٿ� �ִ� �� ��ȣ ���� �迭
GraphType g; //�׷��� g

/*
�Լ��� : choose
���� : int distance �迭, int n, int found �迭
���� : minpos
���� : distance �迭�� �ִ� ���� ���� ���� ��带 �����Ͽ� idx�� return�ϴ� �Լ�
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
	//ã�� ����� idx�� ������ minpos�� ��ȯ
	return minpos;
}

/*
�Լ��� : print_path
���� : int start, int end
���� : �ִ� ��θ� ����ϴ� �Լ�
*/
void print_path(int start, int end)
{
	int i = 0;
	int start_loc; //���ۿ��� ��� ��ȣ�� ������ ����
	int end_loc; //�������� ��� ��ȣ�� ������ ����
	while (i < 553) //while������ ���ۿ��� �������� ��� ��ȣ�� ã��
	{
		if (strcmp(station[start], st_num[i]) == 0)
			start_loc = i;
		if (strcmp(station[end], st_num[i]) == 0)
			end_loc = i;
		i++;
	}
	int j = 1;
	int path_tmp[MAX_VERTICES]; //��� �ӽ� ���� ����
	int trans_time = 0; //�� ȯ�� �ð� ��¿�
	int trans_count = 0; //ȯ�� Ƚ�� �����
	int total; //�� �ҿ�ð� ��¿�
	path_tmp[0] = end; //0�� ������ ����
	while (path_tmp[j - 1] != start) //while ������ ��� ����
	{
		path_tmp[j] = path[path_tmp[j - 1]];
		j++;
	}
	printf("<���>\n");
	char tmp[20];
	int j_tmp = j; //��γ��� �� ���� �ӽ� ����, ������ �� ��¿�
	int weight_tmp = 0; //�������� ȯ�� ��η� ���� �þ ȯ�½ð����� ������ ���� ex) (�����߾�)���->(1ȣ��)��� <����>

	/*
	���۳�忡�� �ٷ� ȯ���� ��� ó�� �ڵ�
	ex) ���ۿ��� ������� �Է����� ��
	���۳�尡 1ȣ�� ������� �����Ǿ��µ�
	�� �����߾� ������� ȯ���ϴ� Case
	���� ���� ��� ó���ϴ� �Լ�
	*/
	if (abs(path_tmp[j - 1] - path_tmp[j - 2]) != 1)
	{
		weight_tmp = g.weight[path_tmp[j - 1]][path_tmp[j - 2]];  //���ۿ� -> ���ۿ�(�ٸ�ȣ��) ����Ÿ�µ� �ҿ�� �ð� ����
		j--; //�̴� �ʿ���� ����̹Ƿ� j-- �Ͽ� ȯ���� ���¿��� ��� ��� ����
		j_tmp--; //������ ���� ����
	}
	while (j > 1) //��߿����� ������ �ٷ� �� �������� ��θ� ���
	{
		if (path_tmp[j - 1] < 62) strcpy(tmp, "1ȣ��");
		else if (62 <= path_tmp[j - 1] && path_tmp[j - 1] < 101) strcpy(tmp, "1����");
		else if (101 <= path_tmp[j - 1] && path_tmp[j - 1] < 144) strcpy(tmp, "2ȣ��");
		else if (144 <= path_tmp[j - 1] && path_tmp[j - 1] < 154) strcpy(tmp, "2����");
		else if (154 <= path_tmp[j - 1] && path_tmp[j - 1] < 197) strcpy(tmp, "3ȣ��");
		else if (197 <= path_tmp[j - 1] && path_tmp[j - 1] < 245) strcpy(tmp, "4ȣ��");
		else if (245 <= path_tmp[j - 1] && path_tmp[j - 1] < 289) strcpy(tmp, "5ȣ��");
		else if (289 <= path_tmp[j - 1] && path_tmp[j - 1] < 297) strcpy(tmp, "5����");
		else if (297 <= path_tmp[j - 1] && path_tmp[j - 1] < 335) strcpy(tmp, "6����");
		else if (335 <= path_tmp[j - 1] && path_tmp[j - 1] < 377) strcpy(tmp, "7ȣ��");
		else if (377 <= path_tmp[j - 1] && path_tmp[j - 1] < 394) strcpy(tmp, "8ȣ��");
		else if (394 <= path_tmp[j - 1] && path_tmp[j - 1] < 419) strcpy(tmp, "9ȣ��");
		else if (101 <= path_tmp[j - 1] && path_tmp[j - 1] < 444) strcpy(tmp, "�д缱");
		else if (144 <= path_tmp[j - 1] && path_tmp[j - 1] < 473) strcpy(tmp, "��õ1��");
		else if (154 <= path_tmp[j - 1] && path_tmp[j - 1] < 501) strcpy(tmp, "�߾Ӽ�");
		else if (197 <= path_tmp[j - 1] && path_tmp[j - 1] < 522) strcpy(tmp, "���ἱ");
		else if (245 <= path_tmp[j - 1] && path_tmp[j - 1] < 542) strcpy(tmp, "���Ǽ�");
		else if (289 <= path_tmp[j - 1] && path_tmp[j - 1] < 552) strcpy(tmp, "����ö��");
		//csv�� �а� ������ ������ �̿��� �ش� ���� ��ȣ������ Ȯ��

		//ȯ���� ��� ȯ�� ��� �ڵ�
		if ((abs(path_tmp[j - 1] - path_tmp[j - 2]) > 1 || strcmp(st_name[path_tmp[j - 1]], st_name[path_tmp[j - 2]]) == 0) && j != 2)
		{
			int time_temp = g.weight[path_tmp[j - 1]][path_tmp[j - 2]]; //ȯ�� �ҿ�ð��� ����
			if (time_temp > 200) time_temp -= 200;  //�ҿ�ð��� 200���� ū ���, �� �ּ�ȯ������ ���� ��� 200��ŭ ����

			printf("-><ȯ�� : �ҿ�ð� %d ��> %s", time_temp, st_name[path_tmp[j - 1]]); // ȯ�� �ҿ�ð� ���
			trans_time = trans_time + g.weight[path_tmp[j - 1]][path_tmp[j - 2]]; //�� ȯ�½ð��� �߰�
			trans_count++; //ȯ�� Ƚ�� �߰�
			j = j - 2; //ȯ���� �������� �ѱ������ j=j-2
			j_tmp--; //������ �� ����
		}
		else //ȯ�� �ƴѰ��
		{
			printf("-><%s> %s", tmp, st_name[path_tmp[j - 1]]);
			j--;
		}
		if (j != 1) //�ٹٲ� ó���� �ڵ�
			printf("\n");
	}

	/*
	������尡 1ȣ�� ������� �������
	������ ���� �����߾� ����� ����
	1ȣ�� ������� ȯ���Ͽ� ���� �����ϰ� �Ǵ� ���
	*/
	if (strcmp(st_name[path_tmp[0]], st_name[path_tmp[1]]) == 0)
	{
		int total;
		weight_tmp = weight_tmp + g.weight[path_tmp[1]][path_tmp[0]];
		j_tmp--; //�� ������� �ϳ� ����
		printf(" ����\n");
		/*
		�ּ�ȯ������ ���� ���
		�ҿ�ð��� ���������� �����ϴ� �ڵ�
		*/
		if (trans_time > 200) { //�ּ�ȯ������ �������� �� ȯ�½ð��� 200���� ũ�Ƿ�
			trans_time -= trans_count * 200; //ȯ��Ƚ��*200�� ���ָ� ���� ȯ�½ð��� ����
			total = distance[path_tmp[0]] - weight_tmp; //�������� ȯ�� ��η� ���� ȯ�� �ð��� ����
			total -= trans_count * 200;
		}
		else total = distance[path_tmp[0]] - weight_tmp; //�ִܰ�η� ���� ��� ���� �������� �ٷ� �������� ȯ�½ð��� ���ָ� ��

		printf("\n�ҿ�ð� : %d (%d + ȯ�� �ҿ�ð� : %d) ��\n", total, total - trans_time, trans_time);
		printf("������ �� : %d ��\n", j_tmp);
		return; //�� �ҿ�ð� ����� �Լ� ����
	}

	//���������� ���������� ������ ���
	if (path_tmp[0] < 62) strcpy(tmp, "1ȣ��");
	else if (62 <= path_tmp[0] && path_tmp[0] < 101) strcpy(tmp, "1����");
	else if (101 <= path_tmp[0] && path_tmp[0] < 144) strcpy(tmp, "2ȣ��");
	else if (144 <= path_tmp[0] && path_tmp[0] < 154) strcpy(tmp, "2����");
	else if (154 <= path_tmp[0] && path_tmp[0] < 197) strcpy(tmp, "3ȣ��");
	else if (197 <= path_tmp[0] && path_tmp[0] < 245) strcpy(tmp, "4ȣ��");
	else if (245 <= path_tmp[0] && path_tmp[0] < 289) strcpy(tmp, "5ȣ��");
	else if (289 <= path_tmp[0] && path_tmp[0] < 297) strcpy(tmp, "5����");
	else if (297 <= path_tmp[0] && path_tmp[0] < 335) strcpy(tmp, "6����");
	else if (335 <= path_tmp[0] && path_tmp[0] < 377) strcpy(tmp, "7ȣ��");
	else if (377 <= path_tmp[0] && path_tmp[0] < 394) strcpy(tmp, "8ȣ��");
	else if (394 <= path_tmp[0] && path_tmp[0] < 419) strcpy(tmp, "9ȣ��");
	else if (101 <= path_tmp[0] && path_tmp[0] < 444) strcpy(tmp, "�д缱");
	else if (144 <= path_tmp[0] && path_tmp[0] < 473) strcpy(tmp, "��õ1��");
	else if (154 <= path_tmp[0] && path_tmp[0] < 501) strcpy(tmp, "�߾Ӽ�");
	else if (197 <= path_tmp[0] && path_tmp[0] < 522) strcpy(tmp, "���ἱ");
	else if (245 <= path_tmp[0] && path_tmp[0] < 542) strcpy(tmp, "���Ǽ�");
	else if (289 <= path_tmp[0] && path_tmp[0] < 552) strcpy(tmp, "����ö��");
	//csv�� �а� ������ ������ �̿��� ������ �������� ��ȣ������ Ȯ��
	printf("\n-><%s> %s <����> \n", tmp, st_name[path_tmp[0]]);

	/*
	�ּ�ȯ������ ���� ���
	�ҿ�ð��� ���������� �����ϴ� �ڵ�
	*/
	if (trans_time > 200) { //�ּ�ȯ������ �������� �� ȯ�½ð��� 200���� ũ�Ƿ�
		trans_time -= trans_count * 200; //ȯ��Ƚ��*200�� ���ָ� ���� ȯ�½ð��� ����
		total = distance[path_tmp[0]] - weight_tmp; //�������� ȯ�� ��η� ���� ȯ�� �ð��� ����
		total -= trans_count * 200;
	}
	else total = distance[path_tmp[0]] - weight_tmp;  //�ִܰ�η� ���� ��� ���� �������� �ٷ� �������� ȯ�½ð��� ���ָ� ��

	printf("\n�ҿ�ð� : %d (%d + ȯ�� �ҿ�ð� : %d) ��\n", total, total - trans_time, trans_time);
	printf("������ �� : %d ��\n", j_tmp);
	return; //�� �ҿ�ð� ����� �Լ� ����
}

/*
�Լ��� : shortest_path
���� : GraphType* g, int start
���� : �׷��� g�� �̿��� ���� ��忡�� �����ϴ� �ִ� ��θ� ã�� �Լ�
*/
void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	int s = start;
	for (i = 0; i < g->n; i++) //�ʱ�ȭ �ڵ�
	{
		distance[i] = g->weight[start][i];
		found[i] = FALSE;
		path[i] = s;
	}
	found[start] = TRUE; //���� ��� �湮 ǥ��
	distance[start] = 0; //���� ������ ���� 0����
	for (i = 0; i < g->n - 1; i++) {
		u = choose(distance, g->n, found); //���� ���� ���� ���� idx�� ã�� ���� u��
		found[u] = TRUE; //u �湮ǥ��
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w])
				{ //������ �Ÿ����� (u������ �Ÿ�+ u���� w������ �Ÿ�)�� �� ª���� distance�� path�� ����
					distance[w] = distance[u] + g->weight[u][w];
					path[w] = u;
				}
	}
}

/*
�Լ��� : load_line_csv
���� : ��� �뼱�� csv ������ �о� �׷���g�� �����ϴ� �Լ�
*/
void load_line_csv()
{
	int max = 9999;
	char line[9999];
	char* p;
	g.n = 0; //���� 0���� �ʱ�ȭ
	for (int k = 0; k < MAX_VERTICES; k++)
	{
		for (int l = 0; l < MAX_VERTICES; l++)
		{
			g.weight[k][l] = 9999; //��� weight�� 9999�� �ʱ�ȭ
		}
	}
	int x = 0; //�׷����� weight�� �����Ҷ� ����� ����1
	int y; //�׷����� weight�� �����Ҷ� ����� ����2
	int j = 0; //���� �� ������ ������ ����
	//1ȣ�� csv �ε�
	FILE* in = fopen("1ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����(������ ����)
	while (!feof(in)) {
		y = 0;
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); //, �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 62) break; //1ȣ�� �� ���� ���� 62���̹Ƿ� ����� ������ 62���� ����
	}
	fclose(in);//���� ����

	//1���� csv �ε�
	int temp_y = y; //���� y���� �ӽ÷� ���� 
	in = fopen("1����.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� �� ���� ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 101) break; //����� ������ ���±��� ������ ���� ���� + 1������ ���� ������ 101���� ����
	}
	fclose(in); //���� ����

	//2ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("2ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ",");  // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 144) break; //����� ������ ���±��� ������ ���� ���� + 2ȣ���� ���� ������ 144���� ����
	}
	fclose(in); //���� ����

	//2���� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("2����.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL) {
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 154) break; //����� ������ ���±��� ������ ���� ���� + 2������ ���� ������ 154���� ����
	}
	fclose(in); //���� ����

	//3ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("3ȣ��.csv", "r");//���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 197) break; //����� ������ ���±��� ������ ���� ���� + 3ȣ���� ���� ������ 197���� ����
	}
	fclose(in); //���� ����

	//4ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("4ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p);//weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 245) break; //����� ������ ���±��� ������ ���� ���� + 4ȣ���� ���� ������ 245���� ����
	}
	fclose(in); //���� ����

	//5ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("5ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 289) break; //����� ������ ���±��� ������ ���� ���� + 5ȣ���� ���� ������ 289���� ����
	}
	fclose(in); //���� ����

	//5���� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("5����.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y;  //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 297) break; //����� ������ ���±��� ������ ���� ���� + 5������ ���� ������ 297���� ����
	}
	fclose(in); //���� ����

	//6ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("6ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y;  //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 335) break; //����� ������ ���±��� ������ ���� ���� + 6ȣ���� ���� ������ 335���� ����
	}
	fclose(in); //���� ����

	//7ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("7ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 377) break; //����� ������ ���±��� ������ ���� ���� + 7ȣ���� ���� ������ 377���� ����
	}
	fclose(in); //���� ����

	//8ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("8ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 394) break; //����� ������ ���±��� ������ ���� ���� + 8ȣ���� ���� ������ 394���� ����
	}
	fclose(in); //���� ����

	//9ȣ�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("9ȣ��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 419) break;  //����� ������ ���±��� ������ ���� ���� + 9ȣ���� ���� ������ 419���� ����
	}
	fclose(in); //���� ����

	//�д缱 csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("�д缱.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 444) break; //����� ������ ���±��� ������ ���� ���� + �д缱�� ���� ������ 444���� ����
	}
	fclose(in); //���� ����

	//��õ1�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("��õ1��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in);  //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 473) break; //����� ������ ���±��� ������ ���� ���� + ��õ1ȣ���� ���� ������ 473���� ����
	}
	fclose(in); //���� ����

	//�߾Ӽ� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("�߾Ӽ�.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ","); //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 501) break; //����� ������ ���±��� ������ ���� ���� + �߾Ӽ��� ���� ������ 501���� ����
	}
	fclose(in); //���� ����

	//���ἱ csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("���ἱ.csv", "r");  //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ",");  //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 522) break; //����� ������ ���±��� ������ ���� ���� + ���ἱ�� ���� ������ 522���� ����
	}
	fclose(in); //���� ����

	//���Ǽ� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("���Ǽ�.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ",");  //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 542) break; //����� ������ ���±��� ������ ���� ���� + ���Ǽ��� ���� ������ 542���� ����
	}
	fclose(in); //���� ����

	//����ö�� csv �ε�
	temp_y = y; //���� y���� �ӽ÷� ����
	in = fopen("����ö��.csv", "r"); //���� ����
	fgets(line, max, in); //ù �� ����
	while (!feof(in)) {
		y = temp_y; //y�� �ӽ������ߴ� temp_y������ �ʱ�ȭ
		fgets(line, max, in); //���� ������ ����
		p = strtok(line, ","); // , �������� �ϳ��� �и�
		strcpy(station[j], p); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� station �迭�� ����
		j++;
		p = strtok(NULL, ",");  //, �������� ������ ����
		while (p != NULL)
		{
			g.weight[x][y] = atoi(p); //weight�� �׷��� g�� �־���
			y++;
			p = strtok(NULL, ","); // , �������� ������ ����
		}
		x++;
		g.n++;
		if (g.n == 552) break; //����� ������ ���±��� ������ ���� ���� + ����ö���� ���� ������ 552���� ����
	}
	fclose(in); //���� ����

	//���̸� csv �ε�
	char line_n[9999];
	char* p_n;
	int loc = 0; //�迭�� �ε����� �̿��� ����
	in = fopen("���̸�.csv", "r"); //���� ����
	fgets(line_n, max, in); //ù �� ����
	while (!feof(in)) {
		fgets(line_n, max, in); //���� �� ���� ����
		p_n = strtok(line_n, ","); // , �������� �ϳ��� �и�
		strcpy(st_num[loc], p_n); //ù ���ڿ��� ���� ��ȣ�̹Ƿ� �̸� st_num �迭�� ����
		p_n = strtok(NULL, "\n"); // , �������� ���� �ϳ��� �и�
		strcpy(st_name[loc], p_n); //���� ���ڿ��� ���� �ѱ� �̸��̹Ƿ� �̸� st_name �迭�� ����
		loc++;
		if (loc == MAX_VERTICES) //�� ���� ������ 552���� �Ǹ� while ���� ����
			break;
	}
	fclose(in); //���� ����
}

/*
�Լ��� : load_trans_csv
���� : int how
���� : ��� ���(�ִܰŸ�or�ּ�ȯ��)�� ����
ȯ������ csv ������ �о� �׷��� g�� �����ϴ� �Լ�
*/
void load_trans_csv(int how)
{
	//ȯ������ csv �ε�
	//st_trans_num �迭�� �� ��ȣ���� �����ϰ� ���� ����
	srand(time(NULL)); //���� ������ ���� �õ带 �Ź� ����
	char line_t[9999];
	char* p_t;
	int max = 9999;
	FILE* in = fopen("ȯ������.csv", "r"); //���� ����
	fgets(line_t, max, in); //ù ���� ����
	int num_of_trans = 0; //�� ��ȣ�� ������ �� �ε����� ����� ����
	while (num_of_trans < TRANS_VERTICES)
	{
		fgets(line_t, max, in); // ���� �� ���� ����
		p_t = strtok(line_t, ","); // , �������� �ϳ��� �и�
		strcpy(st_trans_num[num_of_trans], p_t); //st_trans_num �迭�� ����
		num_of_trans++;
	}
	fclose(in); //���� ����

	//ȯ������ csv �ε�2
	//ȯ�½��� ������ weight�� �����ϱ� ���� ����
	in = fopen("ȯ������.csv", "r"); //���� ����
	fgets(line_t, max, in); // ù ���� ����
	int y_pos; //
	int check_line = 0; //�� ��° ���� �д��� üũ�ϱ����� ����
	int find_x; //�׷��� g�� ����� ��ġ�� ����� ����1
	int find_y; //�׷��� g�� ����� ��ġ�� ����� ����1
	while (1)
	{
		fgets(line_t, max, in); //���� �� ���� ����
		y_pos = 0;
		p_t = strtok(line_t, ","); // ,�� �������� �ϳ��� ����
		find_x = 0;
		while (1) //���� ���� ��尡 �׷��� g���� ��� �ִ��� ��ġ�� ã�� �ڵ�
		{
			if (strcmp(st_trans_num[check_line], station[find_x]) == 0)
				break;
			else {
				find_x++;
			}
		}
		p_t = strtok(NULL, ","); // ,�� �������� �ϳ��� ����
		while (1)
		{
			if (atoi(p_t) < 9999 && atoi(p_t) > 0) { //ȯ�� ������ ������ ���
				find_y = 0;
				while (find_y < 553) //�ݴ��� �������� ��尡 �׷��� g���� ��� �ִ��� ��ġ�� ã�� �ڵ�
				{
					if (strcmp(st_trans_num[y_pos], st_num[find_y]) == 0)
						break;
					else {
						find_y++;
					}
				}
				if (how == 1) //�ִܰŸ��� ã�� ���
				{
					int weight_tmp = atoi(p_t);
					int value = rand() % weight_tmp + 1; //ȯ�½ð��� �о�� 0~�����ð� ���� ����
					g.weight[find_x][find_y] = value;
				}
				else if (how == 2) //�ּ�ȯ������ ã�� ���
				{
					int weight_tmp = atoi(p_t);
					int value = rand() % weight_tmp + 1; //ȯ�½ð��� �о�� 0~�����ð� ���� ����
					value = value + 200; //�ּ�ȯ�� ��Ʈ�� ����ϱ� ���� ���⿡ 200�� ���Ͽ� �ִ��� ȯ���� ���ϰ� ����ϵ��� ����
					g.weight[find_x][find_y] = value;
				}
				p_t = strtok(NULL, ",");
				y_pos++;
			}
			else if (atoi(p_t) == 9999 || atoi(p_t) == 0) { //ȯ�°�ΰ� �ƴѰ�� �׳� pass
				p_t = strtok(NULL, ",");
				y_pos++;
			}
			if (y_pos == TRANS_VERTICES) break; //�� ���� �������� ����
		}
		check_line++;
		if (check_line == TRANS_VERTICES) break; //��� ������ ������ �������� ����
	}
	fclose(in); //���� ����
}

int main() {
	load_line_csv(); //��� �뼱�� csv�� �о��
	char start[20]; //���ۿ��� ����ڷκ��� �Է¹��� ����
	int start_i; //���ۿ��� ��� ��ȣ
	char end[20]; //�������� ����ڷκ��� �Է¹��� ����
	int end_i; //�������� ��� ��ȣ
	int how; //�ִܰ������ �ּ�ȯ������ ����� �Է¹��� ����
	while (1) {
		while (1) //��߿� �Է¹޴� while ����
		{
			start_i = 0;
			printf("��߿� : ");
			scanf("%s", start); //��߿� �Է¹޾Ƽ� start ������ �־���
			while (start_i < 553) //��߿��� ��� ��ȣ�� ã�� while ���� �ڵ�
			{
				if (strcmp(st_name[start_i], start) == 0) { //��߿����� �Է¹��� �̸��� st_name�迭�ȿ� ������ �� ���� �̸��� ���� ��� 
					break; //while ���� ����
				}
				start_i++;
			}
			if (start_i == 553) //while ������ ���� �������� �Է¹��� �̸��� �� �̸� �迭�� ���� ���
				printf("[����] �߸��� �̸��Դϴ�.\n");
			else if (start_i < 553)  //�ִ� ���
				break;
		}
		while (1) //������ �Է¹޴� while ����
		{
			end_i = 0;
			printf("������ : ");
			scanf("%s", end); //������ �Է¹޾Ƽ� end ������ �־���
			while (end_i < 553)
			{
				if (strcmp(st_name[end_i], end) == 0) { //���������� �Է¹��� �̸��� st_name�迭�ȿ� ������ �� ���� �̸��� ���� ��� 
					break; //while ���� ����
				}
				end_i++;
			}
			if (end_i == 553) //while ������ ���� �������� �Է¹��� �̸��� �� �̸� �迭�� ���� ���
				printf("[����] �߸��� �̸��Դϴ�.\n");
			else if (end_i < 553) //�ִ� ���
				break;
		}
		if (start_i != end_i) //���ۿ��� ��߿��� �ٸ� ��쿡 ���������� ����
		{
			while (1) { //��� ����� ���
				printf("���? 1. �ִܰ�� 2. �ּ�ȯ��\n:");
				scanf("%d", &how); //����� how ������ ����
				if (how == 1 || how == 2) //1�̳� 2�� �Է¹��� ��� while ���� ����
					break;
				else //1�̳� 2�� �ƴ� �ٸ� ���� �Է����� ���
					printf("1�̳� 2�� �Է��Ͻʽÿ�.\n");
			}
			break;	//��߿�, ������, ��� ����� �Է¹޴� ��ü while ������ ����
		}
		else printf("[����] ��߿��� �������� �����ϴ�.\n"); //��߿��� �������� ���� �̸����� �Է��� ��� �ٽ� ó������ while ����
	}
	load_trans_csv(how); //��� ����� ���ڷ� �־��ְ� ȯ������ csv������ �о��
	if (how == 1) printf("\n�ִܰ��\n");
	else if (how == 2) printf("\n�ּ�ȯ��\n");

	shortest_path(&g, start_i); //���ۿ��� ���Ͽ� �ִܰ�θ� Ž��
	print_path(start_i, end_i); //���ۿ����� ������������ ��θ� ���
	return 0;
}
