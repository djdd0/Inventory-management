#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<conio.h>
#include<time.h>

#define M 50


typedef struct
{
	int num=0;		 		//货品编号
	char name[20]={0};		//货品名称
	int stock=0;			//原始库存
	int price=0;            //货品单价
	int in=0;				//入库数目
	int out=0;				//出库数目
	int amount=0;			//最终库存
	int value=0;            //货品总价
	int warning_value=0;	//警戒值
	int state=0;			//库存状态(是否低于警戒值)
}goods;
goods s[M];			   		//用于存放货品信息
goods r[M];					//用于存放入库货品信息
goods t[M];					//用于存放出库货品信息

struct tm * lt;


void Display();
void Sort();
void Modify();
void Statistics();
void Re_file();
void Stock_in();
void Stock_out();
void Wr_in(int, int);
void Wr_out(int, int);
void Wr_goods();
void Stock_in_statistics(int, int, int, int, int, int);
void Stock_out_statistics(int, int, int, int, int, int);
void Estimate();
void get_time();


int N;
int P;


int main()
{
	int sele;
	Re_file();    //读取货品信息
	Stock_in();
	Stock_out();
	sele=1;
	while(sele)
	{
		system("cls");
		printf("\n\n");
		printf("*********************************************\n");
		printf("*                                           *\n");
		printf("*                                           *\n");
		printf("*       1.查询              2.排序          *\n");
		printf("*                                           *\n");
		printf("*       3.修改              4.统计          *\n");
		printf("*                                           *\n");
		printf("*       5.退出                              *\n");
		printf("*                                           *\n");
		printf("*                                           *\n");
		printf("*********************************************\n");
		printf("请选择功能序号:");
		scanf("%d",&sele);
		switch(sele)
		{
			case 1:Display();break;
			case 2:Sort();break;
			case 3:Modify();break;
			case 4:Statistics();break;
			case 5:exit(0);sele=0;break;
		}
		printf("\n\n按任意键继续...\n");
		getch();
	}
}


void Display()		//显示库存情况
{
	int i,j;
	system("cls");
	Estimate();
	printf("货品编号  货品名称  原始库存  货品单价 入库数目  出库数目  最终库存  货品总价   警戒值\n");
	for(i=0,j=1;i<P;i++,j++)
	{
		if(s[i].state==1)		//库存值小于警戒值红字显示
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
			printf("%-9d  %-10s%-10d%-10d%-10d%-10d%-10d%-10d%-10d\n",s[i].num,s[i].name,s[i].stock,s[i].price,s[i].in,s[i].out,s[i].amount,s[i].value,s[i].warning_value);
			if(j%10==0&&j!=P)		//控制每次显示十行
			{
				printf("按任意键继续...");
				getch();
				puts("\n");
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE);
				printf("货品编号  货品名称  原始库存  货品单价 入库数目  出库数目  最终库存  货品总价   警戒值\n");
			}
		}
		else if(s[i].state==0)	//库存值不小于警戒值白字显示
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE);
			printf("%-9d  %-10s%-10d%-10d%-10d%-10d%-10d%-10d%-10d\n",s[i].num,s[i].name,s[i].stock,s[i].price,s[i].in,s[i].out,s[i].amount,s[i].value,s[i].warning_value);
			if(j%10==0&&j<P)
			{
				HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE);
				printf("按任意键继续...");
				getch();
				puts("\n");
				printf("货品编号  货品名称  原始库存  货品单价 入库数目  出库数目  最终库存  货品总价   警戒值\n");
			}
		}
	}
}


void Sort()    //商品排序
{
	int k,i,j;
	goods t;
	system("cls");
	printf("\n\n");
	printf("*********************************************\n");
	printf("*                                           *\n");
	printf("*      1.按货品编号排序                     *\n");
	printf("*                                           *\n");
	printf("*      2.按库存总量排序                     *\n");
	printf("*                                           *\n");
	printf("*      3.按货品总价排序                     *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");
	printf("\n\n请输入修改选项:");
	scanf("%d",&k);
	switch (k)
	{
		case 1:
			for(i=0;i<P-1;i++)
				for(j=i+1;j<P;j++)
				{
					if(s[i].num>s[j].num)
					{
						t=s[i];
						s[i]=s[j];
						s[j]=t;
					}
				}
			break;
		case 2:
			for(i=0;i<P-1;i++)
				for(j=i+1;j<P;j++)
				{
					if(s[i].amount>s[j].amount)
					{
						t=s[i];
						s[i]=s[j];
						s[j]=t;
					}
				}
			break;
		case 3:
			for(i=0;i<P-1;i++)
				for(j=i+1;j<P;j++)
				{
					if(s[i].value>s[j].value)
					{
						t=s[i];
						s[i]=s[j];
						s[j]=t;
					}
				}
			break;
		default:
			printf("\n请输入正确的选项!");
			break;
	}
	Wr_goods();
	Display();
}


void Modify()        //修改货品信息
{
	int k,m,i,j=-1,t,h;
	char n[20];
	system("cls");
	printf("\n\n");
	printf("*********************************************\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*      1.货品编号         2.货品名称        *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");
	printf("\n\n请输入修改选项:");
	scanf("%d",&k);
	if(k==1)
	{
		printf("请输入货品编号:");
		scanf("%d",&m);
		for(i=0;i<P;i++)
		{
			if(m==i)
			j=m-1;
		}
	}
	else if(k==2)
	{
		printf("请输入货品名称:");
		scanf("%s",&n);
		for(i=0;i<P;i++)
		{
			if(strcmp(n,s[i].name)==0)
				j=i;
		}
	}
	if(j==-1)
	{
		printf("\n没有找到!\n");
	}
	else
	{
		system("cls");
		printf("\n\n货品编号:%d\n货品名称:%s\n",s[j].num,s[j].name);
		printf("*********************************************\n");
		printf("*                                           *\n");
		printf("*       1.货品名称          2.原始库存      *\n");
		printf("*                                           *\n");
		printf("*       3.入库数目          4.出库数目      *\n");
		printf("*                                           *\n");
		printf("*       5.货品单价          6.警戒值        *\n");
		printf("*                                           *\n");
		printf("*********************************************\n");
		printf("请选择修改选项:");
		scanf("%d",&t);
		printf("请输入修改值:");
		switch (t)
		{
			case 1:
				scanf("%s",&n);
				strcpy(s[j].name,n);
				Wr_goods();
				break;
			case 2:
				scanf("%d",&h);
				s[j].stock=h;
				Wr_goods();
				break;
			case 3:
				scanf("%d",&h);
				s[j].in+=h;
				Wr_in(s[j].num,h);
				Stock_in();
				break;
			case 4:
				scanf("%d",&h);
				s[j].out+=h;
				Wr_out(s[j].num,h);
				Stock_out();
				break;
			case 5:
				scanf("%d",&h);
				s[j].price=h;
				Wr_goods();
				break;
			case 6:
				scanf("%d",&h);
				s[j].warning_value=h;
				Wr_goods();
				break;
			default:
				printf("\n请输入正确的选项!");
				break;
		}
		s[j].amount=s[j].stock+s[j].in-s[j].out;
	}
	printf("请确认核对货品信息:\n");
	Display();
}


void Statistics()       //统计数目
{
	int k,i,j,m=0,n=0,y0,m0,d0,y1,m1,d1;
	system("cls");
	printf("\n\n");
	printf("*********************************************\n");
	printf("*                                           *\n");
	printf("*      1.库存总数         2.库存状态        *\n");
	printf("*                                           *\n");
	printf("*      3.货品总价         4.入库统计        *\n");
	printf("*                                           *\n");
	printf("*      5.出库统计                           *\n");
	printf("*                                           *\n");
	printf("*********************************************\n");
	printf("\n\n请输入查询选项:");
	scanf("%d",&k);
	switch (k)
	{
		case 1:
			for(i=0;i<P;i++)
				m=m+s[i].amount;
			printf("库存总数为：%d",m);
			break;
		case 2:
			Estimate();
			for(j=0;j<P;j++)
				if(s[j].state==1)
					n=n+1;
			printf("低于库存警戒值的商品种数为:%d",n);
			break;
		case 3:
			for(i=0;i<P;i++)
				m=m+s[i].value;
			printf("货品总价为：%d",m);
			break;
		case 4:
			printf("请输入查询起始年份:");
			scanf("%d",&y0);
			printf("请输入查询起始月份:");
			scanf("%d",&m0);
			printf("请输入查询起始日期:");
			scanf("%d",&d0);
			printf("请输入查询结束年份:");
			scanf("%d",&y1);
			printf("请输入查询结束月份:");
			scanf("%d",&m1);
			printf("请输入查询结束日期:");
			scanf("%d",&d1);
			Stock_in_statistics(y0,m0,d0,y1,m1,d1);
			break;
		case 5:
			printf("请输入查询起始年份:");
			scanf("%d",&y0);
			printf("请输入查询起始月份:");
			scanf("%d",&m0);
			printf("请输入查询起始日期:");
			scanf("%d",&d0);
			printf("请输入查询结束年份:");
			scanf("%d",&y1);
			printf("请输入查询结束月份:");
			scanf("%d",&m1);
			printf("请输入查询结束日期:");
			scanf("%d",&d1);
			Stock_out_statistics(y0,m0,d0,y1,m1,d1);
			break;
		default:
			printf("\n请输入正确的选项!");
			break;
	}
}


void Re_file()		//读入原始库存文件
{
	FILE*fp;
	N=0;
	fp=fopen("goods.txt","r");
	while(fscanf(fp,"%d%s%d%d%d",&s[N].num,&s[N].name,&s[N].stock,&s[N].price,&s[N].warning_value)!=EOF)N++;
	fclose(fp);
	P=N;
}


void Stock_in()		//读入入库文件
{
	FILE*fp;
	int i,j;
	N=0;
	fp=fopen("stockin.txt","r");
	while(fscanf(fp,"%d%*d%*d%*d%*d%*d%*d%d",&r[N].num,&r[N].in)!=EOF)N++;
	fclose(fp);
	for(i=0;i<P;i++)
	{
		for(j=0;j<N;j++)
		{
			if(r[j].num==s[i].num)
				s[i].in+=r[j].in;
		}
	}
	for(i=0;i<P;i++)
	{
		s[i].amount=s[i].stock+s[i].in-s[i].out;
		s[i].value=s[i].amount*s[i].price;
	}
}


void Stock_out()	//读入出库文件
{
	FILE*fp;
	int i,j;
	N=0;
	fp=fopen("stockout.txt","r");
	while(fscanf(fp,"%d%*d%*d%*d%*d%*d%*d%d",&t[N].num,&t[N].out)!=EOF)N++;
	fclose(fp);
	for(i=0;i<P;i++)
	{
		for(j=0;j<N;j++)
		{
			if(t[j].num==s[i].num)
				s[i].out+=t[j].out;
		}
	}
	for(i=0;i<P;i++)
	{
		s[i].amount=s[i].stock+s[i].in-s[i].out;
		s[i].value=s[i].amount*s[i].price;
	}
}


void Wr_in(int l, int k)       //写入库文件
{
	FILE*fp;
	fp=fopen("stockin.txt","a");
	get_time();
	fprintf(fp,"%-10d%-10d%-10d%-10d%-10d%-10d%-10d%-10d%\n",l,lt->tm_year,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec,k);
	fclose(fp);
}


void Wr_out(int l, int k)       //写出库文件
{
	FILE*fp;
	fp=fopen("stockout.txt","a");
	get_time();
	fprintf(fp,"%-10d%-10d%-10d%-10d%-10d%-10d%-10d%-10d%\n",l,lt->tm_year,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec,k);
	fclose(fp);
}


void Wr_goods()       //写警戒值文件
{
	FILE*fp;
	int i;
	fp=fopen("goods.txt","w");
	for(i=0;i<P;i++)
		fprintf(fp,"%-9d  %-10s%-10d%-10d%-10d\n",s[i].num,s[i].name,s[i].stock,s[i].price,s[i].warning_value);
	fclose(fp);
}


void Stock_in_statistics(int y0, int m0, int d0, int y1, int m1, int d1)		//入库数据统计
{
	FILE*fp;
	int n,in,year,mon,day,hour,min,sec,sum=0;
	N=0;
	fp=fopen("stockin.txt","r");
	system("cls");
	printf(" 序号   出库日期   出库时间  货品编号    货品名称  入库数目  入库总价\n");
	while(fscanf(fp,"%d%d%d%d%d%d%d%d",&n,&year,&mon,&day,&hour,&min,&sec,&in)!=EOF)
	{
		if(year+1900>y0 && year+1900<y1 && mon>m0 && mon<m1 && day>d0 && day<d1)
		{
			N++;
			printf("  %-5d%4d/%2d/%2d  %2d:%2d:%2d     %-10d  %-10s%-10d%-10d\n",N,year+1900,mon,day,hour,min,sec,s[n-1].num,s[n-1].name,in,in*s[n-1].price);
			sum+=in*s[n-1].price;
		}
	}
	printf("\n\n入库货品总价:%d\n",sum);
	fclose(fp);
}


void Stock_out_statistics(int y0, int m0, int d0, int y1, int m1, int d1)		//出库数据统计
{
	FILE*fp;
	int n,in,year,mon,day,hour,min,sec,sum=0;
	N=0;
	fp=fopen("stockout.txt","r");
	system("cls");
	printf(" 序号   出库日期   出库时间  货品编号    货品名称  入库数目  入库总价\n");
	while(fscanf(fp,"%d%d%d%d%d%d%d%d",&n,&year,&mon,&day,&hour,&min,&sec,&in)!=EOF)
	{
		if(year+1900>y0 && year+1900<y1 && mon>m0 && mon<m1 && day>d0 && day<d1)
		{
			printf("  %-5d%4d/%2d/%2d  %2d:%2d:%2d     %-10d  %-10s%-10d%-10d\n",N,year+1900,mon,day,hour,min,sec,s[n-1].num,s[n-1].name,in,in*s[n-1].price);
			sum+=in*s[n-1].price;
		}
		N++;
	}
	printf("\n\n入库货品总价:%d\n",sum);
	fclose(fp);
}


void Estimate()		//判断库存值是否小于警戒值
{
	int i;
	for(i=0;i<P;i++)
	{
		if(s[i].amount>=s[i].warning_value)
			s[i].state=0;
		else if(s[i].amount<s[i].warning_value)
			s[i].state=1;
	}
}


void get_time()
{
	time_t t;
	time (&t);           //获取Unix时间戳。
	lt = localtime (&t); //转为时间结构。
}
