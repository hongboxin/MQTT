/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_conf.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(20/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "20/04/24 16:20:57"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iniparser.h>
#include <dictionary.h>

#include "get_conf.h"

int get_mqtt_conf(char *ini_path,mqtt_data_t *mqtt,int type)
{
	dictionary		*ini = NULL;

	const char		*host;
	int				port;
	
	const char		*username;
	const char		*passwd;

	const char		*clientid;
	const char		*topic;

	const char		*method;
	const char		*jsonid;
	const char		*identifier;
	const char		*version;
	
	int				Qos;

	if( !ini_path || !mqtt )
	{
		printf("Invail input parameter in %s\n",__FUNCTION__);
		return -1;
	}

	ini = iniparser_load( ini_path );
	if( !ini )
	{
		printf("Iniparser Load failure:%s\n",strerror(errno));
		return -1;
	}

	host = iniparser_getstring(ini, "mqtt_server_addr:host", DEFAULT_HOST);
	port = iniparser_getint(ini, "mqtt_server_addr:port", DEFAULT_PORT);

	username = iniparser_getstring(ini, "user_passwd:username", DEFAULT_USERNAME);
	passwd   = iniparser_getstring(ini, "user_passwd:passwd", DEFAULT_PASSWD);

	clientid   = iniparser_getstring(ini, "client_id:id", DEFAULT_CLIENTID);
	identifier = iniparser_getstring(ini, "ali_json:identifier", DEFAULT_IDENTIFIER)
	Qos        = iniparser_getint(ini, "ali_Qos:Qos", DEFAULT_QOS);

	if( type == SUB )
	{
		topic = iniparser_getstring(ini, "sub_topic:topic", DEFAULT_SUBTOPIC);
	}
	else if( type == PUB )
	{
		topic   = iniparser_getstring(ini, "pub_topic:topic", DEFAULT_PUBTOPIC);
		method  = iniparser_getstring(ini, "ali_json:method", DEFAULT_METHOD);
		jsonid  = iniparser_getstring(ini, "ali_json:jsonid", DEFAULT_JSONID);
		version = iniparser_getstring(ini, "ali_json:version", DEFAULT_VERSION);
	}

	strncpy(mqtt->host, host, BUF_SIZE);
	mqtt->port = port;
	strncpy(mqtt->username, username, BUF_SIZE);
	strncpy(mqtt->passwd, passwd, BUF_SIZE);
	strncpy(mqtt->clientid, clientid, BUF_SIZE);
	strncpy(mqtt->topic, topic, BUF_SIZE);
	mqtt->Qos  = Qos;
	if( type == PUB )
	{
		strncpy(mqtt->method, method, BUF_SIZE);
		strncpy(mqtt->identifier, identifier, BUF_SIZE);
		strncpy(mqtt->jsonid, jsonid, BUF_SIZE);
		strncpy(mqtt->version, version, BUF_SIZE);
	}

	iniparser_freedict(ini);
	return 0;
}

