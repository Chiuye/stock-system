#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QVariantList>
//用户
extern QString g_account;
extern QString g_password;
extern QString g_name;
extern QString g_phone;
extern bool g_isvip;
//数据库的用户名、密码
extern QString sqluser;
extern QString sqlpass;
//股票
extern QString g_stockcode;
extern QString g_stockname;
//窗口传值
extern int glink;

#endif // GLOBAL_H
