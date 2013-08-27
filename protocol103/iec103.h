/*
 * iec103.h
 *
 *  Created on: Aug 27, 2013
 *      Author: agu
 */

#ifndef IEC103_H_
#define IEC103_H_

#include <stm32f10x.h>

typedef struct {
	uint8_t cando; //使能 0x01,0x02,0x04 分别表示下三位 0x80 表示立即写入flash
	float fAngRate; //角度偏转倍率	x,y,z相同
	float fHitRate; //冲击系数 x,y,z相同
} TAdjustData; //参数调整数据 //需要保存到flash中

typedef struct {
	uint8_t cando;  //使能 0x01,0x02,0x04 分别表示下三位 0x80 表示立即写入flash
	float jdx[2]; // X轴角度0:基准角度 1:报警角度（相对基准角度的一个范围值，不是绝对门限值）
	float jdy[2]; // Y轴角度0:基准角度 1:报警角度
	float jdz[2]; // Z轴角度0:基准角度 1:报警角度
	float cjl[2]; //垂直冲击量 0:基准冲击量 1:事件冲击量
	float pl[2];  //采样基准，采样范围
} TAlarmLimit;

typedef struct {
	uint16_t msec;
	uint8_t min :6;
	uint8_t iv :2;
	uint8_t hour :5;
	uint8_t su :3;
	uint8_t day :5;
	uint8_t week :3;
	uint8_t mon :4;
	uint8_t mon_res :4;
	uint8_t year :7;
	uint8_t year_res :1;
} TCP56TIME2a, *PCP56TIME2a;

typedef struct {
	uint8_t sec :6;
	uint8_t a :2;
	uint8_t min :6;
	uint8_t b :2;
	uint8_t hour :5;
	uint8_t su :3;
} TClock, *PClock;

typedef struct {
	uint16_t id; //2
	TClock aclock;	//3	 冲击发生时刻
	uint8_t cnt;	//1 //采集周期中的 2S中的冲击次数
	float val; 		//4	 冲击值 的幅度
} TEvent, *PEvent;

typedef struct {
	uint8_t FileName[16];	//下载文件名称
	uint8_t Type;	//0：从bootload下载  1：从应用程序下载
	uint8_t Status;	//下载的状态 0等候 1下载条件成立 2下载开始 3下载数据中 4下载完成
	uint8_t bAskRefreshProg;	//0x55要求将"Routine.PRG"写到程序存储器
	uint8_t nPage;	//文件页
	uint16_t TotalBlockNum;
	uint16_t nBlock;	//在此之前完成的块号
	unsigned long nLen;	//block之后完成的块
} DOWNLOADTYPE;

typedef struct {
	uint8_t nFcb; //桢计数器
	uint8_t addr;
	uint8_t bound;
	TAdjustData adjustData;
	TAlarmLimit limitData;
	TCP56TIME2a aClock;
	TEvent congjEvt;
	TEvent qingxEvt;
	TEvent openEvt;

	DOWNLOADTYPE DownLoad;	    //升级参数
	uint8_t crc;
} T485Equ;

#endif /* IEC103_H_ */
