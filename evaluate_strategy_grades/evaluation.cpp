#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
using namespace std;


/********************************************************************************/
/********************************将学生的代码添加到下面******************************/

//函数声明：
void copy_matrix(int a[30][30], int b[30][30]); //将第二个矩阵copy过去
bool is_logic_right(int GamePanel[30][30], int n, int m);//判断模拟 之后是否logic正确
void find_mine_2(int GamePanel[30][30], int n, int m); //第二种找地雷的函数
bool is_matrix_equal(int a[30][30], int b[30][30]);  //判断2个矩阵是否值 完全相同
void find_mine_1(int GamePanel[30][30], int n, int m); //第一种 找地雷的方法
void probable_matrix_cal(double M_probable[30][30], int GamePanel[30][30], int n, int m); //计算-1未打开空格是地雷的概率,存储到M_probable的double矩阵中
double probable_cnt(double M_probable[30][30], int GamePanel[30][30], int i, int j, int n, int m); //周围是-1点的概率值之和


void machine(int GamePanel[30][30], int n, int m, int &x, int &y)
{

	//(一)开始时，随机选择一个位置： 
	x = rand() % n;
	y = rand() % m;  //开始的时候随机选择一个位置 

	//如果是第一次点击，最好点击4个角落，也就是
	int null_matrix[30][30] = { 0 };
	if (is_matrix_equal(null_matrix, GamePanel))
	{
		x = 0;
		y = 0;
		return;
	}
	else
	{
		//先找一个-1的位置,防止之后的 策略全部失效 
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (GamePanel[i][j] == -1)
				{
					x = i;
					y = j;
					//找到最后一个位置也好 
				}
			}
		}
	}


	//(二)一种重要的量的设置 
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	int whiteNum = 0, mineCnt = 0;  //记录 没有打开的位置的数目  和 确定是地雷的数目



	//(三)在循环中调用 2中逻辑推理，确定的找出那些一定是 地雷的位置！！！在panel中标记为-2 

	while (1)
	{
		//设置一个 copy矩阵，用于判断矩阵是否有更新
		int panel_before[30][30] = { 0 };
		copy_matrix(panel_before, GamePanel);

		//(1)先调用第一种 find_mine
		find_mine_1(GamePanel, n, m);
		//(2)再调用第二种 find_mine
		find_mine_2(GamePanel, n, m);
		//(3)判断GamePanle有没有更新
		if (is_matrix_equal(panel_before, GamePanel))
		{
			break; //说明不能再找到地雷了
		}
	}

	//(四)计算得到一个概率矩阵M_probable,里面存储了那些 “数字”周围没有打开的格子 “是地雷的概率” 

	double M_probable[30][30] = { 0 };//初始化全部为0，只记录大于0的未打开的格子的概率，一定不是地雷设置为0.00001
	probable_matrix_cal(M_probable, GamePanel, n, m);


	//第二部分，优先选择 那些 周围概率之和最小的 一定不是地雷的 格子
	//(五)，通过上面的反复调用两种推理，已经将所有一定是地雷的位置在panel中标记出来了
	//现在那些 地雷已经被全部标记出来了， 现在就是需要看 优先点击 哪一个空格， 次数越少，分数越高
	//为了点击那种 一定周围没有 地雷的 点，我们使用 概率矩阵，对周围出现地雷的 概率进行求和，每次选取和最小
	//的那个 空格进行点击并且返回，这样就可以 尽可能的 打开那些没有和地雷相邻的 一大片
	double min_pro = 999; //每次 从 一定不是地雷 的格子中计算周围的 概率和，并且和 min_pro进行比较
	int min_x = 0;
	int min_y = 0;


	//找出那些一定不是地雷的位置，计算其周围的概率和 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				whiteNum = 0;   // 每一个9宫格内 记录未打开的数目
				mineCnt = 0;    //记录地雷的数目
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
				//下面也就是将 找到最小的 概率之和 的点
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
	if (min_pro < 998) //从这些概率中，选择概率最小的 作为 下一次点击的位置，然后直接返回 
	{
		x = min_x;
		y = min_y;
		return;
	}





	//开始分析以下代码的奥秘

	//(六)如果前面的策略全部失败，那么我们考虑 如果还有300个以上的格子没有打开，优先打开4个角落
	//还有 那些 周围只有一个地雷，但是却有8个未打开格子的位置 
	int count = 0;
	for (int i = 0; i < n; i++)  //存储了n行 m列数
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] == -1)
			{
				count++;  //这里记录 没有打开的格子的总数
			}
		}
	}

	whiteNum = 0;
	if (count > 300)  //在还有大于300个格子没有打开的情况下，我们优先打开4个角落的格子
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
				if (GamePanel[i][j] == 1)  //如果一个格子的地雷确定了，是只有一个（说明它周围地雷少）
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
						//这里面whiteNum记录了 9宫格周围没有打开的 地雷的数目
					}
					int iii = 0;
					if (whiteNum == 8)
					{
						iii = rand() % 8;  //从周围随机选取一个，随机这东西，很有用 
						x = i + round[0][iii];
						y = j + round[1][iii];
						return;
					}
					//如果周围有8个 空格没有打开，那么只有1/8的概率会踩到雷
				}

			}
		}
	}
	//(七)随机+盲猜策略：
	//其实，上述策略 用到了分析那些 本身是-1的位置，如果周围-1比较多，那么可以点击“盲猜”！ 
	//因为“盲猜”有可能打开 一大片，直接(利用前面的2中逻辑推理)锁定胜局！ 

	//第三个层次的策略，没有什么好的空格可以选择了
	//基本上就是 随机 选择稍微好一点的位置
	x = rand() % 20;
	y = rand() % 20;
	int _tryy = 0;
	while (1)
	{
		_tryy++;      //记录循环次数
		x = rand() % n;
		y = rand() % m;  //x 和 y分别随机选择一个数值
		if (GamePanel[x][y] == -1)  //如果 这个x,y处的格子没有打开,然后检查它周围的条件
		{  //这里面分析的是 一个空格本身没有打开的case下，周围的格子的打开情况 

			whiteNum = 0;
			mineCnt = 0;
			for (int i = 0; i < 8; i++)
			{
				if (GamePanel[x + round[0][i]][y + round[1][i]] == -1)  //周围 未打开数 
				{
					whiteNum++;
				}

				else if (GamePanel[x + round[0][i]][y + round[1][i]] == -2) //周围地雷数 
				{
					mineCnt++;
				}
			}
			if (whiteNum >= 7 || mineCnt >= 5)  //如果周围的 未打开的太多，或者 周围的地雷发现就多 ，或者循环大于88次(防止死循环)
			{
				return; // 超过88 就没分了！！！
			}
			else if (_tryy > 88)
			{
				break;
			}
		}
	}



	//(八)第四部分：

	//现在得到了最初级的 概率矩阵，然后在这些概率中查找 查找概率最小的那个位置作为x 和 y的值
	double probable_min = 1;
	int pro_x = x;
	int pro_y = y;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			//忘记不能为0这回事了
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

	//第五部分 --放到函数体中

}


//1.copy_matrix,将第二个int 类型的矩阵的值copy到第一个矩阵
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
//2.判断模拟之后的 矩阵是否会产生矛盾（也就是说，mineCnt > panel_copy[i][j],）如果 合理返回true
bool is_logic_right(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	int emptyNum = 0, mineNum = 0;  //记录 没有地雷的数目  和 确定是地雷的数目

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // 每一个9宫格内 记录未打开的数目
				mineNum = 0;    //记录地雷的数目
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
				//下面也就是将 我们利用逻辑分析 ， 如果找到了mineCnt > panel_copy[i][j]，就返回 false
				if (mineNum > GamePanel[i][j])
				{
					//cout << "第二种逻辑判断 确实被使用了！！！！！！！！！！！！！！！！";//调试分析作用
					return false;
				}
			}
		}
	}
	return true;
}
//3.第二种逻辑判断 找地雷的 函数
void find_mine_2(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	int emptyNum = 0, mineNum = 0;  //记录 没有地雷的数目  和 确定是地雷的数目
	//第五部分：添加的第二种逻辑判断，如果剩余的未打开的空格的数目是剩余的地雷+1，就可以通过模拟，找出那个地雷
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // 每一个9宫格内 记录未打开的数目
				mineNum = 0;    //记录地雷的数目
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

				//现在已经获得了这个 格子 四周的未打开的空格的数目whiteNum，和地雷的数目 mineCnt
				//那么 空格中的地雷数目就应该是：remain_cnt
				int remain_cnt = GamePanel[i][j] - mineNum;
				//只有当 whiteNum==remain_cnt+1的时候，才需要进行模拟
				//而且模拟的次数是 whiteNum;
				if (remain_cnt != 0 && emptyNum == remain_cnt + 1) //剩余的地雷数不能是0，那必然不是地雷
				{
					//(1)copy一个panel矩阵进行模拟使用
					int panel_copy[30][30] = { 0 };
					copy_matrix(panel_copy, GamePanel);
					//(2)借助panel_copy进行模拟,总共模拟whiteNum次
					//依次查找 那些数值是-1的点，然后 给其中的remain_cnt个空格赋值为-2，然后利用判断逻辑合理函数
					int tmp_cnt = emptyNum;//需要模拟的次数

					int flag = 0;          //如果为1说明那个 没有赋值为-2的点一定是地雷，在GamePanel中赋值-2

					while (tmp_cnt >= 1)
					{

						//cout<<"进入模拟"<<endl; 
						for (int ii = 0; ii < 8; ii++)
						{
							//依次查找周围的 8个位置
							if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
							{
								if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
								{
									//每次模拟需要设置tmp_cnt2个地雷!
									int tmp_cnt2 = emptyNum - 1; //模拟 赋值地雷次数

									//进行倒数第tmp_cnt次模拟，并且tmp_cnt--
									tmp_cnt--;
									int kk = ii;
									for (kk = ii; tmp_cnt2 >= 1; kk++) //只有把所有的地雷全部 模拟埋下，才退出循环，利用 取模 运算进行循环
									{
										kk = kk % 8;//kk循环取值 0-7

										if (i + round[0][kk] >= 0 && i + round[0][kk] < n && j + round[1][kk] >= 0 && j + round[1][kk] < m)
										{
											if (GamePanel[i + round[0][kk]][j + round[1][kk]] == -1)
											{
												//只要找到了，就马上赋值为-2
												//总共只能赋值 tmp_cnt2次
												panel_copy[i + round[0][kk]][j + round[1][kk]] = -2;//埋下一个地雷
												tmp_cnt2--;

											}
										}
									}
									//已经是 最后一个地雷kk位置++之后的 kk值了
									//因为先kk++,再进行判断

									//上面这个for循环已经埋下了所有的地雷,可以判断此时的panel_copy是否logic正确
									if (!is_logic_right(panel_copy, n, m))
									{
										//说明那个任然为-1的空格 一定是地雷
										int jj = 0;
										for (jj = 0; jj < 8; jj++) //只有把所有的地雷全部 模拟埋下，才退出循环，利用 取模 运算进行循环
										{
											jj = jj % 8;//jj循环取值 0-7

											if (i + round[0][jj] >= 0 && i + round[0][jj] < n && j + round[1][jj] >= 0 && j + round[1][jj] < m)
											{
												if (panel_copy[i + round[0][jj]][j + round[1][jj]] == -1)
												{
													//可以再GamePanel中将它的值赋值为-2
													GamePanel[i + round[0][jj]][j + round[1][jj]] = -2; //找到一个 一定是地雷
													//是否需要重新计算这个点周围的mineCnt 和 whiteNum,应该不需要，因为我们只用一次 
													//cout << "用第二种逻辑判断找出了地雷!!" << endl;//调试
													flag = 1;//只能找到一个马上跳出所有循环
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
						//这里面有一个死循环 --现在只要从这个循环中出来,应该就perfect了 
						//cout<<"退出模拟"<<endl; 
						if (flag == 1) break;
					}

					//以上进行完 模拟
					if (flag == 1) break;
				}

				//完成一个有地雷的格子 周围 的模拟之后。。。

			}
		}
	}
}
//4.判断2个int类型矩阵的 所有的值是否相同的 函数，如果相同返回true，否则返回false,它的作用就是作为 判断
//进行 逻辑判断之后 ，是否有找出新的地雷-2，如果没有 就直接 退出循环
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
//5.第一种逻辑判断，如果 剩下的未打开的空格数目 == 中间显示的地雷的数目 - 已经找出的地雷的数目，那么它们全是地雷
void find_mine_1(int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	int emptyNum = 0, mineNum = 0;  //记录 没有地雷的数目  和 确定是地雷的数目
	//以下是第一种判断:(找地雷)
	for (int i = 0; i < m; i++)  //行
	{
		for (int j = 0; j < n; j++) //列
		{
			emptyNum = 0;   //注意：panel中-1-未打开，0已经打开且未与地雷相邻，>0表示地雷的数目
			mineNum = 0;
			if (GamePanel[i][j] > 0)    //每次只有遇到与地雷相邻的格子，才需要进行分析
			{
				for (int ii = 0; ii < 8; ii++)
				{
					if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
					{
						if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
						{
							emptyNum++;  //已经未打开的 地方 --emptynum--代表记录 未打开格子数目
						}
						else if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -2)
						{
							mineNum++;  //已经确定是地雷的 地方--minenum--代表记录这个格子周围已经确定的地雷数目
						}
					}
				}
				//然后，如果一个格子周围剩下的大于0的空格数目 和  周围确定的地雷数目相同，那么可以用-2标记出这些地雷
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
//6.计算得到 概率矩阵的函数，将计算的结果放到matrix_probable中
void probable_matrix_cal(double M_probable[30][30], int GamePanel[30][30], int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	int emptyNum = 0, mineNum = 0;  //记录 没有地雷的数目  和 确定是地雷的数目
	//以下进行计算 -1点的概率
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (GamePanel[i][j] > 0)
			{
				emptyNum = 0;   // 每一个9宫格内 记录未打开的数目
				mineNum = 0;    //记录地雷的数目 ,也就是-2的个数,本身GamePanel[i][j]就代表周围的地雷总数
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
				}//主要计算 地雷 和 未打开的数量

				if (emptyNum > 0) //如果周围的 未打开而且没有判断为地雷的 格子的数量大于0,才需要计算它们的概率
				{
					for (int ii = 0; ii < 8; ii++)
					{
						if (i + round[0][ii] >= 0 && i + round[0][ii] < n && j + round[1][ii] >= 0 && j + round[1][ii] < m)
						{
							if (GamePanel[i + round[0][ii]][j + round[1][ii]] == -1)
							{
								//特殊情况，如果地雷的数量 和 [i][j]的数量相同说明 全部都一定不为 地雷
								if (GamePanel[i][j] == mineNum)
								{
									M_probable[i + round[0][ii]][j + round[1][ii]] = 0.00001;
								}
								else//否则，就是正常的情况
								{
									double remain_mine = GamePanel[i][j] - mineNum;//剩余地雷的数量 
									double not_open = emptyNum;  //未打开数目 
									double tmp_pro = remain_mine / not_open;
									if (M_probable[i + round[0][ii]][j + round[1][ii]] == 0)
									{
										M_probable[i + round[0][ii]][j + round[1][ii]] = tmp_pro;
									}
									else
									{
										//原来有一个概率值
										if (tmp_pro > M_probable[i + round[0][ii]][j + round[1][ii]])
										{
											//只有这次计算的概率更大才需要进行更换
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

//7.计算周围的 点 是地雷的 概率之和的 函数
double probable_cnt(double M_probable[30][30], int GamePanel[30][30], int i, int j, int n, int m)
{
	int round[2][8] = { -1,1,0,0,-1,1,1,-1,0,0,-1,1,-1,1,-1,1 }; //还是那个 9宫格偏移
	//
	double tmp_cnt = 0; //记录周围 概率值之和
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

//信息：
//1.Gamepanel的0--n-1行 ，0--m-1列可以使用
//对于这个panel中的信息：（1）未打开的格子--“-1” （2）已经打开的格子--“相邻的地雷数量” 0-8之间
//2.下面的信息我不能使用：mine[i][j]数组,第i行，第j列的位置如果是地雷 mine[i][j]==1,否则为0
//3.k--代表地雷的总数量
//玩家通过Gamepanel的信息，确定下一步需要点击的 坐标位置给到x和 y



int getMineNum(int x, int y, int mine[30][30], int n, int m)   //得到 相连的地雷的数目
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
void extend(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m)//延申作用，将所有的相连空白打开
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
int state(int GamePanel[30][30], int mine[30][30], int n, int m, int k) //当前游戏局面的状态，确定是否只剩下地雷没有打开--如果是，返回1
{
	int i, j, num_mine = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			if (GamePanel[i][j] == -1)num_mine++;
	if (num_mine == k)return 1;
	return 0;
}
int RefreshGPanel(int GamePanel[30][30], int x, int y, int mine[30][30], int n, int m, int k) //如果x,y位置是1--输，如果state==1,--获胜，否则继续游戏
{
	if (mine[x][y] == 1)return -1;
	else  extend(GamePanel, x, y, mine, n, m);
	if (state(GamePanel, mine, n, m, k) == 1) return 1;
	return 0;
}

/********************************将学生的代码添加到上面******************************/
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

	//原来主函数中的n和m的值都设置为20
	n = 20;
	m = 20;
	minenum = 50;
	//总共50个地雷

	srand(int(time(0)));
	printf("正在评测中，请稍等，我们将进行1000局游戏，取平均分*1.3作为最终成绩......\n");

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
					if (iii == 0 || (iii + 1) % 10 == 0) printf("第 %d 局,赢了,得 %d 分.\n", iii + 1, (int)score);
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

					if (iii == 0 || (iii + 1) % 10 == 0)printf("第 %d 局,输了,得 %.2lf 分.\n", iii + 1, score);

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
			if (iii == 0 || (iii + 1) % 10 == 0)printf("第 %d 局,输了,得 %.2lf 分.\n", iii + 1, score);

		}
		if (score > maxscore) maxscore = score;
		if (score < minscore) minscore = score;
		sumscore = sumscore + score;

	}

	int orignscore, finalscore;

	orignscore = ceil(1.3*ceil(sumscore / gamenum));

	if (orignscore > 100) finalscore = 100;
	else finalscore = orignscore;


	printf("该学生最终分数为 %d.\n\n", finalscore);
	printf("该学生胜率为 %.1lf%%\n\n", winnum / 10.0);

	system("pause");
	return 0;
}
