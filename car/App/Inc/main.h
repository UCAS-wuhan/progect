#ifndef __MAIN_H
#define __MAIN_H
         //定义存储接收图像的数组,2400=160*120/8
extern uint8 img[160*120];

#define nrf_debug 0    //0-关闭无线传输， 1-开启无线传输
#define CAR_MODEL 1    //1-号车， 2-2号车， 3-3号车

extern uint8  var1, var2;
extern uint16 var3, var4;
extern uint32 var5, var6;

extern int16 speedwantD_set;
extern int16 speedwantE_set;

extern uint8 front_car,weizhi_turn;
extern int8 tongbu[10];
extern int8 input,cut_1,cut_2;
extern int8 diff;
extern long int speedwant;
extern int8 chao_one,chao_one2;  //此标志为了让前车在同一超车地点只触发一次超车
extern int8 sequence;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反

#endif //__MAIN_H