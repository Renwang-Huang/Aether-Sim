image = imread('map.pgm');
imageCropped = image(1:184,7:290);
bwimage = imageCropped < 100;
imshow(bwimage)
map = binaryOccupancyMap(bwimage,20);%20表示地图分辨率为0.05米/像素
show(map)

validator = validatorOccupancyMap;
validator.Map = map;
planner = plannerHybridAStar(validator,'MinTurningRadius',0.8,'MotionPrimitiveLength',1.2);%MinTurningRadius:转弯半径；MotionPrimitiveLength:运动基元长度
startPose = [7.5 7.5 0]; % [meters, meters, radians]
goalPose = [7.5 1.5 0];
refpath = plan(planner,startPose,goalPose);
show(planner);

