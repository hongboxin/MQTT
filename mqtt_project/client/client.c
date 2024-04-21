/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(21/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "21/04/24 15:49:20"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>

#include "cJSON.h"
#include "dictionary.h"
#include "get_conf.h"
#include "iniparser.h"
#include "logger.h"
#include "mosquitto.h"
#include "mytime.h"
#include "mysignal.h"
#include "temperature.h"

#define INI_PATH	"./aliyun.ini"

void connect_callback(struct mosquitto *mosq, void *obj, int rc);
void print_usage(char *progname);

int main(int argc, char *argv[])
{
	int					rv = 0;
	int					rc = -1;
	int					opt = -1;
	int					daemon_run = 0;
	char				*progname = basename(argv[0]);
	mqtt_data_t			mqtt;
	struct mosquitto	*mosq = NULL;
	char				*log_file = "client.log";
	int					log_level = LOG_LEVEL_INFO;
	int					log_size  = 10;

	struct option		options[] = 
	{
		{"debug",no_argument,NULL,'d'},
		{"background",no_argument,NULL,'b'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while( (opt = getopt_long(argc,argv,"dbh",options,NULL)) != -1 )
	{
		switch(opt)
		{
			case 'd':
				log_file = "console";
				break;

			case 'b':
				daemon_run = 1;
				break;

			case 'h':
				print_usage(argv[0]);
				break;

			default:
				break;
		}
	}

	if( daemon_run )
	{
		daemon(1,0);
	}
	
	install_signal();

	if( log_open(logfile, loglevel, logsize, THREAD_LOCK_NONE) < 0 )
	{
		printf("Initial log system failed:%s\n",strerror(errno));
		return -1;
	}

	memset(&mqtt,0,sizeof(mqtt));
	rc = get_mqtt_conf(INI_PATH,&mqtt,PUB);
	if( rc < 0 )
	{
		log_error("Read INI file failure:%s\n",strerror(errno));
		return -1;
	}

	/* MQTT initialize */
	mosquitto_lib_init();

	/* Creating a MQTT client */
	mosq = mosquitto_new(mqtt.clientid,true,(void *)&mqtt);
	if( !mosq )
	{
		log_error("mosquitto new() failure:%s\n",strerror(errno));
		rv = -2;
		goto cleanup;
	}
	
	/* Setting username and passwd */
	if( mosquitto_username_pw_set(mosq,mqtt.username,mqtt.passwd) != MOSQ_ERR_SUCCESS )
	{
		log_error("mosquitto set failure:%s\n",strerror(errno));
		rv = -3;
		goto cleanup;
	}

	/* Setting callback */
	mosquitto_connect_callback_set(mosq,connect_callback);

	while( !g_signal )
	{
		if(mosquitto_connect(mosq, mqtt.hostname, mqtt.port, KEEP_ALIVE) !=MOSQ_ERR_SUCCESS)
		{
			log_error("mosquitto connect failure:%s\n",strerror(errno));
			goto cleanup;
		}
		printf("Connect successfully!\n");

		mosquitto_loop_forever(mosq,-1,1);
	}

cleanup:
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return rv;
}

void print_usage(char *progname)
{
	printf("%s usage:\n",progname);
	printf("-h(help):print this help information.\n");
	printf("-d(debug):set debug.\n");
	printf("-b(background):running in background.\n");
	return ;
}

void connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
	float			temp;
	char			*msg;
	mqtt_data_t		*mqtt = (mqtt_data_t *)obj;

	cJSON			*root = cJSON_CreateObject();
	cJSON			*item = cJSON_CreateObject();

	memset(root,0,sizeof(root));
	memset(item,0,sizeof(item));

	if( ds18b20_get_temperature(&temp) < 0 )
	{
		log_error("Getting temperature failure:%s\n",strerror(errno));
		return ;
	}

	if( !obj )
	{
		log_error("Invalid argument in %s\n",__FUNCTION__);
		return ;
	}

	cJSON_ADDItemToObject(root,"method",cJSON_CreateString(mqtt->method));
	cJSON_ADDItemToObject(root,"id",cJSON_CreateString(mqtt->jsonid));
	cJSON_ADDItemToObject(root,"params",item);
	cJSON_ADDItemToObject(item,"CurrentTemperature",cJSON_CreateNumber(temp));
	cJSON_ADDItemToObject(root,"version",cJSON_CreateString(mqtt->version));

	msg = cJSON_Print(root);
	printf("%s\n",msg);

	if( !rc )
	{
		if(mosquitto_publish(mosq, NULL, mqtt->topic, strlen(msg), msg, mqtt->Qos, NULL) != MOSQ_ERR_SUCCESS)
		{
			printf("mosquitto publish failure:%s\n",strerror(errno));
			return ;
		}
	}
	printf("mqtt publish successfully!\n");

	mosquitto_disconnect(mosq);
}
