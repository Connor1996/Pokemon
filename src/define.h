#ifndef DEFINE_H
#define DEFINE_H

#define DATABASE_NAME "data.db"

/*--------------------请求类型------------------------*/
#define LOG_IN              2
#define SIGN_UP             3
#define GET_ONLINE_LIST     4
#define GET_OFFLINE_LIST    5
#define GET_USER_BAG        6
#define GET_USER_ACH        7
#define GET_POKEMON_LIST    8
#define GET_POKEMON_INFO    9
#define GAME_WIN            10
#define GAME_LOSE           11

/*--------------------返回状态------------------------*/

#define LOG_IN_SUCCESS      65
#define LOG_IN_FAIL_WP      66
#define LOG_IN_FAIL_AO      67

#define SIGN_UP_SUCCESS     68
#define SIGN_UP_FAIL        69

#define SERVER_ERROR        0
#define QUERY_SUCCESS       1
#define ACCEPT              2


#endif // DEFINE_H
