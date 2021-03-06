/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_key_event.h
 * @brief      KEY 事件函数实现
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */

#include "VCAN_key_event.h"
#include "VCAN_UI_VAR.h"
#include "O_display.h"
#include "O_menu_control.h"
#include "main.h"
#include "O_speed.h"
#include "O_dir.h"
#include "VCAN_camera.h"
#include "VCAN_LCD.h"
//#include "NRF24L0.h"
//#include "NRF24L0_MSG.h"

int8 flag_key_select=0;
int8 flag_key_l_u_0=0;
int8 flag_key_l_u_1=0;
int8 flag_key_l_u_2=0;
int8 flag_key_l_u_3=0;
int8 flag_key_l_u_4=0;
int8 flag_key_l_u_5=0;
int8 flag_key_l_u_6=0;

//-------------------------------------------------
//                 舵机控制
//输入
//输出 void
//功能 计算中心加权并控制舵机输出
//日期
//作者
//--------------------------------------------------
void key_event_init()
{

    key_init(KEY_MAX);  //初始化 全部 按键
//    var_init();   //初始化临时变量
#if nrf_debug
    //同步全部数据并显示
    var_syn(VAR_MAX);       //同步全部 ,必须先同步再显示全部，因为有可能同步失败。
    var_display(VAR_MAX);   //显示全部
#endif

}


/*******************************************************************************
* Function Name: void deal_key_event()
* Input Param  : 无
* Output Param : 无
* Description  : 按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/
void deal_key_event()
{
    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //获得按键就进行处理
    {
        color_init();
        shua_one=0; //按下按键就允许特殊页面刷屏一次
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_U:     //上
              //  printf("上");
                deal_key_up();
                break;

            case KEY_D:      //下
              //  printf("下");
                deal_key_down();
                break;

            case KEY_L:      //左
                key_flag_clear=1;
              //  printf("左");
                deal_key_left();
                break;

            case KEY_R:      //右
              //  printf("右");
                key_flag_clear=1;
                deal_key_right();
                break;

            case KEY_B:      //选择
                {
                  key_flag_clear=1;
                  deal_key_select();
               //   printf("中");

                }
                break;

            case KEY_A:      //独立按键
              {
               //   printf("独立按键");
                  switch(flag_key_select)
                  {
                    case 0:
                      {

                      }
                      break;

                    case 1:
                      {

                      }
                      break;

                  case 2:
                      {
                      }
                      break;

                  case 3:
                      {
                      }
                      break;

                 }

              }
                break;

            case KEY_START:
              //  save_var(CAR_CTRL,CAR_START);
              //  var_syn(CAR_CTRL);
              //  var_display(CAR_CTRL);
                break;
            case KEY_STOP:
               // save_var(CAR_CTRL,CAR_STOP);
               // var_syn(CAR_CTRL);
               // var_display(CAR_CTRL);
                break;

            default:
                break;
            }
        }
        else if(keymsg.status == KEY_HOLD)
        {
            switch(keymsg.key)
            {

              Site_t site;
              site.x = 13;
              site.y = 3;

            case KEY_U:                //长按上按键
                {
                   //deal_key_up_1();
                 //  printf("长按下按键");
                }
                break;

            case KEY_D:                //长按下按键
                {
                   //deal_key_down_1();
                 //  printf("长按上按键");
                }
                break;

            case KEY_L:
              //  var_select(VAR_PREV_HOLD);
                break;

            case KEY_R:
              //  var_select(VAR_NEXT_HOLD);
                    SD_save_flag=1;
                    LCD_str(site,"sd_OK...", BLACK,RED);
                break;

            case KEY_B:                //长按 OK 键 同步全部数据并显示
              //  var_syn(VAR_MAX);       //同步全部 ,必须先同步再显示全部，因为有可能同步失败。
              //  var_display(VAR_MAX);   //显示全部

                    record_ext();       //存储
                    LCD_str(site,"record_OK...", BLACK,RED);

                break;

            case KEY_START:
              //  save_var(CAR_CTRL,CAR_START);
              //  while(var_syn(CAR_CTRL)==0);
                var_display(CAR_CTRL);
                break;

            case KEY_STOP:
              //  save_var(CAR_CTRL,CAR_STOP);
              //  while(var_syn(CAR_CTRL)==0);
              //  var_display(CAR_CTRL);

            default:            //不需要处理 cancel
                break;
            }
        }
        else
        {
            //KEY_UP ,不进行处理
        }
    }
}

 /*******************************************************************************
* Function Name: void deal_key_select()
* Input Param  : 无
* Output Param : 无
* Description  : 中间按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_select()
{
 switch(flag_key_select)
  {
   case 0:       //表示现在停留在菜单页面
    {
         switch(flag_key_l_u_0)
            {
             case 1:       //显示菜单页面
              {
                   flag_key_select=1;
                   flag_key_l_u_0=0;
              }
              break;

            case 2:        //显示图像页面
              {
                   flag_key_select=2;
                   flag_key_l_u_0=0;
              }
              break;

            case 3:        //显示舵机参数页面
              {
                   flag_key_select=3;
                   flag_key_l_u_0=0;
              }
              break;

            case 4:        //显示电机参数页面
              {
                   flag_key_select=4;
                   flag_key_l_u_0=0;
              }
              break;
            case 5:       //开车页面（不刷屏）
              {
                flag_key_select=5;
                flag_key_l_u_0=0;
                DELAY_MS(500) ;
                car_start_key();   //按键发车函数
              }
              break;
            case 6:        //显示电机参数页面
              {
                   flag_key_select=6;
                   flag_key_l_u_0=0;
              }
              break;
            }

    }
    break;
  case 1:        //表示现在停留在赛道显示页面
    {
       switch(flag_key_l_u_1)
        {
         case 0:       //表示现在停留在赛道显示页的“上一页”
          {
               flag_key_select=0;
               flag_key_l_u_1=0;

//               zhizhen=5;
          }
          break;
         case 1:       //表示现在停留在赛道显示页的“下一页”
          {
               flag_key_select=2;
               flag_key_l_u_1=0;

//               img_sd_exit();
          }
          break;
        }
    }
    break;
  case 2:        //表示现在停留在舵机信息显示页面
    {
         switch(flag_key_l_u_2)
          {
           case 0:       //表示现在停留在舵机信息显示页的“上一页”
            {
                 flag_key_select=1;
                 flag_key_l_u_2=0;
            }
            break;
           case 7:       //表示现在停留在舵机信息显示页的“下一页”
            {
                 flag_key_select=3;
                 flag_key_l_u_2=0;
            }
            break;
           case 8:       //表示现在停留在舵机信息显示页的“返回菜单”
            {
                 flag_key_select=0;
                 flag_key_l_u_2=0;
            }
            break;
          }
    }
    break;
  case 3:        //表示现在停留在电机信息显示页面
    {
         switch(flag_key_l_u_3)
          {
           case 0:       //表示现在停留在电机信息显示页的“上一页”
            {
                 flag_key_select=2;
                 flag_key_l_u_3=0;
            }
            break;
           case 7:       //表示现在停留在电机信息显示页的“下一页”
            {
                 flag_key_select=4;
                 flag_key_l_u_3=0;
            }
            break;
           case 8:       //表示现在停留在电机信息显示页的“返回菜单”
            {
                 flag_key_select=0;
                 flag_key_l_u_3=0;
            }
            break;
          }
    }
    break;
  case 4:        //表示现在停留在其他信息显示页面
    {
         switch(flag_key_l_u_4)
          {
           case 0:       //表示现在停留在电机信息显示页的“上一页”
            {
                 flag_key_select=3;
                 flag_key_l_u_4=0;
            }
            break;
           case 7:       //表示现在停留在电机信息显示页的“下一页”
            {
                 flag_key_select=6;
                 flag_key_l_u_4=0;
            }
            break;
           case 8:       //表示现在停留在电机信息显示页的“返回菜单”
            {
                 flag_key_select=0;
                 flag_key_l_u_4=0;
            }
            break;
          }
    }
    break;
  case 5:        //表示现在停留在开车显示页面
    {
      switch(flag_key_l_u_5)
      {
        case 1:       //表示现在停留在开车显示页的“停车”
            {
                 flag_key_select=0;
                 flag_key_l_u_5=0;

                 car_stop(0);
            }
            break;
        case 2:       //表示现在停留在开车显示页的“返回菜单”
            {
                 flag_key_select=0;
                 flag_key_l_u_5=0;
            }
            break;
      }
    }
    break;
  case 6:        //表示现在停留在其他信息显示页面
    {
         switch(flag_key_l_u_6)
          {
           case 0:       //表示现在停留在电机信息显示页的“上一页”
            {
                 flag_key_select=4;
                 flag_key_l_u_6=0;
            }
            break;
           case 7:       //表示现在停留在电机信息显示页的“下一页”
            {
                 flag_key_select=0;
                 flag_key_l_u_6=0;
            }
            break;
           case 8:       //表示现在停留在电机信息显示页的“返回菜单”
            {
                 flag_key_select=0;
                 flag_key_l_u_6=0;
            }
            break;
          }
    }
    break;
  default:
    {
        flag_key_select=0;
        flag_key_l_u_0=0;
        flag_key_l_u_1=0;
        flag_key_l_u_2=0;
        flag_key_l_u_3=0;
        flag_key_l_u_4=0;
        flag_key_l_u_5=0;
        flag_key_l_u_6=0;
    }
    break;
  }

}


 /*******************************************************************************
* Function Name: void deal_key_left()
* Input Param  : 无
* Output Param : 无
* Description  : 左按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_left()
{
    switch (flag_key_select)
    {
    case 0:
      {

      }
      break;
    case 1:
      {
          switch (flag_key_l_u_1)      //调整赛道信息的参数
            {
            case 0:    //上一页
              {

              }
              break;
            case 1:    //下一页
              {

              }
              break;
            }
      }
    case 2:
      {
          switch (flag_key_l_u_2)      //调整舵机的参数
            {
            case 0:     //上一页
              {

              }
              break;
            case 1:
              {
                   Kp1-=0.5;
              }
              break;
            case 2:
              {
                   Kp2-=0.05;
              }
              break;
            case 3:
              {
                   Kd-=0.5;
              }
              break;
            case 4:
              {
                MID_dir_duty--;
                Mid_duty--;
              }
              break;
            case 5:
              {
               //   Servos_output-=10;
              }
              break;
            case 6:
              {
                 //  DJ_Kd--;
              }
              break;
            case 7:      //下一页
              {

              }
              break;
            case 8:      //用于返回菜单
              {

              }
              break;

            }
      }
    case 3:
      {
          switch (flag_key_l_u_3)      //调整电机的参数
            {
            case 0:       //上一页
              {

              }
              break;
            case 1:
              {
                 //  KP2-=5;
               p1-=0.0001;
              }
              break;
            case 2:

              {
                speedwantD_set-=5;
                speedwantD=speedwantD_set;
              }
              break;
            case 3:
              {
                speedwantE_set-=5;
                speedwantE=speedwantE_set;
              }
              break;
            case 4:
              {
                KP2-=5;
              }
              break;
            case 5:
              {
                Ki2-=1;
              }
              break;
            case 6:
              {
               Kd2-=1;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }
    case 4:
      {
          switch (flag_key_l_u_4)      //调整其他的参数
            {
            case 0:       //上一页
              {

              }
              break;
            case 1:
              {
                camera_boundary--;
              }
              break;
            case 2:
              {
                follow_huan_set[1]--;
              //  game_plan--;
              }
              break;
            case 3:
              {
                follow_huan_set[2]--;

              }
              break;
            case 4:
              {
                follow_huan_set[3]--;
             //   Menu_data_num[2]-=5;  //  flag_chasu--;
              }
              break;
            case 5:
              {
                follow_huan_set[4]--;
            //    Kd2-=1;
             //   Kd22-=1;
              }
              break;
            case 6:
              {
                follow_huan_set[5]--;
            //   p2-=0.01;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }
    case 6:
      {
          switch (flag_key_l_u_6)      //调整shezhi的参数
            {
            case 0:       //上一页
              {

              }
              break;
            case 1:
              {
                   if(origin_chao_cont==1) origin_chao_cont=0;
                   else origin_chao_cont=1;
              }
              break;
            case 2:
              {
                   if(wrz_chao_cont==1) wrz_chao_cont=0;
                   else wrz_chao_cont=1;
              }
              break;
            case 3:
              {
                   if(huan_chao_cont==1) huan_chao_cont=0;
                   else huan_chao_cont=1;
              }
              break;
            case 4:
              {

              }
              break;
            case 5:
              {
            //    Kd2-=1;
             //   Kd22-=1;
              }
              break;
            case 6:
              {
            //   p2-=0.01;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }
    }
}


/*******************************************************************************
* Function Name: void deal_key_right()
* Input Param  : 无
* Output Param : 无
* Description  : 右按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_right()
{
      switch (flag_key_select)
    {
    case 0:
      {

      }
      break;
    case 1:
      {
          switch (flag_key_l_u_1)      //调整赛道信息的参数
            {
            case 0:     //上一页
              {

              }
              break;
            case 1:     //下一页
              {

              }
              break;
            }
      }
    case 2:
      {
          switch (flag_key_l_u_2)      //调整舵机的参数
            {
            case 0:      //上一页
              {

              }
              break;
            case 1:
              {
                  Kp1+=0.5;
              }
              break;
            case 2:
              {
                  Kp2+=0.05;
              }
              break;
            case 3:
              {
                  Kd+=0.5;
              }
              break;
            case 4:
              {
                MID_dir_duty++;
                Mid_duty++;
              }
              break;
            case 5:
              {
                 //  Servos_output+=10;
              }
              break;
            case 6:
              {
                 //  DJ_Kd++;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:      //返回菜单
              {

              }
              break;

            }
      }
    case 3:
      {
          switch (flag_key_l_u_3)      //调整电机的参数
            {
            case 0:       //上一页
              {

              }
              break;
            case 1:
              {
                p1+=0.0001;
                  //  Speed_set_tem+=5;
              }
              break;
            case 2:
              {
                speedwantD_set+=5;
                speedwantD=speedwantD_set;
              }
              break;
            case 3:
              {
                speedwantE_set+=5;
                speedwantE=speedwantE_set;
              }
              break;
            case 4:
              {
               KP2+=5;

              }
              break;
            case 5:
              {
               Ki2+=1;
              }
              break;
            case 6:
              {
                Kd2+=1;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }
    case 4:
      {
          switch (flag_key_l_u_4)      //调整qita的参数
            {
            case 0:       //上一页
              {
              }
              break;
            case 1:
              {
                camera_boundary++;  //  flag_chasu--;
              }
              break;
            case 2:
              {
                follow_huan_set[1]++;
              //  game_plan++;
              }
              break;
            case 3:
              {
                follow_huan_set[2]++;
             //   if(flag_change_speed)   flag_change_speed=0;
             //   else   flag_change_speed=1;
              }
              break;
            case 4:
              {
                follow_huan_set[3]++;
             //    Menu_data_num[2]+=5;//  flag_chasu++;
              }
              break;
            case 5:
              {
                follow_huan_set[4]++;
             //  Kd2+=1;
             //  Kd22+=1;
              }
              break;
            case 6:
              {
                follow_huan_set[5]++;
             //  p2+=0.01;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }
    case 6:
      {
          switch (flag_key_l_u_6)      //调整设置页的参数
            {
            case 0:       //上一页
              {

              }
              break;
            case 1:
              {
                   if(origin_chao_cont==1) origin_chao_cont=0;
                   else origin_chao_cont=1;
              }
              break;
            case 2:
              {
                   if(wrz_chao_cont==1) wrz_chao_cont=0;
                   else wrz_chao_cont=1;
              }
              break;
            case 3:
              {
                   if(huan_chao_cont==1) huan_chao_cont=0;
                   else huan_chao_cont=1;
              }
              break;
            case 4:
              {

              }
              break;
            case 5:
              {
            //    Kd2-=1;
             //   Kd22-=1;
              }
              break;
            case 6:
              {
            //   p2-=0.01;
              }
              break;
            case 7:     //下一页
              {

              }
              break;
            case 8:    //返回菜单
              {

              }
              break;
            }
      }

    }
}


/*******************************************************************************
* Function Name: void deal_key_up()
* Input Param  : 无
* Output Param : 无
* Description  : 上按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_up()
{
  switch (flag_key_select)
  {
  case 0:
    {
      if(flag_key_l_u_0==0)
        flag_key_l_u_0=6;
      else
        flag_key_l_u_0 --;

    }
    break;
  case 1:
    {
      if(flag_key_l_u_1==0)
        flag_key_l_u_1=1;
      else
        flag_key_l_u_1 --;

    }
    break;
  case 2:
    {
      if(flag_key_l_u_2==0)
        flag_key_l_u_2=8;
      else
        flag_key_l_u_2 --;
    }
    break;
  case 3:
    {
      if(flag_key_l_u_3==0)
        flag_key_l_u_3=8;
      else
        flag_key_l_u_3 --;
    }
    break;
  case 4:
    {

      if(flag_key_l_u_4==0)
        flag_key_l_u_4=8;
      else
        flag_key_l_u_4 --;
    }
    break;
  case 5:
    {

      if(flag_key_l_u_5==0)
        flag_key_l_u_5=2;
      else
        flag_key_l_u_5 --;
    }
    break;
  case 6:
    {

      if(flag_key_l_u_6==0)
        flag_key_l_u_6=8;
      else
        flag_key_l_u_6 --;
    }
    break;
  }

}

/*******************************************************************************
* Function Name: void deal_key_down()
* Input Param  : 无
* Output Param : 无
* Description  : 下按键处理
* Author Data  : 2015/5/27  by momo
******************************************************************************/

void deal_key_down()
{

    switch (flag_key_select)
    {
    case 0:
      {
        flag_key_l_u_0 ++;
        if(flag_key_l_u_0>6)
          flag_key_l_u_0=0;
      }
      break;
    case 1:
      {
        flag_key_l_u_1 ++;
        if(flag_key_l_u_1>1)
          flag_key_l_u_1=0;
      }
      break;
    case 2:
      {
        flag_key_l_u_2 ++;
        if(flag_key_l_u_2>8)
          flag_key_l_u_2=0;
      }
      break;
    case 3:
      {
        flag_key_l_u_3 ++;
        if(flag_key_l_u_3>8)
          flag_key_l_u_3=0;
      }
      break;
    case 4:
      {
        flag_key_l_u_4 ++;
        if(flag_key_l_u_4>8)
          flag_key_l_u_4=0;
      }
      break;
    case 5:
      {
        flag_key_l_u_5 ++;
        if(flag_key_l_u_5>2)
          flag_key_l_u_5=0;
      }
      break;

    case 6:
      {
        flag_key_l_u_6 ++;
        if(flag_key_l_u_6>8)
          flag_key_l_u_6=0;
      }
      break;
    }
}


  /*******************************************************************************
  * Function Name: void deal_key_up_1()
  * Input Param  : 无
  * Output Param : 无
  * Description  : 长按上按键处理
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

  void deal_key_up_1()
  {
    /*
    if(flag_key_select==2)
    {
      switch (flag_key_l_r)
      {
      case 0:
        {
          //speed_add_start+=10;
        }
        break;
      case 1:
        {
          //speed_add_max+=10;
        }
        break;
      case 2:
        {
          //speed_add_radio+=10;
        }
        break;
      case 3:
        {
          //speed_want_init+=10;
        }
        break;
      case 4:
        {
          //ZL_value+=10;

        }
        break;
      case 5:
        {
         // ZR_value+=10;

        }
        break;
      case 6:
        {
         // speed_P+=10;

        }
        break;
      case 7:
        {
         // speed_I+=10;

        }
        break;
      case 8:
        {
          //direction_P+=10;
        }
        break;

      case 9:
        {
         // direction_D+=10;
        }
        break;
      case 10:
        {
         // angle_P+=10;
        }
        break;
      }
    }
    else if(flag_key_select==1)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {

          //sensitivity+=10;
        }
        break;
      case 1:
        {

         // ccd_error_value+=10;
        }
        break;
      case 2:
        {
         // flag_zhijiao_double+=10;
        }
        break;
      }
    }
        else if(flag_key_select==0)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {
        //acc_init+=10;
        }
        break;
      }
    }   */
  }

/*******************************************************************************
  * Function Name: void deal_key_down_1()
  * Input Param  : 无
  * Output Param : 无
  * Description  : 长按下按键处理
  * Author Data  : 2015/5/27  by momo
  ******************************************************************************/

  void deal_key_down_1()
  {
    /*
    if(flag_key_select==2)
    {
      switch (flag_key_l_r)
      {
      case 0:
        {
         // speed_add_start-=10;
        }
        break;
      case 1:
        {
          //speed_add_max-=10;
        }
        break;
      case 2:
        {
         // speed_add_radio-=10;
        }
        break;
      case 3:
        {
         // speed_want_init-=10;
        }
        break;
      case 4:
        {

         // ZL_value-=10;

        }
        break;
      case 5:
        {
         // ZR_value-=10;

        }
        break;
      case 6:
        {
         // speed_P-=10;

        }
        break;
      case 7:
        {
         // speed_I-=10;

        }
        break;
      case 8:
        {
         // direction_P-=10;
        }
        break;


      case 9:
        {
         // direction_D-=10;
        }
        break;
      case 10:
        {
         // angle_P-=10;
        }
        break;
      case 11:
        {
         // angle_D-=10;

        }
        break;
      }
    }

    else if(flag_key_select==1)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {

         // sensitivity-=10;
        }
        break;
      case 1:
        {

         // ccd_error_value-=10;
        }
        break;
      case 2:
        {
         // flag_zhijiao_double-=10;
        }
        break;
      }
    }
        else if(flag_key_select==0)
    {
      switch(flag_key_l_r)
      {
      case 0:
        {
        // acc_init-=10;
        }
        break;
      }
    }    */
  }




