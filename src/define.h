#ifndef DEFINE_H
#define DEFINE_H

#define DATABASE_NAME "data.db"

/*--------------------请求类型------------------------*/
#define LOG_IN          2
#define SIGN_UP         3
#define GET_ONLINE_LIST 4
#define GET_USER_BAG    5

/*--------------------返回状态------------------------*/

#define LOG_IN_SUCCESS  65
#define LOG_IN_FAIL_WP  66
#define LOG_IN_FAIL_AO  67

#define SIGN_UP_SUCCESS 68
#define SIGN_UP_FAIL    69

#define SERVER_ERROR    0
#define QUERY_SUCCESS   1


#endif // DEFINE_H
