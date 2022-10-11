while true
    ran;        
    d = input('請選擇難易度 1為簡單(出現7次，每次間隔１秒)，2為困難(出現12次，每次間隔0.5秒)，3為自定義難易度: ');
    switch d
        case 1
            fprintf('您選擇了簡單 \n');
            deg = 7;
            t = 1;
        case 2
            fprintf('您選擇了困難 \n');
            deg = 12;
            t = 0.5;
        case 3
            fprintf('您選擇了自定義，自定義難易度可自訂出現紅色十字的次數及出現的間隔時間 \n');
            deg = input('請輸入您想自訂的數量: ');
            t = input('請輸入您想自訂的間隔時間: ');
    end
    pause(1);
    fprintf('遊戲即將開始\n');
    for i=3 : -1 : 1
        disp(i);
        fprintf('\n');
        pause(1);
    end
    start;
    close;
    fprintf('請輸入順序，格式為[1,2,3,...]\n')
    ans = input('您的答案是: ');
    if(ans == m)
        fprintf('全答對了\n')
        again = input('請問是否再玩一次? 是請輸入1，不要請輸入0: ');
        if(again ~= 1)
            break;
        end
    else
        fprintf('答錯了\n')
        g = grade(ans,m,deg);
        fprintf('您的分數為 %d/%d \n',g,deg)
        again = input('請問是否再玩一次? 是請輸入1，不要請輸入0: ');
        if(again ~= 1)
            break;
        end
    end
end
fprintf('感謝您的遊玩');