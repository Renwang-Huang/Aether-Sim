image = imread('map.pgm');
imageCropped = image(1:184,7:290);
bwimage = imageCropped < 100;
imshow(bwimage)
map = binaryOccupancyMap(bwimage);
show(map)
