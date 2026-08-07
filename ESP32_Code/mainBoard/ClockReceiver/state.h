#ifndef STATE_H
#define STATE_H

enum SystemState
{
    STATE_INIT,   // 初期化
    STATE_HOMING, // ホーミング中
    STATE_RUN,    // 通常運転
    STATE_JUMP,   // JUMP動作
    STATE_ERROR   // エラー
};

void stateInit();
void stateUpdate();

SystemState getState();

#endif