function g = grade(ans,m,deg)
g = 0;
for i=1:deg
    if(ans(i) == m(i))
        g = g+1;
    end
end