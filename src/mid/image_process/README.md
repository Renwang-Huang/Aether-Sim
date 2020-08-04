# ros_darknet与ros_kcf编译使用说明

需要环境

- ubuntu18.04 
- cuda
- opencv-control(3.4.4)
- libopencv-dev(3.2)
- cv-bridge(源码编译)

## 编译

### 编译opencv

因为ros-kcf需要opencv3.4.4固件，而ubuntu18.04自带的libopencv-dev版本为3.2，我们需要重新编译opencv，系统自带的libopencv-dev请勿卸载。

到[opencv github官网](https://github.com/opencv)中下载opencv3.4.4压缩包以及opencv_contrib3.4.4压缩包

将opencv_tontrib3.4.4解压缩并重命名为opencv-contrib剪切至opencv-3.4.4中，如下tree所示

```
├── opencv-3.4.4
│   ├── 3rdparty
│   ├── apps
│   ├── build
│   ├── cmake
│   ├── CMakeLists.txt
│   ├── CONTRIBUTING.md
│   ├── data
│   ├── doc
│   ├── include
│   ├── LICENSE
│   ├── modules
│   ├── opencv_contrib
│   ├── platforms
│   ├── README.md
│   └── samples
```

接下来编译opencv 可参考这个博客[Linux（Ubuntu 18.04）中安装OpenCV + OpenCV_Contrib](https://www.cnblogs.com/zzy1024/p/11405641.html)，注意编译完成后不必安装（make install）

### 编译cv_bridge

请看这个[README.md](https://gitee.com/bingobinlw/cv_bridge/blob/master/README.md)

### 修改CMakeList.txt

将编译完成的opencv-3.4.4/build路径以及cv_bridge相关路径添加到ros_kcf中

修改ros_kcf中

将编译完成的opencv-3.4.4/build路径添加到ros_kcf中tracking以及video_pub中的CMakeList.txt

```
set(OpenCV_DIR "your-path/opencv-3.4.4/build")    
set(cv_bridge_DIR your-path/cv_bridge_ws/devel/share/cv_bridge/cmake)
```

## 其他

darknet_ros编译过程中可能会出现gpu配置出错的问题，可以根据下面的设置适配自己的gpu

关于**<u> -gencode arch=compute_xx,code=sm_xx</u>**的选择可参考下面的一般配置：

```
  Fermi (CUDA 3.2 until CUDA 8) (deprecated from CUDA 9):
  SM20 or SM_20, compute_30 – Older cards such as GeForce 400, 500, 600, GT-630
  
  Kepler (CUDA 5 and later):
  SM30 or SM_30, compute_30 – Kepler architecture (generic – Tesla K40/K80, GeForce 700, GT-730)
  Adds support for unified memory programming
  SM35 or SM_35, compute_35 – More specific Tesla K40
  Adds support for dynamic parallelism. Shows no real benefit over SM30 in my experience.
  SM37 or SM_37, compute_37 – More specific Tesla K80
  Adds a few more registers. Shows no real benefit over SM30 in my experience
  
  Maxwell (CUDA 6 and later):
  SM50 or SM_50, compute_50 – Tesla/Quadro M series
  SM52 or SM_52, compute_52 – Quadro M6000 , GeForce 900, GTX-970, GTX-980, GTX Titan X
  SM53 or SM_53, compute_53 – Tegra (Jetson) TX1 / Tegra X1
  
  Pascal (CUDA 8 and later)
  SM60 or SM_60, compute_60 – Quadro GP100, Tesla P100, DGX-1 (Generic Pascal)
  SM61 or SM_61, compute_61 – GTX 1080, GTX 1070, GTX 1060, GTX 1050, GTX 1030, Titan Xp, Tesla P40, Tesla P4, Discrete GPU on the NVIDIA Drive PX2
  SM62 or SM_62, compute_62 – Integrated GPU on the NVIDIA Drive PX2, Tegra (Jetson) TX2
  
  Volta (CUDA 9 and later)
  SM70 or SM_70, compute_70 – DGX-1 with Volta, Tesla V100, GTX 1180 (GV104), Titan V, Quadro GV100
  SM72 or SM_72, compute_72 – Jetson AGX Xavier
  
  Turing (CUDA 10 and later)
  SM75 or SM_75, compute_75 – GTX Turing – GTX 1660 Ti, RTX 2060, RTX 2070, RTX 2080, Titan RTX, Quadro RTX 4000, Quadro RTX 5000, Quadro RTX 6000, Quadro RTX 8000
  
  
```

  