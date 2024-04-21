/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_conf.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(20/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "20/04/24 16:00:55"
 *                 
 ********************************************************************************/
#ifndef _GET_CONF_H_

#define _GET_CONF_H_

#define BUF_SIZE	512

#define DEFAULT_HOST		"iot-06z00gtm7spgofv.mqtt.iothub.aliyuncs.com"
#define DEFAULT_PORT		1883

#define DEFAULT_USERNAME	"master&k123ztDiync"
#define DEFAULT_PASSWD		"caecefd3150e92485e0b9061d080faf963b4dd708206fbac2b494ad85f092761"

#define DEFAULT_CLIENTID	"k123ztDiync.master|securemode=2,signmethod=hmacsha256,timestamp=1713500550444|"

#define DEFAULT_SUBTOPIC	"/sys/k123ztDiync/master/thing/event/property/set"
#define DEFAULT_PUBTOPIC	"/sys/k123ztDiync/master/thing/event/property/post"

#define DEFAULT_METHOD		"thing.service.property.set"
#define DEFAULT_JSONID		"12345"
#define DEFAULT_IDENTIFIER  "CurrentTemperature"
#define DEFAULT_VERSION		"1.0.0"

#define DEFAULT_ALIVE		60

#define DEFAULT_QOS			0

typedef struct mqtt_data_s
{
	char		host[BUF_SIZE];
	int			port;

	char		username[BUF_SIZE];
	char		passwd[BUF_SIZE];

	char		clientid[BUF_SIZE];
	char		topic[BUF_SIZE];
	int			Qos;

	char		method[BUF_SIZE];
	char		jsonid[BUF_SIZE];
	char		identifier[BUF_SIZE];
	char		version[BUF_SIZE];
}mqtt_data_t;

/* This function is used to obtain configuration file information */
extern int get_mqtt_conf(char *ini_path, mqtt_data_t *mqtt, int type);

#endif


