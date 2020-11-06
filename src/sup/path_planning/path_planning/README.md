# path_planning
 调用ompl库中的RRT*进行全局路径规划,地图使用octomap

- 运行须知

  需要octomap包支持

  ```
  见volans/src/mid/map/cotomap
  ```

  

  安装ompl库

  ```
  sudo apt-get install libfcl-dev libompl-dev
  ```

  编译

  ```
  catkin build simulation px4_control path_planning octomap
  ```

  

- 运行

  ```
   roslaunch simulation ompl3Drrt_px4.launch
  ```

  发布目标点

  ```
  rostopic pub /clicked_point geometry_msgs/PointStamped "header:
    seq: 0
    stamp:
      secs: 0
      nsecs: 0
    frame_id: ''
  point:
    x: 10.0
    y: 0.0
    z: 0.0"
  ```

  

 