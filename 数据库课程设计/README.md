**开发**

源代码在 `./DatabseDesign.zip` 中，使用Django实现。

前端开发参考 [volt-pro](https://demo.themesberg.com/volt-pro/pages/dashboard/dashboard.html)，甚至直接 dev-tool 可以看到源文件, map-box 的 key 都有

数据库使用 `MySQL`，数据来源为：[MARIS](https://maris.iaea.org/explore-mapbox)，需要自己配置，表单名称请在Django后端中 `views.py` 中寻找


> 由于本项目开发时间与软工和其他三门课程设计冲突，开发代码极为潦草，数据库其实只有一个表，`views.py` 中函数过于丑陋，数据库连接固定的，主页的曲线展示的数据是固定的
> 
> 线上答辩，展示视频+PPT，gjh老师很友善
