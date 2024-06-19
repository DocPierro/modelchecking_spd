function f = bra(x)

a=1;
b=5.1/(4*pi*pi);
c=5/pi;
d=6;
h=10;
ff=1/(8*pi);

f = a.*(x(:,2)-b.*x(:,1).^2+c.*x(:,1)-d).^2+h.*(1-ff).*cos(x(:,1))+h;

%[M,~]=size(x);
%for iii=1:M
%	aux=x(iii,:);
%	f=evaluate_objective(aux);							
%end
f = [x f];
