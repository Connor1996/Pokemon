#ifndef DEFINE_H
#define DEFINE_H

#define DATABASE_NAME "data.db"

/*--------------------请求类型------------------------*/
#define LOG_IN              2       // 登陆请求
#define SIGN_UP             3       // 注册请求
#define GET_ONLINE_LIST     4       // 获取在线列表请求
#define GET_OFFLINE_LIST    5       // 获取离线列表请求
#define GET_USER_BAG        6       // 获取用户背包内容请求
#define GET_USER_ACH        7       // 获取用户成就请求
#define GET_POKEMON_LIST    8       // 获取对战精灵列表请求
#define GET_POKEMON_INFO    9       // 获取对战精灵信息请求
#define GAME_WIN            10      // 反馈对战胜利
#define GAME_LOSE           11      // 反馈对战失败
#define LOSE_POKEMON        12      // 反馈损失小精灵
#define GET_ONE_POKEMON     13      // 获得一个新小精灵请求

/*--------------------返回状态------------------------*/

#define LOG_IN_SUCCESS      65      // 登陆成功
#define LOG_IN_FAIL_WP      66      // 登陆失败，密码错误
#define LOG_IN_FAIL_AO      67      // 登陆失败，用户已经在线

#define SIGN_UP_SUCCESS     68      // 注册成功
#define SIGN_UP_FAIL        69      // 注册失败

#define SERVER_ERROR        0       // 服务器错误
#define QUERY_SUCCESS       1       // 查询成功
#define ACCEPT              2       // 请求成功处理


#endif // DEFINE_H
