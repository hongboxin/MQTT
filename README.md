# MQTT

此阶段项目开发，将采用标准的物联网协议MQTT将采样的数据实时上报到阿里云、腾讯云、华为云等平台上。
该项目开发除了满足上述APUE项目要求外，还需要满足：
1、该项目只需要实现客户端即可，服务器端采用阿里云等公有云平台。
2、该项目可以使用mosquitto实现的MQTT库，采用JSON协议格式上报数据。
3、DS18B20温度传感器可以替换成SHT20温度传感器，了解Linux下I2C接口的使用。
4、该程序要求支持配置文件来配置不同云平台的参数，从而实现同一份代码不用任何修改就可以连接三大平台。
