# <center> 自定义生成的生物

## 插件介绍及功能

**根据你的需要,修改生物的各种属性**

目前已支持的属性有

-   absorption：吸收
-   attackDamage：攻击伤害
-   followRange：跟随范围
-   health：生命值
-   jumpStrength：跳跃强度
-   knockbackResistance：击退抵抗
-   lavaMovementSpeed：熔岩移动速度
-   luck：幸运
-   movementSpeed：移动速度
-   underWaterMovementSpeed：水下移动速度

以上属性皆为可选属性，可以选择是否需要再填，不需要可以删除。

注意:请适量修改,否则你可能见不到怪物 (因为跑太快了哈哈哈)

## 插件安装 & 加载

1.  手动安装

    将下载的压缩包解压到`path/to/bds/plugins` 文件夹下

    包括文件夹夹一起 否则会加载不上

2.  使用`lip`包管理器安装
   
    使用命令:
     `lip install github.com/xianyubb/CustomMob`

3. 开启服务器加载插件...

## 配置文件

```json
{
    "version": 1,                              // 配置文件版本（无需配置）
    "mobs": {                                  // 生物配置
        "minecraft:blaze": {                   // 生物名称标准名 
            "absorption": 0.0,                 // 伤害吸收 范围 0-2048 类型 float
            "health": 0.0,                     // 生命值 范围 0-1024 类型 float
            "attackDamage": 0.0,               // 攻击力 范围 0-2048 类型 float
            "speed": {                         // 速度
                "movementSpeed": 0.0,          // 移动速度 范围 0-1024 类型 float
                "lavaMovementSpeed": 0.0,      // 熔岩移动速度 范围 0-1024 类型 float
                "underWaterMovementSpeed": 0.0 // 水下移动速度 范围 0-1024 类型 float
            },
            "luck": 0.0,                       // 幸运值 范围 -1024-1024 类型 float
            "followRange": 0.0,                // 追踪距离 范围 0-2048 类型 float
            "jumpStrength": 0.0,               // 跳跃高度 范围 0-2 类型 float
            "knockbackResistance": 0.0         // 击退抗性 范围 0-1 类型 float
        }
    }
}
```

初次生成配置文件 **mobs** 下为空 需要自己填写设定生物

其他生物的也如此 理论上支持`addon`生物

需要更改的话只需要更改后面的数字即可

## 重载配置文件命令

`/custommob` 或者 `/cm`

## 插件支持版本

BedRock Dedicated Server 1.21.2 协议 768

LeviLamina 0.13.5

## 插件作者及联系方式

Author: xianyubb

QQ: 2149656630

QQGroup: 865286891

## 感谢

感谢[LeviLamina](https://github.com/LeviLamina)提供的 API
