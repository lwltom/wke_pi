#pragma once
#include <limits>
#include "../PiTypeDef.h"
#include <wtypes.h>

Pi_NameSpace_Begin
const int NUM_INT_DIGIT_LENGTH = 30;//字符串形式的数据大小最大支持的位数

const UNLONG NUM_FILE_SIZE_RATE = 1024;                     //文件大小单位进率
const UNLONG NUM_FILE_SIZE_1K = 1024;
const UNLONG NUM_FILE_SIZE_1M = NUM_FILE_SIZE_1K * NUM_FILE_SIZE_RATE;
const UNINT64 NUM_FILE_SIZE_1G = NUM_FILE_SIZE_1M * NUM_FILE_SIZE_RATE;
const UNINT64 NUM_FILE_SIZE_1T = NUM_FILE_SIZE_1G * NUM_FILE_SIZE_RATE;

const int NUM_PERCENT_MAX = 100;  //百分比最大值


//时间
const UNLONG NUM_TIME_RATE = 60;

#define SECOND_TO_NANOSECOND(n) ((n)*1000*1000*1000)        //秒 -> 纳秒
#define SECOND_TO_MILLISECOND(n) ((n)*1000)        //秒 -> 毫秒
#define MILLISECOND_TO_NANOSECOND(n) ((n)*1000*1000)        //毫秒 -> 纳秒
#define MILLISECOND_TO_NANOSECOND100(n) ((n)*1000*10)        //毫秒 -> 100纳秒

#define NANOSECOND100_TO_NANOSECOND(n) ((n)*100)            //100纳秒转纳秒
#define NANOSECOND100_TO_MILLISECOND(n) ((n)/10 / 1000)        //100纳秒 转 毫秒
#define INT_TO_FLOAT(n) (1.0f * (n))                        //整形转成浮点型, 可用于整形相除时, 保留更高的精度
#define PERCENT_TO_DIGIT(n) ( (n) /100.0f)
#define DIGIT_TO_PERCENT(n) ( (n) * 100.0f)
//#define TO_DOUBLE(1.0df * n)
//文件大小转换
#define BYTE_TO_KB(n)       ( INT_TO_FLOAT((n)) / ( 2<<(10-1) )) // -1: 10次方为 再除以2 = 1024
#define BYTE_TO_MB(n)       ( INT_TO_FLOAT((n)) / ( 2<<(20-1) ))
#define BYTE_TO_GB(n)       ( INT_TO_FLOAT((n)) / ( 2<<(30-1) ))

//时间
#define SECOND_TO_MINUTE(n) (n / NUM_TIME_RATE)
#define SECOND_TO_HOUR(n) (n / NUM_TIME_RATE / NUM_TIME_RATE)

#define MINUTE_TO_HOUR(n) (n / NUM_TIME_RATE)
#define MINUTE_TO_SECOND(n) (n * NUM_TIME_RATE)
#define MINUTE_TO_MILLISECOND(n) (n * NUM_TIME_RATE * 1000)

#define HOUR_TO_MINUTE(n) (n * NUM_TIME_RATE)
#define HOUR_TO_SECOND(n) (n * NUM_TIME_RATE * NUM_TIME_RATE)



//去掉魔数
#define  INDEX_TO_COUNT(n) ((n) + 1)
#define  COUNT_TO_INDEX(n) ((n) - 1)

//位操作
//对一个数值进行移除某个标识位的操作
#define TakeOut(nInt, nErase) (nInt&~nErase)

bool FlowBigger(double n1, double n2);
bool FlowEqual(double n1, double n2);


class CPiMath
{
public:
	CPiMath();
	~CPiMath();
public:
	/************************************************************************
	fun:	获取等比例扩大或缩放后的最大大小
	param:
	memo:
	************************************************************************/
	static SIZE GetKeepRadio(const SIZE& szSrc, const SIZE& szLimit);
protected:
private:
};

Pi_NameSpace_End
