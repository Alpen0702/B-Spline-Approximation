# B-Spline-Approximation
这里是2022秋-应用几何造型基础的课程作业：在代数距离和几何距离下分别拟合B样条曲线，并讨论其区别。<br>
## 项目路线
本项目的基本路线如下：<br><br>
1.由用户给定B-Spline的参数：h、p、n以及距离模式（algebraic/geometric）。<br><br>
2.在曲线上计算knots：在曲线的头尾分别重复p+1个相同的knots，而在中间[p,h+1]范围上均匀分布knots。<br><br>
3.计算给定数据点集D[]在曲线上对应点的参数。这里采用的是centripetal的方法，其参数alpha可以自定义。<br><br>
4.利用**步骤2、3**的结果，计算出矩阵N、Q，然后计算控制点集P[]。此时这条B-Spline曲线已经确定。<br><br>
5.对几何距离模式（geometric），用牛顿法寻找给定数据点集D[]到曲线的最近点集，用这些最近点集的参数t[]回代重复**步骤4**，直到曲线不再明显改变。<br><br>
6.绘制曲线。实时监听用户对给定数据点集D[]的修改，并返回**步骤3**重绘曲线。<br><br>
## 使用说明
本项目是在Windows10环境下用Visual Studio 2022开发的，主要调用的库包括GL/freeglut.h和GL/freeglut_ext.h。<br><br>
下载项目zip文件并解压后，打开B-Spline Approximation.sln即可运行。如果运行失败，请联系我获得支持；你也可以在根目录下直接打开B-Spline Approximation.exe来查看项目运行结果，无需自行编译。<br><br>
运行项目后，命令行会提示用户输入B-Spline的参数h、p、n，分别代表控制顶点数-1、曲线次数（阶数-1）、给定数据点数-1，注意：n > h >= p >= 1 && n < 50 && h < 10。程序会检查输入参数的合法性。完成检查后，用户可以输入0：algebraic或1：geometric以选择不同的距离模式。收到Parameters checked的提示后，按任意键进入图形界面。<br><br>
在图形界面中，首先会给出均匀分布在半径为0.8的圆上的n+1个给定数据点，以及对应距离模式下生成的控制顶点和样条曲线。用户可以用鼠标左键自由拖拽给定数据点，程序会实时更新对应的控制顶点和样条曲线。另外，程序也提供了鼠标滚轮缩放的功能。<br><br>
在图形界面中，蓝色的方形是给定数据点，绿色的方形是控制顶点，红色粗线是B样条曲线，橙色圆点是给定数据点在样条曲线上对应参数的点，橙色细线将这些点与对应的给定数据点相连。**需要注意的是，在样条曲线的起点与终点处，蓝色方形、绿色方形与橙色圆点是重合的。这是因为拟合方法确定了首末的给定数据点与控制顶点重合，且与对应参数点也重合。**<br><br>
## 讨论
我们对两种距离模式造成拟合曲线的差异进行讨论。在定义上，代数距离模式是最小化给定数据点集到曲线上对应参数点集的距离，而几何距离模式则是最小化给定数据点集到曲线的直线距离。因此，几何距离会明显小于代数距离。在实际测试中也是如此，在几何距离模式下，蓝色点总是与曲线贴得很紧密，且它们与曲线上对应参数点集的连线总是垂直于该处的切线；相比之下，代数距离模式中蓝色点就可能与曲线距离远得多，且这一现象在控制顶点数量少、曲线次数低的情况下尤其明显。<br><br>
<center class="half">
    <img src="https://github.com/Alpen0702/B-Spline-Approximation/blob/master/resources/pre_al.png" width="200"/>
    <img src="https://github.com/Alpen0702/B-Spline-Approximation/blob/master/resources/pre_geo.png" width="200"/>
</center>
如上图，注意看，其实两张图中给定数据点（蓝色方形）相对位置是基本一致的，只是为了显示控制顶点而缩小了右图而已。但两条曲线的形状截然不同。几何距离模式下的拟合效果要好得多，但也使得这条曲线过分地扭曲，以至于把控制顶点挤压到很远的位置。<br><br>
另外，几何距离模式下的曲线容易产生突变，很微小的顶点拖动可能会带来很明显的形状变化，从而导致较强的不稳定性。就我而言，代数距离模式对顶点移动的反馈更缓和，更符合直觉的判断；而几何距离模式使人难以判断顶点拖动带来的变化，曲线的变化速度也更为剧烈，这是对交互设计不友好的。<br><br>

<center>
    <img style="border-radius: 0.3125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
    src="https://github.com/Alpen0702/B-Spline-Approximation/blob/master/resources/al.gif">
    <img style="border-radius: 0.3125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
    src="https://github.com/Alpen0702/B-Spline-Approximation/blob/master/resources/geo.gif">
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">左图</div>
     
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">右图</div>
</center>
几何距离模式下，点间顺序是可以发生改变的：拖动数据点逾越另一个数据点即可；但在代数距离模式下，数据点是有先后顺序之分的，因为数据点对应的参数是有先后顺序的。后者支持让曲线以一定的顺序经过各个数据点，因此更方便复杂形态曲线的设计；前者则不具备这样的功能。<br><br>
几何距离模式追求绝对最小距离，需要在代数距离模式的基础上再做多次迭代来搜索最小值点，因此其速度明显慢于代数距离模式，且在给定数据点数量大的情形下显得尤为缓慢。另一方面，在本项目中，几何距离模式是采用牛顿法来寻找最小值点的，但这种方式可能会陷入局部最优解，导致给定数据点找到的曲线上对应点并非实际最近点、所求出曲线并非实际最优曲线。这也是本项目后续值得反思和改进的问题之一。<br><br>
综上所述，尽管代数距离模式的拟合可能不够精细，但在次数足够大、控制顶点数足够多的情形下表现已经足够令人满意，其更高的效率、更好的稳定性和可理解性使之在造型设计方面具有更好的实用价值；几何距离模式则更为精确，更适用于数学研究等追求极致拟合甚至是插值的情形。<br><br>
## 联系作者
有任何问题，欢迎通过llzju@zju.edu.cn联系我。<br><br>
## *Reference*<br>
*矩阵的LU分解：https://blog.csdn.net/qq_40688707/article/details/89256737*<br>
*B样条曲线拟合：https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/INT-APP/CURVE-APP-global.html*<br>
*迭代法求几何拟合解：***基于迭代最近点的B样条曲线拟合方法研究. 中国图象图形学报. 肖轶军,丁明跃,彭嘉雄. 2000(005)007****<br>
