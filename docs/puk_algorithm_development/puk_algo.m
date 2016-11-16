function [ answer ] = puk_algo(upper_tol, lower_tol, puk_vec, exp_vec)
answer = 'false';
bitCode = zeros(16, 1);
last = 0;
index = 1;
for i = 1 : 1 : size(puk_vec, 2)
   if (puk_vec(i) >= upper_tol) && (last == 0) && (index < 17)
       last = 1;
       bitCode(index) = last;
       index = index + 1;
   elseif (puk_vec(i) <= lower_tol) && (last == 1) && (index < 17)
       last = 0;
       bitCode(index) = last;
       index = index + 1;
   end 
end
if bitCode == exp_vec
    answer = 'true';
end