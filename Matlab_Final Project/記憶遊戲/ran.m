aa = [2 2 ; 5 2; 8 2; 2 5; 5 5; 8 5; 2 8; 5 8; 8 8];
    figure;
    for i=1 : 9
        xy = aa(i,:);
        [p_x, p_y] =  circle(xy);
        plot(p_x , p_y,'b');
        axis equal
        hold on
    end
