function [ answer ] = puk_algo(upper_tol, lower_tol, puk_vec, exp_vec)
answer = 'false';
% start algo
bitCode = zeros(16, 1);
last = 0;
index = 1;
% get base category
for i = 1 : 1 : size(puk_vec, 2)
   if (puk_vec(i) >= upper_tol) && (last == 0) && (index < 17)
       last = 1;
       bitCode(index) = last;
       index = index + 1;
   elseif (puk_vec(i) <= lower_tol) && (last == 1) && (index < 17)
       last = 0;
       bitCode(index) = last;
       if index == 2
           rill_1 = i;
       elseif index == 4
           rill_2 = i;
       elseif index == 6
           rill_2_2 = i;
       elseif index == 8
           rill_1_2 = i;
       end
       index = index + 1;
   end 
end
% get final category
cat5 = [1, 0, 1, 0, 1, 0, 1 , 0, 1, 0, 0, 0, 0,0,0,0]';
if bitCode == cat5
    % check width of rills
    counter1 = 0;
    counter2 = 0;
    counter3 = 0;
    counter4 = 0;
    for i = 1:1:2
        with_end1 = 1;
        with_end2 = 1;
        while (with_end1 == 1) || (with_end2 == 1) && (with_end1 == 1)
           if puk_vec(rill_1 + counter3) <= -2660
              counter3 = counter3 + 1;
           else
              with_end1 = 0;
           end
           if (puk_vec(rill_1_2 + counter4) <= -2660) && (with_end2 == 1)
              counter4 = counter4 + 1;
           else
               with_end2 = 0;
           end
        end
        if i == 1
            counter1 = counter3;
            counter2 = counter4;
        end
    end
    if (counter1 >= 9) && (counter1 <= 16) && (counter2 >= 9) && (counter2 <= 16) && (counter3 >= 9) && (counter3 <= 16) && (counter4 >= 9) && (counter4 <= 16)
        bitCode(16) = 1;
    elseif (counter1 >= 20) && (counter1 <= 27) && (counter2 >= 20) && (counter2 <= 27) && (counter3 >= 9) && (counter3 <= 16) && (counter4 >= 9) && (counter4 <= 16)
        bitCode(15) = 1;
    elseif (counter1 >= 20) && (counter1 <= 27) && (counter2 >= 20) && (counter2 <= 27) && (counter3 >= 20) && (counter3 <= 27) && (counter4 >= 20) && (counter4 <= 27)
        bitCode(16) = 1;
        bitCode(15) = 1;
    end
end
% end algo
if bitCode == exp_vec
    answer = 'true';
end