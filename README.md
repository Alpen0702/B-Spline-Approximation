# B-Spline-Approximation
这里是2022秋-应用几何造型基础的课程作业：在代数距离和几何距离下分别拟合B样条曲线，并讨论其区别。<br>
## 项目路线
本项目的基本路线如下：<br>
1.由用户给定B-Spline的参数：h、p、n以及距离模式（algebraic/geometric）。<br>
2.在曲线上计算knots：在曲线的头尾分别重复p+1个相同的knots，而在中间[p,h+1]范围上均匀分布knots。<br>
3.计算给定数据点集D[]在曲线上对应点的参数。这里采用的是centripetal的方法，其参数alpha可以自定义。<br>
4.利用2、3的结果，计算出矩阵N、Q，然后计算控制点集P[]。此时这条B-Spline曲线已经确定。<br>
5.对几何距离模式（geometric），用牛顿法寻找给定数据点集D[]到曲线的最近点集，用这些最近点集的参数t[]回代重复步骤4，直到曲线不再明显改变。<br>
6.绘制曲线。实时监听用户对给定数据点集D[]的修改，并返回步骤3重绘曲线。<br>
## 使用说明
## 讨论
有任何问题，欢迎通过llzju@zju.edu.cn联系我哦。<br><br>
## *Reference*<br>
*矩阵的LU分解：https://blog.csdn.net/qq_40688707/article/details/89256737*<br>
*B样条曲线拟合：https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/INT-APP/CURVE-APP-global.html*<br>
*迭代法求几何拟合解：***基于迭代最近点的B样条曲线拟合方法研究. 中国图象图形学报. 肖轶军,丁明跃,彭嘉雄. 2000(005)007****<br>
