#include "Inf_DS1302Z.h"

/**
 * @brief 7系数延迟3s之后的误差大概在0.3s左右
 *        8系数延迟3s之后的误差大概在0.006s左右
 * @param us
 */
void Inf_Delay_us(uint32_t us)
{
    uint32_t count = us * 8;
    // 8次 1us  =>  一次是125ns   机械周期是72MHz  一次是13ns
    while (count--)
    {
        __NOP();
    }
}

/**
 * @brief 读取一个寄存器的值
 *
 * @param reg
 * @return uint8_t
 */
uint8_t Inf_DS1302Z_Read_Byte(uint8_t reg_addr)
{
    // 确保是一个读指令 =>  最低位为1
    reg_addr |= 0x01;

    uint8_t data = 0;
    // 1. 初始化引脚
    DS_RST_L;
    DS_CLK_L;

    // 2. 拉高RST引脚 => 准备发送指令
    DS_RST_H;
    Inf_Delay_us(5);

    // 3. 循环8次  低位优先  发送指令
    for (uint8_t i = 0U; i < 8U; i++)
    {
        if (reg_addr & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }
        DS_CLK_H;
        Inf_Delay_us(1);

        // 4. 重置时钟引脚  => 准备发送第二次的数据
        DS_CLK_L;
    }
    // 芯片厂家也注意到了这一点 => 规定指令的最高位一定是1  => 释放IO引脚
    // 5. 循环8次  低位优先  接收数据
    // 读取之前一定要释放IO引脚
    DS_IO_H;
    for (uint8_t i = 0; i < 8; i++)
    {
        // 特殊点 => 发送完读指令之后  直接可以读取第一位数据
        data |= (DS_IO_READ << i);
        if (i < 7U)
        {
            DS_CLK_H;
            DS_CLK_L;
        }
    }

    // 6. 结束读取 => 释放RST引脚
    DS_RST_L;

    return data;
}

/**
 * @brief 写入一个寄存器的值
 *
 * @param reg
 * @param data
 */
void Inf_DS1302Z_Write_Byte(uint8_t reg_addr, uint8_t data)
{
    // 初始的地址确认 => 确认是一个写指令  => 最低位写0
    reg_addr &= (0xfe);

    // 1. 初始化引脚
    DS_RST_L;
    DS_CLK_L;

    // 2. 拉高RST引脚 => 准备发送指令
    DS_RST_H;
    Inf_Delay_us(5);

    // 3. 循环8次  低位优先  发送指令
    for (uint8_t i = 0U; i < 8U; i++)
    {
        if (reg_addr & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }

        DS_CLK_H;
        Inf_Delay_us(1);
        DS_CLK_L;
    }
    // 4. 准备写数据
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }
        DS_CLK_H;
        Inf_Delay_us(1);
        DS_CLK_L;
    }

    /* CE/RST must return low after the last data bit to close the access. */
    DS_RST_L;
}
