function y = safeRegPM(x)

safeReg = load('tmp/safeReg.mat');
p1_dis = ceil(x(:,1)/10);
y= p1_dis;
for i=1:size(y,1)
    y(i) = (p1_dis(i)>size(safeReg.paramSpace,1) || safeReg.paramSpace(p1_dis(i))>0)*2000;
end
end
