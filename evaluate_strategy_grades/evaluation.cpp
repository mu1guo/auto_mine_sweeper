#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
using namespace std;


/********************************************************************************/
/********************************��ѧ���Ĵ�����ӵ�����******************************/

//����������
void copy_matrix(int a[30][30], int b[30][30]); //���ڶ�������copy��ȥ
bool is_logic_right(int GamePanel[30][30], int n, int m);//�ж�ģ�� ֮���Ƿ�logic��ȷ
void find_mine_2(int GamePanel[30][30], int n, int m); //�ڶ����ҵ��׵ĺ���
bool is_matrix_equal(int a[30][30], int b[30][30]);  //�ж�2�������Ƿ�ֵ ��ȫ��ͬ
void find_mine_1(int GamePanel[30][30], int n, int m); //��һ�� �ҵ��׵ķ���
void probable_matrix_cal(double M_probable[30][30], int GamePanel[30][30], int n, int m); //����-1δ�򿪿ո��ǵ��׵ĸ���,�洢��M_probable��double������
double probable_cnt(double M_probable[30][30], int GamePanel[30][30], int i, int j, int n, int m); //��Χ��-1��ĸ���ֵ֮��


void machine(int GamePanel[30][30], int n, int m, int &x, int &y)
{

	//(һ)��ʼʱ�����ѡ��һ��λ�ã� 
	x = rand() % n;
	y = rand() % m;  //��ʼ��ʱ�����ѡ��һ��λ�� 

	//����ǵ�һ�ε������õ��4�����䣬Ҳ����
	int null_matrix[30][30] = { 0 };
	if (is_matrix_equal(null_matrix, GamePanel))
	{
		x = 0;
		y = 0;
		return;
	}
	else
	{
		//����һ��-1��λ��,��ֹ֮��� ����ȫ��ʧЧ 
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (GamePanel[i][j] == -1)
				{
					x = i;
					y = j;
					//�ҵ����һ��λ��Ҳ�� 
				}
			}
		}
	}


	//(��)һ����Ҫ���������� 
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	int whiteNum = 0, mineCnt = 0;  //��¼ û�д򿪵�λ�õ���Ŀ  �� ȷ���ǵ��׵���Ŀ



	//(��)��ѭ���е��� 2���߼�����ȷ�����ҳ���Щһ���� ���׵�λ�ã�������panel�б��Ϊ-2 

	while (1)
	{
		//����һ�� copy���������жϾ����Ƿ��и���
		int panel_before[30][30] = { 0 };
		copy_matrix(panel_before, GamePanel);

		//(1)�ȵ��õ�һ�� find_mine
		find_mine_1(GamePanel, n, m);
		//(2)�ٵ��õڶ��� find_mine
		find_mine_2(GamePanel, n, m);
		//(3)�ж�GamePanle��û�и���
		if (is_matrix_equal(panel_before, GamePanel))
		{
			break; //˵���������ҵ�������
		}
	}

	//(��)����õ�һ�����ʾ���M_probable,����洢����Щ �����֡���Χû�д򿪵ĸ��� ���ǵ��׵ĸ��ʡ� 

	double M_probable[30][30] = { 0 };//��ʼ��ȫ��Ϊ0��ֻ��¼����0��δ�򿪵ĸ��ӵĸ��ʣ�һ�����ǵ�������Ϊ0.00001
	probable_matrix_cal(M_probable, GamePanel, n, m);


	//�ڶ����֣�����ѡ�� ��Щ ��Χ����֮����С�� һ�����ǵ��׵� ����
	//(��)��ͨ������ķ����������������Ѿ�������һ���ǵ��׵�λ����panel�б�ǳ�����
	//������Щ �����Ѿ���ȫ����ǳ����ˣ� ���ھ�����Ҫ�� ���ȵ�� ��һ���ո� ����Խ�٣�����Խ��
	//Ϊ�˵������ һ����Χû�� ���׵� �㣬����ʹ�� ���ʾ��󣬶���Χ���ֵ��׵� ���ʽ�����ͣ�ÿ��ѡȡ����С
	//���Ǹ� �ո���е�����ҷ��أ������Ϳ��� �����ܵ� ����Щû�к͵������ڵ� һ��Ƭ
	double min_pro = 999; //ÿ�� �� һ�����ǵ��� �ĸ����м�����Χ�� ���ʺͣ����Һ� min_pro���бȽ�
	int min_x = 0;
	int min_y = 0;


	//�ҳ���Щһ�����ǵ��׵�λ�ã���������Χ�ĸ��ʺ� 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				whiteNum = 0;   // ÿһ��9������ ��¼δ�򿪵���Ŀ
				mineCnt = 0;    //��¼���׵���Ŀ
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							whiteNum++;
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineCnt++;
						}
					}
				}
				//����Ҳ���ǽ� �ҵ���С�� ����֮�� �ĵ�
				if (whiteNum > 0 && mineCnt == GamePanel[i][j])
				{
					for (int ii = 0; ii < 8; ii++)
					{
						if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
						{
							if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
							{
								double tmp_cnt = probable_cnt(M_probable, GamePanel, i + round[0][ii], j + round[1][ii], n, m);
								if (tmp_cnt <= min_pro)
								{
									min_x = i + round[0][ii];
									min_y = j + round[1][ii];
									min_pro = tmp_cnt;
								}
							}
						}
					}
				}
			}
		}
	}
	if (min_pro < 998) //����Щ�����У�ѡ�������С�� ��Ϊ ��һ�ε����λ�ã�Ȼ��ֱ�ӷ��� 
	{
		x = min_x;
		y = min_y;
		return;
	}





	//��ʼ�������´���İ���

	//(��)���ǰ��Ĳ���ȫ��ʧ�ܣ���ô���ǿ��� �������300�����ϵĸ���û�д򿪣����ȴ�4������
	//���� ��Щ ��Χֻ��һ�����ף�����ȴ��8��δ�򿪸��ӵ�λ�� 
	int count = 0;
	for (int i = 0; i < n; i++)  //�洢��n�� m����
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] == -1)
			{
				count++;  //�����¼ û�д򿪵ĸ��ӵ�����
			}
		}
	}

	whiteNum = 0;
	if (count > 300)  //�ڻ��д���300������û�д򿪵�����£��������ȴ�4������ĸ���
	{
		if (GamePanel[0][0] == -1)
		{
			x = 0; y = 0;
			return;
		}
		else if (GamePanel[0][m - 1] == -1)
		{
			x = 0; y = m - 1;
			return;
		}
		if (GamePanel[n - 1][0] == -1)
		{
			x = n - 1; y = 0;
			return;
		}
		if (GamePanel[n - 1][m - 1] == -1)
		{
			x = n - 1; y = m - 1;
			return;
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (GamePanel[i][j] == 1)  //���һ�����ӵĵ���ȷ���ˣ���ֻ��һ����˵������Χ�����٣�
				{
					for (int ii = 0; ii < 8; ii++)
					{
						if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
						{
							if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
							{
								whiteNum++;
							}
						}
						//������whiteNum��¼�� 9������Χû�д򿪵� ���׵���Ŀ
					}
					int iii = 0;
					if (whiteNum == 8)
					{
						iii = rand() % 8;  //����Χ���ѡȡһ��������ⶫ���������� 
						x = i + round[0][iii];
						y = j + round[1][iii];
						return;
					}
					//�����Χ��8�� �ո�û�д򿪣���ôֻ��1/8�ĸ��ʻ�ȵ���
				}

			}
		}
	}
	//(��)���+ä�²��ԣ�
	//��ʵ���������� �õ��˷�����Щ ������-1��λ�ã������Χ-1�Ƚ϶࣬��ô���Ե����ä�¡��� 
	//��Ϊ��ä�¡��п��ܴ� һ��Ƭ��ֱ��(����ǰ���2���߼�����)����ʤ�֣� 

	//��������εĲ��ԣ�û��ʲô�õĿո����ѡ����
	//�����Ͼ��� ��� ѡ����΢��һ���λ��
	x = rand() % 20;
	y = rand() % 20;
	int _tryy = 0;
	while (1)
	{
		_tryy++;      //��¼ѭ������
		x = rand() % n;
		y = rand() % m;  //x �� y�ֱ����ѡ��һ����ֵ
		if (GamePanel[x][y] == -1)  //��� ���x,y���ĸ���û�д�,Ȼ��������Χ������
		{  //������������� һ���ո���û�д򿪵�case�£���Χ�ĸ��ӵĴ���� 

			whiteNum = 0;
			mineCnt = 0;
			for (int i = 0; i < 8; i++)
			{
				if (GamePanel[x + round[0][i]][y + round[1][i]] == -1)  //��Χ δ���� 
				{
					whiteNum++;
				}

				else if (GamePanel[x + round[0][i]][y + round[1][i]] == -2) //��Χ������ 
				{
					mineCnt++;
				}
			}
			if (whiteNum >= 7 || mineCnt >= 5)  //�����Χ�� δ�򿪵�̫�࣬���� ��Χ�ĵ��׷��־Ͷ� ������ѭ������88��(��ֹ��ѭ��)
			{
				return; // ����88 ��û���ˣ�����
			}
			else if (_tryy > 88)
			{
				break;
			}
		}
	}



	//(��)���Ĳ��֣�

	//���ڵõ���������� ���ʾ���Ȼ������Щ�����в��� ���Ҹ�����С���Ǹ�λ����Ϊx �� y��ֵ
	double probable_min = 1;
	int pro_x = x;
	int pro_y = y;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			//���ǲ���Ϊ0�������
			if (M_probable[i][j] != 0 && M_probable[i][j] <= probable_min)
			{
				pro_x = i;
				pro_y = j;
			}
		}
	}
	if (probable_min < 0.99)
	{
		x = pro_x;
		y = pro_y;
	}

	return;

	//���岿�� --�ŵ���������

}


//1.copy_matrix,���ڶ���int ���͵ľ����ֵcopy����һ������
void copy_matrix(int a[30][30], int b[30][30])
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			a[i][j] = b[i][j];
		}
	}
}
//2.�ж�ģ��֮��� �����Ƿ�����ì�ܣ�Ҳ����˵��mineCnt > panel_copy[i][j],����� ������true
bool is_logic_right(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	int emptyNum = 0, mineNum = 0;  //��¼ û�е��׵���Ŀ  �� ȷ���ǵ��׵���Ŀ

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // ÿһ��9������ ��¼δ�򿪵���Ŀ
				mineNum = 0;    //��¼���׵���Ŀ
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							emptyNum++;
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineNum++;
						}
					}
				}
				//����Ҳ���ǽ� ���������߼����� �� ����ҵ���mineCnt > panel_copy[i][j]���ͷ��� false
				if (mineNum > GamePanel[i][j])
				{
					//cout << "�ڶ����߼��ж� ȷʵ��ʹ���ˣ�������������������������������";//���Է�������
					return false;
				}
			}
		}
	}
	return true;
}
//3.�ڶ����߼��ж� �ҵ��׵� ����
void find_mine_2(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	int emptyNum = 0, mineNum = 0;  //��¼ û�е��׵���Ŀ  �� ȷ���ǵ��׵���Ŀ
	//���岿�֣���ӵĵڶ����߼��жϣ����ʣ���δ�򿪵Ŀո����Ŀ��ʣ��ĵ���+1���Ϳ���ͨ��ģ�⣬�ҳ��Ǹ�����
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // ÿһ��9������ ��¼δ�򿪵���Ŀ
				mineNum = 0;    //��¼���׵���Ŀ
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							emptyNum++;
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineNum++;
						}
					}
				}

				//�����Ѿ��������� ���� ���ܵ�δ�򿪵Ŀո����ĿwhiteNum���͵��׵���Ŀ mineCnt
				//��ô �ո��еĵ�����Ŀ��Ӧ���ǣ�remain_cnt
				int remain_cnt = GamePanel[i][j] - mineNum;
				//ֻ�е� whiteNum==remain_cnt+1��ʱ�򣬲���Ҫ����ģ��
				//����ģ��Ĵ����� whiteNum;
				if (remain_cnt != 0 && emptyNum == remain_cnt + 1) //ʣ��ĵ�����������0���Ǳ�Ȼ���ǵ���
				{
					//(1)copyһ��panel�������ģ��ʹ��
					int panel_copy[30][30] = { 0 };
					copy_matrix(panel_copy, GamePanel);
					//(2)����panel_copy����ģ��,�ܹ�ģ��whiteNum��
					//���β��� ��Щ��ֵ��-1�ĵ㣬Ȼ�� �����е�remain_cnt���ո�ֵΪ-2��Ȼ�������ж��߼�������
					int tmp_cnt = emptyNum;//��Ҫģ��Ĵ���

					int flag = 0;          //���Ϊ1˵���Ǹ� û�и�ֵΪ-2�ĵ�һ���ǵ��ף���GamePanel�и�ֵ-2

					while (tmp_cnt >= 1)
					{

						//cout<<"����ģ��"<<endl; 
						for (int ii = 0; ii < 8; ii++)
						{
							//���β�����Χ�� 8��λ��
							if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
							{
								if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
								{
									//ÿ��ģ����Ҫ����tmp_cnt2������!
									int tmp_cnt2 = emptyNum - 1; //ģ�� ��ֵ���״���

									//���е�����tmp_cnt��ģ�⣬����tmp_cnt--
									tmp_cnt--;
									int kk = ii;
									for (kk = ii; tmp_cnt2 >= 1; kk++) //ֻ�а����еĵ���ȫ�� ģ�����£����˳�ѭ�������� ȡģ �������ѭ��
									{
										kk = kk % 8;//kkѭ��ȡֵ 0-7

										if (i + round[0][kk] >= 0 && i + round[0][kk] < n && j + round[1][kk] >= 0 && j + round[1][kk] < m)
										{
											if (GamePanel[i + round[0][kk]][j + round[1][kk]] == -1)
											{
												//ֻҪ�ҵ��ˣ������ϸ�ֵΪ-2
												//�ܹ�ֻ�ܸ�ֵ tmp_cnt2��
												panel_copy[i + round[0][kk]][j + round[1][kk]] = -2;//����һ������
												tmp_cnt2--;

											}
										}
									}
									//�Ѿ��� ���һ������kkλ��++֮��� kkֵ��
									//��Ϊ��kk++,�ٽ����ж�

									//�������forѭ���Ѿ����������еĵ���,�����жϴ�ʱ��panel_copy�Ƿ�logic��ȷ
									if (!is_logic_right(panel_copy, n, m))
									{
										//˵���Ǹ���ȻΪ-1�Ŀո� һ���ǵ���
										int jj = 0;
										for (jj = 0; jj < 8; jj++) //ֻ�а����еĵ���ȫ�� ģ�����£����˳�ѭ�������� ȡģ �������ѭ��
										{
											jj = jj % 8;//jjѭ��ȡֵ 0-7

											if (i + round[0][jj] >= 0 && i + round[0][jj] < n && j + round[1][jj] >= 0 && j + round[1][jj] < m)
											{
												if (panel_copy[i + round[0][jj]][j + round[1][jj]] == -1)
												{
													//������GamePanel�н�����ֵ��ֵΪ-2
													GamePanel[i + round[0][jj]][j + round[1][jj]] = -2; //�ҵ�һ�� һ���ǵ���
													//�Ƿ���Ҫ���¼����������Χ��mineCnt �� whiteNum,Ӧ�ò���Ҫ����Ϊ����ֻ��һ�� 
													//cout << "�õڶ����߼��ж��ҳ��˵���!!" << endl;//����
													flag = 1;//ֻ���ҵ�һ��������������ѭ��
													break;
												}
											}
										}
									}

								}

								//else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
								//{
								//	//mineCnt++;
								//}
							}
							if (flag == 1) break;
						}
						//��������һ����ѭ�� --����ֻҪ�����ѭ���г���,Ӧ�þ�perfect�� 
						//cout<<"�˳�ģ��"<<endl; 
						if (flag == 1) break;
					}

					//���Ͻ����� ģ��
					if (flag == 1) break;
				}

				//���һ���е��׵ĸ��� ��Χ ��ģ��֮�󡣡���

			}
		}
	}
}
//4.�ж�2��int���;���� ���е�ֵ�Ƿ���ͬ�� �����������ͬ����true�����򷵻�false,�������þ�����Ϊ �ж�
//���� �߼��ж�֮�� ���Ƿ����ҳ��µĵ���-2�����û�� ��ֱ�� �˳�ѭ��
bool is_matrix_equal(int a[30][30], int b[30][30])
{
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (a[i][j] != b[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
//5.��һ���߼��жϣ���� ʣ�µ�δ�򿪵Ŀո���Ŀ == �м���ʾ�ĵ��׵���Ŀ - �Ѿ��ҳ��ĵ��׵���Ŀ����ô����ȫ�ǵ���
void find_mine_1(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	int emptyNum = 0, mineNum = 0;  //��¼ û�е��׵���Ŀ  �� ȷ���ǵ��׵���Ŀ
	//�����ǵ�һ���ж�:(�ҵ���)
	for (int i = 0; i < m; i++)  //��
	{
		for (int j = 0; j < n; j++) //��
		{
			emptyNum = 0;   //ע�⣺panel��-1-δ�򿪣�0�Ѿ�����δ��������ڣ�>0��ʾ���׵���Ŀ
			mineNum = 0;
			if (GamePanel[i][j] > 0)    //ÿ��ֻ��������������ڵĸ��ӣ�����Ҫ���з���
			{
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							emptyNum++;  //�Ѿ�δ�򿪵� �ط� --emptynum--�����¼ δ�򿪸�����Ŀ
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineNum++;  //�Ѿ�ȷ���ǵ��׵� �ط�--minenum--�����¼���������Χ�Ѿ�ȷ���ĵ�����Ŀ
						}
					}
				}
				//Ȼ�����һ��������Χʣ�µĴ���0�Ŀո���Ŀ ��  ��Χȷ���ĵ�����Ŀ��ͬ����ô������-2��ǳ���Щ����
				if (GamePanel[i][j] - mineNum == emptyNum && emptyNum > 0)
				{
					for (int ii = 0; ii < 8; ii++)
					{
						if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
						{
							if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
							{
								GamePanel[i + round[0][ii]][j + round[1][ii]] = -2;
							}
						}
					}
				}
			}
		}
	}
}
//6.����õ� ���ʾ���ĺ�����������Ľ���ŵ�matrix_probable��
void probable_matrix_cal(double M_probable[30][30], int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	int emptyNum = 0, mineNum = 0;  //��¼ û�е��׵���Ŀ  �� ȷ���ǵ��׵���Ŀ
	//���½��м��� -1��ĸ���
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // ÿһ��9������ ��¼δ�򿪵���Ŀ
				mineNum = 0;    //��¼���׵���Ŀ ,Ҳ����-2�ĸ���,����GamePanel[i][j]�ʹ�����Χ�ĵ�������
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							emptyNum++;
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineNum++;
						}
					}
				}//��Ҫ���� ���� �� δ�򿪵�����

				if (emptyNum > 0) //�����Χ�� δ�򿪶���û���ж�Ϊ���׵� ���ӵ���������0,����Ҫ�������ǵĸ���
				{
					for (int ii = 0; ii < 8; ii++)
					{
						if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
						{
							if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
							{
								//���������������׵����� �� [i][j]��������ͬ˵�� ȫ����һ����Ϊ ����
								if (GamePanel[i][j] == mineNum)
								{
									M_probable[i + round[0][ii]][j + round[1][ii]] = 0.00001;
								}
								else//���򣬾������������
								{
									double remain_mine = GamePanel[i][j] - mineNum;//ʣ����׵����� 
									double not_open = emptyNum;  //δ����Ŀ 
									double tmp_pro = remain_mine / not_open;
									if (M_probable[i + round[0][ii]][j + round[1][ii]] == 0)
									{
										M_probable[i + round[0][ii]][j + round[1][ii]] = tmp_pro;
									}
									else
									{
										//ԭ����һ������ֵ
										if (tmp_pro > M_probable[i + round[0][ii]][j + round[1][ii]])
										{
											//ֻ����μ���ĸ��ʸ������Ҫ���и���
											M_probable[i + round[0][ii]][j + round[1][ii]] = tmp_pro;
										}
									}
								}

							}

						}
					}
				}

			}
		}
	}
}

//7.������Χ�� �� �ǵ��׵� ����֮�͵� ����
double probable_cnt(double M_probable[30][30], int GamePanel[30][30], int i, int j, int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //�����Ǹ� 9����ƫ��
	//
	double tmp_cnt = 0; //��¼��Χ ����ֵ֮��
	for (int ii = 0; ii < 8; ii++)
	{
		if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
		{
			if (M_probable[i + round[0][ii]][j + round[1][ii]] > 0)
			{
				tmp_cnt += M_probable[i + round[0][ii]][j + round[1][ii]];
			}

		}
	}
	return tmp_cnt;
}

//��Ϣ��
//1.Gamepanel��0--n-1�� ��0--m-1�п���ʹ��
//�������panel�е���Ϣ����1��δ�򿪵ĸ���--��-1�� ��2���Ѿ��򿪵ĸ���--�����ڵĵ��������� 0-8֮��
//2.�������Ϣ�Ҳ���ʹ�ã�mine[i][j]����,��i�У���j�е�λ������ǵ��� mine[i][j]==1,����Ϊ0
//3.k--������׵�������
//���ͨ��Gamepanel����Ϣ��ȷ����һ����Ҫ����� ����λ�ø���x�� y



int getMineNum(int x, int y, int mine[30][30], int n, int m)   //�õ� �����ĵ��׵���Ŀ
{
	int num_mine = 0;
	int round1[8] = { -1,1,0,0,-1,1,1,-1 };
	int round2[8] = { 0,0,-1,1,-1,1,-1,1 };
	for (int i = 0; i < 8; i++)
	{
		if (x + round1[i] >= 0 && x + round1[i] < n && y + round2[i] >= 0 && y + round2[i] < m)
		{
			if (mine[x + round1[i]][y + round2[i]] == 1)num_mine++;
		}
	}
	return num_mine;
}
void extend(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m)//�������ã������е������հ״�
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 };
	if (GamePanel[x][y] == -1)
	{
		if (getMineNum(x, y, mine, n, m) > 0)
		{
			GamePanel[x][y] = getMineNum(x, y, mine, n, m);
			return;
		}
		if (getMineNum(x, y, mine, n, m) == 0)
		{
			GamePanel[x][y] = 0;
			for (int i = 0; i < 8; i++)
			{
				if (x + round[0][i] >= 0 && x + round[0][i] < n && y + round[1][i] >= 0 && y + round[1][i] < m)
					extend(GamePanel, x + round[0][i], y + round[1][i], mine, n, m);
			}
		}
	}
}
int state(int GamePanel[30][30], int mine[30][30], int n, int m, int k) //��ǰ��Ϸ�����״̬��ȷ���Ƿ�ֻʣ�µ���û�д�--����ǣ�����1
{
	int i, j, num_mine = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			if (GamePanel[i][j] == -1)num_mine++;
	if (num_mine == k)return 1;
	return 0;
}
int RefreshGPanel(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m, int k) //���x,yλ����1--�䣬���state==1,--��ʤ�����������Ϸ
{
	if (mine[x][y] == 1)return -1;
	else  extend(GamePanel, x, y, mine, n, m);
	if (state(GamePanel, mine, n, m, k) == 1) return 1;
	return 0;
}

/********************************��ѧ���Ĵ�����ӵ�����******************************/
/********************************************************************************/



int main()
{
	int GamePanel[30][30];
	int temp[30][30];
	int mine[30][30];
	int iii, ii, n, m, k, i, j, minenum, x, y, win = 0;
	int bonus;
	double score;
	double  maxscore = -999, minscore = 999, sumscore = 0;
	int gamenum = 1000;
	int winnum = 0;

	//ԭ���������е�n��m��ֵ������Ϊ20
	n = 20;
	m = 20;
	minenum = 50;
	//�ܹ�50������

	srand(int(time(0)));
	printf("���������У����Եȣ����ǽ�����1000����Ϸ��ȡƽ����*1.3��Ϊ���ճɼ�......\n");

	for (iii = 0; iii < gamenum; iii++)
	{

		for (i = 0; i < n; i++)
			for (j = 0; j < m; j++)
			{
				GamePanel[i][j] = -1;
				mine[i][j] = 0;
			}

		int cnt = 0;

		for (ii = 0; ii < 400; ii++)
		{
			for (i = 0; i < 20; i++)
				for (j = 0; j < 20; j++)
					temp[i][j] = GamePanel[i][j];
			machine(temp, n, m, x, y);

			if (ii == 0)
			{
				for (k = 0; k < minenum; k++)
				{
					i = rand() % 20;
					j = rand() % 20;
					while ((i == x && j == y) || mine[i][j] != 0)
					{
						i = rand() % 20;
						j = rand() % 20;
					}
					mine[i][j] = 1;
				}
			}
			if (GamePanel[x][y] == -1)
			{
				cnt++;
				win = RefreshGPanel(GamePanel, x, y, mine, n, m, minenum);
				if (win == 1)
				{
					if (cnt < 80) bonus = 35;
					else if (cnt < 90) bonus = 30;
					else if (cnt < 100) bonus = 25;
					else if (cnt < 120) bonus = 20;
					else if (cnt < 150) bonus = 15;
					else if (cnt < 200) bonus = 10;
					else if (cnt < 250) bonus = 5;
					else bonus = 0;
					score = 65.0 + bonus;
					if (iii == 0 || (iii + 1) % 10 == 0) printf("�� %d ��,Ӯ��,�� %d ��.\n", iii + 1, (int)score);
					winnum++;

					break;
				}
				else if (win == -1)
				{
					score = 0;
					for (i = 0; i < n; i++)
						for (j = 0; j < m; j++)
						{
							if (GamePanel[i][j] != -1)
								score = score + 1;
						}
					score = score / 7;

					if (iii == 0 || (iii + 1) % 10 == 0)printf("�� %d ��,����,�� %.2lf ��.\n", iii + 1, score);

					break;
				}
			}

		}
		if (win == 0)
		{
			score = 0;
			for (i = 0; i < n; i++)
				for (j = 0; j < m; j++)
				{
					if (GamePanel[i][j] != -1)
						score = score + 1;
				}
			score = score / 7;
			if (iii == 0 || (iii + 1) % 10 == 0)printf("�� %d ��,����,�� %.2lf ��.\n", iii + 1, score);

		}
		if (score > maxscore) maxscore = score;
		if (score < minscore) minscore = score;
		sumscore = sumscore + score;

	}

	int orignscore, finalscore;

	orignscore = ceil(1.3*ceil(sumscore / gamenum));

	if (orignscore > 100) finalscore = 100;
	else finalscore = orignscore;


	printf("��ѧ�����շ���Ϊ %d.\n\n", finalscore);
	printf("��ѧ��ʤ��Ϊ %.1lf%%\n\n", winnum / 10.0);

	system("pause");
	return 0;
}
