function [vec] = cutToLenght(puk_vec)
vec = zeros(232, 1);
i = 1;
y = 1;
ready = 0;

while ready == 0 || ready == 1
    if puk_vec(i) > -2680 && ready == 0
        ready = 1;
    end
    if ready == 1
        vec(y) = puk_vec(i);
        y = y + 1;
    end 
    if y == 233
        ready = 2;
    end
    i = i + 1;
end 