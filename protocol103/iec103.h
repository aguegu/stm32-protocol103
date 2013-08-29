/*
 * iec103.h
 *
 *  Created on: Aug 27, 2013
 *      Author: agu
 */

#ifndef IEC103_H_
#define IEC103_H_

#include <stm32f10x.h>

/*
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

 */

typedef struct {
	short len;
	unsigned char buf[256]; //记录所指向的内存
} TRecord;

//下行	下发
typedef struct {
	uint8_t _func :4;  // 原来这里是对命令进行分组处理,用于对任务进行预先准备　用于功能准备
	uint8_t _fcv :1;  //帧计数有效位FCV
	//FCV=1：表示FCB位有效；FCV=0：表示FCB位无效

	uint8_t _fcb :1;	//帧计数位FCB
	//当帧计数有效位FCV=1时，FCB表示每个站连续的发送/确认或者请求/响应服务的变化位。
	//FCB位用来防止信息传输的丢失和重复

	uint8_t _prm :1;	//启动标志位PRM
	//PRM =1：表示此帧报文来自启动站；PRM =0：表示此帧报文来自从动站

	uint8_t _dir :1;	//传输方向位DIR DIR=0：表示此帧报文是由主站发出的下行报文；
	//DIR=1：表示此帧报文是由终端发出的上行报文
} TCTRL_DOWN;

//上行	 上传
typedef struct {
	uint8_t _func :4; //1:功能
	uint8_t _dfc :1; //DFC(数据流控制位)。 DFC=0表示从站可以接受数据，
	//DF =1表示从站缓冲区已满，无法接受新数据。
	//本规约规定(SF6)从站上送报文中DFC始终为0，即可以接受新数据，主站应能够处理DFC=1的报文

	uint8_t _acd :1;	//请求访问位ACD
	//ACD位用于上行响应报文中.ACD=1 表示终端有重要事件等待访问
	//ACD=0表示终端无事件数据等待访问.

	uint8_t _prm :1;
	//PRM =1：表示此帧报文来自启动站；PRM =0：表示此帧报文来自从动站
	uint8_t _dir :1;
} TCTRL_UP;

typedef union {
	TCTRL_UP up;
	TCTRL_DOWN down;
	uint8_t val;
} TCTRL;

typedef struct {
	uint8_t _begin;
	TCTRL _ctrl;
	uint8_t _addr;
	uint8_t _crc;
	uint8_t _end;
} TFIX_PKG;

//_TFt_pkg_head
typedef struct {
	uint8_t _begin;
	uint8_t _len;
	uint8_t _len_cfm;
	uint8_t _begin_cfm;
	TCTRL _ctrl;
	uint8_t _addr;
} TDLY_HEAD; //68 可变

typedef struct {
	TDLY_HEAD * pHead; //报文头
	uint8_t * pCrc; //校验码
	uint8_t * pEnd; //结束码

	unsigned char * pAsduBuf; //数据正文区
	unsigned char cAsduLen;
} TDLY_FRAME;

typedef struct {
	TRecord rec;
	TFIX_PKG * p10;
	TDLY_FRAME a68;
	TCTRL * pCtrl;
	unsigned char * pAddr;
} TFRAME;

#endif /* IEC103_H_ */
