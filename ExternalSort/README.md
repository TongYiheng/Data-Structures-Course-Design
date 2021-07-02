## 文件结构

### Project
- lab_2.pro
- lab_2.pro.user

### Headers
- mainwindow.h: 主窗口类的声明，包括externalSorter的声明（为了方便在externalSorter的实现中需要向ui输出一些信息，所以将externaoSorter的接口封装到了mainwindow类中）
- minHeap.h: 小根堆类的声明
- minLoserTree.h: 最小输者树类的声明

### Sources
- main.cpp: 主函数源文件
- mainwindow.cpp: 主窗口源文件，包括externalSorter的实现
- minHeap.cpp: 小根堆实现源文件
- minLoserTree.cpp: 最小输者树实现源文件

### Forms
- mainwindow.ui: 界面设计

### Resources
- images.qrc: 图标资源

### Pictures
- icon.png: 图标