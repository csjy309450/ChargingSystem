ChargingSystem
===

羽毛球场地预约系统
---

#1 需求分析

##1.1 参数配置

###1.1.1 可用场地 

```
{A, B, C, D}
```

###1.1.2 每日场地可用时间

```
周一~周末 09:00~22:00
```

###1.1.3 时段收费

```
周一~周五
	9:00 ~ 12:00 30元/时
	12:00 ~ 18:00 50元/时
	18:00 ~ 20:00 80元/时
	20:00 ~ 22:00 60元/时
周六~周⽇
	9:00 ~ 12:00 40元/时
	12:00 ~ 18:00 50元/时
	18:00 ~ 22:00 60元/时
```

###1.1.4 标记设定

```
取消预定 C
```

###1.1.5 取消预定罚金(占总费用)

```
周一~周五 50%
周六~周⽇ 25%
```

##1.2 输入

###1.2.1 预定

```
格式：	{⽤户ID} {预订⽇期 yyyy~MM~dd} {预订时间段 HH:mm~HH:mm} {场地}
样例：	U123 2016-06-02 20:00~22:00 A
说明：	
	*时间段的起⽌时间必然为整⼩时, 否则报错;
	*如预订与已有预订冲突, 也会报错.
```

###1.2.2 取消预定

```
格式：	{⽤户ID} {预订⽇期 yyyy~MM~dd} {预订时间段 HH:mm~HH:mm} {场地} {取消标记}
样例：	U123 2016-06-02 20:00~22:00 A C
要求:		
	*取消标记只能是C, 若为其他字符则报错;
	*时间段的起⽌时间必然为整⼩时, 否则报错;
	*只能完整取消之前的预订, 不能取消部分时间段;
	*取消预订的请求, 必须与之前的预订请求严格匹配, 需要匹配的项有⽤户ID, 预订⽇期, 预订时间段, 场地.
```

###1.2.3 打印场馆收⼊汇总

```
格式：输⼊⼀个空⾏，代表打印收⼊汇总
```

##1.3 输出

###1.3.1 收出汇总信息输出

>收入汇总信息要求以不同的场地分组打印各场地的预约和消约项带来的收入信息

*要求*

```
*如果同⼀场地同⼀时间段有多条预定记录，则显示多条
*收⼊记录以时间顺序升序排列
```

*示例*

```
> 收⼊汇总
> ---
> 场地:A
> 2016-06-02 09:00~10:00 违约⾦ 15元
> 2016-06-02 10:00~12:00 60元
> 2016-06-03 20:00~22:00 120元
> ⼩计： 195元
> 		
> 场地:B
> 2016-06-04 09:00~10:00 40元
> ⼩计： 40元
> 
> 场地:C
> ⼩计： 0元
> 
	 场地:D
> ⼩计： 0元
> ---
> 总计: 235元
```

###1.3.2 错误信息输出

#2 详细设计

##2.0 系统模块简述

> 羽毛球场地预约系统，按功能划分为6个子模块

| Name             |          |
| ---------------- | -------- |
| cs_configuration | 系统配置模块   |
| cs_exception     | 差错校验模块   |
| cs_database      | 数据库模块    |
| cs_log           | 日志模块     |
| user_interface   | 用户使用接口模块 |
| tools            | 通用功能模块   |

##2.1 系统配置模块(cs_configuration)

###2.1.1 配置文件说明

> 配置文件规定收费系统的所有规则，包括：可用场地，每日可用时间，时段费用，违约金，输入格式的正则表达式等

###2.1.2 配置文件数据字典

> @：配置字段名标识
>
> \#：	行注释标识
>
> {}:	一个字段的所有配置信息含在一对大括号中，且其中每个取值也包含在一对{}中并以“ ,”分隔，如@site={{A},{B},{C}};

| Name            |                      |
| --------------- | -------------------- |
| week            | 周天取值字典               |
| date_format     | 日期字符串的正字表达式          |
| time_format     | 时间字符串的正字表达式          |
| week_day_format | 周天字符串的正字表达式          |
| site            | 有效场馆设定               |
| usable_time     | 一周内的场馆可用日及每天场馆可用时间设定 |
| charge          | 每天时段收费规则设定           |
| penalty         | 违约金设定                |
| book_format     | 预约字符串正则表达式           |
| cancle_format   | 消约字符串正则表达式           |

> 样例

```
# global field
@week={...}; # open day in a week
@date_format={...}; # regular expression of date (yyyy-mm-dd).
@time_format={...}; # regular expression of time (hh:mm).
@week_day_format={...}; # regular expression of week day.
# system field
@site={...}; # usable sport site.
@usable_time={...}; # usable time in a day.
@charge={...}; # charge in a day.
@penalty={...}; # penalty.
@book_format={...}; # regular expression of book input.
@cancle_format={...}; # regular expression of cancle book input.
```

###2.1.3 定义的数据结构

| Name                          | Description                              |
| ----------------------------- | ---------------------------------------- |
| struct baste_conf_data        | 配置项数据对象(抽象类)，不同类型的配置文件可能定义不同的配置参数项，派生类必须实现其配置项成员变量 |
| struct badminton_conf_data    | 羽毛球收费系统的配置项数据对象(派生于baste_conf_data)      |
| struct serialize_conf_data    | 序列化的配置项数据对象，以序列化字符串方式处理配置项，方便从配置文件读取和分割配置项(派生于baste_conf_data) |
| class base_configuration      | 配置文件控制对象(抽象类)，不同的配置文件读取及操作需由派生类自定义       |
| class badminton_configuration | 定义了羽毛球系统的配置文件控制对象(派生于base_configuration) |

##2.2 检错模块(cs_exception)

> 为其他模块提供基础差错校验服务，如序列化字符串的格式检测。并定义了各种已知类型的错误代码。

###2.2.1 输入格式检错

> *通过设置配置文件中的正则表达式开检测输入字符串的格式是否匹配预订或取消项
> *格式检测应包含，用户id, 日期, 时间, 场地, 消约标识的正则表达式校验
> *其中规定用户id以字母U开头后面为流水号，日期要求识别闰年非闰年，场地要求不可预约有效场地外

```
function：
	Error_Num cs_exception::field_format_check(const string & str, const Format_Type _type)对各个字段的序列化格式的正则化校验
```

###2.2.2 输入有效性检错

> 实际上输入数据的有效性，或者说数据内容的正确性不由cs_exception模块负责。因为要验证数据的有效性牵扯多个其他模块如tools、cs_configuration、cs_database、cs_log。出于模块内聚性的考虑将这这一部分验证操作交由user_interface模块负责。cs_exception只提供最基本的差错检验方法。

```
*预订输入有效性检测：
1) 预订日期是否符合逻辑, 如是否需要检测输入预约不能早于某个设定日期等(题目未定义，但留有校验接口)
2) 查看时间段是否有效（是否存在预定开始时刻小于等于结束时刻，或时间超出场馆工作时间）
3) 查看预订记录中是否有冲突预订（与已预订记录存在“日期 时间”交叉）
*取消输入有效性检测：
4) 查找数据库中是否已有该项目
```

```
function：
	ERROR_NUMBER user_interface::record_available_check(BC_table_item & _item); //总的有效性检测接口
	Error_Num user_interface::isDateAvailable(const CS_date & _item); //对应检测项 1)
	Error_Num user_interface::isTimeRangeAvailable(const Time_range & _item);//对应检测项 2)
	Error_Num user_interface::isRecordCollision(const BC_table_item & _item);//对应检测项 3)，需要使用数据库模块提供的查询服务
long user_interface::isRecordExist(const BC_table_item & _item);////对应检测项 4),需要使用数据库模块提供的查询服务和删除服务
```

###2.2.4 数据类型

| Name                    | Description |
| ----------------------- | ----------- |
| typedef int Error_Num   | 错误类型        |
| typedef int Format_Type | 格式校验类型      |
| class CS_exception_base | 检错对象抽象基类    |
| class BC_exception      | 羽毛球系统检错对象   |

###2.2.3 软件配置项检测

> 与输入有效性检测类似，软件配置项的差错检验，在cs_configuration模块中实现，cs_exception只提供最基本的差错检验方法。

##2.3 数据库模块(cs_database)

>存储各个场馆的有效预定及取消预定记录
>备份数据记录
>系统出错回滚

###2.3.1 数据类型

| Name                   | Description |
| ---------------------- | ----------- |
| struct CS_table_base   | 数据表基类对象     |
| struct BC_table_base   | 预约/消约数据表对象  |
| class CS_database_base | 数据库基类对象     |
| class BC_database      | 羽毛球系统数据库对象  |

##2.4日志模块(cs_log)

> 记录所有事务，并负责向屏幕打印的服务

###4.2.1 数据类型

| Name              | Description |
| ----------------- | ----------- |
| class CS_Log_base | 日志对象基类      |
| class BC_Log      | 羽毛球系统日志对象   |

##2.5 用户使用接口(user_interface)

###2.5.1 输入控制

```
对输入的预约项或消约项进行序列化格式校验
function:
ERROR_NUMBER input_format_check(string & str); //定义于user_interface对象中，内部封装了cs_exception对象的field_format_check()函数
```

###2.5.2 输出控制

###2.5.3 清算子模块(BC_liquidation)

>定义于user_interface模块的清算子模块主要用于对数据库中的已有项进行清算，统计收入。会调用cs_configuration模块提供的服务读取并校验收费标准，会调用cs_database提供的查询服务接口获取数据库项目

####2.5.3.1 数据类型

##2.6 工具模块(tools)

>工具模块定义所有公共的数据类型和公用的方法

###2.6.1 定义公用数据类型

| Name                      | Description |
| ------------------------- | ----------- |
| typedef double Fee        | 金额数据类型      |
| typedef double Proportion | 百分比数据类型     |
| class CS_week_day         | 周天对象        |
| class CS_time             | 时间对象        |
| class CS_date             | 日期对象        |
| class Ser_record          | 序列化字段项对象    |
| class Time_range          | 时间区间对象      |
| class Day_range           | 日期区间对象      |
| class Usable_time_item    | 场地可用时间表项    |
| class Time_charge_item    | 场地时段收费表项    |
| class Charge_item         | 场地收费表项      |
| class Penalty_item        | 违约金表项       |
| class table_item_base     | 数据库记录基类     |
| class BC_table_item       | 数据库记录对象     |
| class class file_reader   | 文件读写对象      |

###2.6.2 定义公用的方法

| Name                                     |           |
| ---------------------------------------- | --------- |
| template<typename T> T str2number(string snum, const string type) | 字符串->数值转换 |
| template<typename T> string number2str(T num, unsigned int _width) | 数值转换->字符串 |
| void splite_item(string &value, vector<string> &vec) | 序列化表项切分   |

#3 测试用例

##3.1 输入项格式检测

> <用例1>: 包含作业文档中用例

```
U001 2016-02-28 09:00~22:00 A
U001 2016-06-02 00:00~22:00 A
U001 2016-06-02 22:00~22:00 A
U001 2000-02-29 22:00~22:00 A
abcdefghijklmnopqrst1234567890
U0012016-06-0222:00~22:00A
U00_12016-06-02_22:00~22:00_A
SSXAA 2016-06-02 22:00~22:00 A
U0120 2016-06-02 22:00~22:00 A
U001 0000-06-02 22:00~22:00 A
U001 012-06-02 22:00~22:00 A
U001 001929-06-02 22:00~22:00 A
U001 2011-02-29 22:00~22:00 A
U001 aaaa-06-02 22:00~22:00 A
U001 2016-19-02 22:00~22:00 A
U001 2016-00-02 22:00~22:00 A
U001 2016-06-00 22:00~22:00 A
U001 2016-06-33 22:00~22:00 A
U001 2016-06-02 22:90~22:00 A
U001 2016-06-02 aa:aa~22:00 A
U001 2016~06~02 aa:aa-22:00 A
U001 2016-06-02 aa:aa~22:00 AD
U001 2016-06-02 aa:aa~22:00 3
```

> 正确输出：

```
#4个 (注：此过程仅为格式字符串检测，所以22:00~22:00或22:00~02:00这种输入是合法的，其数据的有效性在后面校验，实际上加入有效性校验后只有第一条输入合法)
> Success: the booking is accepted!
...
#19个
> Error: the booking is invalid!
...
```

##3.2 输入项有效性校验

> <用例2>：

```
U002 2017-08-01 19:00~22:00 A
U001 2016-06-02 22:00~22:00 A
U002 2017-08-01 23:10~20:10 A
U002 2017-08-01 21:00~22:10 A
U002 2017-08-01 14:00~22:10 A
U003 2017-08-02 13:00~17:00 B
U003 2017-08-03 15:00~16:00 C
U005 2017-08-05 09:00~11:00 D
U002 2017-08-01 19:00~22:00 A C
U003 2017-08-01 18:00~20:00 A
U002 2017-08-01 19:00~22:00 A C
```

> 有效输出：

```
> Success: the booking is accepted!
> Error: the booking is invalid! #预约时间无效（预约时间为零）
> Error: the booking is invalid! #预约时间无效（前时刻晚于后时刻）
> Error: the booking is invalid! #预约时间无效（超时）
> Error: the booking is invalid! #预约时间无效（超时）
> Success: the booking is accepted!
> Success: the booking is accepted!
> Success: the booking is accepted!
> Success: the booking is accepted! #注：实际上接收消约输入提示应将“book”用“cancle”代替，因为是消约操作成功(程序中按照文档需求设定)
> Success: the booking is accepted!
> Error: the booking being cancelled does not exist! #当前消约项不存在
```

> <用例3>: 作业文档中用例

```
U002 2017-08-01 19:00~22:00 A
U003 2017-08-01 18:00~20:00 A
U002 2017-08-01 19:00~22:00 A C
U002 2017-08-01 19:00~22:00 A C
U003 2017-08-01 18:00~20:00 A
U003 2017-08-02 13:00~17:00 B
```

> 有效输出

```
> Success: the booking is accepted!
> Error: the booking conflicts with existing bookings!
> Success: the booking is accepted!
> Error: the booking being cancelled does not exist!
> Success: the booking is accepted!
> Success: the booking is accepted!
```

##3.3 输出收入汇总检验

><用例4>：（用<例2>输入的基础上测试收入）

```
> 收入汇总
> ---
> 场地:A
> 2017-08-01 19:00~22:00 违约金 100元
> 2017-08-01 18:00~20:00 160元
> 小计: 260元
>
> 场地:B
> 2017-08-02 13:00~17:00 200元
> 小计: 200元
>
> 场地:C
> 2017-08-03 15:00~16:00 50元
> 小计: 50元
>
> 场地:D
> 2017-08-05 09:00~11:00 80元
> 小计: 80元
> ---
> 总计: 590
```

> <用例5>：（用<例3>输入的基础上测试收入）

```
> 收入汇总
> ---
> 场地:A
> 2017-08-01 19:00~22:00 违约金 100元
> 2017-08-01 18:00~20:00 160元
> 小计: 260元
>
> 场地:B
> 2017-08-02 13:00~17:00 200元
> 小计: 200元
>
> 场地:C
> 小计: 0元
>
> 场地:D
> 小计: 0元
> ---
> 总计: 460
```

#4 项目进度

##4.0 整体进度

> 按照需求说明书的要求实现了如下功能：
>
> ​	1) 正确识别并录入有效输入项，包括预约和消约项
> ​	2) 输入空格按要求格式打印各场馆细节和汇总收入信息

##4.1 cs_configuration 模块进度

> 实现功能如下：
>
> ​	1) 配置文件的读取，并根据配置文件设置系统必要参数

##4.2 cs_exception模块进度

> 实现如下功能：
>
> ​	1) 按照需求定义了各种格式校验类型和错误类型
> ​	2) 实现各种序列化数据的格式校验

##4.3 cs_database模块进度

> 实现如下功能：
>
> ​	1) 分别定义了数据库，表等数据结构
> ​	2) 实现对表的增删改查的功能
> ​	注：未实现将数据库读写到硬盘的功能，但预留了接口 

##4.4 cs_log模块进度

> 实现如下功能：
>
> ​	1) 实现了各种数据的类型向屏幕的输出控制
> ​	2) 预留了写日志接口，但未实现

##4.5 user_interface模块进度

> 实现如下功能：
>
> ​	1) 调用cs_configuration的接口加载配置信息
> ​	2) 实现了输入预约/消约信息的有效性检测
> ​	3) 有效预约/消约项入库
> ​	4) 实现收入清算功能，根据配置信息中收入信息计算预约/消约项带来的收入

##4.6 tools模块进度

> 实现如下功能：
>
> ​	1) 定义了程序运行过程中使用的全部数据类型，如日期段，时间段等及其相应操做，如求时间段交集，判断是否包含，时间做差等
> ​	2) 定义程序常用的公共方法，如字符串到各类型的转换

#5 使用说明文档

> 本项目为使用Visual Studio 2015集成开发环境开发的基于C++标准库开发的控制台应用程序，未使用第三方库。

##5.1 编译安装说明

> 1. 安装任意版本的Visual Studio 2015集成开发环境
> 2. 解压源文件包到目录下，如.../Visual Studio 2015/Projects/ChargingSystem
> 3. 找到源文件根目录下ChargingSystem.sln文件，打开工程文件
> 4. 选择vs的build菜单项编译源文件

## 5.2 配置文件使用说明

> \* 配置项名以“@”字符开始“=”结束
> \* 配置项属性值包含在一对“{}”中，子属性同样必须包含在一对“{}”中
> \* 配置项以”;“结束
> \* 一条配置项占用一行

```
#global field
@week={{1},...}; #open day in a week
@date_format={{...}}; #regular expression of date
@time_format={{...}}; #regular expression of time
@week_day_format={{[1-7]}}; #regular expression of week day
#system field
@site={{A},...}; # 场地的设定
@usable_time={{[1~7]_({{09:00~22:00}})},...}; #一周内的可用时间设定
@charge={{[1~5]_({{[09:00~12:00]_(30)}})}}; #每天各时段收费设定
@penalty={{[1~5]_(50)},...}; #每天违约金支付比例设定
@book_format={{...}}; #regular expression of book
@cancle_format={{...}}; #regular expression of cancle
```

## 5.3 程序使用说明

> 1. 键盘组合按键 home+R 呼出运行窗口，输出cmd打开windows终端界面
> 2. 新建配置文件ChargingSystem.conf，设置后将配置文件放到程序exe文件同一目录下
> 3. 输入cd 命令，进入exe程序目录下
> 4. 输入ChargingSystem.exe开始运行程序
> 5. 程序开始运行后首先询问是否加载配置文件，若配置加载失败会有相应错误提示，此时用户应先设置好配置文件再重新输入y加载配置文件，期间程序不会退出

> 使用示例：

```
Debug>ChargingSystem.exe
> 是否加载系统配置 Y/N ?
y
> Warn: loading configuration...
> Success: config aready!
> 收入汇总
> ---
> 场地:A
...
```

###测试用例使用

>程序启动服务后输入test指令即可打印<用例1>和<用例2>的测试结果
>* 注：test指令只有在Debug模式中才能使用

> 效果如下：

```
> Warn: 输入格式检测通过率: 23 / 23
> Success: the booking is accepted!
> Error: the booking is invalid!
> Error: the booking is invalid!
> Error: the booking is invalid!
> Error: the booking is invalid!
> Success: the booking is accepted!
> Success: the booking is accepted!
> Success: the booking is accepted!
> Success: the booking is accepted!
> Success: the booking is accepted!
> Error: the booking being cancelled does not exist!
> Warn: 输入有效性检测通过率: 11 / 11
```

