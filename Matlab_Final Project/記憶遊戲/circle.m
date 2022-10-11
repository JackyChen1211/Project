function [p_x , p_y] = circle(xy)
theta = 0:0.1:2*360;
deg = theta*pi/180;
p_x = xy(1)+1*cos(deg);
p_y = xy(2)+1*sin(deg);