# dashboard-app-stm32
STM32F103C8T6(CBT6) for Dashboard-APP(application)
IDE:keil5 
程序不能直接烧写使用,因为修改了程序的初始地址，直接烧写使用程序并不能正确的运行
需要利用keil5生成bin文件，然后利用dashboard-app-stm32把bin文件通过USB烧写到单片机中
如果觉得自己不需要iap功能，可以对把工程的地址和栈的偏移量等修改为0x0800 0000即可。
具体为：
全局搜索变量VECT_TAB_OFFSET，修改起数值为0x0800 0000U
点击option选择target，把IROM1的start地址修改为0x0800 0000
然后重新编译 并烧写即可。

简单的使用了ucos-III
其中音乐频谱是抄的网上的代码
