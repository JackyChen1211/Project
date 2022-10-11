clear m;
clear ans;
for i=1 : deg
        clear [x1,y1,x2,y2];
        clear h;
        m(i) = ceil(rand(1)*9);
        [x1,y1,x2,y2] = point (m(i));
        h = plot(x1,y1,'r',x2,y2,'r');
        pause(t);
        delete(h);
        pause(t);
end