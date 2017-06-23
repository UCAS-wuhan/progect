/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */
#include "common.h"
#include "include.h"

long int speedwant=0;

//===============设置变量===============
//速度
int16 speedwantD_set=100;
int16 speedwantE_set=100;
//起跑线超车控制标志
uint8 origin_chao_cont=  0 ;
//直道超车控制
uint8 wrz_chao_cont=     0 ;
//环道超车控制标志
uint8 huan_chao_cont=    0 ;
//SD卡操作模式
uint8 SD_op_mode=        0 ;     //  0-保存图像  1-连续读图像  2-单步读图像(单步暂时未写，可以直接单步调试)
//超车次数设置
uint8 chao_car_cnt_set=  5 ;
//=======================================
uint8 CAR_MODEL;    //1-号车， 2-2号车， 3-3号车
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;
int16 var_wire[6]={0};
uint8 chao_cnt_total=0;
uint8 front_car=0,weizhi_turn=0;
int8 tongbu[10];
int8 diff=0;
int8 cut_1=0,cut_2=0;
int8 chao_huan=0,chao_zhi=0,Ramp_tong=0;  //此标志为了让前车在同一超车地点只触发一次超车

char info=0;

int8 sequence=0;  //车的顺序，为0的时候则前后车标志位与var1相同，1则相反
//解压后的图像，19200=160*120
uint8  img[160*120];                                     //解压后的图像

//-----------开车超声波检测前后车
void detection_front_back()
{
  if( gpio_get(receive_flag)==0 )   front_car=1;
  else front_car=0;
}
//处理NRF接受到的数据
void recive_nrf()
{
  var_wire[0]=var1;
  var_wire[1]=var2;
//  var_wire[2]=var3;
//  var_wire[3]=var4;
//  var_wire[4]=var5;
//  var_wire[5]=var6;
  if(var3==1) var_wire[3]=-var4;
  else var_wire[3]=var4;
  if(var5==1) var_wire[5]=-var6;
  else var_wire[5]=var6;
}
//NRF发送数据
void send_nrf()
{
  //var1（uint8） 用来表示图像，每次换一幅图像就是一个脉冲，
  //如果环道 fiag_huan   就是 2-3 转换
  if( fiag_huan )
  {
    if(var3==2) var3=3;
    else var3=2;
  }
  else
  {
    if(var3==0) var3=1;
    else var3=0;
  }
  //var2(uint8)  用来表示 当前速度 speedwant
  var5=speedwant;
  //var3(uint16) 用来表示 左轮PWM波正负
  //var4(uint16) 用来表示 左轮PWM波
  if(speedaboutangle2<0)
  {
    var1=1;
    var4=abs(speedaboutangle2);
  }
  else
  {
    var1=0;
    var4=speedaboutangle2;
  }
  //var5(uint32) 用来表示 右轮PWM波正负
  //var6(uint32) 用来表示 右轮PWM波
  if(speedaboutangle<0)
  {
    var2=1;
    var6=abs(speedaboutangle);
  }
  else
  {
    var2=0;
    var6=speedaboutangle;
  }
  updata_var(VAR1);
  updata_var(VAR2);
  updata_var(VAR3);
  updata_var(VAR4);
  updata_var(VAR5);
  updata_var(VAR6);
  var_syn(VAR_MAX);       //同步全部 ,必须先同步再显示全部，因为有可能同步失败。
}
//SD卡保存和处理
void SD_save_deal()
{
  if(( SD_save_flag==0 || SD_save_flag==1) && Motor_En==1 )       //
  {
    img_sd_save(imgbuff,CAMERA_SIZE);
  }
  if( yiting==1 &&  abs(leftval)<10 && abs(rightval)<10 )    //
  {
    SD_save_flag=1;
  }
}
//超声波接收不到数据时计数清零
void ceju_zhi0()
{
  static uint8 n_cut=0;
  if(gpio_get(receive_flag)==0)
  {
    n_cut++ ;
    if( n_cut>10)
    {
      n_cut=0;
      pit_close (PIT3);  //关闭计数器（此时计数值清零）
      timevar=0;
      s_distance=0;
    }
  }
  else
  {
    n_cut=0;
    //能接收到数据时自动校准前后车（超车情况除外）
    if( front_car==1 && L_wrz_flag==0 && R_wrz_flag==0 && fiag_huan==0 ) uart_putchar(UART3,turn_car);
  }
}

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       山外 无线调试 测试实验
 */
void  main(void)
{
    com_e     com;                               //无线通讯
    nrf_result_e nrf_result;

    init_sys();     //总初始化
    if(boma[5]==1)  img_sd_init( SD_op_mode );

    //------------开车检测前后车
    detection_front_back();
    //---------------------------
    while(1)
    {
//        uart_putchar(UART3,Start);
        /************************ 图像采集和显示  ***********************/
        if(SD_op_mode==0) camera_get_img();                           //摄像头采集获取图像
        else img_sd_read( buff, CAMERA_SIZE , zhizhen );              //SD卡获取图像
        if( zhizhen<((picture_sum-1)*CAMERA_SIZE+4) && flag_key_select==1 && flag_key_l_u_1==1 && boma[5]==1) zhizhen+=CAMERA_SIZE;
//            printf("666");

        /************************ 图像解压        ***********************/
        if(SD_op_mode==0) img_extract1(img,imgbuff);          //摄像头图像解压
        else img_extract1(img,buff);                          //SD卡  图像解压

        /************************ 图像处理        ***********************/
        median_line_extract(img);

        /*******************      起跑线      ***************************/
        if(Ramp_flag==0) start_line(img);

        /*******************      障碍      ***************************/
        if(never_obstacle_flag==0&&Ramp_flag==0&&fiag_huan==0&&obstancle_acc==0&&shi_zi_count==0)
        {
//          avoid_obstacle(img);                    //躲避障碍
        }

        quan_dir_duty_new();				    //算出每一帧图像的中线权重

//        hswy_sendimg(img);         //红书伟业上位机图像传输

#if nrf_debug
        /********************** 无线发送和接收数据  ***********************/
        if( 0 )     //为 0 表示发送端  1 表示接收端
        {
          do
          {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
          }while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出

          //--------处理接收数据-----------
        recive_nrf();
//        vcan_sendware(var_wire, sizeof(var_wire));
        }
        else
        //--------开始发送数据-----------
         send_nrf();
#endif


        /*********************** 按键消息 处理  ***********************/
        deal_key_event();

        LCD_show();

        if(SD_op_mode==0 && boma[5]==1)  SD_save_deal();

        Ramp_mesure();    //坡道识别

        ceju_zhi0();      //测距清零

    }
}



