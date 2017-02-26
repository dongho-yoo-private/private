//
//  schedule_manager.cc
//  af-chaos_framework
//
//  Created by CJ0048 on 2014/04/09.
//  Copyright (c) 2014年 B04350. All rights reserved.
//

#include "schedule/schedule_manager.h"
#include "application/application_manager.h"
#include <core/library/lib_time.h>

CHAOS_BEGIN_NAMESPACE

ScheduleManager::ScheduleManager()
{
}

ScheduleManager::~ScheduleManager()
{
}

Bool ScheduleManager::Initialize()
{
  Bool ret = true;
  
  //構造体初期化
  for(S32 i=0; i<MAX_SCHEDULE_COUNT; i++){
    schedule_list_[i].isUsed = false;
    schedule_list_[i].added_time = 0;
    schedule_list_[i].time_interval = 0;
    schedule_list_[i].isOnce = false;
    schedule_list_[i].times = 1;
    schedule_list_[i].callback = 0;
    schedule_list_[i].user_param = 0;;
  }
  
  return ret;
}

Bool ScheduleManager::Terminate()
{
  Bool ret = true;
  
  return ret;
}

//スケジュールを登録する
S32 ScheduleManager::AddSchedule(F32 interval, TSchedullerCallback callback, void* user_param)
{
  
  for(S32 i=0; i<MAX_SCHEDULE_COUNT; i++)
  {
    //最初に見つかった配列に登録する
    if(schedule_list_[i].isUsed == false)
    {
      schedule_list_[i].added_time = adk::LIB_TimeGetCurrentMsec(0);
      schedule_list_[i].time_interval = interval;
      //intervalが0に指定されたものは１回しか実行しない
      if(interval == 0)
      {
        schedule_list_[i].isOnce = true;
      }
      else
      {
        schedule_list_[i].isOnce = false;
      }
      schedule_list_[i].times = 0;
      schedule_list_[i].callback = callback;
      schedule_list_[i].user_param = user_param;
      
      return i;
    }
    
    //配列に登録できない場合
    if(i >= MAX_SCHEDULE_COUNT)
    {
      printf("スケジュールリストに登録できません。\n");
      ADK_ASSERT(0);
    }
  }
  
  return -1;
}

//一度だけスケジュールを実行する
S32 ScheduleManager::AddScheduleOnce(F32 interval, TSchedullerCallback callback, void* user_param)
{
  for(S32 i=0; i<MAX_SCHEDULE_COUNT; i++)
  {
    //最初に見つかった配列に登録する
    if(schedule_list_[i].isUsed == false)
    {
      schedule_list_[i].added_time = adk::LIB_TimeGetCurrentMsec(0);
      schedule_list_[i].time_interval = interval;
      schedule_list_[i].isOnce = true;
      schedule_list_[i].times = 0;
      schedule_list_[i].callback = callback;
      schedule_list_[i].user_param = user_param;
      
      return i;
    }
    
    //配列に登録できない場合
    if(i >= MAX_SCHEDULE_COUNT)
    {
      printf("スケジュールリストに登録できません。\n");
      ADK_ASSERT(0);
    }
  }
  
  return -1;
}

//スケジュールを解除する
void ScheduleManager::RemoveSchedule(S32 id)
{
  //該当idのisUsedをfalseにする
  schedule_list_[id].isUsed = false;
}

//スケジュールを実行する
void ScheduleManager::DispatchSchedule()
{
  for(S32 i=0; i<MAX_SCHEDULE_COUNT; i++)
  {
    if(schedule_list_[i].isUsed == true)
    {
      //スケジュール登録時からの経過時間を計算する
      int elapsed_time = adk::LIB_TimeGetCurrentMsec(0) - schedule_list_[i].added_time;
      //次に実行するべき時間を計算する
      int next_interval = schedule_list_[i].time_interval * schedule_list_[i].times;
      if(elapsed_time >= next_interval)
      {
        //時間がきたものは実行する
        schedule_list_[i].callback(schedule_list_[i].user_param);
        //１回しか実行しないものはスケジュール解除する
        if(schedule_list_[i].isOnce == true)
        {
          RemoveSchedule(i);
        }
        else
        {
          //実行回数を+1する
          schedule_list_[i].times++;
        }
      }
    }
  }
}

CHAOS_END_NAMESPACE
