function w = widthAtAmplitude(startIdx, array, ratio)
if nargin < 3
    ratio = 0.5;
end
w = findRightAmplitutePoint(startIdx, array, ratio) - findLeftAmplitutePoint(startIdx, array, ratio);
end



function i = findRightAmplitutePoint(startIdx, array, ratio)
amp = array(startIdx);
if amp<0
    array = -array;
    amp = -amp;
end

i = startIdx;
while i < length(array) && array(i)~=amp*ratio && ~(array(i)>amp*ratio  && array(i+1)<amp*ratio)
    i = i+1;
end

if i == length(array) && array(i-1)~=amp*ratio && ~(array(i-1)>amp*ratio && array(i)<amp*ratio)
    i = inf;
end
end

function i = findLeftAmplitutePoint(startIdx, array, ratio)
amp = array(startIdx);
if amp<0
    array = -array;
    amp = -amp;
end

i = startIdx - 1;
while i > 1 && array(i)~=amp*ratio && ~(array(i)>amp*ratio && array(i-1)<amp*ratio)
    i = i-1;
end

if i == 1 && array(i+1)~=amp*ratio && ~(array(i)>amp*ratio && array(i+1)<amp*ratio)
    i = -inf;
end
end